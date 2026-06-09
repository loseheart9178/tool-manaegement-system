# 工厂工具管理信息系统

本项目为《程序设计课程实践》项目 1：工厂工具管理信息系统。

## 目录结构

```text
.
├── src/        # C 源文件
├── include/    # 自定义头文件
├── data/       # 工具信息、借还记录等数据文件
├── docs/       # 接口文档、设计文档
├── report/     # 实验报告 doc/pdf
├── ppt/        # 答辩 PPT
└── build/      # 编译输出目录
```

## 分工

- 同学 A / 组长：工具基础信息管理、数据持久化、主菜单、登录、代码整合。
- 同学 B：工具借还管理、统计分析、测试截图、PPT 初稿。

## 编译建议

后续源文件完成后，可使用如下命令编译：

```bash
gcc src/*.c -Iinclude -o build/tool_manager
```

运行：

```bash
./build/tool_manager
```
