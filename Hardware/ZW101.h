#ifndef __ZW101_H__
#define __ZW101_H__

#include "stm32f10x.h"
#include <stdint.h>
#include <string.h>
#include "Serial.h"
#include "Delay.h"
#include "OLED.h"


#define RX_BUFFER_SIZE                          99
#define ZW101_THUMB_PRINT                       1
#define ZW101_VERIFY                            2


#define ZW101_NONE_STATE                        0
#define ZW101_HANDSHAKE_STATE                   1
#define ZW101_VERIFY_STATE                      2
#define ZW101_PRINT_STATE                       3

/* 握手确认位 */
#define ZW101_HANDSHAKE_SUCCESS                 0x00
#define ZW101_HANDSHAKE_FAIL                    0x01
#define ZW101_HANDSHAKE_ERROR                   0x02

/* 指纹验证确认位 */
#define ZW101_VERIFY_PASS                       0x01
#define ZW101_VERIFY_NOT_PASS                   0x02
#define ZW101_VERIFY_ERROR                      0x03

extern uint8_t zwState;
extern uint8_t rxBuffer[];
// extern uint8_t frameComplete;
extern uint8_t rxBufferCount;

typedef struct 
{
    uint8_t commandArray[20];
    uint8_t length;
    uint8_t replyLength;
}ZW101Command;

extern ZW101Command handShake;
extern ZW101Command verification;
/* 基本信息指令 EF 01 FF FF FF FF 01 00 03 0F 00 13 */
extern ZW101Command baseInformation;
/* 录入指纹指令 EF 01 FF FF FF FF 01 00 08 31 00 01 03 00 09 00 47 */
extern ZW101Command thumbPrint;


void ZW101_Init(void);
void ZW101_HandShakeCommand(void);
uint8_t ZW101_HandShakeVerify(void);
void ZW101_ThumbPrintCommand(void);
void ZW101_VerifyThumbCommand(void);
uint8_t ZW101_VerifyThumb(void);
void ZW101_BaseInformationCommand(void);
void FrameComplete(void);
void FrameNotComplete(void);
uint8_t GetFrameComplete(void);
void Clear_Buffer(void);
void Thumb_OpenDoor(void);

#endif
