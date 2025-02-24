#ifndef __PASSWORD_H__
#define __PASSWORD_H__
#include "Keyboard.h"
#include "Mode.h"

#define PASSWORD_LEN 4
#define PASSWORD_ERR 0x00
#define PASSWORD_OK 0x01
#define PASSWORD_NOT_ENOUGH 0x02
#define PASSWORD_ENOUGH 0x03
#define PASSWORD_LESS 0x04
#define PASSWORD_DEL_SUSS 0x05
#define PASSWORD_DEL_FAIL 0x06
#define PASSWORD_DUMMY 0xFF

static uint8_t password[4] = {1, 2, 3, 8};
static uint8_t password_in[4] = {0};
static uint8_t passwordIndex = 0;

uint8_t Password_Component(uint8_t keyValue);

#endif
