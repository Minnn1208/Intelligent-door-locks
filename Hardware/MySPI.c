#include "stm32f10x.h"                  // Device header
#include "Delay.h"

void MySPI_W_SS(uint8_t BitValue)
{
	GPIO_WriteBit(GPIOA, GPIO_Pin_4, (BitAction)BitValue);
}

void MySPI_W_SCK(uint8_t BitValue)
{
	GPIO_WriteBit(GPIOA, GPIO_Pin_5, (BitAction)BitValue);
}

void MySPI_W_MOSI(uint8_t BitValue)
{
	GPIO_WriteBit(GPIOA, GPIO_Pin_7, (BitAction)BitValue);
}


uint8_t MySPI_R_MISO(void)
{
	return GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_6);
}

void MySPI_Init(void)
{
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	GPIO_InitTypeDef GPIO_Initstructure;
	GPIO_Initstructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Initstructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_7;
	GPIO_Initstructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_Initstructure);
	
	GPIO_Initstructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Initstructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_Initstructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_Initstructure);
	
	MySPI_W_SS(1);
	MySPI_W_SCK(0);

}

void MySPI_Start(void)
{
	MySPI_W_SS(0);
}

void MySPI_Stop(void)
{
	MySPI_W_SS(1);
}

/*
// 此方法从阅读性上来讲更好一点
uint8_t MySPI_SwapByte(uint8_t ByteSend)
{
	uint8_t ByteReceive = 0x00;
	uint8_t i = 0;
	
	for(i = 0; i < 8; i ++)
	{
		MySPI_W_MOSI(ByteSend & (0x80 >> i));
		MySPI_W_SCK(1);
		if(MySPI_R_MISO() == 1)
			ByteReceive |= (0x80 >> i);
		MySPI_W_SCK(0);
	}
	
	return ByteReceive;
}
*/

void MySPI_WriteByte(uint8_t byteSend)
{
	for (uint8_t i = 0; i < 8; i++)
	{
		MySPI_W_SCK(1);
		MySPI_W_MOSI(byteSend & 0x80);
		byteSend <<= 1;
		Delay_us(5);
		MySPI_W_SCK(0);
		Delay_us(5);
	}
	
}

uint8_t MySPI_ReadByte(void)
{
	uint8_t byteReceive = 0x00;
	for (uint8_t i = 0; i < 8; i++)
	{
		MySPI_W_SCK(1);
		if(MySPI_R_MISO() == 1)
			byteReceive |= (0x80 >> i);
		Delay_us(5);
		MySPI_W_SCK(0);
		Delay_us(5);
	}
	return byteReceive;
	
}

uint8_t MySPI_SwapByte(uint8_t ByteSend)
{
	uint8_t i = 0;
	
	for(i = 0; i < 8; i ++)
	{
		MySPI_W_MOSI(ByteSend & 0x80);
		ByteSend <<= 1;
		MySPI_W_SCK(1);
		if(MySPI_R_MISO() == 1)
			ByteSend |= 0x01;
		MySPI_W_SCK(0);
	}
	
	return ByteSend;
}

