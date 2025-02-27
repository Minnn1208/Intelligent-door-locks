#include "Mode.h"
#include <string.h>

ModeStatus currentState;

// 初始化模式状态
void Mode_Init(ModeStatus *modeStatus) {
    modeStatus->currentMode = MODE_IDLE;
    modeStatus->isActive = 0;  // 默认未激活
}

// 设置新的模式
void Mode_Set(ModeStatus *modeStatus, uint8_t newMode) 
{
    // 确保新模式是有效的模式
    if (newMode == MODE_NONE || newMode == MODE_FINGERPRINT || 
        newMode == MODE_PASSWORD || newMode == MODE_ADMIN || newMode == MODE_IDLE) {
        modeStatus->currentMode = newMode;
        modeStatus->isActive = (newMode != MODE_NONE);  // 如果是无模式，则未激活
    }

    // 如果进入管理员模式，执行特定指令
    if (newMode == MODE_ADMIN) {
        AdminMode_ExecuteCommand();
    }
}

// 获取当前模式
uint8_t Mode_GetCurrent(const ModeStatus *modeStatus) 
{
    return modeStatus->currentMode;
}

// 判断模式是否激活
uint8_t Mode_IsActive(const ModeStatus *modeStatus) 
{
    return modeStatus->isActive;
}

// 将模式转换为字符串
const char* Mode_ToString(uint8_t mode) 
{
    switch (mode) {
        case MODE_FINGERPRINT: return "FINGERPRINT";
        case MODE_PASSWORD:    return "PASSWORD";
        case MODE_ADMIN:       return "ADMIN";
        case MODE_NONE:        return "NONE";
        default:               return "UNKNOWN";
    }
}

// 判断是否允许解锁
uint8_t Mode_IsUnlockAllowed(const ModeStatus *modeStatus) 
{
    // 管理员模式下不允许解锁
    if (modeStatus->currentMode == MODE_ADMIN) {
        return 0;
    }
    // 其他模式下允许解锁
    return 1;
}

// 管理员模式下执行特定指令
void AdminMode_ExecuteCommand(void)
 {
    // 在这里实现管理员模式的特定指令
    
}