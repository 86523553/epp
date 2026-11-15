# 工具脚本

| 脚本 | 作用 | 依赖 |
|---|---|---|
| `gen_from_krnln.py` | `krnln.txt`（易语言支持库导出）→ `.elib` 文本库 + 组件头文件 + 公共事件/方法头 | 无（标准库） |
| `gen_corelib.py` | 中文命令文档（GBK）→ `.elib` | 无 |
| `gen_comp_lib.py` | 组件库头文件集合 → 组件清单 | 无 |
| `pack_epp.py` | 打 `.epp` 单文件支持库（stored zip）；`--list` / `--dump` 查包 | 无 |
| `gen_placeholder_icons.py` | 生成/重生成 `assets/icons/` 占位图标 | Pillow |

## 典型链路

```bash
# 1. 元数据：krnln.txt → .elib + 头文件
python tools/gen_from_krnln.py krnln.txt -o out_krnln

# 2. 打包成单文件支持库（必须 --store，压缩包 IDE 读不了）
python tools/pack_epp.py --name 核心支持库 --elib out_krnln/核心支持库.elib ^
    --comp-dir out_krnln/include --out 核心支持库.epp --store

# 3. 检查
python tools/pack_epp.py --list 核心支持库.epp
```

`krnln.txt` 是易语言支持库的文本导出（命令 / 数据类型 / 常量 / 组件的全量元数据），
不属于本仓库，需要自己从易语言环境导出。

`.epp` 为什么必须 stored：IDE 里的读包函数 `epp_zip_entry()` 只读中央目录 + 偏移直取字节，不实现 inflate（约 60 行、零依赖，这是刻意的取舍）。
