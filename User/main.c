#include "stm32f10x.h"                  // Device header
#include "OLED.h"
#include "Password.h"
#include "Delay.h"
#include "ZW101.h"

static uint8_t RxData = 0;
uint8_t keyValue = 0x00;
uint8_t count = 0;

int main(void)
{
	
	OLED_Init();
	Keyboard_Init();
	ZW101_Init();
	

	ZW101_VerifyThumbCommand();

	

	while(1)
	{
		/* OLED_ShowHexNum(0, 0, rxBuffer[9], 2, OLED_8X16);
		OLED_ShowHexNum(0, 20, rxBuffer[26], 2, OLED_8X16);
		OLED_ShowHexNum(0, 40, rxBuffer[43], 2, OLED_8X16);
		OLED_Update(); */
		switch (ZW101_VerifyThumb())
		{
		case ZW101_VERIFY_PASS:
			OLED_Clear();
			OLED_Update();
			OLED_ShowString(0, 0, "PASS", OLED_8X16);
			OLED_Update();
			break;
		case ZW101_VERIFY_NOT_PASS:
			OLED_Clear();
			OLED_Update();
			OLED_ShowString(0, 0, "NOT PASS", OLED_8X16);
			OLED_Update();
			break;

		default:
			OLED_ShowString(0, 0, "ERROR", OLED_8X16);
			OLED_Update();
			break;
		}
		
		Delay_ms(500);
	}

}


void USART1_IRQHandler(void)
{
	if(USART_GetITStatus(USART1, USART_IT_RXNE) == SET)
	{
		rxBuffer[count] = USART_ReceiveData(USART1);
		count ++;

		if(count == 51 && zwState == ZW101_VERIFY_STATE)
		{
			FrameComplete();
		}

		USART_ClearITPendingBit(USART1, USART_IT_RXNE);
	}
}
