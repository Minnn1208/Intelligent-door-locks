#include "stm32f10x.h"                  // Device header
#include "OLED.h"
#include "Keyboard.h"
#include "Delay.h"

static uint8_t password[4] = {1, 2, 3, 8};
static uint8_t password_in[4] = {0};

int main(void)
{
	uint8_t i = 0;
	uint8_t keyValue = 0;

	OLED_Init();
	Keyboard_Init();

	OLED_ShowString(0, 0, "Current Password:", OLED_8X16);
	OLED_Update();

	while(1)
	{
		keyValue = Key_Scan();

		if(keyValue == 99)
			continue;

		if(keyValue == 10)
		{
			OLED_ClearArea(0, 20, 127, 63);
			OLED_Update();
			if(password_in[0] == password[0] && password_in[1] == password[1] && password_in[2] == password[2] 
				&& password_in[3] == password[3])
			{
				OLED_ShowString(0, 30, "Password Correct", OLED_8X16);
			}
			else
			{
				OLED_ShowString(0, 30, "Password Error", OLED_8X16);
			}
			OLED_Update();
			i = 0;
			Delay_ms(500);
			OLED_ClearArea(0, 20, 127, 63);
			OLED_Update();
			continue;
		}

		password_in[i] = keyValue;
		OLED_ShowNum(30* i, 20, password_in[i], 1, OLED_8X16);
		OLED_Update();
		if(i != 3)
			i ++;
		else
			i = 0;
		
		
	}

}
