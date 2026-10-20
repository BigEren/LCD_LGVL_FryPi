#include "stdio.h"
#include "stdlib.h"
#include "string.h"

#include "ui_Game2048Page.h"
#include "ui_helpers.h"
#include "ui_MenuPage.h"
#include "HWDataAccess.h"

//#define STACK_DEPTH 5
#define MATRIX_SIZE 4

// typedef struct
// {
//     uint16_t Data[STACK_DEPTH];
//     uint8_t Top_Point;
// } Game2048Stack_t;

// static void Stack_Init(Game2048Stack_t * stack);
// static uint8_t Stack_Push(Game2048Stack_t * stack, uint16_t dataIn);
// static uint16_t Stack_Pop(Game2048Stack_t * stack);
// static uint8_t Stack_IsEmpty(Game2048Stack_t * stack);
// static void Stack_Clear(Game2048Stack_t * stack);

typedef struct 
{
    uint32_t score;
    uint16_t matrix[MATRIX_SIZE][MATRIX_SIZE];
    char *btn_map[MATRIX_SIZE * MATRIX_SIZE + MATRIX_SIZE];
    uint8_t is_over;
} ui_Game2048_t;

#define GAME_2048_TEXT_BLACK_COLOR      lv_color_hex(0X000000)
#define GAME_2048_TEXT_WHITE_COLOR      lv_color_hex(0Xffffff)

#define GAME_2048_NUMBER_EMPTY_COLOR    lv_color_hex(0x023535)
#define GAME_2048_NUMBER_2_COLOR        lv_color_hex(0x023535)
#define GAME_2048_NUMBER_4_COLOR        lv_color_hex(0x265C4B)
#define GAME_2048_NUMBER_8_COLOR        lv_color_hex(0x146551)
#define GAME_2048_NUMBER_16_COLOR       lv_color_hex(0x146551)
#define GAME_2048_NUMBER_32_COLOR       lv_color_hex(0x007566)
#define GAME_2048_NUMBER_64_COLOR       lv_color_hex(0x007566)
#define GAME_2048_NUMBER_128_COLOR      lv_color_hex(0x009A8D)
#define GAME_2048_NUMBER_256_COLOR      lv_color_hex(0x008F8C)
#define GAME_2048_NUMBER_512_COLOR      lv_color_hex(0x0CABAB)
#define GAME_2048_NUMBER_1024_COLOR     lv_color_hex(0x8FC1B5)
#define GAME_2048_NUMBER_2048_COLOR     lv_color_hex(0x8FD1B5)

static char *Str2048Nums[]={"2","4","8","16","32","64","128","256","512","1024","2048"};

static void Game2048_Init(void);
static void init_matrix_num(uint16_t matrix[MATRIX_SIZE][MATRIX_SIZE]);
static void update_btnm_map(char *btnm_map[], uint16_t matrix[MATRIX_SIZE][MATRIX_SIZE]);
static void addRandom(uint16_t matrix[MATRIX_SIZE][MATRIX_SIZE]);
static uint8_t move_up(uint32_t * score, uint16_t matrix[MATRIX_SIZE][MATRIX_SIZE]);
static uint8_t move_down(uint32_t * score, uint16_t matrix[MATRIX_SIZE][MATRIX_SIZE]);
static uint8_t move_right(uint32_t * score, uint16_t matrix[MATRIX_SIZE][MATRIX_SIZE]);
static uint8_t move_left(uint32_t * score, uint16_t matrix[MATRIX_SIZE][MATRIX_SIZE]);
static uint8_t game_over(uint16_t matrix[MATRIX_SIZE][MATRIX_SIZE]);
static lv_color_t get_num_color(uint16_t num);

Page_t Page_Game_2048 = {ui_Game2048Page_screen_init, ui_Game2048Page_screen_deinit, &ui_Game2048Page};

ui_Game2048_t Game_2048;

