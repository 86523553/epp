#pragma once

#include <component_support.h>
#include "公共事件.h"
#include "公共方法.h"

// @component printer
// @name 打印机
// @description 打印机（由 krnln 5.7.64 生成，属性/方法/事件全量）
// @name_prefix 打印机
// @flags 窗口组件|功能提供者
class CSuPrinter final : public sulang::controls::BasicControl<CWnd> {
public:

    // @name 左边
    // @type 整数型
    // @group 属性
    SULANG_MFC_PROPERTY(left);

    // @name 顶边
    // @type 整数型
    // @group 属性
    SULANG_MFC_PROPERTY(top);

    // @name 宽度
    // @type 整数型
    // @group 属性
    SULANG_MFC_PROPERTY(width);

    // @name 高度
    // @type 整数型
    // @group 属性
    SULANG_MFC_PROPERTY(height);

    // @name 标记
    // @type 文本型
    // @group 属性
    SULANG_MFC_PROPERTY(tag);

    // @name 可视
    // @type 逻辑型
    // @group 属性
    SULANG_MFC_PROPERTY(visible);

    // @name 禁止
    // @type 逻辑型
    // @group 属性
    SULANG_MFC_PROPERTY(disable);

    // @name 鼠标指针
    // @type 字节集
    // @ptype 光标
    // @group 属性
    SULANG_MFC_PROPERTY(MousePointer);

    // @name 绘画单位
    // @description 本属性指定绘画时所使用的单位，坐标系的 X 轴从左到右，Y 轴从上到下
    // @type 整数型
    // @ptype 整数型(选择)
    // @choices 0:像素点／1: 0.1毫米／2: 0.01毫米／3: 0.01英寸／4: 0.001英寸／5: 1/1440英寸
    // @group 属性
    SULANG_MFC_PROPERTY(unit);

    // @name 画笔类型
    // @type 整数型
    // @ptype 整数型(选择)
    // @choices 0:空笔／1:直线／2:划线／3:点线／4:点划线／5:双点划线／6:内直线
    // @group 属性
    SULANG_MFC_PROPERTY(PenStyle);

    // @name 画出方式
    // @description 有关各种画出方式的说明为： 0、黑色：始终画出黑色；1、非或笔：或笔的相反；2、与非笔：背景色以及画笔反相二者共有颜色的组合；3、非复制笔：复制笔的相反；4、与笔非：画笔以及已有颜色反相二者共有颜色的组合；5、反转：已有颜色的反相；6、异或笔：画笔的颜色以及已有颜色的组合，只取其一；7、非与笔：与笔的相反；8、与笔：画笔和已有二者共有颜色的组合；9、非异或笔：异或笔的相反；10、无操作：原有保持不变。该设置实际上关闭画笔；11、或非笔：已有颜色与画笔颜色反相的组合；12、复制笔（缺省值）：画笔颜色；13、或笔非：画笔颜色与已有颜色的反相的组合；14、或笔：画笔颜色与已有颜色的组合；15、白色：始终画出白色
    // @type 整数型
    // @ptype 整数型(选择)
    // @choices 0:黑色／1:非或笔／2:与非笔／3:非复制笔／4:与笔非／5:反转／6:异或笔／7:非与笔／8:与笔／9:非异或笔／10:无操作／11:或非笔／12:复制笔／13:或笔非／14:或笔／15:白色
    // @group 属性
    SULANG_MFC_PROPERTY(DrawRop2);

    // @name 画笔粗细
    // @description 使用现行绘画单位，如果为0，表示画笔的粗细为一个像素点。注意：当画笔类型为划线、点线、点划线、双点划线时，本属性无效，画笔宽度始终为一个像素点
    // @type 整数型
    // @group 属性
    SULANG_MFC_PROPERTY(PenWidth);

    // @name 画笔颜色
    // @type 整数型
    // @ptype 颜色
    // @group 属性
    SULANG_MFC_PROPERTY(PenColor);

