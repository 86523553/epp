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

    // @name 取设备句柄
    // @method
    // @description 如当前用户程序正在进行打印作业，则返回对应的打印机设备句柄（即HDC），否则返回 0
    SULANG_MFC_METHOD(GetHDC);

    // @name 开始打印
    // @method
    // @description 返回真表示已经成功地进入了打印作业，进入打印作业后文本写出位置将被重置为 0 。如果执行本命令时已经在打印作业中或者用户在打印设置对话框中选择了取消，返回假。进入打印后必须调用“结束打印”或者“取消打印”方法来退出打印。进入打印前易程序进行的所有打印绘画操作都将被忽略
    SULANG_MFC_METHOD(StartDoc);

    // @name 结束打印
    // @method
    // @description 如果当前已经进入了打印作业，调用本命令可完成此作业。结束打印后易程序进行的后续打印绘画操作都将被忽略
    SULANG_MFC_METHOD(EndDoc);

    // @name 取消打印
    // @method
    // @description 如果当前已经进入了打印作业，调用本命令可立即中止且取消该作业。如果操作系统的打印管理器正在处理该打印作业（打印管理器正在运行并且允许后台打印），那么该作业将被删除且使打印机不接收任何信息。如果打印管理器不是正在处理该作业（没有选用后台打印），部分或全部数据可能在此之前已发送到打印机。此时，打印机驱动程序将尽可能使打印机立即复位并终止该作业。取消打印后易程序进行的后续打印绘画操作都将被忽略
    SULANG_MFC_METHOD(AbortDoc);

    // @name 换页
    // @method
    // @description 调用本命令使打印机完成当前页的打印，文本写出位置被重置为 0 ，并走纸到下一页左上角。如果换页成功返回真，否则将自动取消打印并返回假
    SULANG_MFC_METHOD(NewPage);

    // @name 开始下一份
    // @method
    // @description 调用本命令后将结束在当前页上的打印作业，并将当前打印份数加一，当前页号置回到首页页号，以进入下一份拷贝的打印。如果成功返回真，失败将自动取消打印并返回假
    SULANG_MFC_METHOD(NewCopy);

    // @name 画点
    // @method
    SULANG_MFC_METHOD(SetPixel);

    // @name 画直线
    // @method
    // @description 使用画笔在画板上画出一条直线
    SULANG_MFC_METHOD(LineTo);

    // @name 画椭圆
    // @method
    SULANG_MFC_METHOD(ellipse);

    // @name 画弧线
    // @method
    // @description 使用画笔在画板上画出一条弧线
    SULANG_MFC_METHOD(ArcTo);

    // @name 画弦
    // @method
    SULANG_MFC_METHOD(chord);

    // @name 画饼
    // @method
    SULANG_MFC_METHOD(pie);

    // @name 画矩形
    // @method
    // @description 使用画笔在画板上画出一个矩形，矩形的内部使用刷子填充
    SULANG_MFC_METHOD(DrawRect);

    // @name 画渐变矩形
    // @method
    SULANG_MFC_METHOD(DrawJBRect);

    // @name 填充矩形
    // @method
    // @description 将画板上指定的矩形区域用当前刷子填充
    SULANG_MFC_METHOD(FillRect);

    // @name 画圆角矩形
    // @method
    // @description 使用画笔在画板上画出一个圆角矩形，圆角矩形的内部使用刷子填充
    SULANG_MFC_METHOD(RoundRect);

    // @name 画多边形
    // @method
    SULANG_MFC_METHOD(polygon);

    // @name 置写出位置
    // @method
    // @description 设置下次使用“写文本行”或“写出”命令输出数据时的位置
    SULANG_MFC_METHOD(SetWritePos);

    // @name 写文本行
    // @method
    // @description 在当前写出位置写出指定的文本、数值、逻辑值或日期时间，并将现行写出位置调整到下行行首。支持自动换页
    SULANG_MFC_METHOD(print);

    // @name 写出
    // @method
    // @description 在当前写出位置处写出指定的文本、数值、逻辑值或日期时间。自动调整现行写出位置到所写出数据的末位置
    SULANG_MFC_METHOD(write);

    // @name 定位写出
    // @method
    // @description 在指定写出位置处写出指定的文本、数值、逻辑值或日期时间，不改变现行写出位置
    SULANG_MFC_METHOD(say);

    // @name 取宽度
    // @method
    // @description 返回指定数据的写出宽度，使用当前绘画单位。如果调用本命令时尚未进入打印，将返回 0
    SULANG_MFC_METHOD(GetWidth);

    // @name 取高度
    // @method
    // @description 返回指定数据的写出高度，使用当前绘画单位。如果调用本命令时尚未进入打印，将返回 0
    SULANG_MFC_METHOD(GetHeight);

    // @name 画图片
    // @method
    SULANG_MFC_METHOD(DrawPic);

    // @name 取图片宽度
    // @method
    // @description 返回指定图片的宽度。如果调用本命令时尚未进入打印，使用像素点为单位，否则使用当前绘画单位
    SULANG_MFC_METHOD(GetPicWidth);

    // @name 取图片高度
    // @method
    // @description 返回指定图片的高度。如果调用本命令时尚未进入打印，使用像素点为单位，否则使用当前绘画单位
    SULANG_MFC_METHOD(GetPicHeight);

    // @name 置自定义纸张类型
    // @method
    // @description 添加或修改自定义纸张类型。本方法仅在Windows NT/2000/XP下有效，且要求具有打印机驱动的完全控制权限。注意：太小或太大的纸张大小和页边距，可能不被特定的打印所支持。本方法所设置的自定义纸张类型，可用于“开始打印()”的第三个参数“纸张类型”。添加或修改后的自定义纸张类型可以在“当前默认打印机驱动所属计算机”的“开始-设置-打印机和传真-文件-服务器属性”中看到（仅限Windows NT/2000/XP系统）。
    SULANG_MFC_METHOD(SetCustomPaperType);

    // @name 删除自定义纸张类型
    // @method
    // @description 删除指定自定义纸张类型。本方法仅在Windows NT/2000/XP下有效，且要求具有打印机驱动的完全控制权限。
    SULANG_MFC_METHOD(DeleteCustomPaperType);

    // @name 取自定义纸张大小
    // @method
    // @description 取指定自定义纸张类型的纸张大小及边距，并写入后面的六个参数中。如果指定的自定义纸张类型不存在，返回“假”。本方法仅在Windows NT/2000/XP下有效，且要求具有打印机驱动的完全控制权限。
    SULANG_MFC_METHOD(GetCustomPaperType);

    // @name 取所有纸张类型
    // @method
    // @description 取当前默认打印机所支持的所有纸张类型（包括系统定义纸张类型和自定义纸张类型），返回一维文本数组，其中存放了的各纸张类型的名称。本方法仅在Windows NT/2000/XP下有效。
    SULANG_MFC_METHOD(SetCustomPaperType);

    // @name 单位转换
    // @method
    // @description 将像素单位座标值转换到当前绘画单位，或将当前绘画单位座标值转换到像素单位。注意在执行本方法之前，必须已经开始打印
    SULANG_MFC_METHOD(UnitCnv);

    // @name 取设备名称
    // @method
    // @description 如果已经开始打印，则返回当前打印机设备的名称
    SULANG_MFC_METHOD(GetPrinterDeviceName);

};