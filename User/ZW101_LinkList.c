#include "ZW101_LinkList.h"

/**
 * @brief 初始化链表
 * @param list 链表管理结构体指针
 */
void ZW101_InitList(ZW101LinkedList* list) {
    list->head = NULL;
    list->tail = NULL;
    list->count = 0;
}

/**
 * @brief 添加节点到链表尾部
 * @param list 链表管理结构体指针
 * @param data 数据包内容
 * @param dataLength 数据包长度
 * @return true 成功，false 失败
 */
bool ZW101_AddNode(ZW101LinkedList* list, uint8_t* data, uint16_t dataLength) {
    ZW101Node* newNode = (ZW101Node*)malloc(sizeof(ZW101Node));
    if (!newNode) {
        return false;  // 内存分配失败
    }

    // 初始化新节点
    newNode->data = (uint8_t*)malloc(dataLength);
    if (!newNode->data) {
        free(newNode);
        return false;  // 内存分配失败
    }
    memcpy(newNode->data, data, dataLength);
    newNode->dataLength = dataLength;
    newNode->next = NULL;

    // 插入节点
    if (list->head == NULL) {
        list->head = newNode;
    } else {
        list->tail->next = newNode;
    }
    list->tail = newNode;  // 更新尾指针
    list->count++;

    return true;
}

/**
 * @brief 删除链表中的节点
 * @param list 链表管理结构体指针
 * @param data 要删除的数据包内容
 * @param dataLength 要删除的数据包长度
 * @return true 成功，false 失败
 */
bool ZW101_DeleteNode(ZW101LinkedList* list, uint8_t* data, uint16_t dataLength) {
    ZW101Node* current = list->head;
    ZW101Node* previous = NULL;

    while (current) {
        // 比较数据包内容和长度
        if (current->dataLength == dataLength && memcmp(current->data, data, dataLength) == 0) {
            // 找到要删除的节点
            if (previous) {
                previous->next = current->next;
            } else {
                list->head = current->next;
            }

            // 更新尾指针
            if (current == list->tail) {
                list->tail = previous;
            }

            // 释放节点内存
            free(current->data);
            free(current);
            list->count--;
            return true;
        }

        previous = current;
        current = current->next;
    }

    return false;  // 未找到节点
}

/**
 * @brief 查找链表中的节点
 * @param list 链表管理结构体指针
 * @param data 要查找的数据包内容
 * @param dataLength 要查找的数据包长度
 * @return 找到的节点指针，未找到返回 NULL
 */
ZW101Node* ZW101_FindNode(ZW101LinkedList* list, uint8_t* data, uint16_t dataLength) {
    ZW101Node* current = list->head;

    while (current) {
        if (current->dataLength == dataLength && memcmp(current->data, data, dataLength) == 0) {
            return current;  // 找到节点
        }
        current = current->next;
    }

    return NULL;  // 未找到节点
}

/**
 * @brief 清空链表
 * @param list 链表管理结构体指针
 */
void ZW101_ClearList(ZW101LinkedList* list) {
    ZW101Node* current = list->head;

    while (current) {
        ZW101Node* next = current->next;
        free(current->data);
        free(current);
        current = next;
    }

    list->head = NULL;
    list->tail = NULL;
    list->count = 0;
}

/**
 * @brief 获取链表节点数量
 * @param list 链表管理结构体指针
 * @return 节点数量
 */
uint16_t ZW101_GetListCount(ZW101LinkedList* list) {
    return list->count;
}