#ifndef __MFRC522_H
#define __MFRC522_H
#include "MFRC522_Reg.h"
#include "stm32f10x.h"

void MFRC522_Init(void);
void MFRC522_Write(uint8_t address, uint8_t value);
uint8_t MFRC522_Read(uint8_t address);
void MFRC522_Reset(void);
void MFRC522_AntennaOn(void);
void MFRC522_AntennaOff(void);
uint8_t MFRC522_Request(uint8_t reqCode, uint8_t *tagType);

/* char MFRC522_Reset(void);
void Write_MFRC522(unsigned char Address, unsigned char value);
unsigned char Read_MFRC522(unsigned char Address);  
void MFRC522_AntennaOn(void);
void MFRC522_AntennaOff(void);
char MFRC522_Request(unsigned char req_code,unsigned char *pTagType);
char MFRC522_Anticoll(unsigned char *pSnr);
char MFRC522_SelectTag(unsigned char *pSnr);
char MFRC522_AuthState(unsigned char auth_mode,unsigned char addr,unsigned char *pKey,unsigned char *pSnr);
char MFRC522_Read(unsigned char addr,unsigned char *pData);
char MFRC522_Halt(void);
char MFRC522_Write(unsigned char addr,unsigned char *pData); */

#endif
