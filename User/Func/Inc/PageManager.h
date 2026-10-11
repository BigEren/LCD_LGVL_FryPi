#ifndef __PAGE_MANAGER_H__
#define __PAGE_MANAGER_H__

#include "ui.h"

// 页面栈深
#define MAX_DEPTH 6

/** @brief 页面结构体 
 *  @note 页面结构体，包含页面初始化函数指针、去初始化函数指针和页面对象指针
 */
typedef struct
{
    void (*page_init)(void);        // 页面初始化函数指针
    void (*page_deinit)(void);      // 页面去初始化函数指针
    lv_obj_t **page_obj;            // 页面对象指针，指向页面的根对象
} Page_t;   

typedef struct
{
    Page_t* Pages[MAX_DEPTH];   // 页面栈，存储页面结构体指针
    uint8_t top;                     // 栈顶指针，指向当前页面在栈中的位置
} PageStack_t;


Page_t* Page_Get_NowPage(void);
void Page_Back(void);
void Page_Back_Bottom(void);
void Page_Load(Page_t* page);
void Pages_Init(void);

#endif /* __PAGE_MANAGER_H__ */
