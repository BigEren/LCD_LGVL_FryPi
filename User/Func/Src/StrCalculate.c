#include "StrCalculate.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * @brief 字符串入栈
 * @param stack 字符串栈指针
 * @param strin 字符串输入
 * @return uint8_t 0 成功 -1 失败
 */
uint8_t strput(StrStack_t *stack, char strin)
{
    if(stack->Top_Point >= sizeof(stack->strque) - 1)
    {
        return -1;
    }

    stack->strque[stack->Top_Point++] = strin;
    stack->strque[stack->Top_Point] = '\0';   // 保证字符串结尾
    return 0;
}

/**
 * @brief 字符串出栈
 * @param stack 字符串栈指针
 * @return uint8_t 0 成功 -1 失败
 */
uint8_t strdel(StrStack_t *stack)
{
    if(stack->Top_Point == 0)
    {
        return -1;
    }
    stack->strque[--stack->Top_Point] = '\0';
    return 0;
}

/**
 * @brief 字符串栈是否为空
 * @param stack 字符串栈指针
 * @return uint8_t 0 空 1 不为空
 */
uint8_t strstack_isEmpty(StrStack_t *stack)
{
    return (uint8_t)(stack->Top_Point == 0);
}

/**
 * @brief 字符串栈清空
 * @param stack 字符串栈指针
 */
void strclear(StrStack_t *stack)
{
    while(!strstack_isEmpty(stack))
    {
        strdel(stack);
    }
}

/**
 * @brief 数字栈入栈
 * @param stack 数字栈指针
 * @param numIn 数字输入
 * @return uint8_t 0 成功 -1 失败
 */
uint8_t NumStackPut(NumStack_t *stack, double numIn)
{
    if(stack->Top_Point >= CAL_DEPTH - 1)
    {
        return -1;
    }

    stack->numque[stack->Top_Point++] = numIn;
    return 0;
}

/**
 * @brief 数字栈出栈
 * @param stack 数字栈指针
 * @return uint8_t 0 成功 -1 失败
 */
uint8_t NumStackDel(NumStack_t *stack)
{
    if(stack->Top_Point == 0)
    {
        return -1;
    }
    stack->numque[--stack->Top_Point] = NULL;
    return 0;
}

/**
 * @brief 数字栈是否为空
 * @param stack 数字栈指针
 * @return uint8_t 0 空 1 不为空
 */
uint8_t NumStack_isEmpty(NumStack_t *stack)
{
    return (uint8_t)(stack->Top_Point == 0);
}

/**
 * @brief 数字栈清空
 * @param stack 数字栈指针
 */
void NumStackClear(NumStack_t *stack)
{
    while(!NumStack_isEmpty(stack))
    {
        NumStackDel(stack);
    }
}

/**
 * @brief 符号栈入栈
 * @param stack 符号栈指针
 * @param symIn 符号输入
 * @return uint8_t 0 成功 -1 失败
 */
uint8_t SymStackPut(SymStack_t *stack, char symIn)
{
    if(stack->Top_Point >= CAL_DEPTH - 1)
    {
        return -1;
    }

    stack->symque[stack->Top_Point++] = symIn;
    return 0;
}

/**
 * @brief 符号栈出栈
 * @param stack 符号栈指针
 * @return uint8_t 0 成功 -1 失败
 */
uint8_t SymStackDel(SymStack_t *stack)
{
    if(stack->Top_Point == 0)
    {
        return -1;
    }
    stack->symque[--stack->Top_Point] = NULL;
    return 0;
}

/**
 * @brief 符号栈是否为空
 * @param stack 符号栈指针
 * @return uint8_t 0 空 1 不为空
 */
uint8_t SymStack_isEmpty(SymStack_t *stack)
{
    return (uint8_t)(stack->Top_Point == 0);
}

/**
 * @brief 符号栈清空
 * @param stack 符号栈指针
 */
void SymStackClear(SymStack_t *stack)
{
    while(!SymStack_isEmpty(stack))
    {
        SymStackDel(stack);
    }
}

/**
 * @brief 符号栈判断是否高优先级
 * @param topSym 栈顶符号
 * @param presentSym 当前符号
 * @return uint8_t 0 否 1 是
 */
uint8_t SymisHighPriority(char topSym, char presentSym)
{
    if(topSym == '*' || topSym == '/')
    {
        return 1;
    }
    else if (topSym == '+')
    {
        if (presentSym == '-')
        {
            return 1;
        }
        else
        {
            return 0;
        }
    }
    else if (topSym == '-')
    {
        if (presentSym == '+')
        {
            return 1;
        }
        else
        {
            return 0;
        }
    }
    else
    {
        return 0;
    }
}

/**
 * @brief 计算一个表达式
 * @param numStack 数字栈指针
 * @param symStack 符号栈指针
 * @note 计算一个表达式, 将结果入栈
 */
