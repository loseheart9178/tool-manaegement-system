# 同学 A 接口文档：工具信息管理与系统基础模块

## 1. 负责范围

同学 A 负责工具基础信息管理、工具数据文件读写、系统登录、主菜单导航和最终代码整合。

主要功能包括：

- 显示所有工具信息
- 添加工具信息
- 删除工具信息
- 修改工具信息
- 查询工具信息
- 工具信息保存到文件
- 从文件加载工具信息
- 登录验证
- 主菜单与退出自动保存

## 2. 共享数据结构

工具信息结构体定义在 `include/tool.h`。

```c
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
```

字段说明：

| 字段 | 含义 | 要求 |
|---|---|---|
| id | 工具编号 | 主键，唯一，不能为空 |
| name | 工具名称 | 不能为空 |
| type | 工具类型 | 如电动工具、测量工具、维修工具 |
| stock | 当前库存数量 | 必须大于等于 0 |
| status | 工具状态 | 建议只允许“正常”或“损坏” |
| borrowed_count | 已借出数量 | 必须大于等于 0 |
| location | 仓库存放位置 | 如 A01、B02 |
| price | 工具单价 | 必须大于等于 0 |

## 3. 建议文件

建议同学 A 创建并实现以下文件：

```text
src/tool.c
src/storage.c
src/menu.c
include/storage.h
include/menu.h
```

## 4. 对外接口设计

### 4.1 工具信息管理接口

```c
void show_all_tools(Tool tools[], int tool_count);
```

功能：列表展示全部工具信息。

输入：

- `tools[]`：工具数组
- `tool_count`：当前工具数量

输出：无。直接在控制台打印工具列表。

---

```c
int add_tool(Tool tools[], int *tool_count, int max_tools);
```

功能：从键盘录入新工具，并添加到工具数组。

输入：

- `tools[]`：工具数组
- `tool_count`：工具数量指针，添加成功后需要加 1
- `max_tools`：数组最大容量

返回值：

- `1`：添加成功
- `0`：添加失败，如编号重复、数组已满、输入非法

注意：

- 添加前必须调用编号查重逻辑
- 库存、已借数量、单价不能为负数

---

```c
int delete_tool(Tool tools[], int *tool_count, const char tool_id[]);
```

功能：根据工具编号删除工具。

输入：

- `tools[]`：工具数组
- `tool_count`：工具数量指针
- `tool_id[]`：要删除的工具编号

返回值：

- `1`：删除成功
- `0`：未找到或用户取消删除

注意：

- 删除前需要二次确认
- 如果工具已借出数量大于 0，建议禁止删除

---

```c
int update_tool(Tool tools[], int tool_count, const char tool_id[]);
```

功能：根据工具编号修改工具信息。

输入：

- `tools[]`：工具数组
- `tool_count`：工具数量
- `tool_id[]`：要修改的工具编号

返回值：

- `1`：修改成功
- `0`：未找到或输入非法

注意：

- 工具编号不能修改
- 修改库存时要保证库存数量不能小于 0

---

```c
void search_tools(Tool tools[], int tool_count);
```

功能：支持按工具编号、名称、类型查询工具。

输入：

- `tools[]`：工具数组
- `tool_count`：工具数量

输出：无。直接打印匹配结果。

建议：

- 编号使用精准查询
- 名称和类型支持关键字模糊查询

---

```c
int find_tool_index_by_id(Tool tools[], int tool_count, const char tool_id[]);
```

功能：根据工具编号查找工具下标。

返回值：

- 找到：返回数组下标
- 未找到：返回 `-1`

该函数会被同学 B 的借还模块调用，因此函数名、参数和返回值不要随意修改。

## 5. 文件读写接口

```c
int load_tools(const char filename[], Tool tools[], int *tool_count, int max_tools);
```

功能：程序启动时从文件读取工具信息。

返回值：

- `1`：加载成功
- `0`：文件不存在或加载失败

---

```c
int save_tools(const char filename[], Tool tools[], int tool_count);
```

功能：退出系统前保存工具信息。

返回值：

- `1`：保存成功
- `0`：保存失败

建议文件路径：

```text
data/tools.dat
```

## 6. 系统基础接口

```c
int login(void);
void show_main_menu(void);
void run_system(void);
```

说明：

- `login` 负责用户名和密码验证
- `show_main_menu` 只负责显示菜单
- `run_system` 负责菜单循环、调用各模块功能、退出前保存数据

## 7. 需要给同学 B 提供的接口

同学 B 的借还模块需要调用以下内容：

```c
int find_tool_index_by_id(Tool tools[], int tool_count, const char tool_id[]);
int save_tools(const char filename[], Tool tools[], int tool_count);
```

同学 B 会直接修改工具数组中的 `stock` 和 `borrowed_count`，所以工具数组需要在主程序中统一管理。

## 8. 测试要求

至少完成以下测试：

- 添加正常工具
- 添加重复编号工具
- 添加负数库存工具
- 删除不存在的工具
- 删除已借出的工具
- 修改工具名称、类型、库存、位置、单价
- 按编号查询
- 按名称模糊查询
- 按类型模糊查询
- 退出后重新启动，检查工具数据是否保留
