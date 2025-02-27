# ifndef __KEYBOARD_H
# define __KEYBOARD_H

#include "stm32f10x.h"                  // Device header
#include "Mode.h"
#include <stdint.h>
#include "Delay.h"


#define KEY_VALUE_1()               (Key[3][0])
#define KEY_VALUE_2()               (Key[3][1])
#define KEY_VALUE_3()               (Key[3][2])
#define KEY_VALUE_4()               (Key[2][0])
#define KEY_VALUE_5()               (Key[2][1])
#define KEY_VALUE_6()               (Key[2][2])
#define KEY_VALUE_7()               (Key[1][0])
#define KEY_VALUE_8()               (Key[1][1])
#define KEY_VALUE_9()               (Key[1][2])
#define KEY_VALUE_10()              (Key[3][3])
#define KEY_VALUE_11()              (Key[2][3])
#define KEY_VALUE_12()              (Key[1][3])
#define KEY_VALUE_13()              (Key[0][1])
#define KEY_VALUE_14()              (Key[0][2])
#define KEY_VALUE_15()              (Key[0][3])
#define KEY_VALUE_0()               (Key[0][0])


/* 
#define KET_COL1_GPIOX           GPIOB
#define KET_COL1_GPIO_PORT       GPIO_PortSourceGPIOB
#define KET_COL1_GPIO_PIN        GPIO_Pin_11
#define KET_COL1_PIN_SOC         GPIO_PinSource11
#define KET_COL1_EXTI_LINE       EXTI_Line11
#define KET_COL1_IRQN            EXTI15_10_IRQn
#define KET_COL2_GPIOX           GPIOB
#define KET_COL2_GPIO_PORT       GPIO_PortSourceGPIOB
#define KET_COL2_GPIO_PIN        GPIO_Pin_12
#define KET_COL2_PIN_SOC         GPIO_PinSource12
#define KET_COL2_EXTI_LINE       EXTI_Line12
#define KET_COL2_IRQN            EXTI15_10_IRQn
#define KET_COL3_GPIOX           GPIOB
#define KET_COL3_GPIO_PORT       GPIO_PortSourceGPIOB
#define KET_COL3_GPIO_PIN        GPIO_Pin_13
#define KET_COL3_PIN_SOC         GPIO_PinSource13
#define KET_COL3_EXTI_LINE       EXTI_Line13
#define KET_COL3_IRQN            EXTI15_10_IRQn
#define KET_COL4_GPIOX           GPIOB
#define KET_COL4_GPIO_PORT       GPIO_PortSourceGPIOB
#define KET_COL4_GPIO_PIN        GPIO_Pin_14
#define KET_COL4_PIN_SOC         GPIO_PinSource14
#define KET_COL4_EXTI_LINE       EXTI_Line14
#define KET_COL4_IRQN            EXTI15_10_IRQn

#define KET_COL1_GPIOX           GPIOB
#define KET_COL1_GPIO_PORT       GPIO_PortSourceGPIOB
#define KET_COL1_GPIO_PIN        GPIO_Pin_11
#define KET_COL1_PIN_SOC         GPIO_PinSource11
#define KET_COL1_EXTI_LINE       EXTI_Line11
#define KET_COL1_IRQN            EXTI15_10_IRQn
#define KET_COL2_GPIOX           GPIOB
#define KET_COL2_GPIO_PORT       GPIO_PortSourceGPIOB
#define KET_COL2_GPIO_PIN        GPIO_Pin_12
#define KET_COL2_PIN_SOC         GPIO_PinSource12
#define KET_COL2_EXTI_LINE       EXTI_Line12
#define KET_COL2_IRQN            EXTI15_10_IRQn
#define KET_COL3_GPIOX           GPIOB
#define KET_COL3_GPIO_PORT       GPIO_PortSourceGPIOB
#define KET_COL3_GPIO_PIN        GPIO_Pin_13
#define KET_COL3_PIN_SOC         GPIO_PinSource13
#define KET_COL3_EXTI_LINE       EXTI_Line13
#define KET_COL3_IRQN            EXTI15_10_IRQn
#define KET_COL4_GPIOX           GPIOB
#define KET_COL4_GPIO_PORT       GPIO_PortSourceGPIOB
#define KET_COL4_GPIO_PIN        GPIO_Pin_14
#define KET_COL4_PIN_SOC         GPIO_PinSource14
#define KET_COL4_EXTI_LINE       EXTI_Line14
#define KET_COL4_IRQN            EXTI15_10_IRQn
 */
struct Key_Colum_Struct
{
    GPIO_TypeDef* GPIOx;
    uint8_t GPIO_PortSource;
    uint16_t GPIO_Pin;
    uint8_t GPIO_PinSource;
    uint16_t EXTI_Line;
    uint8_t NVIC_IRQChannel;
};

struct Key_Row_Struct
{
    GPIO_TypeDef* GPIOx;
    uint16_t GPIO_Pin;
    uint8_t colORrow;
};

extern uint8_t Key[4][4];
extern struct Key_Colum_Struct Key_Column[4];
extern struct Key_Row_Struct Key_Row[4];
void Keyboard_Init(void);
uint8_t Key_Scan(void);

# endif
