#include "stm32f10x.h"                  // Device header
#include <stdio.h>
#include <stdarg.h>

uint8_t Serial_TxPacket[4];
uint8_t Serial_RxPacket[4];
uint8_t Serial_RxFlag;

void Serial_Init(void)
{
	// 开启USART1和GPIOA的时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	// 初始化GPIO
	GPIO_InitTypeDef GPIO_Initstructure;
	GPIO_Initstructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Initstructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_Initstructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_Initstructure);
	GPIO_Initstructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Initstructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_Initstructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_Initstructure);
	
	// 初始化USART
	USART_InitTypeDef USART_InitStructure;
	USART_InitStructure.USART_BaudRate = 9600;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_Init(USART1, &USART_InitStructure);
	
	// 配置接收中断
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	// NVIC分组
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	// NVIC初始化
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_Init(&NVIC_InitStructure);
	
	
	// 开启USART
	USART_Cmd(USART1, ENABLE);
}

/*
* 发送数据
*/

void Serial_SendByte(uint8_t Byte)
{
	USART_SendData(USART1, Byte);
	while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET); // 不需要将标志位清零，因为发送数据的时候会自动清零
}

/*
* 发送数组
*/
void Serial_SendArray(uint8_t *Array, uint16_t Length)
{
	uint16_t i;
	
	for(i = 0; i < Length; i ++)
	{
		Serial_SendByte(Array[i]);
	}
}

/*
* 发送字符串
*/
void Serial_SendString(char *String)
{
	uint8_t i = 0;
	
	while(String[i] != '\0')
	{
		Serial_SendByte(String[i]);
		i ++;
	}
}

/*
* 指数分辨进位
*/
uint32_t Serial_Pow(uint32_t X, uint32_t Y)
{
	uint32_t Result = 1;
	while(Y --)
	{
		Result *= X;
	}
	return Result;
}

/*
* 发送整数
*/
void Serial_SendNumber(uint32_t Number, uint8_t Length)
{
	uint8_t i;
	for(i = Length; i > 0; i --)
	{
		Serial_SendByte(Number / Serial_Pow(10, i - 1) % 10 + '0');
	}
}

/*
* fputc重定向
* 需要stdio头文件和魔法棒中勾选Use MicroLIB
*/
int fputc(int ch, FILE *f)
{
	Serial_SendByte(ch);
	return ch;
}

/*
* 可变参数Printf
*/
void Serial_Printf(char *format, ...)
{
	char String[100];
	va_list arg;
	va_start(arg, format);
	
	vsprintf(String, format, arg);
	va_end(arg);
	Serial_SendString(String);
}

void Serial_SendPacket(void)
{
	Serial_SendByte(0xFF);
	Serial_SendArray(Serial_TxPacket, 4);
	Serial_SendByte(0xFE);
}

/*
* 接收到数据时返回接收数据的状态
* 自动清除状态
*/
uint8_t Serial_GetRxFlag(void)
{
	if(Serial_RxFlag == 1)
	{
		Serial_RxFlag = 0;
		return 1;
	}
	
	return 0;
}



