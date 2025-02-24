#ifndef __ZW101_LINKLIST_H__
#define __ZW101_LINKLIST_H__

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

// 数据包节点结构体
typedef struct ZW101Node {
    uint8_t* data;              // 数据包内容
    uint16_t dataLength;        // 数据包长度
    struct ZW101Node* next;     // 指向下一个节点
} ZW101Node;

// 链表管理结构体
typedef struct {
    ZW101Node* head;            // 链表头指针
    ZW101Node* tail;            // 链表尾指针（优化尾部插入）
    uint16_t count;             // 链表节点数量
} ZW101LinkedList;

// 函数声明

/**
 * @brief 初始化链表
 * @param list 链表管理结构体指针
 */
void ZW101_InitList(ZW101LinkedList* list);

/**
 * @brief 添加节点到链表尾部
 * @param list 链表管理结构体指针
 * @param data 数据包内容
 * @param dataLength 数据包长度
 * @return true 成功，false 失败
 */
bool ZW101_AddNode(ZW101LinkedList* list, uint8_t* data, uint16_t dataLength);

/**
 * @brief 删除链表中的节点
 * @param list 链表管理结构体指针
 * @param data 要删除的数据包内容
 * @param dataLength 要删除的数据包长度
 * @return true 成功，false 失败
 */
bool ZW101_DeleteNode(ZW101LinkedList* list, uint8_t* data, uint16_t dataLength);

/**
 * @brief 查找链表中的节点
 * @param list 链表管理结构体指针
 * @param data 要查找的数据包内容
 * @param dataLength 要查找的数据包长度
 * @return 找到的节点指针，未找到返回 NULL
 */
ZW101Node* ZW101_FindNode(ZW101LinkedList* list, uint8_t* data, uint16_t dataLength);

/**
 * @brief 清空链表
 * @param list 链表管理结构体指针
 */
void ZW101_ClearList(ZW101LinkedList* list);

/**
 * @brief 获取链表节点数量
 * @param list 链表管理结构体指针
 * @return 节点数量
 */
uint16_t ZW101_GetListCount(ZW101LinkedList* list);

#endif // __ZW101_LINKLIST_H__