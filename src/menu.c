#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tool.h"
#include "storage.h"
#include "borrow.h"
#include "statistics.h"
#include "menu.h"

#define MAX_TOOLS 200
#define MAX_RECORDS 500
#define TOOLS_FILE "data/tools.dat"
#define RECORDS_FILE "data/borrow_records.dat"
#define ADMIN_USER "admin"
#define ADMIN_PASS "admin123"
#define MAX_LOGIN_ATTEMPTS 3

/* 用户名密码验证，最多 3 次尝试 */
int login(void)
{
    char user[32], pass[32];
    int attempts = 0;

    printf("========================================\n");
    printf("   工厂工具管理信息系统 - 登录\n");
    printf("========================================\n");

    while (attempts < MAX_LOGIN_ATTEMPTS)
    {
        printf("用户名：");
        scanf("%31s", user);
        while (getchar() != '\n');

        printf("密码：");
        scanf("%31s", pass);
        while (getchar() != '\n');

        if (strcmp(user, ADMIN_USER) == 0 && strcmp(pass, ADMIN_PASS) == 0)
        {
            printf("登录成功！\n\n");
            return 1;
        }

        attempts++;
        printf("用户名或密码错误，还剩 %d 次机会。\n", MAX_LOGIN_ATTEMPTS - attempts);
    }

    printf("登录失败次数过多，系统退出。\n");
    return 0;
}

/* 显示主菜单 */
void show_main_menu(void)
{
    printf("\n");
    printf("========================================\n");
    printf("         工具管理 - 主菜单\n");
    printf("========================================\n");
    printf("  1. 显示所有工具\n");
    printf("  2. 添加工具\n");
    printf("  3. 删除工具\n");
    printf("  4. 修改工具\n");
    printf("  5. 查询工具\n");
    printf("  6. 借用工具\n");
    printf("  7. 归还工具\n");
    printf("  8. 查看借还记录\n");
    printf("  9. 库存统计\n");
    printf(" 10. 低库存预警\n");
    printf(" 11. 借用排行榜\n");
    printf("  0. 退出系统\n");
    printf("========================================\n");
    printf("请选择：");
}

/* 主循环：登录 → 加载数据 → 菜单调度 → 退出保存 */
void run_system(void)
{
    if (!login())
        return;

    Tool tools[MAX_TOOLS];
    int tool_count = 0;

    BorrowRecord records[MAX_RECORDS];
    int record_count = 0;

    /* 加载已有数据 */
    if (load_tools(TOOLS_FILE, tools, &tool_count, MAX_TOOLS))
        printf("已加载 %d 条工具记录。\n", tool_count);
    else
        printf("未找到工具数据文件，将从零开始。\n");

    if (load_records(RECORDS_FILE, records, &record_count, MAX_RECORDS))
        printf("已加载 %d 条借还记录。\n", record_count);
    else
        printf("未找到借还记录文件，将从零开始。\n");

    int running = 1;
    while (running)
    {
        show_main_menu();

        int choice;
        if (scanf("%d", &choice) != 1)
        {
            printf("输入无效，请重新选择。\n");
            while (getchar() != '\n');
            continue;
        }
        while (getchar() != '\n');

        char id[TOOL_ID_LEN];
        int threshold;

        switch (choice)
        {
        case 1:
            show_all_tools(tools, tool_count);
            break;
        case 2:
            add_tool(tools, &tool_count, MAX_TOOLS);
            break;
        case 3:
            printf("请输入要删除的工具编号：");
            scanf("%19s", id);
            while (getchar() != '\n');
            delete_tool(tools, &tool_count, id);
            break;
        case 4:
            printf("请输入要修改的工具编号：");
            scanf("%19s", id);
            while (getchar() != '\n');
            update_tool(tools, tool_count, id);
            break;
        case 5:
            search_tools(tools, tool_count);
            break;
        case 6:
            borrow_tool(tools, tool_count, records, &record_count, MAX_RECORDS);
            break;
        case 7:
            return_tool(tools, tool_count, records, record_count);
            break;
        case 8:
            show_all_records(records, record_count);
            break;
        case 9:
            show_inventory_summary(tools, tool_count);
            break;
        case 10:
            printf("请输入安全库存阈值：");
            if (scanf("%d", &threshold) != 1 || threshold < 0)
            {
                printf("阈值必须为大于等于 0 的整数。\n");
                while (getchar() != '\n');
            }
            else
            {
                while (getchar() != '\n');
                show_low_stock_tools(tools, tool_count, threshold);
            }
            break;
        case 11:
            show_borrow_ranking(tools, tool_count, records, record_count);
            break;
        case 0:
            if (save_tools(TOOLS_FILE, tools, tool_count))
                printf("工具数据已保存（%d 条记录）。\n", tool_count);
            else
                printf("工具数据保存失败！\n");

            if (save_records(RECORDS_FILE, records, record_count))
                printf("借还记录已保存（%d 条记录）。\n", record_count);
            else
                printf("借还记录保存失败！\n");

            running = 0;
            break;
        default:
            printf("无效选项，请重新选择。\n");
            break;
        }
    }

    printf("系统已退出，再见。\n");
}
