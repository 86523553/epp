# 表格码编辑器 (table code editor) — rebuild mechanics

Everything here is either **seen** (read straight off `J:/e/e.exe` or the two reference
screenshots), **derived** (worked out from what was seen), or **estimated** (a best guess,
marked `estimated` in `mechanics.json`).

Reference images (both uniform upscales, factor **S = 2.25**, of the same 96-DPI layout):

| file | size | native size |
|---|---|---|
| `clipboard-…569Z-3f038153.png` (table view) | 2200 × 1875 | 978 × 833 |
| `clipboard-…568Z-c13527a8.png` (code view) | 2103 × 1459 | 935 × 648 |

All numbers below are **native pixels** (divide the screenshot value by 2.25).

---

## 1. Column labels — seen in the binary

`e.exe` keeps its editor labels as **plain GBK text** in `.data`. No decoding needed.

| label | file offset | notes |
|---|---|---|
| `类 型` | `0x26ef28` | note the space between the two characters |
| `返回值类型` | `0x26ef30` | |
| `参数名` | `0x26ef3c` | |
| `备 注` | `0x26ef44` | space between characters |
| `全局变量名` | `0x26ef58` | |
| `成员名` | `0x26ef64` | |
| `数据类型名` | `0x26ef6c` | |
| `私有成员名` | `0x26ef78` | |
| `变量名` | `0x26ef84` | |
| `易包` | `0x26ef8c` | |
| `子程序名` | `0x26ef9c` | |
| `窗口程序集名` | `0x26efa8` | |
| `保 留` | `0x26efb8` | space between characters |
| `基 类` | `0x26efc0` | space between characters |
| `类名` | `0x26efc8` | |
| `程序集名` | `0x26efd0` | |

Checkbox / flag labels, a second cluster:

| label | file offset |
|---|---|
| `可空` | `0x2699dc` |
| `参考` | `0x2699e4` |
| `静态` | `0x2699ec` |
| `公开` | `0x2699f4` |
| `数组` | `0x24b10c`, `0x25eb9d`, `0x264e6c`, `0x26be77` |

Editor defaults: `程序集1` / `方法1` / `子程序1` / `变量1` / `数据类型1` at `0x66ee80–0x66eea8`.

**Not found by any search:** the string `Editor`. **Searched:** every byte of the
3,338,240-byte file for UTF-16 and for ASCII. Absence is not proof — the class name may be
built at run time.

> The paint routine itself has **not** been read out of the disassembly; only the label
> strings were found. Every geometric number below is **measured from the screenshots**.
> See `unk_85c52ae75de7685d`.

---

## 2. Colours

| role | value | how |
|---|---|---|
| page background | `#FFFFFF` | seen |
| declaration-table header (green) | `#E6EDE4` | seen |
| secondary header (pale blue) | `#D9E3F0` | seen |
| window chrome / toolbar / status bar | `#F0F0F0` | seen |
| code-view gutter | `#ECECEC` | seen |
| grid dot | `#C0C0C0` | seen |
| body text | `#000000` | seen |
| keyword / true-false | `#000080` | seen |
| number | `#800000` | seen |
| name / type | `#0000FF` | seen |
| comment | `#008000` | seen |
| selected row | `#D9E3F0` | estimated |
| toolbar rule | `#A8A8A8` | seen |
| tab-strip / gutter edge rule | `#9C9C9C` | seen |
| status-bar top rule | `#808080` | seen |

---

## 3. Vertical structure of the table view

Measured band by band off the reference (native y):

```
y0..9      #F0F0F0   toolbar
y10        #A8A8A8   toolbar rule
y11..22    white     tab strip (a white tab at x 0..80, chrome to its right)
y23        #9C9C9C   tab rule
y24..75    white     blank sheet
y76..94    GREEN     header, h=19
y95        rule
y96..114   white     data row, h=19
y115       rule
y116..134  GREEN     header, h=19
y135       rule
y136..154  white     data row
y155       rule
y156..173  white     data row, h=18
y174       rule
...        (blank gap, then the 参数 table at y200)
y200..218  GREEN     header, h=19
y240..258  GREEN     header, h=19   (变量 table)
y342..360  BLUE      header, h=19   (数据类型 table)
y~699      sheet ends
y800..833  #F0F0F0   status bar, with a #808080 rule on top
```

**Every band is 19 px of fill + 1 px of rule = 20 px pitch, header included.**

### Table tops, verified against the reference