void CalculateOne(NumStack_t *numStack, SymStack_t *symStack)
{
    caldata_t temp;
    temp.datatype = NUMBER_TYPE;
    temp.symbol = NULL;

    // 计算数字栈中的顶部两数,结果存到temp中
    if (symStack->symque[symStack->Top_Point - 1] == '+')
    {
        temp.number = numStack->numque[numStack->Top_Point - 2] + numStack->numque[numStack->Top_Point - 1];
    }
    else if (symStack->symque[symStack->Top_Point - 1] == '-')
    {
        temp.number = numStack->numque[numStack->Top_Point - 2] - numStack->numque[numStack->Top_Point - 1];
    }
    else if (symStack->symque[symStack->Top_Point - 1] == '*')
    {
        temp.number = numStack->numque[numStack->Top_Point - 2] * numStack->numque[numStack->Top_Point - 1];
    }
    else if (symStack->symque[symStack->Top_Point - 1] == '/')
    {
        temp.number = numStack->numque[numStack->Top_Point - 2] / numStack->numque[numStack->Top_Point - 1];
    }

    // 将结果入栈
    NumStackDel(numStack);
    NumStackDel(numStack);
    NumStackPut(numStack, temp.number);
    SymStackDel(symStack);
    return;
}

/**
 * @brief 数字和符号分离
 * @param strIn 输入字符串指针
 * @param strLen 输入字符串长度
 * @param NumStack 数字栈指针
 * @param SymStack 符号栈指针
 * @return uint8_t 0 成功 -1 失败
 */
uint8_t NumSymSeparate(char *strIn, uint8_t strLen, NumStack_t *NumStack, SymStack_t *SymStack)
{
    NumStackClear(NumStack);
    SymStackClear(SymStack);
    
    caldata_t temp, temp_pre;
    uint8_t NumBehindePoint_Flag = 0;   // 标志位, 数字是否在小数点后, 后多少位
    temp.datatype = NUMBER_TYPE;
    temp.symbol = NULL;
    temp.number = 0;
    temp_pre = temp;
    temp_pre.datatype = SYMBOL_TYPE;

    if (strIn[0] > '9' || strIn[0] < '0')
    {
        return 1;
    }
    int i;
    for(i = 0; i < strLen; i++)
    {
        if (strIn[i] == '.')
        {
            temp.datatype = POINT_TYPE;
            if (temp_pre.datatype != NUMBER_TYPE)
            {
                return 2;
            }
            temp_pre = temp;
        }
        else if (strIn[i] >= '0' && strIn[i] <= '9')
        {
            // 溢出判断
            if (NumStack->Top_Point > CAL_DEPTH || SymStack->Top_Point > CAL_DEPTH)
            {
                return 3;
            }
            temp.datatype = NUMBER_TYPE;
            temp.number = (double)(strIn[i] - '0');
            temp.symbol = NULL;
            
            if (temp_pre.datatype == NUMBER_TYPE)
            {
                if (NumBehindePoint_Flag == 0)
                {
                    temp.number += NumStack->numque[NumStack->Top_Point - 1] * 10;
                }
                else
                {
                    NumBehindePoint_Flag++;
                    uint8_t j = NumBehindePoint_Flag;
                    while(j--)
                    {
                        temp.number /= 10;
                    }
                    temp.number += NumStack->numque[NumStack->Top_Point - 1];
                }
                NumStackDel(NumStack);
                NumStackPut(NumStack, temp.number);
            }
            else if (temp_pre.datatype == POINT_TYPE)
            {
                NumBehindePoint_Flag = 1;
                temp.number /= 10;
                temp.number += NumStack->numque[NumStack->Top_Point - 1];
                NumStackDel(NumStack);
                NumStackPut(NumStack, temp.number);
            }
            else
            {
                NumStackPut(NumStack, temp.number);
            }
            temp_pre = temp;
        }
        else if (strIn[i] == '+' || strIn[i] == '-' || strIn[i] == '*' || strIn[i] == '/')
        {
            // 溢出判断
            if (NumStack->Top_Point > CAL_DEPTH || SymStack->Top_Point > CAL_DEPTH)
            {
                return 4;
            }
            // temp 存储符号
            temp.datatype = SYMBOL_TYPE;
            temp.symbol = strIn[i];
            temp.number = 0;

            NumBehindePoint_Flag = 0;       // 重置标志位
            // 符号不能连续出现
            if (temp_pre.datatype == SYMBOL_TYPE)
            {
                return 5;
            }
            else
            {
                // 例如: 1+2*3-4/5=10，+ 先入栈，* 入栈，- 小于 *，所以计算 * ，-入栈
                if ((!SymStack_isEmpty(SymStack)) && SymisHighPriority(SymStack->symque[SymStack->Top_Point - 1], temp.symbol))
                {
                    CalculateOne(NumStack, SymStack);
                    SymStackPut(SymStack, temp.symbol);
                }
                else
                {
                    SymStackPut(SymStack, temp.symbol);
                }
                temp_pre = temp;
            }
        }
    }
    return 0;
}

/**
 * @brief 字符串计算
 * @param strIn 输入字符串指针
 * @param NumStack 数字栈指针
 * @param SymStack 符号栈指针
 * @return uint8_t 0 成功 -1 失败
 */
uint8_t StrCalculate(char *strIn, NumStack_t *NumStack, SymStack_t *SymStack)
{
    if (NumSymSeparate(strIn, strlen(strIn), NumStack, SymStack))
    {
        NumStackClear(NumStack);
        SymStackClear(SymStack);
        return -1;
    }
    else
    {
        while (!SymStack_isEmpty(SymStack))
        {
            CalculateOne(NumStack, SymStack);
        }
    }
    return 0;
}

uint8_t isIntNumber(double num)
{
    if (num == (int)num)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

