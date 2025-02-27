#include "Password.h"
#include "stm32f10x.h"
#include "OLED.h"
#include "Delay.h"


uint8_t Password_Input(uint8_t keyValue)
{
    if(passwordIndex >= PASSWORD_LEN)
        return PASSWORD_ENOUGH;

    password_in[passwordIndex] = keyValue;
	
	if(passwordIndex <= 3)
		passwordIndex ++;
    
    return PASSWORD_NOT_ENOUGH;
}

uint8_t Password_Verify(void)
{
    if(passwordIndex < PASSWORD_LEN)
        return PASSWORD_LESS;

    if(password_in[0] == password[0] && password_in[1] == password[1])
    {
        if(password_in[2] == password[2] && password_in[3] == password[3])
            return PASSWORD_OK;
    }

    return PASSWORD_ERR;
}

uint8_t Password_Delete(void)
{
    if (passwordIndex <= 0)
        return PASSWORD_DEL_FAIL;
    
    password_in[passwordIndex - 1] = 0;
    passwordIndex --;

    return PASSWORD_DEL_SUSS;
}


uint8_t Password_Component(uint8_t *keyValue)
{
    uint8_t status = PASSWORD_DUMMY;

    

    // 无符号数不可能小于0
    if(*keyValue <= 9)
    {
        status = Password_Input(*keyValue);
        if(status == PASSWORD_ENOUGH)
        {
            OLED_ClearArea(0, 20, 127, 63);
            // 可能有问题
            OLED_ShowString(0, 40, "Password Enough", OLED_6X8);
            OLED_Update();
            Delay_ms(500);
           
            OLED_ClearArea(0, 20, 127, 63);
            for (uint8_t i = 0; i < PASSWORD_LEN; i++)
            {
                OLED_ShowNum(30* i, 20, password_in[i], 1, OLED_8X16);
            }
            OLED_Update();
            
        }
        else if(status == PASSWORD_NOT_ENOUGH)
        {
            OLED_ShowNum(30 * (passwordIndex - 1), 20, password_in[passwordIndex - 1], 1, OLED_8X16);
	        OLED_Update();
        }

        
        status = PASSWORD_DUMMY;
    }

    /* 确认密码 */
    if(*keyValue == 10)
    {
        status = Password_Verify();
        if(status == PASSWORD_OK)
        {
            OLED_ClearArea(0, 0, 127, 63);
            OLED_ShowString(0, 20, "Password Correct", OLED_8X16);
            OLED_Update();
            Delay_ms(500);
        }
        else if(status == PASSWORD_ERR)
        {
            OLED_ClearArea(0, 20, 127, 63);
            OLED_ShowString(0, 20, "Password ERROR", OLED_8X16);
            OLED_Update();
            Delay_ms(500);
        }

        for (uint8_t i = 0; i < PASSWORD_LEN; i++)
        {
            password_in[i] = 0;
        }
        passwordIndex = 0;

        OLED_ClearArea(0, 20, 127, 63);
        OLED_Update();

        Mode_Set(&currentState, MODE_IDLE);
        status = PASSWORD_DUMMY;
    }

    /* 删除最后一位密码 */
    if(*keyValue == 13)
    {
        status = Password_Delete();
        if(status == PASSWORD_DEL_SUSS)
        {
            //TODO： 这里清除的像素坐标需要调整
            OLED_ClearArea(30 * (passwordIndex), 20, 8, 20);
            OLED_Update();
        }
        else if(status == PASSWORD_DEL_FAIL)
        {
            OLED_ClearArea(0, 20, 127, 63);
            OLED_ShowString(0, 20, "No Password", OLED_8X16);
            OLED_Update();
            
            uint8_t len = passwordIndex + 1 > 4 ? PASSWORD_LEN : passwordIndex + 1;

            for (uint8_t i = 0; i < len; i++)
            {
                password_in[i] = 0;
            }

            Delay_ms(500);
            OLED_ClearArea(0, 20, 127, 63);
            OLED_Update();

            Mode_Set(&currentState, MODE_IDLE);
        }
    }

    /* 重置键值 */
    *keyValue = 99;

    return status;
}

