#ifndef __MFRC522_H
#define __MFRC522_H

void MFRC522_Init(void);
void MFRC522_Write(uint8_t address, uint8_t value);
uint8_t MFRC522_Read(uint8_t address);

#endif