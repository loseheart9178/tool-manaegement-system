const fs = require("fs");
const path = require("path");
const pptxgen = require("pptxgenjs");

const ROOT = path.resolve(__dirname, "../..");
const ASSET_DIR = path.join(ROOT, "deliverables", "assets");
const PPT_DIR = path.join(ROOT, "ppt");
const FINAL_DIR = path.join(ROOT, "deliverables", "final");

fs.mkdirSync(PPT_DIR, { recursive: true });
fs.mkdirSync(FINAL_DIR, { recursive: true });

const pptx = new pptxgen();
pptx.defineLayout({ name: "WIDE", width: 13.333, height: 7.5 });
pptx.layout = "WIDE";
pptx.author = "卢世豪、丁伟哲";
pptx.subject = "程序设计课程实践";
pptx.title = "工厂工具管理信息系统答辩PPT";
pptx.lang = "zh-CN";
pptx.theme = {
  headFontFace: "SimHei",
  bodyFontFace: "SimSun",
  lang: "zh-CN"
};

const C = {
  navy: "12355B",
  blue: "1F5D99",
  pale: "EEF4FA",
  pale2: "F7FAFC",
  line: "B8C5D6",
  gray: "536173",
  dark: "1F2937",
  white: "FFFFFF",
  red: "9A3412",
  green: "276749",
};

function addHeader(slide, section, title, page) {
  slide.background = { color: C.white };
  slide.addShape(pptx.ShapeType.rect, { x: 0, y: 0, w: 13.333, h: 0.22, fill: { color: C.navy }, line: { color: C.navy } });
  slide.addText(section, { x: 0.62, y: 0.46, w: 2.1, h: 0.24, fontSize: 9.5, color: C.blue, bold: true, margin: 0 });
  slide.addText(title, { x: 0.62, y: 0.78, w: 9.8, h: 0.52, fontFace: "SimHei", fontSize: 23, bold: true, color: C.navy, margin: 0 });
  slide.addShape(pptx.ShapeType.line, { x: 0.62, y: 1.35, w: 12.0, h: 0, line: { color: C.line, width: 1 } });
  slide.addText(String(page).padStart(2, "0"), { x: 12.05, y: 0.66, w: 0.55, h: 0.35, fontSize: 12, color: C.gray, align: "right", margin: 0 });
}

function addFooter(slide) {
  slide.addText("程序设计课程实践 · 工厂工具管理信息系统", {
    x: 0.62, y: 7.08, w: 5.4, h: 0.22, fontSize: 8.5, color: "7B8794", margin: 0
  });
  slide.addShape(pptx.ShapeType.line, { x: 0.62, y: 6.96, w: 12.0, h: 0, line: { color: "E2E8F0", width: 1 } });
}

function bullet(slide, items, x, y, w, h, fontSize = 15) {
  const runs = [];
  items.forEach((item, i) => {
    runs.push({ text: item, options: { bullet: { type: "bullet" }, breakLine: i !== items.length - 1 } });
  });
  slide.addText(runs, {
    x, y, w, h,
    fontFace: "SimSun",
    fontSize,
    color: C.dark,
    paraSpaceAfterPt: 7,
    fit: "shrink",
    margin: 0.04,
  });
}

function noteBox(slide, x, y, w, h, title, text, fill = C.pale) {
  slide.addShape(pptx.ShapeType.rect, { x, y, w, h, fill: { color: fill }, line: { color: C.line, width: 1 } });
  slide.addText(title, { x: x + 0.18, y: y + 0.15, w: w - 0.36, h: 0.28, fontFace: "SimHei", fontSize: 13.5, bold: true, color: C.navy, margin: 0 });
  slide.addText(text, { x: x + 0.18, y: y + 0.52, w: w - 0.36, h: h - 0.65, fontSize: 11.5, color: C.dark, fit: "shrink", breakLine: false, margin: 0 });
}

function addTable(slide, rows, x, y, w, h, colW) {
  slide.addTable(rows, {
    x, y, w, h,
    colW,
    border: { type: "solid", color: C.line, pt: 0.75 },
    margin: 0.08,
    fontFace: "SimSun",
    fontSize: 10.5,
    color: C.dark,
    valign: "mid",
    fit: "shrink",
    autoFit: false,
    fill: { color: C.white },
    bold: false,
  });
  slide.addShape(pptx.ShapeType.rect, { x, y, w, h: 0.36, fill: { color: C.pale }, line: { color: C.line, width: 0.75 } });
}

