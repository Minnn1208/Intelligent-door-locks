#include "stm32f10x.h"                  // Device header
#include "Mode.h"
#include "OLED.h"
#include "Password.h"
#include "Delay.h"
#include "ZW101.h"
#include "Servo.h"


uint8_t keyValue = 99;
uint8_t colAndRow = 99;


int main(void)
{
	
	Mode_Init(&currentState);
	OLED_Init();
	Keyboard_Init();
	ZW101_Init();
	Servo_Init();
	Servo_SetAngle(90);
	Delay_ms(80);
	
	
	
	
	ZW101_HandShakeCommand();
	/* Delay_ms(100); */
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
		switch (currentState.currentMode)
		{
		case MODE_FINGERPRINT:
			Thumb_OpenDoor();
			break;
		case MODE_PASSWORD:
			Password_Component(&keyValue);
			break;
			
		default:
			OLED_Clear();
			OLED_Update();
			break;
		}

		switch (Mode_GetCurrent(&currentState))
		{
		case MODE_IDLE:
			OLED_ClearArea(0, 50, 127, 63);
			OLED_ShowString(0, 50, "IDLE", OLED_6X8);
			OLED_Update();
			Delay_ms(20);
			break;
		case MODE_PASSWORD:
			OLED_ClearArea(0, 50, 127, 63);
			OLED_ShowString(0, 50, "PASSWORD", OLED_6X8);
			OLED_Update();
			Delay_ms(20);
			break;
		case MODE_FINGERPRINT:
			OLED_ClearArea(0, 50, 127, 63);
			OLED_ShowString(0, 50, "FINGERPRINT", OLED_6X8);
			OLED_Update();
			Delay_ms(20);
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
		if(Mode_GetCurrent(&currentState) == MODE_IDLE)
		{Mode_Set(&currentState, MODE_FINGERPRINT);}

		EXTI_ClearITPendingBit(EXTI_Line15);
		return;
	}

	if(EXTI_GetITStatus(EXTI_Line14) == SET)
	{
		delay(1000);
		if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_14) == Bit_RESET)
		{
			for(uint8_t row = 0; row < 4; row ++)
			{
				GPIO_SetBits(GPIOB, Key_Row[row].GPIO_Pin);
				if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_14) == Bit_SET)
				{ 
					colAndRow = 4 * row + 4;
				}
				GPIO_ResetBits(GPIOB, Key_Row[row].GPIO_Pin);
			}
		}

		EXTI_ClearITPendingBit(EXTI_Line14);
	} 
	
	if(EXTI_GetITStatus(EXTI_Line13) == SET)
	{
		delay(1000);
		if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_13) == Bit_RESET)
		{
			for(uint8_t row = 0; row < 4; row ++)
			{
				GPIO_SetBits(GPIOB, Key_Row[row].GPIO_Pin);
				if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_13) == Bit_SET)
				{ 
					colAndRow = 4 * row + 3;
				}
				GPIO_ResetBits(GPIOB, Key_Row[row].GPIO_Pin);
			}
		}

		EXTI_ClearITPendingBit(EXTI_Line13);
	}

	if(EXTI_GetITStatus(EXTI_Line12) == SET)
	{
		delay(1000);
		if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_12) == Bit_RESET)
		{
			for(uint8_t row = 0; row < 4; row ++)
			{
				GPIO_SetBits(GPIOB, Key_Row[row].GPIO_Pin);
				if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_12) == Bit_SET)
				{ 
					colAndRow = 4 * row + 2;
				}
				GPIO_ResetBits(GPIOB, Key_Row[row].GPIO_Pin);
			}
		}

		EXTI_ClearITPendingBit(EXTI_Line12);
	}
	
	if(EXTI_GetITStatus(EXTI_Line11) == SET)
	{
		delay(1000);
		if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_11) == Bit_RESET)
		{
			for(uint8_t row = 0; row < 4; row ++)
			{
				GPIO_SetBits(GPIOB, Key_Row[row].GPIO_Pin);
				if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_11) == Bit_SET)
				{ 
					colAndRow = 4 * row + 1;
				}
				GPIO_ResetBits(GPIOB, Key_Row[row].GPIO_Pin);
			}
		}

		EXTI_ClearITPendingBit(EXTI_Line11);
	} 

	KeyValueRemap(colAndRow, &keyValue);
}
