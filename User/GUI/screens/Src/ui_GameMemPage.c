#include "stdio.h"
#include "stdlib.h"
#include "string.h"

#include "ui_GameMemPage.h"
#include "ui_helpers.h"
#include "ui_MenuPage.h"
#include "HWDataAccess.h"

#define MATRIX_SIZE 4

typedef struct
{
    uint8_t matrix[MATRIX_SIZE * MATRIX_SIZE];
    char *btnm_map[MATRIX_SIZE * MATRIX_SIZE + MATRIX_SIZE];
    uint8_t label_opa_map[MATRIX_SIZE * MATRIX_SIZE];
    int8_t previous_id;
    int8_t present_id;
} GameMem_t;

static char * num_list[] = {"0","1","2","3","4","5","6","7","8"};
void Game_Mem_Init(void);
void list_rand_number(uint8_t arry[], uint8_t max_count, uint8_t count);
void updata_btnm(char * btnm_map[], uint8_t matrix[MATRIX_SIZE * MATRIX_SIZE]);
uint8_t Game_Mem_Finish(void);

Page_t Page_GameMem = {ui_GameMemPage_screen_init, ui_GameMemPage_screen_deinit, &ui_GameMem_Page};

GameMem_t Game_Mem;

lv_obj_t *ui_GameMem_Page;
lv_obj_t *ui_GameMem_BtnM;
lv_obj_t *ui_new_game_btn;

void ui_event_GameMemBtnM_handler(lv_event_t *e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t *target = lv_event_get_target(e);

    if (event_code == LV_EVENT_VALUE_CHANGED)
    {
        uint32_t id = lv_btnmatrix_get_selected_btn(target);
        Game_Mem.present_id = id;
        if (Game_Mem.previous_id != Game_Mem.present_id)
        {
            if (Game_Mem.matrix[Game_Mem.present_id] == Game_Mem.matrix[Game_Mem.previous_id])
            {
                lv_btnmatrix_set_btn_ctrl(ui_GameMem_BtnM, Game_Mem.present_id, LV_BTNMATRIX_CTRL_HIDDEN);
                lv_btnmatrix_set_btn_ctrl(ui_GameMem_BtnM, Game_Mem.previous_id, LV_BTNMATRIX_CTRL_HIDDEN);
                Game_Mem.matrix[Game_Mem.present_id] = 0;
                Game_Mem.matrix[Game_Mem.previous_id] = 0;
                if(Game_Mem_Finish())
                {
                    lv_obj_clear_flag(ui_new_game_btn, LV_OBJ_FLAG_HIDDEN);
                }
                Game_Mem.previous_id = Game_Mem.present_id;
            }
            else
            {
                if (Game_Mem.previous_id >= 0 && Game_Mem.previous_id < MATRIX_SIZE * MATRIX_SIZE)
                {
                    Game_Mem.label_opa_map[Game_Mem.previous_id] = LV_OPA_0;
                }
                Game_Mem.label_opa_map[Game_Mem.present_id] = LV_OPA_100;
                Game_Mem.previous_id = Game_Mem.present_id;
            }
        }
    }

    if (event_code == LV_EVENT_DRAW_PART_BEGIN)
    {
        lv_obj_draw_part_dsc_t *dsc = lv_event_get_param(e);

        if ((dsc->id >= 0) && (dsc->label_dsc))
        {
            dsc->rect_dsc->bg_color = lv_color_hex(0xB07010);
            dsc->label_dsc->color = lv_color_white();
            dsc->label_dsc->opa = Game_Mem.label_opa_map[dsc->id];
        }
    }
}

