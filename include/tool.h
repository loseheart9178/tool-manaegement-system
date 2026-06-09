#ifndef TOOL_H
#define TOOL_H

#define TOOL_ID_LEN 20
#define TOOL_NAME_LEN 50
#define TOOL_TYPE_LEN 30
#define TOOL_STATUS_LEN 10
#define TOOL_LOCATION_LEN 50

typedef struct
{
    char id[TOOL_ID_LEN];
    char name[TOOL_NAME_LEN];
    char type[TOOL_TYPE_LEN];
    int stock;
    char status[TOOL_STATUS_LEN];
    int borrowed_count;
    char location[TOOL_LOCATION_LEN];
    double price;
} Tool;

/* ---- 工具信息管理接口 ---- */
int find_tool_index_by_id(Tool tools[], int tool_count, const char tool_id[]);
void show_all_tools(Tool tools[], int tool_count);
int add_tool(Tool tools[], int *tool_count, int max_tools);
int delete_tool(Tool tools[], int *tool_count, const char tool_id[]);
int update_tool(Tool tools[], int tool_count, const char tool_id[]);
void search_tools(Tool tools[], int tool_count);

#endif // TOOL_H