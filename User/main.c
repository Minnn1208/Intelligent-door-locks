#include "stm32f10x.h"                  // Device header
#include "OLED.h"
#include "Password.h"
#include "Delay.h"
#include "ZW101.h"

#define MAIN_IDLE					0x00
#define MAIN_THUMB_VERIFY			0x01
#define MAIN_THUMB_REGISTER			0x02
#define MAIN_KEYBOARD				0x03
#define MAIN_ADMIN_PAGE				0x04
#define MAIN_SLEEP					0xFF


uint8_t adminEnable = 0x00;
uint8_t currentState = MAIN_IDLE;
uint8_t keyValue = 0x00;

void Admin_Enable()
{
	adminEnable = 0xFF;
}

void Admin_Disable()
{
	adminEnable = 0x00;
}

uint8_t Admin_GetState()
{
	return adminEnable;
}


void Thumb_OpenDoor(void)
{
	uint8_t state = 0x00;
	ZW101_VerifyThumbCommand();
	state = ZW101_VerifyThumb();
	if(state == ZW101_VERIFY_PASS)
	{
		OLED_Clear();
		OLED_ShowString(0, 0, "Thumb Pass", OLED_6X8);
		OLED_Update();
	}
	else if(state == ZW101_VERIFY_NOT_PASS)
	{
		OLED_Clear();
		OLED_ShowString(0, 0, "Thumb Not Pass", OLED_6X8);
		OLED_Update();
		
	}
	else
	{
		OLED_Clear();
		OLED_ShowString(0, 0, "ZW101_VERIFY_ERROR", OLED_6X8);
		OLED_Update();
	}
	/* else
	{
		OLED_Clear();
		OLED_ShowString(0, 0, "HandShake ERR", OLED_6X8);
		OLED_Update();
		Delay_ms(1000);
	} */
	
	currentState = MAIN_IDLE;
}

int main(void)
{
	
	OLED_Init();
	Keyboard_Init();
	ZW101_Init();
	Delay_ms(500);

	ZW101_HandShakeCommand();
	Delay_ms(100);
	switch (ZW101_HandShakeVerify())
	{
		case ZW101_HANDSHAKE_SUCCESS:
			OLED_ShowString(0, 0, "ShakeHand Success", OLED_6X8);
			break;
		case ZW101_HANDSHAKE_FAIL:
			OLED_ShowString(0, 0, "ShakeHand Fail", OLED_6X8);
			break;
		
		default:
			OLED_ShowString(0, 0, "ShakeHand Error", OLED_6X8);
			break;
	}
	OLED_Update();

	while(1)
	{

		
		switch (currentState | Admin_GetState())
		{
		case MAIN_THUMB_VERIFY:
			Thumb_OpenDoor();
			
			break;
		
		default:
			/* OLED_Clear();
			OLED_Update(); */
			break;
		}

		OLED_ClearArea(0, 20, 127, 63);
		OLED_ShowHexNum(0, 20, rxBuffer[0], 2, OLED_8X16);
		OLED_Update();
		

/* 		if(testState == 1)
		{
			OLED_Clear();
			OLED_ShowString(0, 0, "High", OLED_8X16);
			OLED_Update();
			testState = 0;
			Delay_ms(100);
		}
		else
		{
			OLED_Clear();
			OLED_ShowString(0, 0, "Low", OLED_8X16);
			OLED_Update();
			Delay_ms(10);
		} */

		/* OLED_Clear();
		OLED_ShowHexNum(0, 0, GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_15), 2, OLED_8X16);
		OLED_Update(); */
		/* switch (ZW101_VerifyThumb())
		{
		case ZW101_VERIFY_PASS:
			OLED_Clear();
			OLED_ShowString(0, 0, "PASS", OLED_8X16);
			OLED_Update();
			break;
		case ZW101_VERIFY_NOT_PASS:
			OLED_Clear();
			OLED_ShowString(0, 0, "NOT PASS", OLED_8X16);
			OLED_Update();
			break;

		default:
			OLED_Clear();
			OLED_ShowString(0, 0, "ERROR", OLED_8X16);
			OLED_Update();
			break;
		} */
		

		/* OLED_ShowHexNum(0, 0, rxBuffer[8], 2, OLED_6X8);
		OLED_ShowHexNum(15, 0, rxBuffer[9], 2, OLED_6X8);
		OLED_ShowHexNum(30, 0, rxBuffer[10], 2, OLED_6X8);
		OLED_Update();
		Delay_ms(500);  */
	}

}


void USART1_IRQHandler(void)
{
	if(USART_GetITStatus(USART1, USART_IT_RXNE) == SET)
	{
		rxBuffer[rxBufferCount] = USART_ReceiveData(USART1);
		rxBufferCount ++;
		
		if(rxBufferCount == verification.replyLength && zwState == ZW101_VERIFY_STATE)
			FrameComplete();

		if(rxBufferCount == handShake.replyLength && zwState == ZW101_HANDSHAKE_STATE)
			FrameComplete();

		USART_ClearITPendingBit(USART1, USART_IT_RXNE);
	}
}

void EXTI15_10_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line15) == SET)
	{
		currentState = MAIN_THUMB_VERIFY;
		
		EXTI_ClearITPendingBit(EXTI_Line15);
	}
	
}
