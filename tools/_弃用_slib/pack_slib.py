#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""打包单文件支持库 .slib（zip 容器）。

目录结构（包内）：
    manifest.json    中文元信息，查看器/IDE 支持库页直接读
    命令.txt         中文命令表  @分类/@命令/英文名/返回/说明/参数
    组件.txt         中文组件表  组件 -> 属性/事件/方法（全中文，给属性框和查看器）
    include\*.h      给编译器
    src\*.cpp        源码库才需要
    *.a / *.lib      静态库

用法：
    python pack_slib.py --name 核心支持库 \
        --elib ../../运行环境/核心支持库/核心支持库.elib \
        --comp-dir ../../运行环境/组件库/核心组件库/include \
        --static-dir ../../运行环境/静态库 \
        --out ../../运行环境/核心支持库.slib

    python pack_slib.py --list  核心支持库.slib     # 列出包内容（查看器原型）
    python pack_slib.py --dump 核心支持库.slib 命令  # 打印中文命令表
"""
import argparse
import json
import os
import re
import sys
import zipfile

# ── 读 .elib（# 键=值 / @分类 / @命令 / 英文名= / 返回= / 说明= / 参数=）──
def parse_elib(path):
    meta, cats, cmds = {}, [], []
    cur = None
    cat = ""
    for raw in open(path, encoding="utf-8", errors="replace").read().splitlines():
        s = raw.strip()
        if not s:
            continue
        if s.startswith("#"):
            k, _, v = s[1:].partition("=")
            meta[k.strip()] = v.strip()
            continue
        if s.startswith("@分类"):
            cat = s[3:].strip()
            cats.append(cat)
            continue
        if s.startswith("@命令"):
            cur = {"name": s[3:].strip(), "cat": cat, "en": "", "ret": "", "desc": "", "params": []}
            cmds.append(cur)
            continue
        if cur is None:
            continue
        if s.startswith("英文名="):
            cur["en"] = s[4:].strip()
        elif s.startswith("返回="):
            cur["ret"] = s[3:].strip()
        elif s.startswith("说明="):
            cur["desc"] = s[3:].strip()
        elif s.startswith("参数="):
            p = s[3:].split("|")
            while len(p) < 4:
                p.append("")
            cur["params"].append({"name": p[0], "type": p[1], "desc": p[2], "opt": p[3]})
    return meta, cats, cmds


# ── 解析组件头（// @name / @type / @group 属性|事件|方法）──
def parse_components(inc_dir):
    comps = []
    for fn in sorted(os.listdir(inc_dir)):
        if not fn.lower().endswith(".h"):
            continue
        txt = open(os.path.join(inc_dir, fn), encoding="utf-8", errors="replace").read()
        if "@component" not in txt:
            continue
        comp = {"file": fn, "name": "", "en": "", "win32": "", "props": [], "events": [], "methods": []}
        lines = txt.splitlines()
        i = 0
        while i < len(lines):
            s = lines[i].strip()
            if s.startswith("// @component"):
                comp["en"] = s.split(None, 2)[-1].strip()
            elif s.startswith("// @name ") and not comp["name"]:
                comp["name"] = s[9:].strip()
            elif s.startswith("// @win32 "):
                comp["win32"] = s[9:].strip()
            i += 1
        # 逐条宏，回溯它上方的注释
        for i, s in enumerate(lines):
            st = s.strip()
            kind = None
            if st.startswith("SULANG_MFC_PROPERTY("):
                kind = "props"
            elif st.startswith("SULANG_MFC_EVENT("):
                kind = "events"
            elif st.startswith("SULANG_MFC_METHOD("):
                kind = "methods"
            if not kind:
                continue
            item = {"name": "", "type": "", "desc": "", "choices": "", "ret": ""}
            for j in range(i - 1, max(-1, i - 9), -1):
                t = lines[j].strip()
                if t.startswith("// @name "):
                    item["name"] = t[9:].strip()
                elif t.startswith("// @description "):
                    item["desc"] = t[15:].strip()
                elif t.startswith("// @type "):
                    item["type"] = t[9:].strip()
                elif t.startswith("// @choices "):
                    item["choices"] = t[11:].strip()
                elif t.startswith("// @ret "):
                    item["ret"] = t[7:].strip()
                elif t.startswith("// @"):
                    continue
                elif t == "":
                    continue
                else:
                    break
            if item["name"]:
                comp[kind].append(item)
        if comp["name"]:
            comps.append(comp)
    return comps


def write_texts(tmp, meta, cats, cmds, comps):
    L = ["# 核心命令表（中文，人机共读）",
         "# 格式：@分类 名 / @命令 名 / 英文名= / 返回= / 说明= / 参数=名|类型|说明|可选",
         ""]
    order = []
    for c in cmds:
        if c["cat"] not in order:
            order.append(c["cat"])
    for cat in (order or cats):
        L.append("@分类 " + cat)
        for c in [x for x in cmds if x["cat"] == cat]:
            L.append("@命令 " + c["name"])
            if c["en"]:
                L.append("英文名=" + c["en"])
            if c["ret"]:
                L.append("返回=" + c["ret"])
            if c["desc"]:
                L.append("说明=" + c["desc"])
            for p in c["params"]:
                L.append("参数=%s|%s|%s|%s" % (p["name"], p["type"], p["desc"], p["opt"]))
        L.append("")
    open(os.path.join(tmp, "命令.txt"), "w", encoding="utf-8", newline="\n").write("\n".join(L))

    C = ["# 组件表（中文，人机共读）—— 属性框/查看器直接读本文件，不需要解析 .h",
         "# 格式：@组件 名 / 文件= / win32= / @属性 名 / @事件 名 / @方法 名",
         ""]
    for c in comps:
        C.append("@组件 " + c["name"])
        C.append("文件=" + c["file"])
        if c["en"]:
            C.append("英文名=" + c["en"])
        if c["win32"]:
            C.append("win32=" + c["win32"])
        C.append("属性数=%d" % len(c["props"]))
        for p in c["props"]:
            C.append("@属性 " + p["name"])
            if p["type"]:
                C.append("类型=" + p["type"])
            if p["choices"]:
                C.append("可选值=" + p["choices"])
            if p["desc"]:
                C.append("说明=" + p["desc"])
        C.append("事件数=%d" % len(c["events"]))
        for e in c["events"]:
            C.append("@事件 " + e["name"] + (("|" + e["ret"]) if e["ret"] else ""))
            if e["desc"]:
                C.append("说明=" + e["desc"])
        C.append("方法数=%d" % len(c["methods"]))
        for m in c["methods"]:
            C.append("@方法 " + m["name"] + (("|" + m["ret"]) if m["ret"] else ""))
            if m["desc"]:
                C.append("说明=" + m["desc"])
        C.append("")
    open(os.path.join(tmp, "组件.txt"), "w", encoding="utf-8", newline="\n").write("\n".join(C))


def cmd_pack(a):
    tmp = os.path.join(os.path.dirname(os.path.abspath(a.out)) or ".", "_pack_tmp")
    os.makedirs(tmp, exist_ok=True)
    meta, cats, cmds = ({}, [], [])
    if a.elib:
        meta, cats, cmds = parse_elib(a.elib)
    comps = parse_components(a.comp_dir) if a.comp_dir else []
    write_texts(tmp, meta, cats, cmds, comps)

    man = {
        "格式": "slib",
        "格式版本": 1,
        "名称": a.name,
        "版本": meta.get("版本", "1.0.0"),
        "说明": meta.get("说明", ""),
        "组件名": a.comp_name or (a.name + "组件"),
        "统计": {"命令": len(cmds), "分类": len(set(c["cat"] for c in cmds)),
                 "组件": len(comps),
                 "属性": sum(len(c["props"]) for c in comps),
                 "事件": sum(len(c["events"]) for c in comps),
                 "方法": sum(len(c["methods"]) for c in comps)},
        "文件": {"命令表": "命令.txt", "组件表": "组件.txt"},
    }
    mp = os.path.join(tmp, "manifest.json")
    open(mp, "w", encoding="utf-8", newline="\n").write(json.dumps(man, ensure_ascii=False, indent=2))

    n = 0
    with zipfile.ZipFile(a.out, "w", zipfile.ZIP_DEFLATED, compresslevel=9) as z:
        z.write(mp, "manifest.json")
        z.write(os.path.join(tmp, "命令.txt"), "命令.txt")
        if comps:
            z.write(os.path.join(tmp, "组件.txt"), "组件.txt")
        if a.comp_dir:
            for fn in sorted(os.listdir(a.comp_dir)):
                if fn.lower().endswith(".h"):
                    z.write(os.path.join(a.comp_dir, fn), "include/" + fn)
                    n += 1
        if a.static_dir:
            for fn in sorted(os.listdir(a.static_dir)):
                fp = os.path.join(a.static_dir, fn)
                if os.path.isfile(fp) and fn.lower().endswith((".a", ".lib")):
                    z.write(fp, "lib/" + fn)
                elif fn.lower() == "include" and os.path.isdir(fp):
                    for h in sorted(os.listdir(fp)):
                        z.write(os.path.join(fp, h), "include/" + h)
                        n += 1
    print("已生成:", a.out, "%.1f KB" % (os.path.getsize(a.out) / 1024))
    print("统计:", json.dumps(man["统计"], ensure_ascii=False))
    print("头文件 %d 个" % n)


def cmd_list(path):
    with zipfile.ZipFile(path) as z:
        man = json.loads(z.read("manifest.json").decode("utf-8"))
        print("名称:", man["名称"], "版本:", man["版本"])
        print("统计:", json.dumps(man["统计"], ensure_ascii=False))
        print("--- 包内文件 ---")
        for i in z.infolist():
            print("  %-24s %7d B" % (i.filename, i.file_size))


def cmd_dump(path, what):
    fn = "命令.txt" if what.startswith("命令") else "组件.txt"
    with zipfile.ZipFile(path) as z:
        print(z.read(fn).decode("utf-8"))


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--name")
    ap.add_argument("--comp-name", default="")
    ap.add_argument("--elib")
    ap.add_argument("--comp-dir")
    ap.add_argument("--static-dir")
    ap.add_argument("--out")
    ap.add_argument("--list", metavar="SLIB")
    ap.add_argument("--dump", nargs=2, metavar=("SLIB", "命令|组件"))
    a = ap.parse_args()
    if a.list:
        return cmd_list(a.list)
    if a.dump:
        return cmd_dump(a.dump[0], a.dump[1])
    if not (a.name and a.out):
        ap.print_help()
        return 1
    return cmd_pack(a)


if __name__ == "__main__":
    sys.exit(main())
