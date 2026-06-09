#ifndef STATISTICS_H
#define STATISTICS_H

#include "tool.h"
#include "borrow.h"

void show_inventory_summary(Tool tools[], int tool_count);
void show_low_stock_tools(Tool tools[], int tool_count, int threshold);
void show_borrow_ranking(Tool tools[], int tool_count, BorrowRecord records[], int record_count);

#endif
