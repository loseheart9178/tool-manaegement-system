#include <stdio.h>
#include <string.h>
#include "tool.h"

/* ---- 表格对齐辅助函数（按显示宽度而非字节数对齐） ---- */

/* 计算 UTF-8 字符串的显示宽度：ASCII 为 1，中文等宽字符为 2 */
static int disp_width(const char *s)
{
    int w = 0;
    while (*s)
    {
        if ((*s & 0x80) == 0)           /* 单字节 ASCII */
        {
            w++;
            s++;
        }
        else if ((*s & 0xE0) == 0xC0)   /* 双字节 UTF-8 */
        {
            w += 2;
            s += 2;
        }
        else if ((*s & 0xF0) == 0xE0)   /* 三字节 UTF-8（中文） */
        {
            w += 2;
            s += 3;
        }
        else if ((*s & 0xF8) == 0xF0)   /* 四字节 UTF-8 */
        {
            w += 2;
            s += 4;
        }
        else                             /* 非法序列兜底 */
        {
            w++;
            s++;
        }
    }
    return w;
}

/* 左对齐打印字符串，用空格补齐到 field_w 个显示列 */
static void print_left(const char *s, int field_w)
{
    int dw = disp_width(s);
    fputs(s, stdout);
    for (int i = dw; i < field_w; i++) putchar(' ');
}

/* 右对齐打印字符串，用空格补齐到 field_w 个显示列 */
static void print_right(const char *s, int field_w)
{
    int dw = disp_width(s);
    for (int i = dw; i < field_w; i++) putchar(' ');
    fputs(s, stdout);
}

/* ---- 工具管理核心函数 ---- */

/* 按编号精确查找工具下标，未找到返回 -1。该函数会被同学 B 调用，签名不得修改。 */
int find_tool_index_by_id(Tool tools[], int tool_count, const char tool_id[])
{
    for (int i = 0; i < tool_count; i++)
    {
        if (strcmp(tools[i].id, tool_id) == 0)
            return i;
    }
    return -1;
}

/* 列表展示全部工具信息 */
void show_all_tools(Tool tools[], int tool_count)
{
    if (tool_count == 0)
    {
        printf("暂无工具记录。\n");
        return;
    }

    printf("\n");
    /* 表头 */
    print_left("编号", 8);   putchar(' ');
    print_left("名称", 26);  putchar(' ');
    print_left("类型", 12);  putchar(' ');
    print_right("库存", 6);  putchar(' ');
    print_right("状态", 6);  putchar(' ');
    print_right("已借出", 6); putchar(' ');
    print_left("位置", 8);   putchar(' ');
    print_right("单价", 10); putchar('\n');

    /* 分隔线 */
    for (int i = 0; i < 89; i++) putchar('-');
    putchar('\n');

    for (int i = 0; i < tool_count; i++)
    {
        print_left(tools[i].id, 8);    putchar(' ');
        print_left(tools[i].name, 26); putchar(' ');
        print_left(tools[i].type, 12); putchar(' ');
        printf("%6d", tools[i].stock); putchar(' ');
        print_right(tools[i].status, 6); putchar(' ');
        printf("%6d", tools[i].borrowed_count); putchar(' ');
        print_left(tools[i].location, 8); putchar(' ');
        printf("%10.2f\n", tools[i].price);
    }

    /* 底部分隔线 */
    for (int i = 0; i < 89; i++) putchar('-');
    putchar('\n');
    printf("共 %d 条记录。\n", tool_count);
}

