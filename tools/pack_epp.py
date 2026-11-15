#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""打包单文件支持库 .epp（易屁屁 epp 库格式，zip 容器，人机共读）。

包内结构：
    manifest.json    中文元信息（名称/版本/统计/文件索引），查看器与支持库页直接读
    命令.txt         中文命令表   @分类 / @命令 / 英文名= / 返回= / 说明= / 参数=
    组件.txt         中文组件表   @组件 / @属性 / @事件 / @方法（全中文，无需解析 .h）
    include/*.h      给编译器
    lib/*.a|*.lib    静态库（可选）
    src/*.cpp        源码库（可选）

用法：
    python pack_epp.py --name 核心支持库 \
        --elib ../../运行环境/核心支持库/核心支持库.elib \
        --comp-dir ../../运行环境/组件库/核心组件库/include \
        --static-dir ../../运行环境/静态库 \
        --out ../../运行环境/核心支持库.epp

    python pack_epp.py --list 核心支持库.epp        # 列包（查看器原型）
    python pack_epp.py --dump 核心支持库.epp 命令    # 打印中文命令表
    python pack_epp.py --dump 核心支持库.epp 组件    # 打印中文组件表
"""
import argparse
import json
import os
import sys
import zipfile

FMT = "epp"
EXT = ".epp"


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
            while len(p) < 5:
                p.append("")
            cur["params"].append({"name": p[0], "type": p[1], "desc": p[2],
                                  "opt": p[3], "attr": p[4]})
    return meta, cats, cmds


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
        for s in lines:
            t = s.strip()
            if t.startswith("// @component") and not comp["en"]:
                comp["en"] = t.split(None, 2)[-1].strip()
            elif t.startswith("// @name ") and not comp["name"]:
                comp["name"] = t[9:].strip()
            elif t.startswith("// @win32 ") and not comp["win32"]:
                comp["win32"] = t[9:].strip()
        for i, s in enumerate(lines):
            st = s.strip()
            if st.startswith("SULANG_MFC_PROPERTY("):
                kind = "props"
            elif st.startswith("SULANG_MFC_EVENT("):
                kind = "events"
            elif st.startswith("SULANG_MFC_METHOD("):
                kind = "methods"
            else:
                continue
            item = {"name": "", "type": "", "desc": "", "choices": "", "ret": ""}
            for j in range(i - 1, max(-1, i - 40), -1):
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
                elif t.startswith("// @") or t == "":
                    continue
                else:
                    break
            if item["name"]:
                comp[kind].append(item)
        if comp["name"]:
            comps.append(comp)
    return comps


def parse_common(path):
    """解析公共事件/公共方法头（全组件共享），返回条目列表。"""
    items = []
    if not os.path.exists(path):
        return items
    lines = open(path, encoding="utf-8", errors="replace").read().splitlines()
    for i, s in enumerate(lines):
        st = s.strip()
        if not (st.startswith("SULANG_MFC_EVENT(") or st.startswith("SULANG_MFC_METHOD(")):
            continue
        it = {"name": "", "desc": "", "ret": "", "type": ""}
        for j in range(i - 1, max(-1, i - 40), -1):
            t = lines[j].strip()
            if t.startswith("// @name "):
                it["name"] = t[9:].strip()
            elif t.startswith("// @description "):
                it["desc"] = t[15:].strip()
            elif t.startswith("// @ret "):
                it["ret"] = t[7:].strip()
            elif t.startswith("// @type "):
                it["type"] = t[9:].strip()
            elif t.startswith("// @") or t == "":
                continue
            else:
                break
        if it["name"]:
            items.append(it)
    return items


def write_texts(tmp, cmds, comps, common_evt=None, common_meth=None):
    L = ["# 核心命令表（中文，人机共读）",
         "# 格式：@分类 名 / @命令 名 / 英文名= / 返回= / 说明= / 参数=名|类型|说明|可省略|属性", ""]
    order = []
    for c in cmds:
        if c["cat"] not in order:
            order.append(c["cat"])
    for cat in order:
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
                L.append("参数=%s|%s|%s|%s|%s" % (p["name"], p["type"], p["desc"],
                                                   p["opt"], p.get("attr", "")))
        L.append("")
    open(os.path.join(tmp, "命令.txt"), "w", encoding="utf-8", newline="\n").write("\n".join(L))

    C = ["# 组件表（中文，人机共读）—— 属性框与查看器直接读本文件，不需要解析 .h",
         "# 格式：@组件 名 / 文件= / win32= / @属性 名 / @事件 名 / @方法 名", ""]
    if common_evt:
        C.append("# ===== 公共事件（全部组件共享，共 %d 条）=====" % len(common_evt))
        C.append("@公共事件 %d" % len(common_evt))
        for e in common_evt:
            C.append("@事件 " + e["name"] + (("|" + e["ret"]) if e["ret"] else ""))
            if e["desc"]:
                C.append("说明=" + e["desc"])
        C.append("")
    if common_meth:
        C.append("# ===== 公共方法（全部组件共享，共 %d 条）=====" % len(common_meth))
        C.append("@公共方法 %d" % len(common_meth))
        for m in common_meth:
            C.append("@方法 " + m["name"] + (("|" + m["ret"]) if m["ret"] else ""))
            if m["desc"]:
                C.append("说明=" + m["desc"])
        C.append("")
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
    meta, _cats, cmds = ({}, [], [])
    if a.elib:
        meta, _cats, cmds = parse_elib(a.elib)
    comps = parse_components(a.comp_dir) if a.comp_dir else []
    common_evt, common_meth = [], []
    if a.comp_dir:
        common_evt = parse_common(os.path.join(a.comp_dir, "公共事件.h"))
        common_meth = parse_common(os.path.join(a.comp_dir, "公共方法.h"))
    write_texts(tmp, cmds, comps, common_evt, common_meth)

    # 数据类型.txt / 常量.txt 由 gen_from_krnln.py 生成（跟 .elib 同目录），原样带进包
    if a.elib:
        import shutil as _cp
        for extra in ("数据类型.txt", "常量.txt"):
            src_tp = os.path.join(os.path.dirname(os.path.abspath(a.elib)), extra)
            if os.path.exists(src_tp):
                _cp.copyfile(src_tp, os.path.join(tmp, extra))

    man = {
        "格式": FMT,
        "格式版本": 1,
        "生成器": "pack_epp.py",
        "名称": a.name,
        "版本": meta.get("版本", "1.0.0"),
        "说明": meta.get("说明", ""),
        "统计": {"命令": len(cmds), "分类": len(set(c["cat"] for c in cmds)),
                 "组件": len(comps),
                 "属性": sum(len(c["props"]) for c in comps),
                 "事件": sum(len(c["events"]) for c in comps),
                 "方法": sum(len(c["methods"]) for c in comps),
                 "公共事件": len(common_evt), "公共方法": len(common_meth)},
        "文件": {"命令表": "命令.txt", "数据类型表": "数据类型.txt", "组件表": "组件.txt", "常量表": "常量.txt"},
    }
    mp = os.path.join(tmp, "manifest.json")
    open(mp, "w", encoding="utf-8", newline="\n").write(json.dumps(man, ensure_ascii=False, indent=2))

    nh = 0
    done_hdr = set()
    comp = zipfile.ZIP_STORED if a.store else zipfile.ZIP_DEFLATED
    kw = {} if a.store else {"compresslevel": 9}
    with zipfile.ZipFile(a.out, "w", comp, **kw) as z:
        z.write(mp, "manifest.json")
        z.write(os.path.join(tmp, "命令.txt"), "命令.txt")
        tp = os.path.join(tmp, "数据类型.txt")
        if os.path.exists(tp):
            z.write(tp, "数据类型.txt")     # 51 个数据类型（易语言原序），支持库页直接读
        cp2 = os.path.join(tmp, "常量.txt")
        if os.path.exists(cp2):
            z.write(cp2, "常量.txt")        # 335 个常量（名称/类型/值/英文名/说明）
        if comps:
            z.write(os.path.join(tmp, "组件.txt"), "组件.txt")
        if a.comp_dir:
            for fn in sorted(os.listdir(a.comp_dir)):
                if fn.lower().endswith(".h"):
                    z.write(os.path.join(a.comp_dir, fn), "include/" + fn)
                    done_hdr.add(fn); nh += 1
        for inc in (a.inc_dir or []):
            if not os.path.isdir(inc):
                print("  ! 头文件目录不存在:", inc); continue
            for h in sorted(os.listdir(inc)):
                if not h.lower().endswith(".h"): continue
                if h in done_hdr:
                    print("  ! 头文件重名已跳过:", h); continue
                z.write(os.path.join(inc, h), "include/" + h)
                done_hdr.add(h); nh += 1
        if a.static_dir:
            for fn in sorted(os.listdir(a.static_dir)):
                fp = os.path.join(a.static_dir, fn)
                if os.path.isfile(fp) and fn.lower().endswith((".a", ".lib")):
                    z.write(fp, "lib/" + fn)
                elif fn.lower() == "include" and os.path.isdir(fp):
                    for h in sorted(os.listdir(fp)):
                        z.write(os.path.join(fp, h), "include/" + h)
                        nh += 1
    print("已生成: %s  %.1f KB" % (a.out, os.path.getsize(a.out) / 1024))
    print("统计:", json.dumps(man["统计"], ensure_ascii=False))
    print("头文件 %d 个" % nh)
    import shutil as _sh
    _sh.rmtree(tmp, ignore_errors=True)   # 打完就清中间目录，别在 环境\ 里留 _pack_tmp


def cmd_list(path):
    with zipfile.ZipFile(path) as z:
        man = json.loads(z.read("manifest.json").decode("utf-8"))
        print("格式:", man.get("格式"), "| 名称:", man["名称"], "| 版本:", man["版本"])
        print("统计:", json.dumps(man["统计"], ensure_ascii=False))
        print("--- 包内文件 ---")
        for i in z.infolist():
            print("  %-26s %8d B" % (i.filename, i.file_size))


def cmd_dump(path, what):
    fn = "命令.txt" if what.startswith("命令") else (
         "数据类型.txt" if what.startswith("数据类型") else "组件.txt")
    with zipfile.ZipFile(path) as z:
        print(z.read(fn).decode("utf-8"))


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--name")
    ap.add_argument("--elib")
    ap.add_argument("--comp-dir")
    ap.add_argument("--static-dir")
    ap.add_argument("--out")
    ap.add_argument("--list", metavar="EPP")
    ap.add_argument("--dump", nargs=2, metavar=("EPP", "命令|组件"))
    ap.add_argument("--inc-dir", action="append", help="额外头文件目录，可重复（如 静态库\include）")
    ap.add_argument("--store", action="store_true", help="不压缩（stored），给 C++ 直接用偏移读")
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
