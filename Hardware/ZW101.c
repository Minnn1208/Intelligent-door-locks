#include "stm32f10x.h"
#include "ZW101.h"
#include <string.h>

// 初始化外部变量
uint8_t zwState = 0;
uint8_t rxBuffer[RX_BUFFER_SIZE];
uint8_t frameComplete = 0;

ZW101Command verification = {
    /* 验证指纹指令 EF 01 FF FF FF FF 01 00 08 32 FF FF FF FF 03 04 3A */
    .commandArrary = {0xEF, 0x01, 0xFF, 0xFF, 0xFF, 0xFF, 0x01, 0x00, 0x08, 0x32, 0xFF, 0xFF, 0xFF, 0xFF, 0x03, 0x04, 0x3A},
    .length = 17,
    .replyLength = 0
};

ZW101Command baseInformation = {
    .commandArrary = {0xEF, 0x01, 0xFF, 0xFF, 0xFF, 0xFF, 0x01, 0x00, 0x03, 0x0F, 0x00, 0x13},
    .length = 12,
    .replyLength = 0
};

ZW101Command thumbPrint = {
    .commandArrary = {0xEF, 0x01, 0xFF, 0xFF, 0xFF, 0xFF, 0x01, 0x00, 0x08, 0x31, 0x00, 0x01, 0x03, 0x00, 0x09, 0x00, 0x47},
    .length = 17,
    .replyLength = 0
};


/// @brief ZW101初始化
/// @param none  
void ZW101_Init(void)
{
    Serial_Init();
    
}

/// @brief 输入指纹指令
/// @param  none
void ZW101_ThumbPrintCommand(void)
{
    zwState = ZW101_THUMB_PRINT;
    Serial_SendArray(thumbPrint.commandArrary, thumbPrint.length);
}


/// @brief 验证指纹指令 
/// @param  none
void ZW101_VerifyThumbCommand(void)
{
    zwState =  ZW101_VERIFY_STATE;
    Serial_SendArray(verification.commandArrary, verification.length);
}


/// @brief 指纹验证 
/// @param  none
/// @return 指纹是否验证成功
uint8_t ZW101_VerifyThumb(void)
{
    if(GetFrameComplete() != 1)
        return ZW101_VERIFY_ERROR;

    if(rxBuffer[9] != 0x00 || rxBuffer[26] != 0x00 || rxBuffer[43] != 0x00)     
        return ZW101_VERIFY_NOT_PASS;

    Clear_Buffer();
    FrameNotComplete();
    
    return ZW101_VERIFY_PASS;
}


/// @brief 获取基本信息
/// @param  none
void ZW101_BaseInformationCommand(void)
{
    Serial_SendArray(baseInformation.commandArrary, baseInformation.length);
}


void FrameComplete(void)
{
    frameComplete = 1;
}

void FrameNotComplete(void)
{
    frameComplete = 0;
}

uint8_t GetFrameComplete(void)
{
    return frameComplete;
}

void Clear_Buffer(void)
{
    memset(rxBuffer, 0, sizeof(rxBuffer));
}






