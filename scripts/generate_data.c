#include <stdio.h>
#include <string.h>
#include "tool.h"

int main(void)
{
    Tool tools[] = {
        {"T001", "电钻", "电动工具", 10, "正常", 2, "A01", 299.00},
        {"T002", "角磨机", "电动工具", 5, "正常", 1, "A02", 450.00},
        {"T003", "数字万用表", "测量工具", 20, "正常", 3, "B01", 89.00},
        {"T004", "激光水平仪", "测量工具", 8, "正常", 0, "B02", 320.00},
        {"T005", "扳手套装", "维修工具", 15, "正常", 5, "C01", 150.00},
        {"T006", "螺丝刀套装", "维修工具", 30, "正常", 2, "C02", 68.00},
        {"T007", "电焊机", "电动工具", 3, "损坏", 0, "A03", 1200.00},
        {"T008", "游标卡尺", "测量工具", 12, "正常", 1, "B03", 55.00},
        {"T009", "冲击钻", "电动工具", 6, "正常", 0, "A04", 680.00},
        {"T010", "热风枪", "电动工具", 4, "正常", 1, "A05", 175.00},
        {"T011", "扭矩扳手", "维修工具", 7, "正常", 2, "C03", 260.00},
        {"T012", "绝缘电阻测试仪", "测量工具", 5, "正常", 0, "B04", 520.00},
    };

    int tool_count = sizeof(tools) / sizeof(tools[0]);

    FILE *fp = fopen("data/tools.dat", "wb");
    if (fp == NULL)
    {
        printf("无法创建文件 data/tools.dat\n");
        return 1;
    }

    fwrite(&tool_count, sizeof(int), 1, fp);
    for (int i = 0; i < tool_count; i++)
    {
        fwrite(&tools[i], sizeof(Tool), 1, fp);
    }

    fclose(fp);
    printf("已生成 data/tools.dat，包含 %d 条工具记录。\n", tool_count);
    return 0;
}
