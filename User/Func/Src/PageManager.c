#include "PageManager.h"
#include "ui_HomePage.h"
#include "ui_MenuPage.h"

PageStack_t PageStack;

/** @brief 初始化页面栈
 *  @param Stack 页面栈指针
 *  @note 初始化页面栈，将栈顶指针设为0，所有页面指针设为NULL
 */
static void PageStack_Init(PageStack_t* Stack)
{
    Stack->top = 0;
    for (uint8_t i = 0; i < MAX_DEPTH; i++)
    {
        Stack->Pages[i] = NULL;
    }
}

/** @brief 压入页面栈
 *  @param Stack 页面栈指针
 *  @note 压栈操作，将页面指针压入栈中，栈顶指针增加1
 *  @param page 页面指针
 *  @return 0 成功
 *  @return -1 栈满
 */
static uint8_t PageStack_Push(PageStack_t* Stack, Page_t* page)
{
    if (Stack->top < MAX_DEPTH)
    {
        Stack->Pages[Stack->top] = page;
        Stack->top++;
        return 0;
    }
    return -1; // 栈满
}

/** @brief 弹出页面栈
 *  @param Stack 页面栈指针
 *  @note 弹栈操作，将栈顶页面指针弹出栈中，栈顶指针减少1
 *  @return 0 成功
 *  @return -1 栈空
 */
static uint8_t PageStack_Pop(PageStack_t* Stack)
{
    if (Stack->top > 0)
    {
        Stack->top--;
        if (Stack->Pages[Stack->top] != NULL) 
        {
            Stack->Pages[Stack->top]->page_deinit();
            Stack->Pages[Stack->top] = NULL;
        }
        return 0;
    }
    return -1; // 栈空
}

/** @brief 判断页面栈是否为空
 *  @param Stack 页面栈指针
 *  @return 0 栈空
 *  @return 1 栈非空
 */
static uint8_t PageStack_isEmpty(PageStack_t* Stack)
{
    return Stack->top == 0;
}

/** @brief 获取页面栈栈顶页面指针
 *  @param Stack 页面栈指针
 *  @return 栈顶页面指针
 */
static Page_t* PageStack_GetTopPage(PageStack_t* Stack)
{
    if (Stack->top == 0)
    {
        return NULL;
    }
    return Stack->Pages[Stack->top - 1];
}

/** @brief 获取当前页面指针
 *  @return 当前页面指针
 */
Page_t* Page_Get_NowPage(void)
{
    return PageStack_GetTopPage(&PageStack);
}

/** @brief 返回上一页
 *  @note 返回上一页，将当前页面栈弹出，切换到栈顶页面
 */
void Page_Back(void)
{
    if (PageStack_isEmpty(&PageStack))
        return;

    PageStack_Pop(&PageStack);

    if (PageStack_isEmpty(&PageStack))
    {
        PageStack_Push(&PageStack, &Page_Home);
        PageStack_Push(&PageStack, &Page_Menu);
    }

    Page_t *nowPage = PageStack_GetTopPage(&PageStack);

    nowPage->page_init();

    lv_scr_load_anim(*nowPage->page_obj,
                     LV_SCR_LOAD_ANIM_MOVE_RIGHT,
                     100, 0, true);
}

/** @brief 返回到最底部页面
 *  @note 返回到最底部页面，将当前页面栈弹出，切换到栈顶页面
 */
void Page_Back_Bottom(void)
{
    if (PageStack_isEmpty(&PageStack))
    {
        return;
    }

    while (PageStack.top > 1)
    {
        PageStack_Pop(&PageStack);
    }
    
    Page_t* nowPage = PageStack.Pages[PageStack.top - 1];
    nowPage->page_init();
    // 切换动画
    lv_scr_load_anim(*nowPage->page_obj, LV_SCR_LOAD_ANIM_MOVE_RIGHT, 100, 0, true);
}

/** @brief 加载新页面
 *  @param page 新页面指针
 *  @note 加载新页面，将新页面压入栈中，切换到新页面
 */
void Page_Load(Page_t* page)
{
    if (PageStack.top >= MAX_DEPTH)
    {
        return;
    }

    // 如果堆栈非空，反初始化当前页面
    if(!PageStack_isEmpty(&PageStack))
    {
        PageStack_GetTopPage(&PageStack)->page_deinit();
    }

    // 压入新页面栈
    PageStack_Push(&PageStack, page);
    // 初始化新页面
    page->page_init();
    // 切换动画
    lv_scr_load_anim(*page->page_obj, LV_SCR_LOAD_ANIM_MOVE_RIGHT, 100, 0, true);
}

/** @brief 初始化页面栈
 *  @note 初始化页面栈，将栈顶指针设为0，所有页面指针设为NULL
 */
void Pages_Init(void)
{
    PageStack_Init(&PageStack);
    // 初始化首页
    PageStack_Push(&PageStack, &Page_Home);
    Page_t* nowPage = PageStack_GetTopPage(&PageStack);
    nowPage->page_init();
    lv_disp_load_scr(*nowPage->page_obj);
}
