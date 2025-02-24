#include "stm32f10x.h"                  // Device header
#include "Mode.h"
#include "OLED.h"
#include "Password.h"
#include "Delay.h"
#include "ZW101.h"



uint8_t keyValue = 0x00;


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
	Delay_ms(1000);
	
	Mode_Set(&mainState, MODE_IDLE);
}


int main(void)
{
	Mode_Init(&mainState);
	OLED_Init();
	Keyboard_Init();
	ZW101_Init();
	Delay_ms(80);

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

		keyValue = Key_Scan();
		/* keyValue = Key_Scan();
		OLED_Clear();
		OLED_ShowNum(30, 30, keyValue, 2, OLED_8X16);
		OLED_Update(); */
		
		switch (mainState.currentMode)
		{
		case MODE_FINGERPRINT:
			Thumb_OpenDoor();
			break;
		case MODE_PASSWORD:
			Password_Component(keyValue);
			break;
			
		default:
			OLED_Clear();
			OLED_Update();
			break;
		}

		switch (Mode_GetCurrent(&mainState))
		{
		case MODE_IDLE:
			OLED_ClearArea(0, 50, 127, 63);
			OLED_ShowString(0, 50, "IDLE", OLED_6X8);
			OLED_Update();
			break;
		case MODE_PASSWORD:
			OLED_ClearArea(0, 50, 127, 63);
			OLED_ShowString(0, 50, "PASSWORD", OLED_6X8);
			OLED_Update();
			break;
		case MODE_FINGERPRINT:
			OLED_ClearArea(0, 50, 127, 63);
			OLED_ShowString(0, 50, "FINGERPRINT", OLED_6X8);
			OLED_Update();
			break;
		default:
			break;
		}
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
		if(Mode_GetCurrent(&mainState) == MODE_IDLE)
		{Mode_Set(&mainState, MODE_FINGERPRINT);}

		EXTI_ClearITPendingBit(EXTI_Line15);
	}
	
}
