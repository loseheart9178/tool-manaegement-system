#include <stdio.h>
#include <string.h>
#include "borrow.h"

int main(void)
{
    BorrowRecord records[] = {
        {1, "T001", "张三", "2026-05-01", "2026-06-01", 2, "已借"},
        {2, "T002", "李四", "2026-05-10", "2026-06-10", 1, "已还"},
        {3, "T003", "王五", "2026-05-15", "2026-06-15", 3, "已借"},
        {4, "T005", "赵六", "2026-05-20", "2026-06-20", 5, "已借"},
        {5, "T001", "李四", "2026-04-01", "2026-05-01", 2, "已还"},
        {6, "T003", "张三", "2026-04-15", "2026-05-15", 1, "已还"},
        {7, "T006", "王五", "2026-06-01", "2026-07-01", 2, "已借"},
        {8, "T008", "赵六", "2026-06-05", "2026-07-05", 1, "已借"},
    };

    int record_count = sizeof(records) / sizeof(records[0]);

    FILE *fp = fopen("data/borrow_records.dat", "wb");
    if (fp == NULL)
    {
        printf("无法创建文件 data/borrow_records.dat\n");
        return 1;
    }

    fwrite(&record_count, sizeof(int), 1, fp);
    for (int i = 0; i < record_count; i++)
    {
        fwrite(&records[i], sizeof(BorrowRecord), 1, fp);
    }

    fclose(fp);
    printf("已生成 data/borrow_records.dat，包含 %d 条借还记录。\n", record_count);
    return 0;
}
