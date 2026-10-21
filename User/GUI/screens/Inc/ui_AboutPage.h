#ifndef __UI_ABOUTPAGE_H__
#define __UI_ABOUTPAGE_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "ui.h"
extern lv_obj_t * ui_AboutPage;
extern Page_t Page_About;

void ui_AboutPage_screen_init(void);
void ui_AboutPage_screen_deinit(void);

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif /*__UI_ABOUTPAGE_H__*/