lv_obj_t * ui_Game2048Page;
lv_obj_t * ui_Game2048BtnM;
lv_obj_t * ui_Game2048ScLabel;

static void ui_event_Game2048Page(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t *target = lv_event_get_target(e);

    if (event_code == LV_EVENT_GESTURE)
    {
        if (Game_2048.is_over) return;

        if (!Game_2048.is_over)
        {
            uint8_t moved = 0;
            if(lv_indev_get_gesture_dir(lv_indev_get_act()) == LV_DIR_LEFT)
            {
                moved = move_left(&Game_2048.score, Game_2048.matrix);
            }
            else if(lv_indev_get_gesture_dir(lv_indev_get_act()) == LV_DIR_RIGHT)
            {
                moved = move_right(&Game_2048.score, Game_2048.matrix);
            }
            else if(lv_indev_get_gesture_dir(lv_indev_get_act()) == LV_DIR_TOP)
            {
                moved = move_up(&Game_2048.score, Game_2048.matrix);
            }
            else if(lv_indev_get_gesture_dir(lv_indev_get_act()) == LV_DIR_BOTTOM)
            {
                moved = move_down(&Game_2048.score, Game_2048.matrix);
            }
            if (moved)
            {
                addRandom(Game_2048.matrix);
                update_btnm_map(Game_2048.btn_map, Game_2048.matrix);
                lv_btnmatrix_set_map(ui_Game2048BtnM, Game_2048.btn_map);
                char score_str[16];
                snprintf(score_str, sizeof(score_str), "Score:%u", Game_2048.score);
                lv_label_set_text(ui_Game2048ScLabel, score_str);

                Game_2048.is_over = game_over(Game_2048.matrix);
            }
        }
    }
}

static void ui_event_Game2048BtnM(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    lv_obj_t *target = lv_event_get_target(e);
    if (event_code == LV_EVENT_DRAW_PART_BEGIN)
    {
        lv_obj_draw_part_dsc_t *dsc = lv_event_get_param(e);

        if ((dsc->id >= 0) && (dsc->label_dsc))
        {
            uint16_t x, y, num;
            x = (uint16_t)((dsc->id) / MATRIX_SIZE);
            y = (uint16_t)((dsc->id) % MATRIX_SIZE);
            num = (uint16_t)(1 <<Game_2048.matrix[x][y]);

            dsc->rect_dsc->bg_color = get_num_color(num);
            if (num > 512)
            {
                dsc->label_dsc->color = GAME_2048_TEXT_BLACK_COLOR;
            }
            else
            {
                dsc->label_dsc->color = GAME_2048_TEXT_WHITE_COLOR;
            }
        }
    }
    if (event_code == LV_EVENT_DRAW_PART_END)
    {
        lv_obj_draw_part_dsc_t *dsc = lv_event_get_param(e);
    }
}

static void ui_event_new_game_btn(lv_event_t * e)
{
    lv_event_code_t event_code = lv_event_get_code(e);
    if (event_code == LV_EVENT_LONG_PRESSED)
    {
        Game2048_Init();
        char score_str[16];
        snprintf(score_str, sizeof(score_str), "Score:%u", Game_2048.score);
        lv_label_set_text(ui_Game2048ScLabel, score_str);
    }
}

