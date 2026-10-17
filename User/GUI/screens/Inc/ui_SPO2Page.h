#ifndef __UI_SPO2PAGE_H__
#define __UI_SPO2PAGE_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "ui.h"

extern lv_obj_t * ui_SPO2Page;
extern lv_obj_t * ui_SPO2NumLabel;
extern lv_obj_t * ui_SPO2UnitLabel;
extern lv_obj_t * ui_SPO2NoticeLabel;
extern lv_obj_t * ui_SPO2Icon;

extern Page_t Page_SPO2;

void ui_SPO2Page_screen_init(void);
void ui_SPO2Page_screen_deinit(void);

#ifdef __cplusplus
}
#endif


#endif /* __UI_SPO2PAGE_H__ */