function titleSlide() {
  const s = pptx.addSlide();
  s.background = { color: C.white };
  s.addShape(pptx.ShapeType.rect, { x: 0, y: 0, w: 13.333, h: 7.5, fill: { color: C.white }, line: { color: C.white } });
  s.addShape(pptx.ShapeType.rect, { x: 0, y: 0, w: 13.333, h: 0.26, fill: { color: C.navy }, line: { color: C.navy } });
  s.addText("《程序设计课程实践》综合项目答辩", { x: 0.9, y: 1.15, w: 8.5, h: 0.35, fontSize: 15, color: C.blue, margin: 0 });
  s.addText("工厂工具管理信息系统", { x: 0.9, y: 1.72, w: 9.8, h: 0.72, fontFace: "SimHei", fontSize: 34, bold: true, color: C.navy, margin: 0 });
  s.addShape(pptx.ShapeType.line, { x: 0.92, y: 2.68, w: 6.8, h: 0, line: { color: C.blue, width: 2 } });
  s.addText("基于 C 语言的工具信息管理、借还登记、库存统计与数据持久化设计实现", {
    x: 0.92, y: 3.05, w: 8.3, h: 0.42, fontSize: 15.5, color: C.dark, margin: 0
  });
  s.addText("组长：25051408 卢世豪\n成员：25051415 丁伟哲\n完成时间：2026 年 6 月", {
    x: 0.95, y: 4.65, w: 4.6, h: 0.82, fontSize: 14, color: C.dark, breakLine: false, margin: 0
  });
  s.addShape(pptx.ShapeType.rect, { x: 9.45, y: 1.05, w: 2.85, h: 4.95, fill: { color: C.pale2 }, line: { color: C.line, width: 1 } });
  s.addText("Project 01\nC Programming\nCourse Practice", {
    x: 9.78, y: 2.05, w: 2.2, h: 1.15, fontFace: "Georgia", fontSize: 18, color: C.navy, bold: true, breakLine: false, align: "center", margin: 0
  });
  s.addText("Structure · Array · File I/O · Modular Design", {
    x: 9.75, y: 4.08, w: 2.25, h: 0.72, fontFace: "Georgia", fontSize: 11, color: C.gray, italic: true, align: "center", margin: 0
  });
}

function tocSlide() {
  const s = pptx.addSlide();
  addHeader(s, "CONTENTS", "汇报提纲", 2);
  const items = [
    ["01", "项目背景与需求分析"],
    ["02", "系统总体设计"],
    ["03", "核心数据结构"],
    ["04", "主要功能模块实现"],
    ["05", "测试结果与运行效果"],
    ["06", "项目创新点与总结"],
  ];
  items.forEach((it, idx) => {
    const y = 1.75 + idx * 0.68;
    s.addText(it[0], { x: 1.0, y, w: 0.6, h: 0.28, fontFace: "Georgia", fontSize: 14, bold: true, color: C.blue, margin: 0 });
    s.addText(it[1], { x: 1.78, y: y - 0.02, w: 6.6, h: 0.32, fontSize: 17, color: C.dark, margin: 0 });
    s.addShape(pptx.ShapeType.line, { x: 1.0, y: y + 0.45, w: 10.7, h: 0, line: { color: "E5EAF0", width: 1 } });
  });
  addFooter(s);
}

function backgroundSlide() {
  const s = pptx.addSlide();
  addHeader(s, "01 BACKGROUND", "项目背景与需求分析", 3);
  bullet(s, [
    "工厂工具数量多、类型杂，传统人工登记容易出现漏记、错记和库存不清。",
    "工具借用和归还具有明显的业务流程，需要记录借用人、借用日期、应还日期和状态。",
    "课程任务要求综合使用结构体、数组、链表或数组、函数模块化和文件读写等 C 语言核心知识。",
    "本系统以“工具信息表 + 借还记录表”为数据基础，实现完整的控制台管理流程。"
  ], 0.85, 1.78, 7.1, 3.05, 14.5);
  noteBox(s, 8.5, 1.78, 3.6, 1.1, "系统目标", "实现工具管理、借还登记、记录查询、统计分析和数据持久化。");
  noteBox(s, 8.5, 3.15, 3.6, 1.1, "关键约束", "编号唯一、库存非负、损坏工具不可借、重复归还需拦截。", "FFF8E8");
  noteBox(s, 8.5, 4.52, 3.6, 1.1, "提交要求", "实验报告、源文件、可执行文件、答辩 PPT 四类材料。", "F0FDF4");
  addFooter(s);
}

