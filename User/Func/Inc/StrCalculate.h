#ifndef __STR_CALCULATE_H__
#define __STR_CALCULATE_H__

#include <stdint.h>

#define NUMBER_TYPE 0
#define SYMBOL_TYPE 1
#define POINT_TYPE 3
#define CAL_DEPTH 16

/**
 * @brief 字符串栈
 * 
 * 用于存储输入的字符串，包括数字、符号和点
 */
typedef struct
{
    char strque[20];
    uint8_t Top_Point;

} StrStack_t;

uint8_t strput(StrStack_t *stack, char strin);
uint8_t strdel(StrStack_t *stack);
uint8_t strstack_isEmpty(StrStack_t *stack);
void strclear(StrStack_t *stack);

/**
 * @brief 计算数据
 * 
 * 用于存储计算数据，包括数字、符号和点
 */
typedef struct
{
    char datatype;          // 数据类型
    float number;           // 数字
    char symbol;            // 符号

} caldata_t;

/**
 * @brief 数字栈
 * 
 * 用于存储数字，包括整数和浮点数
 */
typedef struct
{
    float numque[CAL_DEPTH];
    uint8_t Top_Point;

} NumStack_t;

/**
 * @brief 符号栈
 * 
 * 用于存储符号，包括加减乘除
 */
typedef struct
{
    char symque[CAL_DEPTH];
    uint8_t Top_Point;

} SymStack_t;

void NumStackClear(NumStack_t* stack);
void SymStackClear(SymStack_t* stack);
uint8_t NumSymSeparate(char * strIn, uint8_t strlen, NumStack_t * NumStack, SymStack_t * SymStack);
uint8_t StrCalculate(char *strIn, NumStack_t *NumStack, SymStack_t *SymStack);
uint8_t isIntNumber(float num);



#endif /* __STR_CALCULATE_H__ */
