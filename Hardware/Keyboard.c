#include "Keyboard.h"



/* 由上到下对应C4 C3 C2 C1 */
extern struct Key_Colum_Struct Key_Column[4] = {
    {GPIOB, GPIO_PortSourceGPIOB, GPIO_Pin_11, GPIO_PinSource11, EXTI_Line11, EXTI15_10_IRQn},
    {GPIOB, GPIO_PortSourceGPIOB, GPIO_Pin_12, GPIO_PinSource12, EXTI_Line12, EXTI15_10_IRQn},
    {GPIOB, GPIO_PortSourceGPIOB, GPIO_Pin_13, GPIO_PinSource13, EXTI_Line13, EXTI15_10_IRQn},
    {GPIOB, GPIO_PortSourceGPIOB, GPIO_Pin_14, GPIO_PinSource14, EXTI_Line14, EXTI15_10_IRQn}
};

/* 由上到下对应R1 R2 R3 R4 */
extern struct Key_Row_Struct Key_Row[4] = {
    {GPIOB, GPIO_Pin_5, 0},
    {GPIOB, GPIO_Pin_6, 1},
    {GPIOB, GPIO_Pin_7, 2},
    {GPIOB, GPIO_Pin_8, 3}
};

uint8_t Key[4][4] = {0};

void Keyboard_Init(void)
{
    /* 开启时钟 */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);

    /* GPIO初始化 */
    GPIO_InitTypeDef GPIO_InitStructure;
    EXTI_InitTypeDef EXTI_InitStructure;
    NVIC_InitTypeDef  NVIC_InitStructure;


    for(uint8_t i = 0; i < 4; i++)
    {

        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_InitStructure.GPIO_Pin = Key_Column[i].GPIO_Pin;
        GPIO_Init(Key_Column[i].GPIOx, &GPIO_InitStructure);
        GPIO_EXTILineConfig(Key_Column[i].GPIO_PortSource, Key_Column[i].GPIO_PinSource);

        EXTI_InitStructure.EXTI_Line = Key_Column[i].EXTI_Line;
        EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
        EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
        EXTI_InitStructure.EXTI_LineCmd = ENABLE;
        EXTI_Init(&EXTI_InitStructure); 

        NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
        NVIC_InitStructure.NVIC_IRQChannel = Key_Column[i].NVIC_IRQChannel;
        NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
        NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
        NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
        NVIC_Init(&NVIC_InitStructure);
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
        GPIO_ResetBits(Key_Row[i].GPIOx, Key_Row[i].GPIO_Pin);
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


    if(Mode_GetCurrent(&currentState) == MODE_FINGERPRINT)
    {
        return 99;
    }


    if(KEY_VALUE_1() == 1)
    {
        Mode_Set(&currentState, MODE_PASSWORD);
        return 1;
    }
    else if(KEY_VALUE_2() == 1)
    {
        Mode_Set(&currentState, MODE_PASSWORD);
        return 2;
    }
    else if(KEY_VALUE_3() == 1)
    {
        Mode_Set(&currentState, MODE_PASSWORD);
        return 3;
    }
    else if(KEY_VALUE_10() == 1)
        return 10; 
    else if(KEY_VALUE_4()  == 1)
    {
        Mode_Set(&currentState, MODE_PASSWORD);
        return 4;
    }
    else if(KEY_VALUE_5()  == 1)
    {   
        Mode_Set(&currentState, MODE_PASSWORD);
        return 5;
    }
    else if(KEY_VALUE_6()  == 1)
    {
        Mode_Set(&currentState, MODE_PASSWORD);
        return 6;
    }
    else if(KEY_VALUE_11()  == 1)
        return 11;
    else if(KEY_VALUE_7()  == 1)
    {
        Mode_Set(&currentState, MODE_PASSWORD);
        return 7;
    }
    else if(KEY_VALUE_8()  == 1)
    {
        Mode_Set(&currentState, MODE_PASSWORD);
        return 8;
    }
    else if(KEY_VALUE_9()  == 1)
    {
        Mode_Set(&currentState, MODE_PASSWORD);
        return 9;
    }
    else if(KEY_VALUE_12()  == 1)
        return 12;
    else if(KEY_VALUE_0()  == 1)
    {
        Mode_Set(&currentState, MODE_PASSWORD);
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

