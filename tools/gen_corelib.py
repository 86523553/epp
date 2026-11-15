#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""把「核心支持库」中文命令文档（GBK）转换成本项目自己的 .elib 库定义（UTF-8）。

.elib 是我们自己的库格式（纯文本、人可读、IDE 启动时读入）：

    # 库名=核心支持库
    # 版本=1.0.0
    @分类 文本操作
    @命令 取文本长度
    英文名=len
    返回=整数型
    说明=返回文本数据的长度……
    参数=文本数据|文本型|欲检查其长度的文本数据|0

用法：
    python gen_corelib.py                 # 输出到 运行环境/核心支持库/核心支持库.elib
"""
import os
import re
import sys

HERE = os.path.dirname(os.path.abspath(__file__))
ROOT = os.path.dirname(HERE)                       # 开发环境/
PROJ = os.path.dirname(ROOT)                       # 项目根
SRC = os.environ.get('CORE_DOC', r'<su-lang 源码目录>/核心支持库')  # 源文档（GBK，一次性原料）
OUT = os.path.join(PROJ, '运行环境', '核心支持库', '核心支持库.elib')

LIB_NAME = '核心支持库'
LIB_VER = '1.0.0'
LIB_DESC = '文本、数组、字节集、数值、时间、文件、磁盘、网络、系统等基础命令。'
LIB_STATIC = '..\\静态库\\libcore_support.a'

CARD_RE = re.compile(r'^\s*调用格式[：:]\s*(.*)$')
PARAM_RE = re.compile(r'^\s*参数<(\d+)>的名称为[“"](.*?)[”"]，类型为[“"]([^”"]*)[”"](.*)$')


def clean_type(t):
    """'文本型（text）' -> '文本型'"""
    t = t.strip()
    return re.sub(r'（[^）]*）', '', t).strip()


def parse_signature(line):
    """解析调用格式行，返回 (返回类型, 命令名, [(参数类型, 参数名, 可选)])"""
    # 去掉尾部 '- 系统核心支持库->文本操作'
    line = line.split(' - ')[0].strip()
    m = re.match(r'^〈([^〉]*)〉\s*(.*)$', line)
    ret = ''
    if m:
        ret = clean_type(m.group(1))
        line = m.group(2).strip()
    # 命令名取到第一个 （ 或 ( 之前
    k = -1
    for ch in ('（', '('):
        p = line.find(ch)
        if p >= 0 and (k < 0 or p < k):
            k = p
    if k < 0:
        return ret, line, []
    name = line[:k].strip()
    inner = line[k + 1:]
    for ch in ('）', ')'):
        p = inner.rfind(ch)
        if p >= 0:
            inner = inner[:p]
            break
    inner = inner.strip()
    params = []
    if inner and inner not in ('无', '...'):
        # 参数之间用全角/半角逗号分隔；可选参数用 ［］ 包裹
        depth = 0
        buf = ''
        parts = []
        for ch in inner:
            if ch in '［[':
                depth += 1
                buf += ch
            elif ch in '］]':
                depth -= 1
                buf += ch
            elif ch in ',，' and depth == 0:
                parts.append(buf)
                buf = ''
            else:
                buf += ch
        parts.append(buf)
        for p in parts:
            p = p.strip()
            if not p:
                continue
            optional = p.startswith('［') or p.startswith('[')
            p = p.strip('［］[] ').strip()
            if p.endswith('...') or p == '…':
                continue
            # "类型 名称"：类型是不含空格的词，名称可含空格
            bits = p.split(None, 1)
            if len(bits) == 2:
                params.append((clean_type(bits[0]), bits[1].strip(), optional))
            elif len(bits) == 1:
                params.append(('', bits[0], optional))
    return ret, name, params


def parse_doc(path):
    """解析一个分类文档，返回 (分类名, [命令 dict])"""
    text = open(path, 'rb').read().decode('gbk', errors='replace')
    cat = os.path.splitext(os.path.basename(path))[0]
    m = re.search(r'---\s*命令类别[：:]\s*(.*?)\s*---', text)
    if m:
        cat = m.group(1).strip()

    lines = text.splitlines()
    cmds = []
    cur = None
    for raw in lines:
        s = raw.strip()
        mc = CARD_RE.match(raw)
        if mc:
            if cur:
                cmds.append(cur)
            ret, name, params = parse_signature(mc.group(1))
            cur = {'cat': cat, 'name': name, 'ret': ret, 'en': '',
                   'desc': '', 'params': [{'type': t, 'name': n, 'opt': o, 'desc': ''}
                                          for (t, n, o) in params],
                   'raw_params': len(params)}
            continue
        if cur is None:
            continue
        if s.startswith('英文名称'):
            cur['en'] = s.split('：')[-1].split(':')[-1].strip()
            continue
        mp = PARAM_RE.match(raw)
        if mp:
            idx = int(mp.group(1)) - 1
            pname = mp.group(2).strip()
            ptype = clean_type(mp.group(3))
            pdesc = mp.group(4).strip()
            if 0 <= idx < len(cur['params']):
                cur['params'][idx]['name'] = pname
                cur['params'][idx]['type'] = ptype
                cur['params'][idx]['desc'] = pdesc
            else:
                cur['params'].append({'type': ptype, 'name': pname,
                                      'opt': True, 'desc': pdesc})
            continue
        if s.startswith('参数<'):
            # 参数说明的续行（“参数<1>的…”之后可能还有补充）
            continue
        if s.startswith('操作系统需求') or s.startswith('---'):
            continue
        if not s:
            continue
        if s.startswith('调用格式') or s.startswith('英文名称'):
            continue
        # 说明行（第一段），跳过“本命令为…命令。”以外的补充也保留
        if not cur['desc']:
            cur['desc'] = s
        elif not cur['desc'].endswith('。') and len(cur['desc']) < 200:
            cur['desc'] += s
    if cur:
        cmds.append(cur)
    # 丢掉解析不出名字的
    cmds = [c for c in cmds if c['name']]
    return cat, cmds


def main():
    if not os.path.isdir(SRC):
        print('源文档目录不存在：', SRC)
        return 1
    files = sorted(f for f in os.listdir(SRC) if f.lower().endswith('.txt'))
    if not files:
        print('源文档目录里没有 .txt')
        return 1

    cats = []
    total = 0
    for fn in files:
        cat, cmds = parse_doc(os.path.join(SRC, fn))
        if not cmds:
            continue
        cats.append((cat, cmds))
        total += len(cmds)
        print('  %-8s %3d 条' % (cat, len(cmds)))

    os.makedirs(os.path.dirname(OUT), exist_ok=True)
    with open(OUT, 'w', encoding='utf-8', newline='\r\n') as f:
        f.write('# 库名=%s\n' % LIB_NAME)
        f.write('# 版本=%s\n' % LIB_VER)
        f.write('# 说明=%s\n' % LIB_DESC)
        f.write('# 静态库=%s\n' % LIB_STATIC)
        f.write('# 本文件由 开发环境/工具/gen_corelib.py 生成，IDE 启动时读入\n')
        for cat, cmds in cats:
            f.write('\n@分类 %s\n' % cat)
            for c in cmds:
                f.write('@命令 %s\n' % c['name'])
                f.write('英文名=%s\n' % c['en'])
                f.write('返回=%s\n' % c['ret'])
                f.write('说明=%s\n' % c['desc'].replace('\n', ' '))
                for p in c['params']:
                    f.write('参数=%s|%s|%s|%d\n' % (
                        p['name'], p['type'], p['desc'].replace('\n', ' '),
                        1 if p['opt'] else 0))
    print('输出：%s' % OUT)
    print('分类 %d 个，命令 %d 条，%d 字节' % (len(cats), total, os.path.getsize(OUT)))
    return 0


if __name__ == '__main__':
    sys.exit(main())