/* 添加新工具：录入各字段，做去重和数值合法性校验 */
int add_tool(Tool tools[], int *tool_count, int max_tools)
{
    if (*tool_count >= max_tools)
    {
        printf("工具数组已满，无法继续添加。\n");
        return 0;
    }

    Tool t;

    /* 编号 */
    printf("请输入工具编号：");
    scanf("%19s", t.id);
    while (getchar() != '\n');

    if (strlen(t.id) == 0)
    {
        printf("编号不能为空。\n");
        return 0;
    }
    if (find_tool_index_by_id(tools, *tool_count, t.id) != -1)
    {
        printf("编号 %s 已存在，添加失败。\n", t.id);
        return 0;
    }

    /* 名称 */
    printf("请输入工具名称：");
    scanf("%49s", t.name);
    while (getchar() != '\n');
    if (strlen(t.name) == 0)
    {
        printf("名称不能为空。\n");
        return 0;
    }

    /* 类型 */
    printf("请输入工具类型（如 电动工具/测量工具/维修工具）：");
    scanf("%29s", t.type);
    while (getchar() != '\n');

    /* 库存 */
    printf("请输入库存数量：");
    if (scanf("%d", &t.stock) != 1 || t.stock < 0)
    {
        printf("库存必须为大于等于 0 的整数。\n");
        while (getchar() != '\n');
        return 0;
    }
    while (getchar() != '\n');

    /* 状态 */
    printf("请输入工具状态（正常/损坏）：");
    scanf("%9s", t.status);
    while (getchar() != '\n');
    if (strcmp(t.status, "正常") != 0 && strcmp(t.status, "损坏") != 0)
    {
        printf("状态只允许「正常」或「损坏」。\n");
        return 0;
    }

    /* 新工具已借出数量固定为 0 */
    t.borrowed_count = 0;

    /* 位置 */
    printf("请输入存放位置（如 A01）：");
    scanf("%49s", t.location);
    while (getchar() != '\n');

    /* 单价 */
    printf("请输入工具单价：");
    if (scanf("%lf", &t.price) != 1 || t.price < 0)
    {
        printf("单价必须为大于等于 0 的数值。\n");
        while (getchar() != '\n');
        return 0;
    }
    while (getchar() != '\n');

    tools[*tool_count] = t;
    (*tool_count)++;
    printf("工具 %s 添加成功。\n", t.id);
    return 1;
}

/* 按编号删除工具，已借出的工具禁止删除，删除前需二次确认 */
int delete_tool(Tool tools[], int *tool_count, const char tool_id[])
{
    int idx = find_tool_index_by_id(tools, *tool_count, tool_id);
    if (idx == -1)
    {
        printf("未找到编号为 %s 的工具。\n", tool_id);
        return 0;
    }

    if (tools[idx].borrowed_count > 0)
    {
        printf("工具 %s 当前已借出 %d 件，无法删除。请先归还后再操作。\n",
               tools[idx].id, tools[idx].borrowed_count);
        return 0;
    }

    char confirm;
    printf("确定要删除工具 %s（%s）吗？(y/n)：", tools[idx].id, tools[idx].name);
    scanf("%c", &confirm);
    while (getchar() != '\n');

    if (confirm != 'y' && confirm != 'Y')
    {
        printf("已取消删除。\n");
        return 0;
    }

    /* 将后续元素前移 */
    for (int i = idx; i < *tool_count - 1; i++)
        tools[i] = tools[i + 1];

    (*tool_count)--;
    printf("工具 %s 已删除。\n", tool_id);
    return 1;
}

