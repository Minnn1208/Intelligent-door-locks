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

extern uint8_t Key[4][4];

void Keyboard_Init(void);
uint8_t Key_Scan(void);

# endif
