#ifndef __UI_COMPASSPAGE_H__
#define __UI_COMPASSPAGE_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "ui.h"

extern lv_obj_t * ui_CompassPage;
extern lv_obj_t * ui_Compassneedle;
extern lv_obj_t * ui_CompassDirLabel;
extern lv_obj_t * ui_EnvAltitudeLabel;

extern Page_t Page_Compass;

void ui_CompassPage_screen_init(void);
void ui_CompassPage_screen_deinit(void);

LV_IMG_DECLARE(ui_img_compass_needle_png);

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif  /* __UI_COMPASSPAGE_H__ */
