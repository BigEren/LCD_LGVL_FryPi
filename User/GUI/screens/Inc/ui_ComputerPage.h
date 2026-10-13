#ifndef __UI_COMPUTER_PAGE_H__
#define __UI_COMPUTER_PAGE_H__

#include "ui.h"

extern lv_obj_t *ui_ComputerPage;           // 计算器页面对象
extern lv_obj_t *ui_CompageBtnM;            // 计算器按键对象
extern lv_obj_t *ui_CompageTextarea;        // 计算器文本框对象
extern lv_obj_t *ui_CompageBackBtn;         // 计算器返回按钮对象

extern Page_t Page_Computer;

void ui_ComputerPage_screen_init(void);     // 计算器页面初始化函数
void ui_ComputerPage_screen_deinit(void);   // 计算器页面退出函数


#endif  /* __UI_COMPUTER_PAGE_H__ */
