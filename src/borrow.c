#include <stdio.h>
#include <string.h>
#include "borrow.h"

/* 生成新记录编号：无记录时返回 1，否则返回当前最大编号加 1 */
int generate_record_id(BorrowRecord records[], int record_count)
{
    if (record_count == 0)
        return 1;

    int max_id = records[0].record_id;
    for (int i = 1; i < record_count; i++)
    {
        if (records[i].record_id > max_id)
            max_id = records[i].record_id;
    }
    return max_id + 1;
}

/* 按记录编号查找下标，未找到返回 -1 */
int find_record_index_by_id(BorrowRecord records[], int record_count, int record_id)
{
    for (int i = 0; i < record_count; i++)
    {
        if (records[i].record_id == record_id)
            return i;
    }
    return -1;
}

/* 借用工具：校验工具存在性、状态、库存后生成借用记录，并扣减库存 */
int borrow_tool(Tool tools[], int tool_count,
                BorrowRecord records[], int *record_count, int max_records)
{
    if (*record_count >= max_records)
    {
        printf("借还记录已满，无法继续借用。\n");
        return 0;
    }

    char tool_id[TOOL_ID_LEN];
    printf("请输入要借用的工具编号：");
    scanf("%19s", tool_id);
    while (getchar() != '\n');

    int tool_idx = find_tool_index_by_id(tools, tool_count, tool_id);
    if (tool_idx == -1)
    {
        printf("工具编号 %s 不存在。\n", tool_id);
        return 0;
    }

    if (strcmp(tools[tool_idx].status, "损坏") == 0)
    {
        printf("工具 %s 当前状态为「损坏」，无法借用。\n", tools[tool_idx].name);
        return 0;
    }

    int quantity;
    printf("请输入借用数量：");
    if (scanf("%d", &quantity) != 1)
    {
        printf("数量输入非法。\n");
        while (getchar() != '\n');
        return 0;
    }
    while (getchar() != '\n');

    if (quantity <= 0)
    {
        printf("借用数量必须大于 0。\n");
        return 0;
    }
    if (quantity > tools[tool_idx].stock)
    {
        printf("借用数量（%d）超过当前库存（%d）。\n", quantity, tools[tool_idx].stock);
        return 0;
    }

    BorrowRecord r;
    r.record_id = generate_record_id(records, *record_count);
    strncpy(r.tool_id, tool_id, TOOL_ID_LEN - 1);
    r.tool_id[TOOL_ID_LEN - 1] = '\0';

    printf("请输入借用人姓名：");
    scanf("%29s", r.borrower_name);
    while (getchar() != '\n');
    if (strlen(r.borrower_name) == 0)
    {
        printf("借用人姓名不能为空。\n");
        return 0;
    }

    printf("请输入借用日期（YYYY-MM-DD）：");
    scanf("%10s", r.borrow_date);
    while (getchar() != '\n');

    printf("请输入应还日期（YYYY-MM-DD）：");
    scanf("%10s", r.due_date);
    while (getchar() != '\n');

    r.quantity = quantity;
    strncpy(r.status, "已借", BORROW_STATUS_LEN - 1);
    r.status[BORROW_STATUS_LEN - 1] = '\0';

    /* 更新工具库存 */
    tools[tool_idx].stock -= quantity;
    tools[tool_idx].borrowed_count += quantity;

    records[*record_count] = r;
    (*record_count)++;

    printf("借用成功！记录编号：%d，工具：%s，数量：%d\n",
           r.record_id, tools[tool_idx].name, quantity);
    return 1;
}

/* 归还工具：校验记录存在性及状态后恢复库存 */
int return_tool(Tool tools[], int tool_count,
                BorrowRecord records[], int record_count)
{
    int record_id;
    printf("请输入借还记录编号：");
    if (scanf("%d", &record_id) != 1)
    {
        printf("记录编号输入非法。\n");
        while (getchar() != '\n');
        return 0;
    }
    while (getchar() != '\n');

    int rec_idx = find_record_index_by_id(records, record_count, record_id);
    if (rec_idx == -1)
    {
        printf("未找到编号为 %d 的借还记录。\n", record_id);
        return 0;
    }

    if (strcmp(records[rec_idx].status, "已还") == 0)
    {
        printf("记录 %d 已经归还，不能重复归还。\n", record_id);
        return 0;
    }

    int tool_idx = find_tool_index_by_id(tools, tool_count, records[rec_idx].tool_id);
    if (tool_idx == -1)
    {
        printf("记录 %d 对应的工具 %s 不存在，无法归还。\n",
               record_id, records[rec_idx].tool_id);
        return 0;
    }

    /* 恢复库存 */
    tools[tool_idx].stock += records[rec_idx].quantity;
    tools[tool_idx].borrowed_count -= records[rec_idx].quantity;

    /* 更新记录状态 */
    strncpy(records[rec_idx].status, "已还", BORROW_STATUS_LEN - 1);
    records[rec_idx].status[BORROW_STATUS_LEN - 1] = '\0';

    printf("归还成功！记录编号：%d，工具：%s，数量：%d\n",
           record_id, tools[tool_idx].name, records[rec_idx].quantity);
    return 1;
}

/* 列表展示全部借还记录 */
void show_all_records(BorrowRecord records[], int record_count)
{
    if (record_count == 0)
    {
        printf("暂无借还记录。\n");
        return;
    }

    printf("\n");
    printf("%-8s %-12s %-12s %-12s %-12s %6s %6s\n",
           "记录ID", "工具编号", "借用人", "借用日期", "应还日期", "数量", "状态");
    printf("------------------------------------------------------------------------------------------------\n");

    for (int i = 0; i < record_count; i++)
    {
        printf("%-8d %-12s %-12s %-12s %-12s %6d %6s\n",
               records[i].record_id, records[i].tool_id, records[i].borrower_name,
               records[i].borrow_date, records[i].due_date,
               records[i].quantity, records[i].status);
    }
    printf("------------------------------------------------------------------------------------------------\n");
    printf("共 %d 条记录。\n", record_count);
}

/* 从二进制文件加载借还记录 */
int load_records(const char filename[], BorrowRecord records[], int *record_count, int max_records)
{
    FILE *fp = fopen(filename, "rb");
    if (fp == NULL)
        return 0;

    int count;
    if (fread(&count, sizeof(int), 1, fp) != 1)
    {
        fclose(fp);
        return 0;
    }

    if (count < 0)
    {
        fclose(fp);
        return 0;
    }

    int load_count = count < max_records ? count : max_records;
    for (int i = 0; i < load_count; i++)
    {
        if (fread(&records[i], sizeof(BorrowRecord), 1, fp) != 1)
        {
            fclose(fp);
            return 0;
        }
    }

    *record_count = load_count;
    fclose(fp);
    return 1;
}

/* 保存借还记录到二进制文件 */
int save_records(const char filename[], BorrowRecord records[], int record_count)
{
    FILE *fp = fopen(filename, "wb");
    if (fp == NULL)
        return 0;

    if (fwrite(&record_count, sizeof(int), 1, fp) != 1)
    {
        fclose(fp);
        return 0;
    }

    for (int i = 0; i < record_count; i++)
    {
        if (fwrite(&records[i], sizeof(BorrowRecord), 1, fp) != 1)
        {
            fclose(fp);
            return 0;
        }
    }

    fclose(fp);
    return 1;
}