function architectureSlide() {
  const s = pptx.addSlide();
  addHeader(s, "02 DESIGN", "系统总体设计", 4);
  const img = path.join(ASSET_DIR, "system_module_diagram.png");
  if (fs.existsSync(img)) s.addImage({ path: img, x: 0.78, y: 1.58, w: 7.5, h: 4.7 });
  bullet(s, [
    "main.c：程序入口，调用 run_system。",
    "menu.c：登录、加载数据、菜单循环、退出保存。",
    "tool.c / storage.c：工具基础信息管理与工具文件读写。",
    "borrow.c：借用、归还、借还记录展示与记录文件读写。",
    "statistics.c：库存统计、低库存预警和借用排行。"
  ], 8.65, 1.75, 3.55, 4.3, 12.5);
  addFooter(s);
}

function dataStructSlide() {
  const s = pptx.addSlide();
  addHeader(s, "03 DATA STRUCTURE", "核心数据结构设计", 5);
  noteBox(s, 0.78, 1.62, 5.8, 4.55, "Tool 工具信息结构体", "字段：id、name、type、stock、status、borrowed_count、location、price。\n\n设计要点：id 作为主键；stock 表示当前可借库存；borrowed_count 表示已借出数量；status 支持“正常/损坏”。");
  noteBox(s, 6.95, 1.62, 5.6, 4.55, "BorrowRecord 借还记录结构体", "字段：record_id、tool_id、borrower_name、borrow_date、due_date、quantity、status。\n\n设计要点：record_id 自增唯一；tool_id 关联工具表；status 区分“已借/已还”。");
  addFooter(s);
}

function moduleASlide() {
  const s = pptx.addSlide();
  addHeader(s, "04 IMPLEMENTATION", "工具信息管理模块（卢世豪）", 6);
  bullet(s, [
    "添加工具：检查数组容量、工具编号唯一性、库存和单价合法性。",
    "删除工具：根据编号定位，若 borrowed_count > 0 则禁止删除，删除前二次确认。",
    "修改工具：编号不可修改，其他字段支持按需更新，直接回车保留原值。",
    "查询工具：编号精准查询，名称和类型使用 strstr 实现模糊查询。",
    "文件读写：tools.dat 保存工具数量和结构体数组，实现重启不丢失数据。"
  ], 0.85, 1.7, 6.2, 4.65, 14);
  s.addShape(pptx.ShapeType.rect, { x: 7.5, y: 1.75, w: 4.7, h: 3.55, fill: { color: "F8FAFC" }, line: { color: C.line } });
  s.addText("关键函数", { x: 7.78, y: 2.0, w: 2.5, h: 0.28, fontFace: "SimHei", fontSize: 14, color: C.navy, bold: true, margin: 0 });
  s.addText("find_tool_index_by_id()\nshow_all_tools()\nadd_tool()\ndelete_tool()\nupdate_tool()\nsearch_tools()\nload_tools() / save_tools()", {
    x: 7.82, y: 2.48, w: 4.0, h: 2.3, fontFace: "Menlo", fontSize: 12.5, color: C.dark, breakLine: false, margin: 0
  });
  addFooter(s);
}

function moduleBSlide() {
  const s = pptx.addSlide();
  addHeader(s, "04 IMPLEMENTATION", "借还与统计模块（丁伟哲）", 7);
  bullet(s, [
    "借用工具：校验工具存在、状态正常、借用数量大于 0 且不超过库存。",
    "归还工具：通过唯一记录编号定位借用记录，状态为“已还”时禁止重复归还。",
    "库存联动：借用时 stock 减少、borrowed_count 增加；归还时反向恢复。",
    "库存统计：统计工具种类数、库存总数量、在库数量、已借数量和总价值。",
    "借用排行：遍历借还记录累计次数，并使用 qsort 降序排序。"
  ], 0.85, 1.7, 6.1, 4.65, 14);
  s.addShape(pptx.ShapeType.rect, { x: 7.45, y: 1.72, w: 4.85, h: 3.8, fill: { color: "F8FAFC" }, line: { color: C.line } });
  s.addText("库存变化规则", { x: 7.75, y: 2.0, w: 2.8, h: 0.28, fontFace: "SimHei", fontSize: 14, color: C.navy, bold: true, margin: 0 });
  s.addText("借用成功：\nstock -= quantity\nborrowed_count += quantity\n\n归还成功：\nstock += quantity\nborrowed_count -= quantity", {
    x: 7.82, y: 2.48, w: 4.2, h: 2.35, fontFace: "Menlo", fontSize: 12.5, color: C.dark, breakLine: false, margin: 0
  });
  addFooter(s);
}

