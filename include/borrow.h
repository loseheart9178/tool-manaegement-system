#ifndef BORROW_H
#define BORROW_H

#define BORROWER_NAME_LEN 30
#define DATE_LEN 11
#define BORROW_STATUS_LEN 10

typedef struct
{
    int record_id;
    char tool_id[20];
    char borrower_name[BORROWER_NAME_LEN];
    char borrow_date[DATE_LEN];
    char due_date[DATE_LEN];
    int quantity;
    char status[BORROW_STATUS_LEN];
} BorrowRecord;

#include "tool.h"

/* ---- 借还管理接口 ---- */
int borrow_tool(Tool tools[], int tool_count, BorrowRecord records[], int *record_count, int max_records);
int return_tool(Tool tools[], int tool_count, BorrowRecord records[], int record_count);
void show_all_records(BorrowRecord records[], int record_count);
int find_record_index_by_id(BorrowRecord records[], int record_count, int record_id);
int generate_record_id(BorrowRecord records[], int record_count);

/* ---- 借还记录文件读写 ---- */
int load_records(const char filename[], BorrowRecord records[], int *record_count, int max_records);
int save_records(const char filename[], BorrowRecord records[], int record_count);

/* ---- 统计分析接口 ---- */
void show_inventory_summary(Tool tools[], int tool_count);
void show_low_stock_tools(Tool tools[], int tool_count, int threshold);
void show_borrow_ranking(Tool tools[], int tool_count, BorrowRecord records[], int record_count);

#endif