    // @name 刷子类型
    // @description 各种格式刷子将使用当前文本背景颜色作为刷子背景颜色
    // @type 整数型
    // @ptype 整数型(选择)
    // @choices 0:空刷子／1:颜色刷子／2:右斜线／3:直交叉线／4:斜交叉线／5:左斜线／6:横线／7:竖线／8: 5%色点／9: 10%色点／10: 20%色点／11: 25%色点／12: 30%色点／13: 40%色点／14: 50%色点／15: 60%色点／16: 70%色点／17: 75%色点／18: 80%色点／19: 90%色点／20:浅色上对角线／21:浅色下对角线／22:深色上对角线／23:深色下对角线／24:宽上对角线／25:宽下对角线／26:浅色竖线／27:浅色横线／28:窄竖线／29:窄横线／30:深色竖线／31:深色横线／32:上对角虚线／33:下对角虚线／34:横虚线／35:竖虚线／36:小纸屑／37:大纸屑／38:之字形／39:波浪线／40:对角砖形／41:横向砖形／42:纺织物／43:方格呢／44:草皮／45:虚格线／46:点式菱形／47:瓦形／48:棚架／49:球体／50:小网格／51:大网格／52:小棋盘／53:大棋盘／54:空心菱形／55:实心菱形
    // @group 属性
    SULANG_MFC_PROPERTY(BrushStyle);

    // @name 刷子颜色
    // @type 整数型
    // @ptype 颜色
    // @group 属性
    SULANG_MFC_PROPERTY(BrushColor);

    // @name 文本颜色
    // @type 整数型
    // @ptype 颜色
    // @group 属性
    SULANG_MFC_PROPERTY(TextColor);

    // @name 文本背景颜色
    // @type 整数型
    // @ptype 透明颜色
    // @group 属性
    SULANG_MFC_PROPERTY(TextBackColor);

    // @name 字体
    // @type 字体
    // @group 属性
    SULANG_MFC_PROPERTY(font);

    // @name 打印作业名
    // @description 在打印过程中，本属性的内容将显示在由系统自动提供的打印状态对话框及 Windows 打印管理器中。如果为空，默认为“未命名打印作业”
    // @type 文本型
    // @group 属性
    SULANG_MFC_PROPERTY(title);

    // @name 打印份数
    // @description 本属性用作为打印设置对话框中的打印份数编辑框提供初始值，当用户从打印设置对话框中确认返回后，属性值提供用户所实际设置的需要打印份数。需了解的是：如果被选定打印机硬件支持多份拷贝功能，无论用户设置打印份数为多少，打印设置对话框永远返回 1 ，多份打印被转交给打印机硬件去自动完成。如果在进入打印设置对话框前置属性值为 -1 ，表明现行易程序不支持多份打印，此时系统会自动检查当前所选择打印机硬件，如果该打印机硬件支持多份拷贝，将允许用户指定打印份数并启用硬件多份拷贝功能，否则不允许，打印设置对话框返回后属性值保持不变。如果使用“开始打印”方法进入打印时没有调用打印设置对话框，本属性值保持不变
    // @type 整数型
    // @group 属性
    SULANG_MFC_PROPERTY(PrintCopies);

    // @name 多份打印方式
    // @description 本属性用作为打印设置对话框中“份数”组中的“分页”选择框提供初始值，当用户从打印设置对话框中确认返回后，属性值提供用户所实际设置的值。如果使用“开始打印”方法进入打印时没有调用打印设置对话框，本属性值保持不变
    // @type 逻辑型
    // @group 属性
    SULANG_MFC_PROPERTY(Collate);

    // @name 最小页号
    // @description 本属性用作为打印设置对话框提供最小页号值，如果在进入打印设置对话框前置属性值为 -1 ，打印设置对话框中将不允许用户设置页打印范围。如果使用“开始打印”方法进入打印时没有调用打印设置对话框，本属性值保持不变
    // @type 整数型
    // @group 属性
    SULANG_MFC_PROPERTY(MinPage);

    // @name 最大页号
    // @description 本属性用作为打印设置对话框提供最大页号值。如果置为 -1 ，表示最大页号不受限制。如果“最小页号”属性值被置为 -1 ，本属性无意义。如果使用“开始打印”方法进入打印时没有调用打印设置对话框，本属性值保持不变
    // @type 整数型
    // @group 属性
    SULANG_MFC_PROPERTY(MaxPage);

