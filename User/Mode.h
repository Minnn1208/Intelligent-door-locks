#ifndef __MODE_H__
#define __MODE_H__

#include <stdint.h>

// 定义模式宏
#define MODE_NONE          0xFF  // 无模式
#define MODE_IDLE          0x00
#define MODE_FINGERPRINT   0x01  // 指纹解锁模式
#define MODE_PASSWORD      0x02  // 密码解锁模式
#define MODE_ADMIN         0x04  // 管理员模式

// 定义模式状态结构体
typedef struct {
    uint8_t currentMode;  // 当前模式（使用宏定义的值）
    uint8_t isActive;     // 模式是否激活（1: 激活, 0: 未激活）
} ModeStatus;

extern ModeStatus mainState;

// 函数声明
void Mode_Init(ModeStatus *modeStatus);                      // 初始化模式
void Mode_Set(ModeStatus *modeStatus, uint8_t newMode);      // 设置模式
uint8_t Mode_GetCurrent(const ModeStatus *modeStatus);       // 获取当前模式
uint8_t Mode_IsActive(const ModeStatus *modeStatus);         // 判断模式是否激活
const char* Mode_ToString(uint8_t mode);                     // 将模式转换为字符串
uint8_t Mode_IsUnlockAllowed(const ModeStatus *modeStatus);  // 判断是否允许解锁
void AdminMode_ExecuteCommand(void);                             // 管理员模式下执行特定指令

#endif // __MODE_H__
