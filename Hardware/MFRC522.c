#include "stm32f10x.h"
#include "MySPI.h" 
#include "MFRC522_Reg.h"

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

/// @brief 
/// @param command 
/// @param inData 
/// @param inLenByte 
/// @param outData 
/// @param outLenBit 
/// @return 
uint8_t MFRC522_ToCard(uint8_t command, uint8_t *inData, uint8_t inLenByte,uint8_t *outData, uint16_t  *outLenBit)
{
    uint8_t status = MI_ERR;
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

    // 将输入数据写入FIFO数据寄存器
    for (uint8_t i = 0; i < inLenByte; i++)
        MFRC522_Write(FIFODataReg, inData[i]);
    MFRC522_Write(CommandReg, command); // 发送命令

    // 如果是通信命令，设置BitFraming寄存器
    if(command == PCD_TRANSCEIVE)
        SetBitMask(BitFramingReg, 0x80);
    
    // 读取中断寄存器，并等待中断信号
    temp = MFRC522_Read(ComIrqReg);
    uint8_t i = 1500; // 超时计数器

    do
    {
        temp = MFRC522_Read(ComIrqReg); // 读取中断寄存器
        i --; // 递减计数器

    } while ((i != 0) && !(temp & 0x01) && !(temp & waitFor)); // 等待中断信号或超时
    ClearBitMask(BitFramingReg, 0x80); // 清除BitFraming寄存器设置

    // 如果未超时
    if(i != 0)
    {
        status = MI_ERR; // 初始化状态为错误

        // 如果错误寄存器中没有错误标志
        if(!(MFRC522_Read(ErrorReg) & 0x1B))
        {
            status = MI_OK; // 设置状态为成功

            // 如果中断标志为0x01，表示无标签错误
            if(temp & irqEn & 0x01)
                status = MI_NOTAGERR;

            // 如果是通信命令
            if(command == PCD_TRANSCEIVE)
            {
                temp = MFRC522_Read(FIFOLevelReg); // 读取FIFO级别寄存器
                lastBits = MFRC522_Read(ControlReg) & 0x07; // 读取控制寄存器，获取最后一位的位数

                // 计算接收数据的比特长度
                if(lastBits)
                    *outLenBit = (temp - 1) * 8 + lastBits;
                else
                    *outLenBit = temp * 8;

                // 如果FIFO级别为0，设置为1
                if(temp == 0)
                    temp = 1;
                // 如果FIFO级别超过最大长度，设置为最大长度
                if(temp > MAXRLEN)
                    temp = MAXRLEN;

                // 读取FIFO数据寄存器中的数据
                for(i = 0; i < temp; i ++)
                    outData[i] = MFRC522_Read(FIFODataReg);
            }

        }
    }

    SetBitMask(ControlReg, 0x80); // 取消BitFramingReg的设置
    MFRC522_Write(CommandReg,PCD_IDLE); // 设置命令寄存器为IDLE模式

    return status;
    
}

uint8_t MFRC522_Request(uint8_t reqCode, uint8_t * tagType)
{   
    uint8_t status = 0x00;
    uint16_t length;
    uint8_t buf[MAXRLEN] = {0x00};

    ClearBitMask(Status2Reg, 0x08);
    MFRC522_Write(BitFramingReg, 0x07);
    SetBitMask(TxControlReg, 0x03);

    buf[0] = reqCode;
    status = MFRC522_ToCard(PCD_TRANSCEIVE, buf, 1, buf, &length);

    if (status == MI_OK && length == 0x10)
    {
        *tagType = buf[0];
        *(tagType + 1) = buf[1];
    }
    else
    {
        status = MI_ERR;
    }

    return status;
}


uint8_t MFRC522_Reset(void)
{
    MFRC522_RST_H;
	Delay_us (1);             
    MFRC522_RST_L;
	Delay_us (1);                         
    MFRC522_RST_H;
	Delay_us (1);
    
    MFRC522_Write(CommandReg, 0x0F);
    while (MFRC522_Read(CommandReg) & 0x10);
    Delay_us (1);

    MFRC522_Write(ModeReg, 0x3D);
    MFRC522_Write(TReloadRegL, 30);
    MFRC522_Write(TReloadRegH, 0);
    MFRC522_Write(TModeReg, 0x8D);
    MFRC522_Write(TPrescalerReg, 0x3E);
    MFRC522_Write(TxAutoReg, 0x40);
    
    return MI_OK;
}

void MFRC522_AntennaOn(void)
{
    uint8_t temp;
    temp = MFRC522_Read(TxControlReg);
    if (!(temp & 0x03))
    {
        SetBitMask(TxControlReg, 0x03);
    }
}

void MFRC522_AntennaOff(void)
{
    ClearBitMask(TxControlReg, 0x03);
}


uint8_t MFRC522_Anticoll(uint8_t *pSnr)
{
    uint8_t status;
    uint8_t i,snr_check=0;
    uint16_t  unLen;
    uint8_t buffer[MAXRLEN]; 
    

    ClearBitMask(Status2Reg,0x08);
    MFRC522_Write(BitFramingReg,0x00);
    ClearBitMask(CollReg,0x80);
 
    buffer[0] = PICC_ANTICOLL1;
    buffer[1] = 0x20;

    status = MFRC522_ToCard(PCD_TRANSCEIVE, buffer, 2, buffer,&unLen);

    if (status == MI_OK)
    {
    	 for (i=0; i<4; i++)
         {   
             *(pSnr + i)  = buffer[i];
             snr_check ^= buffer[i];
         }
         if (snr_check != buffer[i])
            status = MI_ERR;
    }
    
    SetBitMask(CollReg, 0x80);
    return status;
}
