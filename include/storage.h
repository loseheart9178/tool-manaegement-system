#ifndef STORAGE_H
#define STORAGE_H

#include "tool.h"

int load_tools(const char filename[], Tool tools[], int *tool_count, int max_tools);
int save_tools(const char filename[], Tool tools[], int tool_count);

#endif