void ui_event_NewGameBtnM_handler(lv_event_t *e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    if (event_code == LV_EVENT_CLICKED)
    {
        Game_Mem_Init();
        lv_btnmatrix_set_map(ui_GameMem_BtnM, Game_Mem.btnm_map);
        lv_btnmatrix_clear_btn_ctrl_all(ui_GameMem_BtnM, LV_BTNMATRIX_CTRL_HIDDEN);
        lv_obj_add_flag(ui_new_game_btn, LV_OBJ_FLAG_HIDDEN);
    }
}
void ui_GameMemPage_screen_init(void)
{

    Game_Mem_Init();
    ui_GameMem_Page = lv_obj_create(NULL);;
    ui_GameMem_BtnM = lv_btnmatrix_create(ui_GameMem_Page);
    lv_btnmatrix_set_map(ui_GameMem_BtnM, Game_Mem.btnm_map);
    lv_obj_set_style_text_font(ui_GameMem_BtnM, &ui_font_Cuyuan24, 0);
    lv_btnmatrix_set_one_checked(ui_GameMem_BtnM,true);
    lv_obj_set_style_border_width(ui_GameMem_BtnM,0,0);
    lv_obj_set_style_bg_opa(ui_GameMem_BtnM,0,0);
    lv_obj_set_size(ui_GameMem_BtnM,240,240);
    lv_obj_set_align(ui_GameMem_BtnM,LV_ALIGN_CENTER);


    ui_new_game_btn = lv_btn_create(ui_GameMem_Page);
    lv_obj_align(ui_new_game_btn, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_size(ui_new_game_btn,90,45);
    lv_obj_t * new_game_btn_label = lv_label_create(ui_new_game_btn);
    lv_label_set_text(new_game_btn_label,"重 玩");
    lv_obj_align(new_game_btn_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_text_font(new_game_btn_label, &ui_font_Cuyuan24, 0);
    lv_obj_add_flag(ui_new_game_btn,LV_OBJ_FLAG_HIDDEN);

    //lv_btnmatrix_set_btn_ctrl(ui_GameMem_BtnM,1,LV_BTNMATRIX_CTRL_HIDDEN);
    lv_obj_add_event_cb(ui_GameMem_BtnM, ui_event_GameMemBtnM_handler, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_new_game_btn, ui_event_NewGameBtnM_handler, LV_EVENT_ALL, NULL);
}

void ui_GameMemPage_screen_deinit(void)
{}

void Game_Mem_Init(void)
{
    uint8_t i = 0;
    for (i = 0; i < MATRIX_SIZE * MATRIX_SIZE; i++)
    {
        Game_Mem.matrix[i] = 0;
    }
    for (i = 0; i < MATRIX_SIZE * MATRIX_SIZE; i++)
    {
        Game_Mem.label_opa_map[i] = LV_OPA_0;
    }
    Game_Mem.previous_id = -1;
    Game_Mem.present_id = -1;
    list_rand_number(Game_Mem.matrix, MATRIX_SIZE * MATRIX_SIZE, MATRIX_SIZE * MATRIX_SIZE / 2);
    updata_btnm(Game_Mem.btnm_map, Game_Mem.matrix);
}

void list_rand_number(uint8_t arry[], uint8_t max_count, uint8_t count)
{
    uint8_t i;
    HW_DateTime_Interface_t datetime;
    HWInterface.RealTimeClock.GetTimeDate(&datetime);
    srand((unsigned int)(datetime.Second * 60) + (unsigned int)datetime.Minute);

    for (i = 0; i < max_count; i++)
    {
        arry[i] = (i % count) + 1;
    }
    for (i = max_count - 1; i > 0; i--)
    {
        int8_t idx = rand() % (i + 1);
        uint8_t temp = arry[i];
        arry[i] = arry[idx];
        arry[idx] = temp;
    }
}

void updata_btnm(char * btnm_map[], uint8_t matrix[MATRIX_SIZE * MATRIX_SIZE])
{
    uint8_t i, index = 0;
    for (i = 0; i < MATRIX_SIZE * MATRIX_SIZE; i++)
    {
        if ((index + 1) % (MATRIX_SIZE + 1) == 0)
        {
            btnm_map[index] = "\n";
            index++;
        }
        btnm_map[index] = num_list[matrix[i]];
        index++;
    }
    btnm_map[MATRIX_SIZE * MATRIX_SIZE + MATRIX_SIZE - 1] = "";
}

uint8_t Game_Mem_Finish(void)
{
    for (uint8_t i = 0; i < MATRIX_SIZE * MATRIX_SIZE; i++)
    {
        if (Game_Mem.matrix[i] != 0)
        {
            return 0;
        }
    }
    return 1;
}