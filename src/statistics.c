#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "statistics.h"

/* 库存整体统计：库存总数量、在库数量、已借数量、总价值 */
void show_inventory_summary(Tool tools[], int tool_count)
{
    int total_stock = 0;
    int total_in_library = 0;
    int total_borrowed = 0;
    double total_value = 0.0;

    for (int i = 0; i < tool_count; i++)
    {
        int total = tools[i].stock + tools[i].borrowed_count;
        total_stock += total;
        total_in_library += tools[i].stock;
        total_borrowed += tools[i].borrowed_count;
        total_value += total * tools[i].price;
    }

    printf("\n");
    printf("========================================\n");
    printf("           库存整体统计\n");
    printf("========================================\n");
    printf("  工具种类数：    %d 种\n", tool_count);
    printf("  库存总数量：    %d 件\n", total_stock);
    printf("  当前在库数量：  %d 件\n", total_in_library);
    printf("  已借出数量：    %d 件\n", total_borrowed);
    printf("  库存总价值：    %.2f 元\n", total_value);
    printf("========================================\n");
}

/* 低库存预警：列出库存低于阈值的工具 */
void show_low_stock_tools(Tool tools[], int tool_count, int threshold)
{
    printf("\n低库存预警（阈值：%d 件）：\n", threshold);
    printf("%-12s %-30s %-12s %6s %6s %-10s\n",
           "编号", "名称", "类型", "库存", "已借出", "位置");
    printf("----------------------------------------------------------------------------------------------\n");

    int found = 0;
    for (int i = 0; i < tool_count; i++)
    {
        if (tools[i].stock < threshold)
        {
            printf("%-12s %-30s %-12s %6d %6d %-10s\n",
                   tools[i].id, tools[i].name, tools[i].type,
                   tools[i].stock, tools[i].borrowed_count, tools[i].location);
            found++;
        }
    }

    if (found == 0)
        printf("  所有工具库存均 >= %d 件，无低库存风险。\n", threshold);
    else
    {
        printf("----------------------------------------------------------------------------------------------\n");
        printf("  共 %d 种工具库存低于 %d 件。\n", found, threshold);
    }
}

/* 统计每种工具的借用次数并按降序排列 */
typedef struct
{
    char tool_id[TOOL_ID_LEN];
    int count;
} RankEntry;

static int rank_cmp(const void *a, const void *b)
{
    return ((const RankEntry *)b)->count - ((const RankEntry *)a)->count;
}

void show_borrow_ranking(Tool tools[], int tool_count, BorrowRecord records[], int record_count)
{
    if (tool_count == 0)
    {
        printf("暂无工具数据。\n");
        return;
    }

    /* 初始化排行榜，每种工具初始计数为 0 */
    RankEntry *ranking = malloc(tool_count * sizeof(RankEntry));
    if (ranking == NULL)
    {
        printf("内存分配失败。\n");
        return;
    }

    for (int i = 0; i < tool_count; i++)
    {
        strncpy(ranking[i].tool_id, tools[i].id, TOOL_ID_LEN - 1);
        ranking[i].tool_id[TOOL_ID_LEN - 1] = '\0';
        ranking[i].count = 0;
    }

    /* 遍历所有借还记录，累计每种工具的借用次数 */
    for (int i = 0; i < record_count; i++)
    {
        for (int j = 0; j < tool_count; j++)
        {
            if (strcmp(records[i].tool_id, ranking[j].tool_id) == 0)
            {
                ranking[j].count++;
                break;
            }
        }
    }

    /* 按借用次数降序排序 */
    qsort(ranking, tool_count, sizeof(RankEntry), rank_cmp);

    printf("\n");
    printf("========================================\n");
    printf("         工具借用次数排行榜\n");
    printf("========================================\n");
    printf("%-6s %-12s %-30s %-12s %8s\n", "排名", "编号", "名称", "类型", "借用次数");
    printf("----------------------------------------------------------------------------------------------\n");

    int rank = 0;
    for (int i = 0; i < tool_count; i++)
    {
        if (ranking[i].count == 0)
            continue;

        rank++;
        int tool_idx = find_tool_index_by_id(tools, tool_count, ranking[i].tool_id);
        if (tool_idx != -1)
        {
            printf("%-6d %-12s %-30s %-12s %8d\n",
                   rank, tools[tool_idx].id, tools[tool_idx].name, tools[tool_idx].type,
                   ranking[i].count);
        }
    }

    if (rank == 0)
        printf("  暂无借用记录。\n");
    else
    {
        printf("----------------------------------------------------------------------------------------------\n");
        printf("  共 %d 种工具有借用记录。\n", rank);
    }

    free(ranking);
}