| table | top (native y) | header colour |
|---|---|---|
| 程序集 | 76 | green |
| 子程序 | 116 | green |
| 参数 | 200 | green |
| 变量 | 240 | green |
| 数据类型 | 342 | pale blue |

The implementation pins those five tops directly, so the stack cannot drift.

---

## 4. Horizontal placement

* Left gutter: full-height `#ECECEC` strip at native **x 5.8..18.2** → `GUTTER_X = 6`,
  `GUTTER_W = 12`.
* Tables begin at native **x = 36**.
* Table widths, measured from the green/blue header spans:

| table | left | right | width |
|---|---|---|---|
| 程序集 | 36 | 395 | 360 |
| 子程序 | 36 | 395 | 360 |
| 参数 | 36 | 659 | 624 |
| 变量 | 36 | 659 | 624 |
| 数据类型 | 36 | 443 | 408 |

### Column cells (rebuilt, verified within 1 px)

**程序集** — separators at 107, 179, 215, 251; right edge 395.
Cells **72 / 72 / 36 / 36 / 144**.
Header: `程序集名 | 保 留 | 保 留 | (blank) | 备 注`.

**子程序** — separators at 107, 179, 215, 251, 287; right edge 395.
Cells **72 / 72 / 36 / 36 / 36 / 108**.
Header: `子程序名 | 返回值类型 | 公开 | 易包 | (blank) | 备 注`.

**参数** — separators at 155, 227, 263, 299, 335; right edge 659.
Cells **120 / 72 / 36 / 36 / 36 / 324**.
Header: `参数名 | 类 型 | 参考 | 可空 | 数组 | 备 注`.

**变量** — separators at 155, 227, 263, 299; right edge 659.
Cells **120 / 72 / 36 / 36 / 360**.
Header: `变量名 | 类 型 | 静 态 | 数 组 | 备 注`.

**数据类型** (pale blue header) — separators at 131, 191, 227, 263; right edge 443.
Cells **96 / 60 / 36 / 36 / 180**.
Header: `数据类型名 | 基 类 | 公开 | (blank) | 备 注`.

* Text left inset **5** from the cell's left border.
* CJK advance **12**; glyph ink height **10**; font height **12 px** (9 pt 宋体).

---

## 5. Grid

The grid is **dotted, not solid**, with a **4-px period** along both axes: one `#C0C0C0`
pixel, then three white ones. Measured by counting rule pixels along a row:
a 360-px rule holds 90 dots, i.e. 9 screenshot px between them ÷ 2.25 = 4.

GDI `PS_DOT` does **not** reproduce that spacing (it uses the pen width to set the gap), so
the dots are drawn with an explicit `SetPixelV` loop stepping 4.

The rule sits on the **last pixel** of each band: `y0 + BAND*(r+1) - 1`.
Column splits likewise sit on `x0 + sum(cols[0..i]) - 1`.

---

## 6. Statement (code) area

From the second reference crop (935 × 648 native), measured ink-band starts at screenshot
y 9–32, 50–72, 90–114, 131–155, 172–194, 212–236, 252–277 → 40–41 screenshot px apart
→ **18 native px pitch**.

* Row pitch **18**.
* Left gutter **12** wide, fill `#ECECEC`.
* Flow track at **x ≈ 6** inside the gutter.
* Block **head** rows: a dotted horizontal rule across the row plus a solid black
  right-pointing triangle (▸) on the track. Triangle size **7 × 7** (*estimated*).
* Rows **inside** a block: a dotted vertical rule on the track, no triangle.
* Top-level statements: no track mark at all.
* Text starts at x **14**.

---

## 7. Interaction

| gesture | effect |
|---|---|
| single click | selects the **whole row** (pale blue fill), not one cell |
| double click | opens an in-place cell editor with its own caret; IME text goes into the cell |
| ↑ / ↓ | move the selection one row |
| Enter (editing) | commit the cell |
| Esc (editing) | abandon the edit |
| wheel | scrolls the whole content vertically |

---

## 8. Unresolved

* The exact machine-code paint routine was not recovered — see `unk_85c52ae75de7685d`.
* `#0066CC`-family blues appear in the toolbar of the real window; not reproduced.
* Whether the faint near-white rules at y198–199 are a real divider or JPEG-ish ringing is
  undecided; they are currently not drawn.
* The sheet bottom is pinned at ≈699 by 17 empty 数据类型 member rows; the reference could
  plausibly be 699 or 717.
