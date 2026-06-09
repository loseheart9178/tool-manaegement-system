# 同学 B 接口文档：工具借还管理与统计分析模块

## 1. 负责范围

同学 B 负责工具借还记录管理、库存统计分析、低库存预警、借用次数排行、测试截图和答辩 PPT 初稿。

主要功能包括：

- 借用工具
- 归还工具
- 查看借还记录
- 借还记录保存到文件
- 从文件加载借还记录
- 库存整体统计
- 低库存预警
- 借用次数排行

## 2. 共享数据结构

借还记录结构体定义在 `include/borrow.h`。

```c
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
```

字段说明：

| 字段 | 含义 | 要求 |
|---|---|---|
| record_id | 借还记录编号 | 自增，唯一 |
| tool_id | 工具编号 | 必须能在工具数组中找到 |
| borrower_name | 借用人姓名 | 不能为空 |
| borrow_date | 借用日期 | 建议格式：YYYY-MM-DD |
| due_date | 应还日期 | 建议格式：YYYY-MM-DD |
| quantity | 借用数量 | 必须大于 0，且不能超过当前库存 |
| status | 借用状态 | 建议只允许“已借”或“已还” |

## 3. 建议文件

建议同学 B 创建并实现以下文件：

```text
src/borrow.c
src/statistics.c
include/statistics.h
```

## 4. 借还管理接口

### 4.1 借用工具

```c
int borrow_tool(
    Tool tools[],
    int tool_count,
    BorrowRecord records[],
    int *record_count,
    int max_records
);
```

功能：录入借用信息，生成借还记录，并同步更新工具库存。

输入：

- `tools[]`：工具数组
- `tool_count`：工具数量
- `records[]`：借还记录数组
- `record_count`：借还记录数量指针，借用成功后加 1
- `max_records`：借还记录数组最大容量

返回值：

- `1`：借用成功
- `0`：借用失败

处理规则：

- 工具编号必须存在
- 工具状态为“损坏”时，建议禁止借用
- 借用数量必须大于 0
- 借用数量不能超过当前库存
- 借用成功后：
  - `tools[index].stock -= quantity`
  - `tools[index].borrowed_count += quantity`
  - 新增一条状态为“已借”的借还记录

---

### 4.2 归还工具

```c
int return_tool(
    Tool tools[],
    int tool_count,
    BorrowRecord records[],
    int record_count
);
```

功能：根据借还记录编号办理归还，并恢复库存。

输入：

- `tools[]`：工具数组
- `tool_count`：工具数量
- `records[]`：借还记录数组
- `record_count`：借还记录数量

返回值：

- `1`：归还成功
- `0`：归还失败

处理规则：

- 借还记录编号必须存在
- 状态已经是“已还”的记录不能重复归还
- 归还成功后：
  - `records[index].status` 修改为“已还”
  - `tools[tool_index].stock += records[index].quantity`
  - `tools[tool_index].borrowed_count -= records[index].quantity`

---

### 4.3 查看借还记录

```c
void show_all_records(BorrowRecord records[], int record_count);
```

功能：列表展示全部借还记录。

输入：

- `records[]`：借还记录数组
- `record_count`：借还记录数量

输出：无。直接打印借还记录列表。

建议：

- 按记录编号升序显示
- 输出时包含记录编号、工具编号、借用人、借用日期、应还日期、数量、状态

---

### 4.4 查找借还记录

```c
int find_record_index_by_id(BorrowRecord records[], int record_count, int record_id);
```

功能：根据借还记录编号查找记录下标。

返回值：

- 找到：返回数组下标
- 未找到：返回 `-1`

---

### 4.5 生成记录编号

```c
int generate_record_id(BorrowRecord records[], int record_count);
```

功能：生成新的借还记录编号。

返回值：

- 如果当前没有记录，返回 `1`
- 如果已有记录，返回当前最大记录编号加 `1`

## 5. 文件读写接口

```c
int load_records(
    const char filename[],
    BorrowRecord records[],
    int *record_count,
    int max_records
);
```

功能：程序启动时从文件读取借还记录。

返回值：

- `1`：加载成功
- `0`：文件不存在或加载失败

---

```c
int save_records(
    const char filename[],
    BorrowRecord records[],
    int record_count
);
```

功能：退出系统前保存借还记录。

返回值：

- `1`：保存成功
- `0`：保存失败

建议文件路径：

```text
data/borrow_records.dat
```

## 6. 统计分析接口

### 6.1 库存整体统计

```c
void show_inventory_summary(Tool tools[], int tool_count);
```

功能：统计并显示所有工具的库存总数量和库存总价值。

计算规则：

- 库存总数量：所有工具 `stock + borrowed_count` 之和
- 当前在库数量：所有工具 `stock` 之和
- 已借出数量：所有工具 `borrowed_count` 之和
- 总价值：所有工具 `(stock + borrowed_count) * price` 之和

---

### 6.2 低库存预警

```c
void show_low_stock_tools(Tool tools[], int tool_count, int threshold);
```

功能：显示当前库存低于安全阈值的工具。

输入：

- `threshold`：安全库存阈值，必须大于等于 0

筛选规则：

```c
tools[i].stock < threshold
```

---

### 6.3 借用次数排行

```c
void show_borrow_ranking(
    Tool tools[],
    int tool_count,
    BorrowRecord records[],
    int record_count
);
```

功能：统计每种工具的借用次数，并按次数从高到低显示。

统计规则：

- 每出现一条借用记录，该工具借用次数加 1
- 可以只统计状态为“已借”和“已还”的历史记录

## 7. 需要调用同学 A 提供的接口

同学 B 的模块需要调用：

```c
int find_tool_index_by_id(Tool tools[], int tool_count, const char tool_id[]);
```

用途：

- 借用工具时判断工具是否存在
- 归还工具时找到对应工具并恢复库存
- 排行榜显示工具名称、类型等信息

## 8. 测试要求

至少完成以下测试：

- 借用存在的工具
- 借用不存在的工具
- 借用数量为 0 或负数
- 借用数量超过库存
- 借用损坏状态工具
- 归还正常借出记录
- 重复归还同一条记录
- 归还不存在的记录编号
- 查看全部借还记录
- 统计库存总量和总价值
- 输入安全库存阈值并显示低库存工具
- 借用次数排行是否按降序显示
- 退出后重新启动，检查借还记录是否保留
