#include "Keyboard.h"


struct Key_Struct
{
    GPIO_TypeDef* GPIOx;
    uint16_t GPIO_Pin;
};

/* 由上到下对应C4 C3 C2 C1 */
static struct Key_Struct Key_Column[4] = {
    {GPIOB, GPIO_Pin_11},
    {GPIOB, GPIO_Pin_10},
    {GPIOB, GPIO_Pin_1},
    {GPIOB, GPIO_Pin_0}
};

/* 由上到下对应R1 R2 R3 R4 */
static struct Key_Struct Key_Row[4] = {
    {GPIOA, GPIO_Pin_7},
    {GPIOA, GPIO_Pin_6},
    {GPIOA, GPIO_Pin_5},
    {GPIOA, GPIO_Pin_4}
};

uint8_t Key[4][4] = {0};

void Keyboard_Init(void)
{
    /* 开启时钟 */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

    /* GPIO初始化 */
    GPIO_InitTypeDef GPIO_InitStructure;

    for(uint8_t i = 0; i < 4; i++)
    {
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_InitStructure.GPIO_Pin = Key_Column[i].GPIO_Pin;
        GPIO_Init(Key_Column[i].GPIOx, &GPIO_InitStructure);
    }
    for(uint8_t i = 0; i < 4; i++)
    {
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_InitStructure.GPIO_Pin = Key_Row[i].GPIO_Pin;
        GPIO_Init(Key_Row[i].GPIOx, &GPIO_InitStructure);
    
    }

    for(uint8_t i = 0; i < 4; i++)
    {
        GPIO_SetBits(Key_Row[i].GPIOx, Key_Row[i].GPIO_Pin);
    }

}

uint8_t Key_Scan(void)
{
    uint8_t i, j;
    for(i = 0; i < 4; i++)
    {
        Delay_ms(10);
        GPIO_ResetBits(Key_Row[i].GPIOx, Key_Row[i].GPIO_Pin);
        for(j = 0; j < 4; j++)
        {
            Delay_ms(20);
            if(GPIO_ReadInputDataBit(Key_Column[j].GPIOx, Key_Column[j].GPIO_Pin) == 0)
            {
                Key[i][j] = 1;
            }
            else
            {
                Key[i][j] = 0;
            }
        }
        GPIO_SetBits(Key_Row[i].GPIOx, Key_Row[i].GPIO_Pin);
    }


    if(Mode_GetCurrent(&mainState) == MODE_FINGERPRINT)
    {
        return 99;
    }


    if(KEY_VALUE_1() == 1)
    {
        Mode_Set(&mainState, MODE_PASSWORD);
        return 1;
    }
    else if(KEY_VALUE_2() == 1)
    {
        Mode_Set(&mainState, MODE_PASSWORD);
        return 2;
    }
    else if(KEY_VALUE_3() == 1)
    {
        Mode_Set(&mainState, MODE_PASSWORD);
        return 3;
    }
    else if(KEY_VALUE_10() == 1)
        return 10; 
    else if(KEY_VALUE_4()  == 1)
    {
        Mode_Set(&mainState, MODE_PASSWORD);
        return 4;
    }
    else if(KEY_VALUE_5()  == 1)
    {   
        Mode_Set(&mainState, MODE_PASSWORD);
        return 5;
    }
    else if(KEY_VALUE_6()  == 1)
    {
        Mode_Set(&mainState, MODE_PASSWORD);
        return 6;
    }
    else if(KEY_VALUE_11()  == 1)
        return 11;
    else if(KEY_VALUE_7()  == 1)
    {
        Mode_Set(&mainState, MODE_PASSWORD);
        return 7;
    }
    else if(KEY_VALUE_8()  == 1)
    {
        Mode_Set(&mainState, MODE_PASSWORD);
        return 8;
    }
    else if(KEY_VALUE_9()  == 1)
    {
        Mode_Set(&mainState, MODE_PASSWORD);
        return 9;
    }
    else if(KEY_VALUE_12()  == 1)
        return 12;
    else if(KEY_VALUE_0()  == 1)
    {
        Mode_Set(&mainState, MODE_PASSWORD);
        return 0;
    }
    else if(KEY_VALUE_13()  == 1)
        return 13;
    else if(KEY_VALUE_14()  == 1)
        return 14;
    else if(KEY_VALUE_15()  == 1)
        return 15;
    else
        return 99;

}

