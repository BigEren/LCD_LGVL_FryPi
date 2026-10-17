#include "ui_HRPage.h"
#include "ui_helpers.h"
#include "HWDataAccess.h"

Page_t Page_HR = {ui_HRPage_screen_init, ui_HRPage_screen_deinit, &ui_HRPage};

lv_obj_t *ui_HRPage;
lv_obj_t *ui_HRPageNumLabel;
lv_obj_t *ui_HRPageUnitLabel;
lv_obj_t *ui_HRPaggiconLabel;
lv_obj_t *ui_HRPageNoticeLabel;

lv_timer_t *ui_HRPageTimer;

volatile uint32_t HRPage_HeartRate = 0;

static void HRPage_timer_cb(lv_timer_t * timer)
{
    if (!HWInterface.MAX30102.health_data.heart_rate_valid)
    {
        return;
    }
    uint8_t HR_strbuf[16];
    HRPage_HeartRate = HWInterface.MAX30102.health_data.heart_rate;
    snprintf(HR_strbuf, sizeof(HR_strbuf), "%d", HRPage_HeartRate);
    lv_label_set_text(ui_HRPageNumLabel, HR_strbuf);
}

void ui_HRPage_screen_init(void)
{
    uint8_t value_strbuf[16];

    ui_HRPage = lv_obj_create(NULL);
    lv_obj_clear_flag(ui_HRPage, LV_OBJ_FLAG_SCROLLABLE);      /// Flags

    ui_HRPageNumLabel = lv_label_create(ui_HRPage);
    lv_obj_set_width(ui_HRPageNumLabel, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_HRPageNumLabel, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_align(ui_HRPageNumLabel, LV_ALIGN_CENTER);
    if (!HWInterface.MAX30102.health_data.heart_rate_valid)
    {
        lv_label_set_text(ui_HRPageNumLabel, "00");
    }
    else
    {
        snprintf(value_strbuf, sizeof(value_strbuf), "%d", HWInterface.MAX30102.health_data.heart_rate);
        lv_label_set_text(ui_HRPageNumLabel, value_strbuf);
    }
    lv_obj_set_style_text_font(ui_HRPageNumLabel, &ui_font_Cuyuan80, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_HRPageUnitLabel = lv_label_create(ui_HRPage);
    lv_obj_set_width(ui_HRPageUnitLabel, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_HRPageUnitLabel, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_HRPageUnitLabel, 85);
    lv_obj_set_y(ui_HRPageUnitLabel, 18);
    lv_obj_set_align(ui_HRPageUnitLabel, LV_ALIGN_CENTER);
    lv_label_set_text(ui_HRPageUnitLabel, "次/分");
    lv_obj_set_style_text_color(ui_HRPageUnitLabel, lv_color_hex(0x808080), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_HRPageUnitLabel, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_HRPageUnitLabel, &ui_font_Cuyuan20, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_HRPaggiconLabel = lv_label_create(ui_HRPage);
    lv_obj_set_width(ui_HRPaggiconLabel, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_HRPaggiconLabel, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_HRPaggiconLabel, 0);
    lv_obj_set_y(ui_HRPaggiconLabel, 30);
    lv_obj_set_align(ui_HRPaggiconLabel, LV_ALIGN_TOP_MID);
    lv_label_set_text(ui_HRPaggiconLabel, "");
    lv_obj_set_style_text_color(ui_HRPaggiconLabel, lv_color_hex(0xFF1919), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_HRPaggiconLabel, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_HRPaggiconLabel, &ui_font_iconfont34, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_HRPageNoticeLabel = lv_label_create(ui_HRPage);
    lv_obj_set_width(ui_HRPageNoticeLabel, LV_SIZE_CONTENT);   /// 1
    lv_obj_set_height(ui_HRPageNoticeLabel, LV_SIZE_CONTENT);    /// 1
    lv_obj_set_x(ui_HRPageNoticeLabel, 0);
    lv_obj_set_y(ui_HRPageNoticeLabel, 90);
    lv_obj_set_align(ui_HRPageNoticeLabel, LV_ALIGN_CENTER);
    lv_label_set_text(ui_HRPageNoticeLabel, "正在测量...");
    lv_obj_set_style_text_color(ui_HRPageNoticeLabel, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_opa(ui_HRPageNoticeLabel, 220, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(ui_HRPageNoticeLabel, &ui_font_Cuyuan20, LV_PART_MAIN | LV_STATE_DEFAULT);

    //timer
    ui_HRPageTimer = lv_timer_create(HRPage_timer_cb, 500,  NULL);

}

void ui_HRPage_screen_deinit(void)
{
    if (ui_HRPageTimer != NULL)
    {
       lv_timer_del(ui_HRPageTimer);
       ui_HRPageTimer = NULL;
    }
}
