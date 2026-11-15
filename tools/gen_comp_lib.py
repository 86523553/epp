#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
把 ewinform（makerup）里手写标定的易语言组件表，生成成我们组件库的头文件。

数据源：ewinform/src/mk_class_table.cpp
  42 个组件类 / 694 条属性 / 63 条事件，每条属性带
    属性ID(0x3E9+序号)、UD 数据类型、标志位、中文名、英文名、候选文本表、组件默认值。

产物：运行环境/组件库/核心组件库/include/<key>.h
  格式就是苏浪/我们 IDE 已经在用的支持库注解格式（@name/@type/@default/@group/
  @options + SULANG_MFC_PROPERTY），IDE 只解析不内置规则，库一改属性页自动跟随。

用法：
  python gen_comp_lib.py <mk_class_table.cpp 路径> [--out <目录>] [--dry]
"""
import io
import os
import re
import sys

HERE = os.path.dirname(os.path.abspath(__file__))
PROJ = os.path.dirname(os.path.dirname(HERE))
DEFAULT_OUT = os.path.join(PROJ, '运行环境', '组件库', '核心组件库', 'include')

# ── 属性数据类型（SDK lib2.h 的 UD_*）→ 我们的 @type 文本 ──────────────
UD_NUM = {
    'MK_UD_NONE': 0, 'MK_UD_PICK_SPEC_INT': 1000, 'MK_UD_INT': 1001, 'MK_UD_DOUBLE': 1002,
    'MK_UD_BOOL': 1003, 'MK_UD_DATE_TIME': 1004, 'MK_UD_TEXT': 1005, 'MK_UD_PICK_INT': 1006,
    'MK_UD_PICK_TEXT': 1007, 'MK_UD_EDIT_PICK_TEXT': 1008, 'MK_UD_PIC': 1009,
    'MK_UD_ICON': 1010, 'MK_UD_CURSOR': 1011, 'MK_UD_MUSIC': 1012, 'MK_UD_FONT': 1013,
    'MK_UD_COLOR': 1014, 'MK_UD_COLOR_TRANS': 1015, 'MK_UD_FILE_NAME': 1016,
    'MK_UD_COLOR_BACK': 1017, 'MK_UD_DATA_SOURCE_NAME': 1018, 'MK_UD_DSCOL_NAME': 1020,
    'MK_UD_IMAGE_LIST': 1023, 'MK_UD_CUSTOMIZE': 1024,
}
UD_TYPE = {
    1000: '整数型(选择)',
    1001: '整数型',
    1002: '小数型',
    1003: '逻辑型',
    1004: '日期时间型',
    1005: '文本型',
    1006: '文本型(选择)',
    1007: '文本型(选择)',
    1008: '文本型(选择)',
    1009: '图片型',
    1010: '图标型',
    1011: '光标型',
    1012: '音乐型',
    1013: '字体型',
    1014: '颜色型',
    1015: '颜色型',
    1016: '文本型',
    1017: '颜色型',
    1018: '文本型',
    1020: '文本型',
    1023: '图片型',
    1024: '文本型',
}

# 组件 key（英文）→ 组件箱默认尺寸、默认文本。核心库的类名才是权威，
# 这里只补设计器「拖一个出来」时的初始大小（易语言同款观感）。
SIZES = {
    'window': (380, 250), 'edit': (160, 28), 'picture': (180, 120),
    'shape': (100, 80), 'drawpanel': (200, 150), 'groupbox': (200, 120),
    'label': (88, 24), 'button': (96, 32), 'checkbox': (112, 26), 'radio': (112, 26),
    'combo': (160, 28), 'listbox': (180, 104), 'checklistbox': (180, 104),
    'hscrollbar': (160, 16), 'vscrollbar': (16, 120), 'progress': (160, 18),
    'slider': (160, 28), 'tab': (240, 160), 'animate': (160, 100),
    'datepicker': (140, 24), 'monthcalendar': (220, 160), 'drvcombo': (160, 28),
    'dirtree': (180, 140), 'filelist': (180, 140), 'colorpicker': (100, 24),
    'hyperlink': (120, 24), 'spin': (20, 24), 'commondlg': (32, 32),
    'timer': (32, 32), 'printer': (32, 32), 'udp': (32, 32), 'client': (32, 32),
    'server': (32, 32), 'serialport': (32, 32), 'grid': (240, 140),
    'datasource': (32, 32), 'provider': (32, 32), 'dbprovider': (32, 32),
    'picbtn': (96, 32), 'odbcdb': (32, 32), 'odbcprovider': (32, 32),
    'errcontrol': (32, 32),
}

# 组件 key → 内置图标/绘制样式里认识的键（KIND_STYLE）
STYLE_KEY = {
    'editbox': 'edit', 'picturebox': 'picture', 'shapebox': 'shape',
    'drawpanel': 'drawpanel', 'groupbox': 'groupbox', 'label': 'label',
    'button': 'button', 'checkbox': 'checkbox', 'radiobox': 'radio',
    'combobox': 'combo', 'listbox': 'listbox', 'checklistbox': 'checklistbox',
}

# 易语言里自带标题/内容的组件（新建时填自己的中文名，和易语言一致）
CURSOR_OPTS = '默认型|十字型|手型|I 型|等待|禁止|上下箭头|左右箭头'

# 组件 key（英文）→ 组件箱默认尺寸、默认文本。核心库的类名才是权威，
# 这里只补设计器「拖一个出来」时的初始大小（易语言同款观感）。
TEXTY = {'标签', '按钮', '选择框', '单选框', '图形按钮'}

# 生成构建用的运行期控件声明：@win32 = 系统窗口类（"-" 表示非可视组件，
# 运行期不建窗口），@win32_style = CreateWindowExW 的样式表达式文本。
# 这两行是「库」告诉构建器的，IDE 生成器只负责原样抄进生成的 C++，
# 不在 C++ 里给任何组件写死映射。
WIN32_MAP = {
    '标签': ('Static', 'SS_LEFT|SS_NOTIFY'),
    '按钮': ('Button', 'BS_PUSHBUTTON|BS_NOTIFY'),
    '编辑框': ('Edit', 'ES_AUTOHSCROLL|WS_BORDER'),
    '多行编辑框': ('Edit', 'ES_MULTILINE|ES_AUTOVSCROLL|ES_WANTRETURN|WS_VSCROLL|WS_BORDER'),
    '选择框': ('Button', 'BS_AUTOCHECKBOX|BS_NOTIFY'),
    '单选框': ('Button', 'BS_AUTORADIOBUTTON|BS_NOTIFY'),
    '组合框': ('ComboBox', 'CBS_DROPDOWNLIST|WS_VSCROLL'),
    '列表框': ('ListBox', 'LBS_NOTIFY|WS_VSCROLL'),
    '选择列表框': ('ListBox', 'LBS_NOTIFY|LBS_MULTIPLESEL|WS_VSCROLL'),
    '图片框': ('Static', 'SS_SUNKEN|SS_NOTIFY'),
    '分组框': ('Button', 'BS_GROUPBOX'),
    '图形按钮': ('Button', 'BS_PUSHBUTTON|BS_OWNERDRAW'),
    '外形框': ('Static', 'SS_SUNKEN'),
    '画板': ('Static', 'SS_SUNKEN'),
    '横向滚动条': ('Scrollbar', 'SBS_HORZ'),
    '纵向滚动条': ('Scrollbar', 'SBS_VERT'),
    '进度条': ('msctls_progress32', 'PBS_SMOOTH'),
    '滑块条': ('msctls_trackbar32', 'TBS_AUTOTICKS'),
    '选择夹': ('SysTabControl32', '0'),
    '影像框': ('Static', 'SS_SUNKEN'),
    '日期框': ('SysDateTimePick32', '0'),
    '月历': ('SysMonthCal32', '0'),
    '驱动器框': ('ComboBox', 'CBS_DROPDOWNLIST|WS_VSCROLL'),
    '目录框': ('SysTreeView32', 'TVS_HASLINES|TVS_LINESATROOT|TVS_HASBUTTONS'),
    '文件框': ('ListBox', 'LBS_NOTIFY|WS_VSCROLL'),
    '颜色选择器': ('Button', 'BS_PUSHBUTTON|BS_NOTIFY'),
    '超级链接框': ('Static', 'SS_NOTIFY'),
    '调节器': ('msctls_updown32', 'UDS_ARROWKEYS'),
    '表格': ('SysListView32', 'LVS_REPORT|LVS_SINGLESEL'),
    '通用对话框': ('-', '0'),
    '时钟': ('-', '0'),
    '打印机': ('-', '0'),
    '数据报': ('-', '0'),
    '客户': ('-', '0'),
    '服务器': ('-', '0'),
    '端口': ('-', '0'),
    '数据源': ('-', '0'),
    '通用提供者': ('-', '0'),
    '数据库提供者': ('-', '0'),
    '外部数据库': ('-', '0'),
    '外部数据提供者': ('-', '0'),
    '无法载入的窗口组件': ('Static', 'SS_SUNKEN'),
}


def unquote(s):
    return s or ''


def cstr(v):
    """C++ 字符串字面量 → python str"""
    if v is None:
        return ''
    m = re.match(r'L"(.*)"$', v.strip())
    return m.group(1) if m else ''


def split_top(s, sep=','):
    """按顶层分隔符切分（跳过 {} () 内部）"""
    out, depth, cur = [], 0, []
    for ch in s:
        if ch in '{([':
            depth += 1
        elif ch in '})]':
            depth -= 1
        if ch == sep and depth == 0:
            out.append(''.join(cur).strip())
            cur = []
        else:
            cur.append(ch)
    if cur:
        out.append(''.join(cur).strip())
    return out


def parse_table(path):
    src = io.open(path, encoding='utf-8').read()

    picks = {}
    for m in re.finditer(r'static const wchar_t\* const (\w+)\[\]\s*=\s*\{(.*?)\};', src, re.S):
        picks[m.group(1)] = [cstr(x) for x in re.findall(r'L"[^"]*"', m.group(2))]

    props_arrays, events_arrays = {}, {}
    for m in re.finditer(r'static MkPropInfo (\w+)\[\]\s*=\s*\{(.*?)\n\};', src, re.S):
        rows = []
        for rm in re.finditer(
                r'\{\s*(\d+),\s*(\d+),\s*(MK_UD_\w+),\s*([^,]+?),\s*L"([^"]*)",\s*'
                r'(L"[^"]*"|nullptr),\s*(\w+|nullptr),\s*(\d+),\s*\{\s*(.*?)\s*\}\s*\}',
                m.group(2), re.S):
            d = split_top(rm.group(9))
            b = rm.group(4)
            flags = set()
            for f in re.findall(r'MK_PF_(\w+)', b):
                flags.add(f)
            rows.append({
                'id': int(rm.group(1)), 'index': int(rm.group(2)), 'ud': rm.group(3),
                'flags': flags, 'name': rm.group(5), 'en': cstr(rm.group(6)),
                'pick': None if rm.group(7) == 'nullptr' else rm.group(7),
                'pickCount': int(rm.group(8)), 'def': d,
            })
        props_arrays[m.group(1)] = rows

    for m in re.finditer(r'static MkEventInfo (\w+)\[\]\s*=\s*\{(.*?)\n\};', src, re.S):
        rows = []
        for rm in re.finditer(r'\{\s*(\d+),\s*(\d+),\s*(\d+),\s*L"([^"]*)",\s*(\w+|nullptr)\s*\}',
                              m.group(2)):
            rows.append({'index': int(rm.group(1)), 'argc': int(rm.group(2)),
                         'query': int(rm.group(3)), 'name': rm.group(4),
                         'args': None if rm.group(5) == 'nullptr' else rm.group(5)})
        events_arrays[m.group(1)] = rows

    blk = src[src.index('MkUnitClassInfo mkClassDefs[]'):]
    classes = []
    for m in re.finditer(
            r'\{\s*L"([^"]*)",\s*L"([^"]*)",\s*L"([^"]*)",\s*MKC_(\w+),\s*'
            r'(\d+),\s*(\d+),\s*(\d+),\s*(\d+),\s*(\d+),\s*(\d+),\s*(\w+),\s*(\w+)\s*\}',
            blk):
        classes.append({
            'name': m.group(1), 'en': m.group(2), 'native': m.group(3),
            'kind': m.group(4), 'flags': int(m.group(5)), 'container': int(m.group(6)),
            'invisible': int(m.group(7)), 'pick': int(m.group(8)),
            'propCount': int(m.group(9)), 'eventCount': int(m.group(10)),
            'props': props_arrays.get(m.group(11), []),
            'events': events_arrays.get(m.group(12), []),
        })
    return classes, picks


def key_of(cls):
    en = cls['en'].strip()
    k = re.sub(r'[^0-9a-z]', '', en.lower())
    return STYLE_KEY.get(k, k)


def default_text_of(p, picks, cls_name):
    d = p['def']
    ud = p['ud']
    if d and d[0] == 'MK_UD_NONE':      # 表里没标初值
        if ud == 'MK_UD_BOOL':
            return 'true' if p['name'] == '可视' else 'false'
        if p['name'] == '鼠标指针':
            return CURSOR_OPTS.split('|')[0]
        return ''
    num = d[1] if len(d) > 1 else '0'
    s = cstr(d[3]) if len(d) > 3 else ''
    clr = d[2] if len(d) > 2 else '0'
    extra = d[6] if len(d) > 6 else '0'
    try:
        n = int(num)
    except ValueError:
        n = 0
    if ud in ('MK_UD_PICK_INT', 'MK_UD_PICK_TEXT', 'MK_UD_EDIT_PICK_TEXT', 'MK_UD_PICK_SPEC_INT'):
        opts = picks.get(p['pick'] or '', [])
        if ud == 'MK_UD_PICK_TEXT' and s:
            return s
        if 0 <= n < len(opts):
            return opts[n]
        return opts[0] if opts else ''
    if ud == 'MK_UD_BOOL':
        return 'true' if (extra == '1' or n) else 'false'
    if ud in ('MK_UD_INT', 'MK_UD_DOUBLE'):
        return str(n) if num.lstrip('-').isdigit() else num
    if ud in ('MK_UD_COLOR', 'MK_UD_COLOR_BACK', 'MK_UD_COLOR_TRANS'):
        if extra == '1':
            return '系统默认'
        if extra == '2':
            return '透明'
        m = re.search(r'0x([0-9A-Fa-f]+)', clr)
        if m:
            v = int(m.group(1), 16) & 0xFFFFFF
            return '#%02X%02X%02X' % (v & 0xFF, (v >> 8) & 0xFF, (v >> 16) & 0xFF)
        return '#000000'
    return s


def emit_header(cls, picks, outdir, dry=False):
    name = cls['name']
    key = key_of(cls)
    if name == '窗口':
        return 0, '窗口（IDE 内置，属性来自 .e 窗口 XML）'
    w, h = SIZES.get(key, (96, 24))
    lines = []
    lines.append('#pragma once')
    lines.append('')
    lines.append('#include <component_support.h>')
    lines.append('')
    lines.append('// 由 开发环境/工具/gen_comp_lib.py 从易语言核心库组件表生成，勿手改。')
    lines.append('// @component %s' % key)
    lines.append('// @name %s' % name)
    lines.append('// @description %s（易语言核心库组件，属性/事件/枚举/初值全量）' % name)
    lines.append('// @name_prefix %s' % name)
    lines.append('// @default_text "%s"' % (name if name in TEXTY else ''))
    lines.append('// @default_size %d %d' % (w, h))
    wcls, wstyle = WIN32_MAP.get(name, ('Static', 'SS_SUNKEN'))
    lines.append('// @win32 %s' % wcls)
    lines.append('// @win32_style %s' % wstyle)
    lines.append('class CSu%s final : public sulang::controls::BasicControl<CWnd> {' % (
        re.sub(r'[^0-9A-Za-z]', '', cls['en']) or 'Comp'))
    lines.append('public:')

    nprop = 0
    for p in cls['props']:
        if not p['name']:          # 数量为 0 的类用一条空行占位，跳过
            continue
        udnum = UD_NUM.get(p['ud'], 0)
        typ = UD_TYPE.get(udnum, '文本型')
        # 枚举文本：有候选表就当选择型；鼠标指针是光标资源，但易语言里也走下拉
        opts = picks.get(p['pick'] or '', []) if p['pick'] else []
        if p['name'] == '鼠标指针':
            typ = '文本型(选择)'
            opts = CURSOR_OPTS.split('|')
        default = default_text_of(p, picks, name)
        lines.append('')
        lines.append('    // @name %s' % p['name'])
        lines.append('    // @description %s 属性（核心库 #%d，类型 %s）' % (
            name, p['id'], p['ud']))
        lines.append('    // @type %s' % typ)
        lines.append('    // @default "%s"' % default.replace('"', '\\"'))
        lines.append('    // @group 属性')
        if opts and '选择' in typ:
            lines.append('    // @options %s' % '|'.join(opts))
        lines.append('    SULANG_MFC_PROPERTY(p%d_%s);' % (
            p['id'], re.sub(r'[^0-9A-Za-z_]', '', p['en']) or 'v'))
        nprop += 1

    for ev in cls['events']:
        if not ev['name']:
            continue
        lines.append('')
        lines.append('    // @name %s' % ev['name'])
        lines.append('    // @description %s 事件（核心库事件索引 %d）' % (name, ev['index']))
        if ev['args']:
            for a in picks.get(ev['args'], []):
                lines.append('    // @param_name %s %s' % (a, a))
                lines.append('    // @param_type %s 文本型' % a)
                lines.append('    // @param %s' % a)
        lines.append('    // @group 事件')
        lines.append('    SULANG_MFC_EVENT(e%d_%s);' % (
            ev['index'], re.sub(r'[^0-9A-Za-z_]', '', ev['name']) or 'v'))

    lines.append('};')
    text = '\n'.join(lines) + '\n'
    path = os.path.join(outdir, key + '.h')
    if dry:
        return nprop, '%s  %d 属性 %d 事件' % (key, nprop, len(cls['events']))
    io.open(path, 'w', encoding='utf-8', newline='\n').write(text)
    return nprop, '%s  %2d 属性 %2d 事件 → %s' % (key, nprop, len(cls['events']),
                                                 os.path.basename(path))


def main():
    args = [a for a in sys.argv[1:] if not a.startswith('--')]
    dry = '--dry' in sys.argv
    outdir = DEFAULT_OUT
    for a in sys.argv[1:]:
        if a.startswith('--out='):
            outdir = a.split('=', 1)[1]
    if not args:
        sys.exit('用法: gen_comp_lib.py <ewinform/src/mk_class_table.cpp> [--out=目录] [--dry]')
    classes, picks = parse_table(args[0])
    if not os.path.isdir(outdir):
        sys.exit('输出目录不存在：' + outdir)
    total = 0
    for c in classes:
        n, msg = emit_header(c, picks, outdir, dry)
        total += n
        print('  ' + msg)
    print('合计 %d 个组件类 / %d 条属性' % (len(classes), total))


if __name__ == '__main__':
    main()