void ui_Game2048Page_screen_init(void)
{

    ui_Game2048Page = lv_obj_create(NULL);

    lv_obj_clear_flag(ui_Game2048Page,LV_OBJ_FLAG_SCROLLABLE);
    ui_Game2048BtnM = lv_btnmatrix_create(ui_Game2048Page);
    Game2048_Init();

    lv_obj_set_style_text_font(ui_Game2048BtnM, &ui_font_Cuyuan20, 0);
    lv_btnmatrix_set_one_checked(ui_Game2048BtnM, true);
    lv_obj_clear_flag(ui_Game2048BtnM, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_set_style_border_width(ui_Game2048BtnM,0,0);
    lv_obj_set_style_bg_opa(ui_Game2048BtnM,0,0);
    lv_obj_set_size(ui_Game2048BtnM,240,240);
    lv_obj_set_align(ui_Game2048BtnM,LV_ALIGN_BOTTOM_MID);


    lv_obj_t * new_game_btn = lv_btn_create(ui_Game2048Page);
    lv_obj_align(new_game_btn, LV_ALIGN_TOP_LEFT, 12, 8);
    lv_obj_set_size(new_game_btn,85,35);
    lv_obj_t * new_game_btn_label = lv_label_create(new_game_btn);
    lv_label_set_text(new_game_btn_label, "重 玩");
    lv_obj_align(new_game_btn_label, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_text_font(new_game_btn_label, &ui_font_Cuyuan20, 0);

    ui_Game2048ScLabel = lv_label_create(ui_Game2048Page);
    char score_str[16];
    snprintf(score_str, sizeof(score_str), "Score:%u", Game_2048.score);
    lv_label_set_text(ui_Game2048ScLabel, score_str);
    lv_obj_align(ui_Game2048ScLabel, LV_ALIGN_TOP_RIGHT, -10, 15);
    lv_obj_set_style_text_font(ui_Game2048ScLabel, &ui_font_Cuyuan20, 0);


    lv_obj_add_event_cb(ui_Game2048Page, ui_event_Game2048Page, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(ui_Game2048BtnM, ui_event_Game2048BtnM, LV_EVENT_ALL, NULL);
    lv_obj_add_event_cb(new_game_btn, ui_event_new_game_btn, LV_EVENT_ALL, NULL);

}

void ui_Game2048Page_screen_deinit(void)
{}

static void Game2048_Init(void)
{
    Game_2048.score = 0;
    Game_2048.is_over = 0;

    memset(Game_2048.btn_map, 0, sizeof(Game_2048.btn_map));

    Game_2048.btn_map[4] = "\n";
    Game_2048.btn_map[9] = "\n";
    Game_2048.btn_map[14] = "\n";
    Game_2048.btn_map[19] = "";

    init_matrix_num(Game_2048.matrix);
    update_btnm_map(Game_2048.btn_map, Game_2048.matrix);
    lv_btnmatrix_set_map(ui_Game2048BtnM, Game_2048.btn_map);
}
static void init_matrix_num(uint16_t matrix[MATRIX_SIZE][MATRIX_SIZE])
{
    uint8_t x, y;
    for(x = 0; x < MATRIX_SIZE; x++)
    {
        for(y = 0; y < MATRIX_SIZE; y++)
        {
            matrix[x][y] = 0;
        }
    }
    // 初始化两个随机数
    addRandom(matrix);
    addRandom(matrix);
}
static void update_btnm_map(char *btnm_map[], uint16_t matrix[MATRIX_SIZE][MATRIX_SIZE])
{
    uint8_t x, y, index;
    index = 0;
    for(x = 0; x < MATRIX_SIZE; x++)
    {
        for(y = 0; y < MATRIX_SIZE; y++)
        {
            if (((index + 1) % 5) == 0)
            {
                index++;
            }
            if (matrix[x][y] == 0)
            {
                btnm_map[index] = " ";
            }
            else
            {
                btnm_map[index] = Str2048Nums[matrix[x][y] - 1];
            }
            index++;
        }
    }
}

static void addRandom(uint16_t matrix[MATRIX_SIZE][MATRIX_SIZE])
{
    static uint8_t init_flag = 0;
    uint16_t x, y;
    uint16_t r, len = 0, dest_r = 0;
    if(init_flag == 0)
    {
        HW_DateTime_Interface_t datetime;
        HWInterface.RealTimeClock.GetTimeDate(&datetime);
        srand((int)datetime.Second * 60 + (int)datetime.Minute);
        init_flag = 1;
    }

    // 统计空格
    for(x = 0; x < MATRIX_SIZE; x++)
    {
        for(y = 0; y < MATRIX_SIZE; y++)
        {
            if (matrix[x][y] == 0)
            {
                len++;
            }
        }
    }

    if(len == 0)
    {
        return;
    }

    // 随机选择一个空格
    r = rand() % len;
    for (x = 0; x < MATRIX_SIZE; x++)
    {
        for (y = 0; y < MATRIX_SIZE; y++)
        {
            if (matrix[x][y] == 0)
            {
                if (dest_r == r)
                {
                    matrix[x][y] = ((rand() % 10) >= 9) ? 2 : 1;
                    return;
                }
                dest_r++;
            }
        }
    }
}

static uint8_t merge_one_line(uint32_t *score, uint16_t cells[MATRIX_SIZE])
{
    uint8_t i;
    uint8_t moved = 0;
    uint16_t packed[MATRIX_SIZE];
    uint8_t count = 0;
    uint8_t last_merge = 0;

    /* 第 1 步：压缩 + 合并 */
    for (i = 0; i < MATRIX_SIZE; i++)
    {
        uint16_t cur_num = cells[i];
        if (cur_num == 0)
        {
            continue;
        }

        if (count > 0 && packed[count - 1] == cur_num && last_merge == 0)
        {
            packed[count - 1] = cur_num + 1;
            *score += 1u << (cur_num + 1);
            last_merge = 1;
            moved = 1;
        }
        else
        {
            packed[count++] = cur_num;
            last_merge = 0;
        }
    }
    /* 第 2 步：更新 cells */
    for (i = 0; i < MATRIX_SIZE; i++)
    {
        uint16_t val = (i < count) ? packed[i] : 0;

        if (val != cells[i])
        {
            moved = 1;
        }

        cells[i] = val;
    }
    return moved;
}

/* ============ 上移：近 = 顶部，i=0 是 matrix[0][y] ============ */
static uint8_t move_up(uint32_t *score, uint16_t matrix[MATRIX_SIZE][MATRIX_SIZE])
{
    uint8_t moved = 0;

    for (uint8_t y = 0; y < MATRIX_SIZE; y++)              /* 一列一列 */
    {
        uint16_t cells[MATRIX_SIZE];

        for (uint8_t i = 0; i < MATRIX_SIZE; i++)          /* 读取：上→下 */
            cells[i] = matrix[i][y];

        moved |= merge_one_line(score, cells);

        for (uint8_t i = 0; i < MATRIX_SIZE; i++)          /* 写回：上→下 */
            matrix[i][y] = cells[i];
    }
    return moved;
}

/* ============ 下移：近 = 底部，i=0 是 matrix[3][y] ============ */
static uint8_t move_down(uint32_t *score, uint16_t matrix[MATRIX_SIZE][MATRIX_SIZE])
{
    uint8_t moved = 0;

    for (uint8_t y = 0; y < MATRIX_SIZE; y++)              /* 一列一列 */
    {
        uint16_t cells[MATRIX_SIZE];

        for (uint8_t i = 0; i < MATRIX_SIZE; i++)          /* 读取：下→上 */
            cells[i] = matrix[MATRIX_SIZE - 1 - i][y];

        moved |= merge_one_line(score, cells);

        for (uint8_t i = 0; i < MATRIX_SIZE; i++)          /* 写回：下→上 */
            matrix[MATRIX_SIZE - 1 - i][y] = cells[i];
    }
    return moved;
}

/* ============ 左移：近 = 左侧，i=0 是 matrix[x][0] ============ */
static uint8_t move_left(uint32_t *score, uint16_t matrix[MATRIX_SIZE][MATRIX_SIZE])
{
    uint8_t moved = 0;

    for (uint8_t x = 0; x < MATRIX_SIZE; x++)              /* 一行一行 */
    {
        uint16_t cells[MATRIX_SIZE];

        for (uint8_t i = 0; i < MATRIX_SIZE; i++)          /* 读取：左→右 */
            cells[i] = matrix[x][i];

        moved |= merge_one_line(score, cells);

        for (uint8_t i = 0; i < MATRIX_SIZE; i++)          /* 写回：左→右 */
            matrix[x][i] = cells[i];
    }
    return moved;
}

/* ============ 右移：近 = 右侧，i=0 是 matrix[x][3] ============ */
static uint8_t move_right(uint32_t *score, uint16_t matrix[MATRIX_SIZE][MATRIX_SIZE])
{
    uint8_t moved = 0;

    for (uint8_t x = 0; x < MATRIX_SIZE; x++)              /* 一行一行 */
    {
        uint16_t cells[MATRIX_SIZE];

        for (uint8_t i = 0; i < MATRIX_SIZE; i++)          /* 读取：右→左 */
            cells[i] = matrix[x][MATRIX_SIZE - 1 - i];

        moved |= merge_one_line(score, cells);

        for (uint8_t i = 0; i < MATRIX_SIZE; i++)          /* 写回：右→左 */
            matrix[x][MATRIX_SIZE - 1 - i] = cells[i];
    }
    return moved;
}

static uint8_t game_over(uint16_t matrix[MATRIX_SIZE][MATRIX_SIZE])
{
    for (uint8_t x = 0; x < MATRIX_SIZE; x++)
    {
        for (uint8_t y = 0; y < MATRIX_SIZE; y++)
        {
            if (matrix[x][y] == 0)
            {
                return 0;
            }
            if (x < MATRIX_SIZE - 1 && matrix[x][y] == matrix[x + 1][y])
            {
                return 0;
            }
            if (y < MATRIX_SIZE - 1 && matrix[x][y] == matrix[x][y + 1])
            {
                return 0;
            }
        }
    }
    return 1;
}
static lv_color_t get_num_color(uint16_t num)
{
    lv_color_t color;

    switch (num)
    {
        case 0:     color = GAME_2048_NUMBER_EMPTY_COLOR;   break;
        case 1:     color = GAME_2048_NUMBER_EMPTY_COLOR;   break;
        case 2:     color = GAME_2048_NUMBER_2_COLOR;       break;
        case 4:     color = GAME_2048_NUMBER_4_COLOR;       break;
        case 8:     color = GAME_2048_NUMBER_8_COLOR;       break;
        case 16:    color = GAME_2048_NUMBER_16_COLOR;      break;
        case 32:    color = GAME_2048_NUMBER_32_COLOR;      break;
        case 64:    color = GAME_2048_NUMBER_64_COLOR;      break;
        case 128:   color = GAME_2048_NUMBER_128_COLOR;     break;
        case 256:   color = GAME_2048_NUMBER_256_COLOR;     break;
        case 512:   color = GAME_2048_NUMBER_512_COLOR;     break;
        case 1024:  color = GAME_2048_NUMBER_1024_COLOR;    break;
        case 2048:  color = GAME_2048_NUMBER_2048_COLOR;    break;
        default:    color = GAME_2048_NUMBER_2048_COLOR;    break;
    }

    return color;
}

// static void Stack_Init(Game2048Stack_t * stack)
// {
//     memset(stack->Data, 0, sizeof(stack->Data));
//     stack->Top_Point = 0;
// }

// static uint8_t Stack_Push(Game2048Stack_t * stack, uint16_t dataIn)
// {
//     if(stack->Top_Point >= STACK_DEPTH)
//     {
//         return -1;
//     }
//     stack->Data[stack->Top_Point++] = dataIn;
//     return 0;
// }

// static uint16_t Stack_Pop(Game2048Stack_t * stack)
// {
//     if(stack->Top_Point == 0)
//     {
//         return -1;
//     }
//     return stack->Data[--stack->Top_Point];
// }

// static uint8_t Stack_IsEmpty(Game2048Stack_t * stack)
// {
//     return stack->Top_Point == 0;
// }

// static void Stack_Clear(Game2048Stack_t * stack)
// {
//     while (!Stack_IsEmpty(stack))
//     {
//         Stack_Pop(stack);
//     }
// }
