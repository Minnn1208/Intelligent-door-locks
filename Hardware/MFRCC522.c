#include "stm32f10x.h"
#include "MySPI.h" 
#include "MFRCC522_Reg.h"

void MFRC522_Init(void) 
{
    MySPI_Init();

}

void MFRC522_Write(uint8_t address, uint8_t value)
{
    MySPI_Start();
    MySPI_WriteByte((address << 1) & 0x7E);         // 指定地址
    MySPI_WriteByte(value);                         // 写入数据
    MySPI_Stop();
}

uint8_t MFRC522_Read(uint8_t address)
{
    uint8_t byteReceive;
    MySPI_Start();
    MySPI_WriteByte((address << 1) & 0x7E | 0x80);   // 指定地址
    byteReceive = MySPI_ReadByte();                 // 读取数据
    MySPI_Stop();

    return byteReceive;
}

void SetBitMask(uint8_t reg, uint8_t mask)
{
    uint8_t temp = 0x00;
    temp = MFRC522_Read(reg);
    MFRC522_Write(reg, temp | mask);
}

void ClearBitMask(uint8_t reg,unsigned char mask)  
{
    uint8_t tmp = 0x0;
    tmp = MFRC522_Read(reg);
    MFRC522_Write(reg, tmp & ~mask);
} 

uint8_t MFRC522_ToCard(uint8_t command, uint8_t *inData, uint8_t inLenByte,uint8_t *outData, uint16_t  *outLenBit)
{
    uint8_t irqEn = 0x00; // 中断使能
    uint8_t waitFor = 0x00; // 等待的中断标志
    uint8_t lastBits; // 最后一个字节的剩余位数
    uint8_t temp; // 临时变量

    switch(command)
    {
        case PCD_AUTHENT:
        irqEn = 0x12; // 认证命令的中断使能
        waitFor = 0x10; // 等待的中断标志
        break;
        case PCD_TRANSCEIVE:
            irqEn = 0x77; // 通信命令的中断使能
            waitFor = 0x30; // 等待的中断标志
            break;
        default:
            break;
    }
    
}

uint8_t MFRC522_Request(uint8_t reqCode, uint8_t * tagType)
{   

}

