
#include "ZW101.h"


// 初始化外部变量
uint8_t zwState = 0;
uint8_t rxBuffer[RX_BUFFER_SIZE] = {255};     // 第一位表示接收是否完成，后续才是数据帧
uint8_t rxBufferCount = 0;
uint8_t temp = 0x00;



ZW101Command handShake = {
    .commandArray = {0xEF, 0x01, 0xFF, 0xFF, 0xFF, 0xFF, 0x01, 0x00, 0x03, 0x35, 0x00, 0x39},
    .length = 12,
    .replyLength = 12
};

ZW101Command verification = {
    /* 验证指纹指令 EF 01 FF FF FF FF 01 00 08 32 FF FF FF FF 03 04 3A */
    .commandArray = {0xEF, 0x01, 0xFF, 0xFF, 0xFF, 0xFF, 0x01, 0x00, 0x08, 0x32, 0xFF, 0xFF, 0xFF, 0xFF, 0x03, 0x04, 0x3A},
    .length = 17,
    .replyLength = 52
};

ZW101Command baseInformation = {
    .commandArray = {0xEF, 0x01, 0xFF, 0xFF, 0xFF, 0xFF, 0x01, 0x00, 0x03, 0x0F, 0x00, 0x13},
    .length = 12,
    .replyLength = 0
};

ZW101Command thumbPrint = {
    .commandArray = {0xEF, 0x01, 0xFF, 0xFF, 0xFF, 0xFF, 0x01, 0x00, 0x08, 0x31, 0x00, 0x01, 0x03, 0x00, 0x09, 0x00, 0x47},
    .length = 17,
    .replyLength = 0
};


/// @brief ZW101初始化
/// @param none  
void ZW101_Init(void)
{
    Serial_Init();
    Clear_Buffer();

    /* 配置触摸电平中断 */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);

    GPIO_InitTypeDef GPIO_Initstructure;
	GPIO_Initstructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Initstructure.GPIO_Pin = GPIO_Pin_15;
	GPIO_Initstructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_Initstructure);
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_Pin_15);

    EXTI_InitTypeDef EXTI_InitStructure;
    EXTI_InitStructure.EXTI_Line = EXTI_Line15;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);

    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
    NVIC_InitTypeDef  NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);


    zwState = ZW101_NONE_STATE;
    
}

/// @brief ZW101握手指令
/// @param  none
void ZW101_HandShakeCommand(void)
{
    zwState = ZW101_HANDSHAKE_STATE;
    Serial_SendArray(handShake.commandArray, handShake.length);
}


/// @brief ZW101握手验证
/// @param  none
/// @return 握手是否成功
uint8_t ZW101_HandShakeVerify(void)
{
    temp = 0x00;
    if(GetFrameComplete() != 1)
        temp =  ZW101_HANDSHAKE_ERROR;
    
    if(rxBuffer[10] != 0x00)
        temp = ZW101_HANDSHAKE_FAIL;
    else
        temp = ZW101_HANDSHAKE_SUCCESS;
    /* 验证应答包无论成功与否
    都要设置数据帧接收位为0、清空状态和清除缓存区
    为下一次的接收和状态做准备 */
    
    FrameNotComplete();
    zwState = ZW101_NONE_STATE;
    Clear_Buffer();
    Delay_ms(10);

    return temp;
}

/// @brief 输入指纹指令
/// @param  none
void ZW101_ThumbPrintCommand(void)
{
    zwState = ZW101_THUMB_PRINT;
    Serial_SendArray(thumbPrint.commandArray, thumbPrint.length);
}


/// @brief 验证指纹指令 
/// @param  none
void ZW101_VerifyThumbCommand(void)
{
    zwState =  ZW101_VERIFY_STATE;
    Serial_SendArray(verification.commandArray, verification.length);
    Delay_ms(800);
}


/// @brief 指纹验证 
/// @param  none
/// @return 指纹是否验证成功
uint8_t ZW101_VerifyThumb(void)
{
    temp = 0x00;
    if(GetFrameComplete() != 0x01)
        temp =  ZW101_VERIFY_ERROR;

    if(rxBuffer[10] != 0x00 || rxBuffer[27] != 0x00 || rxBuffer[44] != 0x00)     
        temp =  ZW101_VERIFY_NOT_PASS;
    else
        temp =  ZW101_VERIFY_PASS;

    FrameNotComplete();
    zwState = ZW101_NONE_STATE;
    Clear_Buffer();
    Delay_ms(10);

    return temp;
}


/// @brief 获取基本信息
/// @param  none
void ZW101_BaseInformationCommand(void)
{
    Serial_SendArray(baseInformation.commandArray, baseInformation.length);
}


void FrameComplete(void)
{
    rxBuffer[0] = 0x01;
}

void FrameNotComplete(void)
{
    rxBuffer[0] = 0x00;
}

uint8_t GetFrameComplete(void)
{
    return rxBuffer[0];
}

void Clear_Buffer(void)
{
    rxBufferCount = 1;
    for (uint8_t i = 0; i < RX_BUFFER_SIZE; i++) {
        rxBuffer[i] = 0xFF;
    }
    rxBuffer[0] = 0x00;
}






