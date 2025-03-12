#include "stm32f10x.h"                  // Device header
#include "PWM.h"

void Servo_Init(void)
{
	PWM_Init();
	
}

void Servo_SetAngle(float Angle)
{
	PWM_SetCompare2(Angle / 180 * 2000 + 500);
}

void Servo_OpenDoor(void)
{
	Servo_SetAngle(0);
}

void Servo_CloseDoor(void)
{
	Servo_SetAngle(90)  ;
}