from __future__ import annotations

import shutil
import sys
from pathlib import Path


ROOT = Path(__file__).resolve().parents[2]
FINAL_DIR = ROOT / "deliverables" / "final"
TEMPLATE_ZIP = FINAL_DIR / "第X组-工厂工具管理信息系统-提交材料.zip"


def main() -> int:
    if len(sys.argv) != 2:
        print("用法：python3 scripts/materials/finalize_submission.py <组号>")
        print("示例：python3 scripts/materials/finalize_submission.py 1")
        return 1

    group = sys.argv[1].strip()
    if group.startswith("第") and group.endswith("组"):
        group_text = group
    else:
        group_text = f"第{group}组"

    if not TEMPLATE_ZIP.exists():
        print(f"未找到基础提交包：{TEMPLATE_ZIP}")
        return 1

    final_zip = FINAL_DIR / f"{group_text}.zip"
    shutil.copy2(TEMPLATE_ZIP, final_zip)

    checklist = FINAL_DIR / "提交材料清单.md"
    if checklist.exists():
        text = checklist.read_text(encoding="utf-8")
        text = text.replace("第X组-工厂工具管理信息系统-提交材料.zip", final_zip.name)
        checklist.write_text(text, encoding="utf-8")

    print(f"已生成最终提交包：{final_zip}")
    print("说明：当前机器没有 rar/7z 工具，因此生成的是 zip。若老师严格要求 rar，请用压缩软件将该 zip 内容另存为 rar。")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