    // @name 首页页号
    // @description 本属性用作为打印设置对话框中的首打印页页号编辑框提供初始值，当用户从打印设置对话框中确认返回后，属性值提供用户所实际设置的首打印页号，此值将在“最小页号”属性值及“最大页号”属性值之间。但如果用户选择了全部打印或者打印选择区，本属性值将为 -1 。如果“最小页号”属性值被置为 -1 ，本属性无意义。如果使用“开始打印”方法进入打印时没有调用打印设置对话框，本属性值保持不变
    // @type 整数型
    // @group 属性
    SULANG_MFC_PROPERTY(FromPage);

    // @name 末页页号
    // @description 本属性用作为打印设置对话框中的末打印页页号编辑框提供初始值，如果置为 -1 ，表示不提供。当用户从打印设置对话框中确认返回后，属性值提供用户所实际设置的末打印页号，此值将在“首页页号”属性值及“最大页号”属性值之间。但如果用户选择了全部打印或者打印选择区，本属性值将为 -1 。如果“最小页号”属性值被置为 -1 ，本属性无意义。如果使用“开始打印”方法进入打印时没有调用打印设置对话框，本属性值保持不变
    // @type 整数型
    // @group 属性
    SULANG_MFC_PROPERTY(ToPage);

    // @name 打印选择区
    // @description 本属性用作为打印设置对话框中“选择的范围”选择框提供初始值，为 1 表示真，为 0 表示假。当用户从打印设置对话框中确认返回后，属性值提供用户所实际设置的值。如果在进入打印设置对话框前置属性值为 -1 ，打印设置对话框中将不允许用户选取该选择框。如果使用“开始打印”方法进入打印时没有调用打印设置对话框，本属性值保持不变
    // @type 整数型
    // @group 属性
    SULANG_MFC_PROPERTY(PrintSel);

    // @name 左边空
    // @description 本属性指定打印时打印纸左边所留空白的尺寸，使用现行绘画单位，由系统自动提供支持
    // @type 整数型
    // @group 属性
    SULANG_MFC_PROPERTY(LeftSpace);

    // @name 顶边空
    // @description 本属性指定打印时打印纸顶边所留空白的尺寸，使用现行绘画单位，由系统自动提供支持
    // @type 整数型
    // @group 属性
    SULANG_MFC_PROPERTY(TopSpace);

    // @name 右边空
    // @description 本属性指定打印时打印纸右边所留空白的尺寸，使用现行绘画单位，由系统自动提供支持
    // @type 整数型
    // @group 属性
    SULANG_MFC_PROPERTY(RightSpace);

    // @name 底边空
    // @description 本属性指定打印时打印纸底边所留空白的尺寸，使用现行绘画单位，由系统自动提供支持
    // @type 整数型
    // @group 属性
    SULANG_MFC_PROPERTY(BottomSpace);

    // @name 当前页号
    // @description 本属性提供现行打印页的页号，由系统自行维护。页号值从“首页页号”属性值开始，如果“最小页号”属性值被置为 -1 ，页号值从 1 开始。在以下情况下属性值被自动增一： 1、使用“写文本行”方法打印一行文本并且要打印文本行的高度当前页容纳不下（此时将会首先自动换页）； 2、使用“换页”方法。本属性仅在打印作业过程中有效
    // @type 整数型
    // @group 属性
    SULANG_MFC_PROPERTY(page);

    // @name 已打印份数
    // @description 本属性提供现行所已经打印的份数（含当前份），由系统自行维护。在调用“开始下一份”方法后，本属性值被自动加一
    // @type 整数型
    // @group 属性
    SULANG_MFC_PROPERTY(copies);

    // @name 打印区宽度
    // @description 本属性提供了打印纸实际可打印区域宽度减去左右边空后的值，使用现行绘画单位，仅在打印作业过程中有效
    // @type 整数型
    // @group 属性
    SULANG_MFC_PROPERTY(width);

    // @name 打印区高度
    // @description 本属性提供了打印纸实际可打印区域高度减去上下边空后的值，使用现行绘画单位，仅在打印作业过程中有效
    // @type 整数型
    // @group 属性
    SULANG_MFC_PROPERTY(height);

    // @name 打印被中止
    // @description 如果在打印时使用了系统所提供的打印状态对话框，并且在打印过程中用户已经选择中止打印取消了打印作业（系统自动支持），本属性值为真，否则为假。易程序在进行打印的过程中一旦发现本属性值为真，应该退出此过程
    // @type 逻辑型
    // @group 属性
    SULANG_MFC_PROPERTY(abort);

};