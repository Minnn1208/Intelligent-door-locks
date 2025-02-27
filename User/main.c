#include "stm32f10x.h"                  // Device header
#include "Mode.h"
#include "OLED.h"
#include "Password.h"
#include "Delay.h"
#include "ZW101.h"



uint8_t keyValue = 99;
uint8_t colAndRow = 99;

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
	
	Mode_Set(&currentState, MODE_IDLE);
}


void KeyValueRemap(uint8_t colAndRow, uint8_t *keyValue)
{
	switch (colAndRow)
	{
	case 1:
		*keyValue = 0;							// 数字0
		Mode_Set(&currentState, MODE_PASSWORD);
		break;
	case 2:
	case 3:
	case 4:
		*keyValue = colAndRow + 11;				// 键值13-15
		break;
	case 5:
	case 6:
	case 7:
		*keyValue = colAndRow + 2;				// 数字7-9
		Mode_Set(&currentState, MODE_PASSWORD);
		break;
	case 8:
		*keyValue = colAndRow + 4;				// 键值12
		break;
	case 9:
	case 10:
	case 11:
		*keyValue = colAndRow - 5;				// 数字4-6
		Mode_Set(&currentState, MODE_PASSWORD);
		break;
	case 12:
		*keyValue = colAndRow - 1;				// 键值12
		break;
	case 13:
	case 14:
	case 15:
		*keyValue = colAndRow - 12;				// 数字1-3
		Mode_Set(&currentState, MODE_PASSWORD);
		break;
	case 16:
		*keyValue = colAndRow - 6;				// 键值16
		break;
	default:
		*keyValue = 99;
		break;
	}
}


int main(void)
{
	Mode_Init(&currentState);
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
