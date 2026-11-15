#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""从 krnln.txt（易语言系统核心支持库全量导出）生成 IDE 可读的库文件。

输入：I:\\Hermes备份\\易语言支持库接口全库\\krnln.txt
输出（默认 开发环境/工具/out_krnln/，不覆盖运行环境里的现有文件）：
    核心支持库.elib          688 命令 / 按分类
    公共事件.h               13 个窗口组件通用事件
    components/<comp>.h      42 个组件（属性 + 方法 + 事件）
    data_types.txt           51 个数据类型
    constants.txt            335 个常量
    report.txt               统计与校验

用法：
    python gen_from_krnln.py                 # 生成到 out_krnln/
    python gen_from_krnln.py --src krnln.txt --out out_krnln
"""
import argparse
import os
import re
import sys
from collections import OrderedDict

HERE = os.path.dirname(os.path.abspath(__file__))
ROOT = os.path.dirname(HERE)
PROJ = os.path.dirname(ROOT)
DEF_SRC = r"I:\Hermes备份\易语言支持库接口全库\krnln.txt"
DEF_OUT = os.path.join(HERE, "out_krnln")

# krnln 类型 -> IDE 类型名（未知的原样保留）
TYPE_MAP = {
    "整数型": "整数型", "文本型": "文本型", "逻辑型": "逻辑型", "字节集": "字节集",
    "小数型": "小数型", "双精度小数型": "双精度小数型", "长整数型": "长整数型",
    "日期时间型": "日期时间型", "通用型": "通用型", "子程序指针": "子程序指针",
    "无返回值": "无返回值", "对象": "对象", "数组": "数组",
}

# 组件英文名（krnln 的 数据类型 头部带 英文名=xxx，直接用）
COMMON_EVENTS = [
    ("鼠标左键被按下", "mouse_ldown", "", "用户在本组件上按下鼠标左键时触发。"),
    ("鼠标左键被放开", "mouse_lup", "", "用户在本组件上放开鼠标左键时触发。"),
    ("鼠标右键被按下", "mouse_rdown", "", "用户在本组件上按下鼠标右键时触发。"),
    ("鼠标右键被放开", "mouse_rup", "", "用户在本组件上放开鼠标右键时触发。"),
    ("鼠标位置被移动", "mouse_move", "", "鼠标在本组件范围内移动时触发。"),
    ("滚轮被滚动", "mouse_wheel", "", "用户在本组件上滚动鼠标滚轮时触发。"),
    ("被双击", "dblclick", "", "用户双击本组件时触发。"),
    ("获得焦点", "setfocus", "", "本组件获得输入焦点时触发。"),
    ("失去焦点", "killfocus", "", "本组件失去输入焦点时触发。"),
    ("按下某键", "keydown", "", "本组件拥有焦点时按下键盘按键触发。"),
    ("放开某键", "keyup", "", "本组件拥有焦点时放开键盘按键触发。"),
    ("字符输入", "keypress", "", "本组件拥有焦点时输入字符触发。"),
    ("反馈事件", "feedback", "", "由「调用反馈事件」命令向本组件发送的反馈通知。"),
]


def parse(path):
    lines = open(path, encoding="utf-8", errors="replace").read().splitlines()
    cmds = OrderedDict()          # 名 -> dict
    types = OrderedDict()         # 名 -> dict(members/events/methods)
    members = []                  # 成员命令（全部数据类型的成员命令汇总，按 命令索引 去重）
    by_index = {}                 # 命令索引 -> 成员命令 dict（.成员命令 块与 .命令 块共用同一索引）
    owners = {}                   # 命令索引 -> [所属数据类型名]（来自 .数据类型 段内的 .成员命令）
    consts = []
    sec = ""
    cur_cmd = None
    cur_type = None
    cur_evt = None
    for raw in lines:
        s = raw.strip()
        if not s:
            continue
        if s.startswith("[") and s.endswith("]"):
            sec = s
            cur_cmd = cur_type = cur_evt = None
            continue
        m = re.match(r'^\.命令\s+([^,]+),(.*)$', s)
        if m:
            name, rest = m.group(1).strip(), m.group(2)
            cat = re.search(r'分类=([^,(]+)', rest)
            member = bool(re.search(r'分类=成员命令', rest))
            cur_evt = None   # 命令/成员命令开始，参数别再归给上一个事件（否则「反馈事件」会混入「调用反馈事件」的参数）
            idx = re.search(r'命令索引=(\d+)', rest)
            cur_cmd = {
                "name": name,
                "ret": (re.search(r'返回值=([^,]+)', rest).group(1) if re.search(r'返回值=([^,]+)', rest) else ""),
                "cat": cat.group(1).strip() if cat else "",
                "member": member,
                "en": (re.search(r'英文名=([^,\s]+)', rest).group(1) if re.search(r'英文名=([^,\s]+)', rest) else ""),
                "index": int(idx.group(1)) if idx else 0,
                "owners": [],
                "params": [],
                "desc": "",
            }
            if member:
                if cur_cmd["index"] and cur_cmd["index"] in by_index:
                    prev = by_index[cur_cmd["index"]]
                    # 重复声明：参数已在首条收过，后续再挂就丢进不保存的草稿，避免参数翻倍
                    cur_cmd = prev if not prev["params"] else {"name": name, "params": [], "desc": ""}
                else:
                    members.append(cur_cmd)
                    if cur_cmd["index"]:
                        by_index[cur_cmd["index"]] = cur_cmd
            else:
                cmds[name] = cur_cmd
            continue
        # .成员命令 是 .命令+分类=成员命令 的另一种写法（krnln.txt 两种都有，
        # 内容重复）。命令本体/参数已在 .命令 块里收过，这里只取它挂在哪个
        # .数据类型 段下——这就是本支持库自己给成员命令定归属的地方：
        # 窗口 23 条（取窗口句柄…置父窗口）、外部数据库 19 条（打开/关闭…）、
        # 数据源 71 条……所以不能把 295 条一股脑全塞给「窗口」。
        if s.startswith(".成员命令"):
            m = re.match(r'^\.成员命令\s+([^,]+),(.*)$', s)
            if m and cur_type is not None:
                idx = re.search(r'命令索引=(\d+)', m.group(2))
                key = int(idx.group(1)) if idx else 0
                if key:
                    lst = owners.setdefault(key, [])
                    if cur_type not in lst:
                        lst.append(cur_type)
                    if key not in by_index:
                        # 只有类型段里出现过（.命令 块缺失）时兜底建条目
                        mc = {
                            "name": m.group(1).strip(),
                            "ret": (re.search(r'返回值=([^,]+)', m.group(2)).group(1)
                                    if re.search(r'返回值=([^,]+)', m.group(2)) else ""),
                            "cat": "成员命令",
                            "member": True,
                            "en": (re.search(r'英文名=([^,\s]+)', m.group(2)).group(1)
                                   if re.search(r'英文名=([^,\s]+)', m.group(2)) else ""),
                            "index": key,
                            "owners": [],
                            "params": [],
                            "desc": "",
                        }
                        members.append(mc)
                        by_index[key] = mc
            cur_cmd = cur_evt = None
            continue
        m = re.match(r'^\.参数\s+([^,]+),\s*([^,]*)(.*)$', s)
        if m:
            pname = m.group(1).strip()
            ptype = TYPE_MAP.get(m.group(2).strip(), m.group(2).strip())
            rest = m.group(3)
            out_attr = (re.search(r'属性=([^,]+)', rest).group(1).strip() if re.search(r'属性=([^,]+)', rest) else "")
            opt = 1 if "默认空" in rest else 0
            desc = (re.search(r'说明=(.*)$', rest).group(1).strip() if re.search(r'说明=(.*)$', rest) else "")
            arr = 1 if "数组" in rest else 0
            if cur_evt is not None:
                cur_evt["params"].append((pname, ptype, 0, desc, arr, opt, out_attr))
            elif cur_cmd is not None:
                cur_cmd["params"].append((pname, ptype, 0, desc, arr, opt, out_attr))
            continue
        m = re.match(r'^\.数据类型\s+([^,]+),(.*)$', s)
        if m:
            name, rest = m.group(1).strip(), m.group(2)
            cur_type = name
            types[name] = {
                "name": name,
                "is_component": "类型=窗口组件" in rest,
                "en": (re.search(r'英文名=([^,\s]+)', rest).group(1) if re.search(r'英文名=([^,\s]+)', rest) else name),
                "n_members": int((re.search(r'成员数=(\d+)', rest) or [None, 0])[1]),
                "n_events": int((re.search(r'事件数=(\d+)', rest) or [None, 0])[1]),
                "n_methods": int((re.search(r'成员命令数=(\d+)', rest) or [None, 0])[1]),
                "flags": (re.search(r'属性=([^,]+)', rest).group(1) if re.search(r'属性=([^,]+)', rest) else ""),
                "desc": "",
                "members": [], "events": [], "methods": [],
            }
            cur_cmd = None
            continue
        m = re.match(r'^\.成员\s+([^,]+),\s*([^,]*)(.*)$', s)
        if m and cur_type:
            rest = m.group(3)
            types[cur_type]["members"].append({
                "name": m.group(1).strip(),
                "type": TYPE_MAP.get(m.group(2).strip(), m.group(2).strip()),
                "en": (re.search(r'英文名=([^,\s]+)', rest).group(1) if re.search(r'英文名=([^,\s]+)', rest) else ""),
                "ptype": (re.search(r'属性类型=([^,]+)', rest).group(1) if re.search(r'属性类型=([^,]+)', rest) else ""),
                "choices": (re.search(r'可选值=([^,]+)', rest).group(1) if re.search(r'可选值=([^,]+)', rest) else ""),
                "desc": (re.search(r'说明=(.*)$', rest).group(1).strip() if re.search(r'说明=(.*)$', rest) else ""),
            })
            continue
        m = re.match(r'^\.事件\s+([^,]+),(.*)$', s)
        if m and cur_type:
            rest = m.group(2)
            cur_evt = {
                "name": m.group(1).strip(),
                "ret": (re.search(r'返回值=([^,]+)', rest).group(1) if re.search(r'返回值=([^,]+)', rest) else ""),
                "params": [],
                "desc": "",
            }
            types[cur_type]["events"].append(cur_evt)
            continue
        m = re.match(r'^\.常量\s+([^,]+),\s*([^,]*)', s)
        if m:
            consts.append((m.group(1).strip(), m.group(2).strip(), s))
            continue
        if s.startswith("说明：") or s.startswith("说明:"):
            txt = s.split("：", 1)[-1] if "：" in s else s.split(":", 1)[-1]
            if cur_type is not None and cur_evt is None and cur_cmd is None \
                    and not types[cur_type]["desc"]:
                types[cur_type]["desc"] = txt.strip()   # 类型说明（说明：行紧跟 .数据类型）
            elif cur_evt is not None and not cur_evt["desc"]:
                cur_evt["desc"] = txt.strip()
            elif cur_cmd is not None and not cur_cmd["desc"]:
                cur_cmd["desc"] = txt.strip()
            continue
    # 归属回填：members 里每条成员命令带上所属数据类型（可能多条，按 krnln 原样），
    # 同时把方法挂进对应类型的 methods（report.txt 的「方法」列因此能对上 krnln 声明）
    for mc in members:
        mc["owners"] = owners.get(mc.get("index", 0), [])
        for own in mc["owners"]:
            if own in types:
                types[own]["methods"].append(mc)
    return cmds, members, types, consts


def esc(t):
    return (t or "").replace("|", "／").replace("\n", " ").strip()


def write_elib(cmds, members, out):
    cats = OrderedDict()
    for c in cmds.values():
        cats.setdefault(c["cat"] or "其他", []).append(c)
    # 前 22 个分类的顺序＝krnln.txt 出现顺序＝易语言左栏顺序（流程控制排第一）。
    # 「成员命令」是所有窗口组件共享的方法表——本支持库自己的组织方式就是只写
    # include/公共方法.h（SULANG_MFC_METHOD 声明），不进命令表、不进分类树；
    # 「易向导」是 IDE 内部命令，同样不进。两者都不在这里输出。
    SKIP_CATS = ("成员命令", "易向导")
    lines = [
        # 库名照原版写「系统核心支持库」：提示表「命令所属」那一格要拼成
        # 系统核心支持库->画板，跟易语言一模一样（目录/包名仍是 核心支持库）
        "# 库名=系统核心支持库",
        "# 版本=5.7.64",
        "# 说明=由 krnln.txt 生成（易语言系统核心支持库全量命令表）",
        "# 静态库=..\\静态库\\libcore_support.a",
        "# 本文件由 开发环境/工具/gen_from_krnln.py 生成，IDE 启动时读入",
        "",
    ]
    n = 0
    for cat, items in cats.items():
        if cat in SKIP_CATS:
            continue
        lines.append("@分类 " + cat)
        for c in items:
            lines.append("@命令 " + c["name"])
            if c["en"]:
                lines.append("英文名=" + c["en"])
            if c["ret"]:
                lines.append("返回=" + c["ret"])
            if c["desc"]:
                lines.append("说明=" + esc(c["desc"]))
            for pname, ptype, _ref, pdesc, arr, opt, attr in c["params"]:
                lines.append("参数=%s|%s|%s|%d|%s" % (pname, ptype, esc(pdesc), opt, esc(attr)))
            n += 1
        lines.append("")
    open(out, "w", encoding="utf-8", newline="\n").write("\n".join(lines))
    return n, len(cats)


def write_types(types, out):
    """数据类型表（51 个，krnln.txt 原序＝易语言树顺序）。

    支持库页「数据类型」节点直接读本文件。格式：
        @类型 名 / 类型= / 英文名= / 属性= / 说明= / 成员数= / 事件数=
        @成员 名 / 类型= / 英文名= / 属性类型= / 可选值= / 说明=
        @事件 名 / 说明=    之前只挑了 42 个「窗口组件」写进 组件.txt，把 窗口/报表/菜单/字体 这些
    复合数据类型弄丢了，顺序也是组件表自己的顺序——跟易语言对不上。
    """
    lines = [
        "# 数据类型表（中文，人机共读）—— 支持库页「数据类型」直接读本文件",
        "# 顺序＝krnln.txt 声明顺序（窗口排第一，跟易语言一致），别排序",
        "# 格式：@类型 名 / @成员 名 / @事件 名", "",
    ]
    n_t = n_m = 0
    for t in types.values():
        lines.append("@类型 " + t["name"])
        lines.append("类型=" + ("窗口组件" if t["is_component"] else "复合数据"))
        if t["en"]:
            lines.append("英文名=" + t["en"])
        if t["flags"]:
            lines.append("属性=" + t["flags"])
        if t["desc"]:
            lines.append("说明=" + esc(t["desc"]))
        lines.append("成员数=%d" % len(t["members"]))
        lines.append("事件数=%d" % len(t["events"]))
        lines.append("方法数=%d" % t["n_methods"])   # 成员命令数（krnln 头带的），摘要行「共包含 N 个成员命令」用
        for m in t["members"]:
            lines.append("@成员 " + m["name"])
            lines.append("类型=" + m["type"])
            if m["en"]:
                lines.append("英文名=" + m["en"])
            if m["ptype"]:
                lines.append("属性类型=" + m["ptype"])
            # 可选值拼进说明：提示表备注列一列展示（esc 会把 | 换成 ／）
            desc = m["desc"]
            if m["choices"]:
                desc = (desc + "（可选值：" + m["choices"] + "）") if desc \
                       else ("可选值：" + m["choices"])
            if desc:
                lines.append("说明=" + esc(desc))
            n_m += 1
        for e in t["events"]:
            lines.append("@事件 " + e["name"])
            # 事件有返回值类型（可否被关闭=逻辑型、反馈事件=整数型…），
            # 不写出去 IDE 提示表里就只剩一列「—」，跟易语言对不上
            if e["ret"]:
                lines.append("返回=" + e["ret"])
            # 事件参数（空闲=已空闲时间、托盘事件=操作类型、反馈事件=2 个…）。
            # 紧跟 @事件 写，IDE 解析端按「参数=名|类型|说明」归给当前事件
            for pname, ptype, _ref, pdesc, _arr, _opt, _attr in e["params"]:
                lines.append("参数=%s|%s|%s" % (pname, ptype, esc(pdesc)))
            if e["desc"]:
                lines.append("说明=" + esc(e["desc"]))
        lines.append("")
        n_t += 1
    open(out, "w", encoding="utf-8", newline="\n").write("\n".join(lines))
    return n_t, n_m


def write_common_events(out):
    L = ["#pragma once", "", "#include <component_support.h>", "",
         "// 窗口组件通用事件（易语言核心库基类事件，42 个组件共享）。",
         "// 由 开发环境/工具/gen_from_krnln.py 生成，勿手改。",
         "// @base 窗口组件", ""]
    for cn, en, args, desc in COMMON_EVENTS:
        L += ["// @name " + cn, "// @description " + desc, "// @group 事件",
              "SULANG_MFC_EVENT(%s%s);" % (en, (", " + args) if args else ""), ""]
    open(out, "w", encoding="utf-8", newline="\n").write("\n".join(L))
    return len(COMMON_EVENTS)


def write_common_methods(members, out):
    L = ["#pragma once", "", "#include <component_support.h>", "",
         "// 成员命令总表（窗口组件/数据库/打印机等各自的方法都在这里声明）。",
         "// 每条方法的 // @type 行写明它属于哪个数据类型——这是 krnln 自己的归属，",
         "// 例如「打开」属于 外部数据库，不属于 窗口。IDE 按 @type 挂到对应类型下，",
         "// 不要把所有方法都塞给「窗口」。",
         "// 生成工具：开发环境/工具/gen_from_krnln.py，勿手改。", ""]
    for i, mc in enumerate(members, 1):
        L.append("// @name " + mc["name"])
        if mc.get("owners"):
            L.append("// @type " + ",".join(mc["owners"]))
        if mc["en"]:
            L.append("// @en " + mc["en"])          # 英文名（GetPixel/GetHWnd…），提示表「英文名称」那一格
        if mc["desc"]:
            L.append("// @description " + esc(mc["desc"]))
        L.append("// @ret " + (mc["ret"] or "无返回值"))
        for pname, ptype, _ref, pdesc, arr, opt, _attr in mc["params"]:
            L.append("// @param %s|%s|%s|%d" % (pname, ptype, esc(pdesc), opt))
        L.append("// @group 方法")
        L.append("SULANG_MFC_METHOD(m%04d);" % i)
        L.append("")
    open(out, "w", encoding="utf-8", newline="\n").write("\n".join(L))
    return len(members)


def write_component(t, out_dir):
    cls = "CSu%s" % re.sub(r'\W', '', t["en"])[:28].capitalize()
    L = ["#pragma once", "", "#include <component_support.h>",
         '#include "公共事件.h"', '#include "公共方法.h"', "",
         "// @component " + t["en"].lower(),
         "// @name " + t["name"],
         "// @description %s（由 krnln 5.7.64 生成，属性/方法/事件全量）" % t["name"],
         "// @name_prefix " + t["name"],
         "// @flags " + t["flags"],
         "class %s final : public sulang::controls::BasicControl<CWnd> {" % cls,
         "public:", ""]
    for m in t["members"]:
        L += ["    // @name " + m["name"]]
        if m["desc"]:
            L.append("    // @description " + esc(m["desc"]))
        L.append("    // @type " + m["type"])
        if m["ptype"]:
            L.append("    // @ptype " + m["ptype"])
        if m["choices"]:
            L.append("    // @choices " + esc(m["choices"]))
        L += ["    // @group 属性",
              "    SULANG_MFC_PROPERTY(%s);" % (m["en"] or ("p_" + str(len(L)))), ""]
    for mc in t["methods"]:
        L += ["    // @name " + mc["name"], "    // @method"]
        if mc["desc"]:
            L.append("    // @description " + esc(mc["desc"]))
        L += ["    SULANG_MFC_METHOD(%s);" % (mc["en"] or "m"), ""]
    for e in t["events"]:
        L += ["    // @name " + e["name"], "    // @group 事件"]
        if e["desc"]:
            L.append("    // @description " + esc(e["desc"]))
        L += ["    SULANG_MFC_EVENT(%s);" % (re.sub(r'\W', '', pinyin_fallback(e["name"]) or "evt")), ""]
    L.append("};")
    fn = os.path.join(out_dir, (t["en"].lower() or t["name"]) + ".h")
    open(fn, "w", encoding="utf-8", newline="\n").write("\n".join(L))
    return fn


_EVT_EN = {cn: en for cn, en, _a, _d in COMMON_EVENTS}


def pinyin_fallback(name):
    """事件英文名：通用事件用表里的，其余用 evt+序号由调用方补。"""
    return _EVT_EN.get(name, "")


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--src", default=DEF_SRC)
    ap.add_argument("--out", default=DEF_OUT)
    a = ap.parse_args()
    if not os.path.exists(a.src):
        print("找不到源文件:", a.src)
        return 1
    os.makedirs(a.out, exist_ok=True)
    comp_dir = os.path.join(a.out, "components")
    os.makedirs(comp_dir, exist_ok=True)

    cmds, members, types, consts = parse(a.src)
    n_cmd, n_cat = write_elib(cmds, members, os.path.join(a.out, "核心支持库.elib"))
    n_type, n_typemem = write_types(types, os.path.join(a.out, "数据类型.txt"))
    n_evt = write_common_events(os.path.join(a.out, "公共事件.h"))
    n_meth_all = write_common_methods(members, os.path.join(a.out, "公共方法.h"))
    n_meth = 0   # 组件头上的方法合计，别拿总表条数当起点（否则统计翻倍）

    comps = [t for t in types.values() if t["is_component"]]
    n_prop = n_cevt = 0
    report = ["组件\t属性\t方法\t事件\tkrnln声明(属性/事件/方法)\t校验"]
    for t in comps:
        write_component(t, comp_dir)
        ok = (len(t["members"]) == t["n_members"] and len(t["events"]) == t["n_events"]
              and len(t["methods"]) == t["n_methods"])
        report.append("%s\t%d\t%d\t%d\t%d/%d/%d\t%s" % (
            t["name"], len(t["members"]), len(t["methods"]), len(t["events"]),
            t["n_members"], t["n_events"], t["n_methods"], "OK" if ok else "差异"))
        n_prop += len(t["members"]); n_meth += len(t["methods"]); n_cevt += len(t["events"])

    with open(os.path.join(a.out, "data_types.txt"), "w", encoding="utf-8", newline="\n") as f:
        for t in types.values():
            f.write("%s\t类型=%s\t属性=%s\n" % (t["name"], "窗口组件" if t["is_component"] else "复合数据", t["flags"]))
    # 常量表（IDE 支持库树「常量」节点用）：.常量 行里抠 值/英文名/说明
    #   .常量 黑色, 数值, 值=0, 英文名=black, 说明=常用颜色值
    with open(os.path.join(a.out, "常量.txt"), "w", encoding="utf-8", newline="\n") as f:
        for name, ctype, raw in consts:
            val = re.search(r'值="((?:[^"\\]|\\.)*)"', raw)  # 带引号：\" 是转义的引号字符，别在它那儿截断（引号/换行符等常量）
            if not val:
                val = re.search(r'值=([^,]+)', raw)    # 不带引号：到逗号为止
            en = re.search(r'英文名=([^,\s]+)', raw)
            desc = re.search(r'说明=(.*)$', raw)
            f.write("@常量 %s\n" % name)
            f.write("类型=%s\n" % ctype)
            if val:
                f.write("值=%s\n" % val.group(1).strip())
            if en:
                f.write("英文名=%s\n" % en.group(1))
            if desc:
                f.write("说明=%s\n" % desc.group(1).strip())
            f.write("\n")

    with open(os.path.join(a.out, "report.txt"), "w", encoding="utf-8", newline="\n") as f:
        f.write("\n".join(report) + "\n\n")
        f.write("命令 %d 条 / 分类 %d 个\n" % (n_cmd, n_cat))
        f.write("组件 %d 个：属性 %d / 方法 %d / 事件 %d（另有通用事件 %d）\n" % (
            len(comps), n_prop, n_meth, n_cevt, n_evt))
        f.write("数据类型 %d 个 / 常量 %d 个\n" % (len(types), len(consts)))

    print("输出目录:", a.out)
    print("命令 %d 条 / %d 分类 -> 核心支持库.elib" % (n_cmd, n_cat))
    print("组件 %d 个：属性 %d / 方法 %d / 事件 %d（+通用事件 %d）" % (len(comps), n_prop, n_meth, n_cevt, n_evt))
    print("数据类型 %d / 常量 %d（数据类型.txt：成员 %d 条）" % (len(types), len(consts), n_typemem))
    bad = [r for r in report[1:] if "差异" in r]
    print("校验不一致的组件:", len(bad))
    for r in bad[:8]:
        print("   ", r)
    return 0


if __name__ == "__main__":
    sys.exit(main())