/* 按编号修改工具信息，编号不可修改，库存不能为负 */
int update_tool(Tool tools[], int tool_count, const char tool_id[])
{
    int idx = find_tool_index_by_id(tools, tool_count, tool_id);
    if (idx == -1)
    {
        printf("未找到编号为 %s 的工具。\n", tool_id);
        return 0;
    }

    Tool *t = &tools[idx];
    char buf[256];

    printf("\n当前工具信息：\n");
    printf("  名称: %s\n", t->name);
    printf("  类型: %s\n", t->type);
    printf("  库存: %d\n", t->stock);
    printf("  状态: %s\n", t->status);
    printf("  位置: %s\n", t->location);
    printf("  单价: %.2f\n", t->price);
    printf("（直接回车保留原值）\n\n");

    /* 名称 */
    printf("新名称：");
    fgets(buf, sizeof(buf), stdin);
    if (buf[0] != '\n')
    {
        buf[strcspn(buf, "\n")] = '\0';
        if (strlen(buf) > 0)
            strncpy(t->name, buf, TOOL_NAME_LEN - 1);
    }

    /* 类型 */
    printf("新类型：");
    fgets(buf, sizeof(buf), stdin);
    if (buf[0] != '\n')
    {
        buf[strcspn(buf, "\n")] = '\0';
        if (strlen(buf) > 0)
            strncpy(t->type, buf, TOOL_TYPE_LEN - 1);
    }

    /* 库存 */
    printf("新库存数量：");
    fgets(buf, sizeof(buf), stdin);
    if (buf[0] != '\n')
    {
        int new_stock;
        if (sscanf(buf, "%d", &new_stock) == 1 && new_stock >= 0)
            t->stock = new_stock;
        else
            printf("输入非法，库存未修改。\n");
    }

    /* 状态 */
    printf("新状态（正常/损坏）：");
    fgets(buf, sizeof(buf), stdin);
    if (buf[0] != '\n')
    {
        buf[strcspn(buf, "\n")] = '\0';
        if (strcmp(buf, "正常") == 0 || strcmp(buf, "损坏") == 0)
            strncpy(t->status, buf, TOOL_STATUS_LEN - 1);
        else
            printf("状态非法，未修改。\n");
    }

    /* 位置 */
    printf("新位置：");
    fgets(buf, sizeof(buf), stdin);
    if (buf[0] != '\n')
    {
        buf[strcspn(buf, "\n")] = '\0';
        if (strlen(buf) > 0)
            strncpy(t->location, buf, TOOL_LOCATION_LEN - 1);
    }

    /* 单价 */
    printf("新单价：");
    fgets(buf, sizeof(buf), stdin);
    if (buf[0] != '\n')
    {
        double new_price;
        if (sscanf(buf, "%lf", &new_price) == 1 && new_price >= 0)
            t->price = new_price;
        else
            printf("输入非法，单价未修改。\n");
    }

    printf("工具 %s 修改成功。\n", tool_id);
    return 1;
}

/* 支持按编号（精确）、名称（模糊）、类型（模糊）查询工具 */
void search_tools(Tool tools[], int tool_count)
{
    int choice;
    char keyword[TOOL_NAME_LEN];

    printf("\n查询方式：\n");
    printf("  1. 按编号查询（精确）\n");
    printf("  2. 按名称查询（模糊）\n");
    printf("  3. 按类型查询（模糊）\n");
    printf("请选择：");
    if (scanf("%d", &choice) != 1)
    {
        while (getchar() != '\n');
        return;
    }
    while (getchar() != '\n');

    printf("请输入关键字：");
    scanf("%49s", keyword);
    while (getchar() != '\n');

    printf("\n");
    /* 表头 */
    print_left("编号", 8);   putchar(' ');
    print_left("名称", 26);  putchar(' ');
    print_left("类型", 12);  putchar(' ');
    print_right("库存", 6);  putchar(' ');
    print_right("状态", 6);  putchar(' ');
    print_right("已借出", 6); putchar(' ');
    print_left("位置", 8);   putchar(' ');
    print_right("单价", 10); putchar('\n');

    /* 分隔线 */
    for (int i = 0; i < 89; i++) putchar('-');
    putchar('\n');

    int found = 0;
    for (int i = 0; i < tool_count; i++)
    {
        int match = 0;
        switch (choice)
        {
        case 1: match = (strcmp(tools[i].id, keyword) == 0); break;
        case 2: match = (strstr(tools[i].name, keyword) != NULL); break;
        case 3: match = (strstr(tools[i].type, keyword) != NULL); break;
        default: return;
        }
        if (match)
        {
            print_left(tools[i].id, 8);    putchar(' ');
            print_left(tools[i].name, 26); putchar(' ');
            print_left(tools[i].type, 12); putchar(' ');
            printf("%6d", tools[i].stock); putchar(' ');
            print_right(tools[i].status, 6); putchar(' ');
            printf("%6d", tools[i].borrowed_count); putchar(' ');
            print_left(tools[i].location, 8); putchar(' ');
            printf("%10.2f\n", tools[i].price);
            found++;
        }
    }

    if (found == 0)
        printf("未找到匹配的工具。\n");
    else
    {
        for (int i = 0; i < 89; i++) putchar('-');
        putchar('\n');
        printf("共找到 %d 条匹配记录。\n", found);
    }
}