function testSlide() {
  const s = pptx.addSlide();
  addHeader(s, "05 TEST", "运行效果与测试结果", 8);
  const img = path.join(ASSET_DIR, "run_effect_screenshot.png");
  if (fs.existsSync(img)) s.addImage({ path: img, x: 0.78, y: 1.55, w: 7.5, h: 4.7 });
  const rows = [
    [{ text: "测试项", options: { bold: true } }, { text: "结果", options: { bold: true } }],
    ["登录验证", "账号 admin / 密码 admin123 登录成功"],
    ["数据加载", "加载 12 条工具记录、9 条借还记录"],
    ["库存统计", "总数量 142 件，在库 126 件，已借 16 件"],
    ["低库存预警", "阈值 6 时筛出 4 种低库存工具"],
    ["借用排行", "按历史借用次数降序展示"],
  ];
  s.addTable(rows, { x: 8.55, y: 1.72, w: 3.75, h: 3.25, colW: [1.25, 2.5], border: { color: C.line, pt: 0.75 }, fill: { color: C.white }, fontSize: 9.5, color: C.dark, margin: 0.05, valign: "mid", fit: "shrink" });
  noteBox(s, 8.55, 5.25, 3.75, 0.8, "验证结论", "系统核心流程可运行，数据文件可正常保存与加载。", "F0FDF4");
  addFooter(s);
}

function innovationSlide() {
  const s = pptx.addSlide();
  addHeader(s, "06 SUMMARY", "创新点与项目总结", 9);
  noteBox(s, 0.78, 1.65, 3.55, 2.0, "低库存预警", "设置安全库存阈值后，自动筛选当前库存不足的工具，辅助补货决策。", "F0FDF4");
  noteBox(s, 4.88, 1.65, 3.55, 2.0, "借用次数排行", "根据历史借还记录统计高频工具，帮助分析使用需求。", C.pale);
  noteBox(s, 8.98, 1.65, 3.55, 2.0, "数据持久化", "启动自动加载、退出自动保存，避免程序重启后数据丢失。", "FFF8E8");
  s.addText("项目收获", { x: 0.85, y: 4.25, w: 1.6, h: 0.3, fontFace: "SimHei", fontSize: 15, color: C.navy, bold: true, margin: 0 });
  bullet(s, [
    "将 C 语言基础知识应用到一个完整业务系统中。",
    "理解结构体、数组、文件读写和模块化接口在项目中的作用。",
    "通过两人分工协作，完成从编码、测试到文档和答辩材料的完整提交链路。"
  ], 0.9, 4.72, 11.5, 1.25, 13.5);
  addFooter(s);
}

function thanksSlide() {
  const s = pptx.addSlide();
  s.background = { color: C.white };
  s.addShape(pptx.ShapeType.rect, { x: 0, y: 0, w: 13.333, h: 0.26, fill: { color: C.navy }, line: { color: C.navy } });
  s.addText("谢谢聆听", { x: 0, y: 2.45, w: 13.333, h: 0.6, fontFace: "SimHei", fontSize: 34, bold: true, color: C.navy, align: "center", margin: 0 });
  s.addText("欢迎老师批评指正", { x: 0, y: 3.25, w: 13.333, h: 0.35, fontSize: 16, color: C.gray, align: "center", margin: 0 });
  s.addText("卢世豪 25051408 · 丁伟哲 25051415", { x: 0, y: 4.25, w: 13.333, h: 0.3, fontSize: 13.5, color: C.dark, align: "center", margin: 0 });
}

titleSlide();
tocSlide();
backgroundSlide();
architectureSlide();
dataStructSlide();
moduleASlide();
moduleBSlide();
testSlide();
innovationSlide();
thanksSlide();

const out = path.join(PPT_DIR, "工厂工具管理信息系统-答辩PPT.pptx");
pptx.writeFile({ fileName: out }).then(() => {
  fs.copyFileSync(out, path.join(FINAL_DIR, path.basename(out)));
  console.log(out);
});
