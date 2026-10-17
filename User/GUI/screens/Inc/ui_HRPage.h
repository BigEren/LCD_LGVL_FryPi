#ifndef __UI_HR_PAGE_H__
#define __UI_HR_PAGE_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "ui.h"

extern Page_t Page_HR;

extern lv_obj_t * ui_HRPage;
extern lv_obj_t * ui_HRPageNumLabel;
extern lv_obj_t * ui_HRPageUnitLabel;
extern lv_obj_t * ui_HRPaggiconLabel;
extern lv_obj_t * ui_HRPageNoticeLabel;


void ui_HRPage_screen_init(void);
void ui_HRPage_screen_deinit(void);

#ifdef __cplusplus
}
#endif

#endif /* __UI_HR_PAGE_H__ */
