#include <stdio.h>
#include "storage.h"

int load_tools(const char filename[], Tool tools[], int *tool_count, int max_tools)
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

    int load_count = count < max_tools ? count : max_tools;
    for (int i = 0; i < load_count; i++)
    {
        if (fread(&tools[i], sizeof(Tool), 1, fp) != 1)
        {
            fclose(fp);
            return 0;
        }
    }

    *tool_count = load_count;
    fclose(fp);
    return 1;
}

int save_tools(const char filename[], Tool tools[], int tool_count)
{
    FILE *fp = fopen(filename, "wb");
    if (fp == NULL)
        return 0;

    if (fwrite(&tool_count, sizeof(int), 1, fp) != 1)
    {
        fclose(fp);
        return 0;
    }

    for (int i = 0; i < tool_count; i++)
    {
        if (fwrite(&tools[i], sizeof(Tool), 1, fp) != 1)
        {
            fclose(fp);
            return 0;
        }
    }

    fclose(fp);
    return 1;
}
