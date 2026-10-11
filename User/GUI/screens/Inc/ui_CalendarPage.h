#ifndef __UI_CALENDAR_PAGE_H__
#define __UI_CALENDAR_PAGE_H__


#ifdef __cplusplus
extern "C" {
#endif

#include "ui.h"

extern lv_obj_t * ui_CalendarPage;
extern lv_obj_t * ui_CalendarPageCalendar;

extern Page_t Page_Calendar;

void ui_CalendarPage_screen_Init(void);
void ui_CalendarPage_screen_deinit(void);


#ifdef __cplusplus
}
#endif

#endif /* __UI_CALENDAR_PAGE_H__ */
