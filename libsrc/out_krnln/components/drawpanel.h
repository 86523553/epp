#pragma once

#include <component_support.h>
#include "公共事件.h"
#include "公共方法.h"

// @component drawpanel
// @name 画板
// @description 画板（由 krnln 5.7.64 生成，属性/方法/事件全量）
// @name_prefix 画板
// @flags 窗口组件|容器|默认跳过Tab停留
class CSuDrawpanel final : public sulang::controls::BasicControl<CWnd> {
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

    // @name 边框
    // @type 整数型
    // @ptype 整数型(选择)
    // @choices 0:无边框／1:凹入式／2:凸出式／3:浅凹入式／4:镜框式／5:单线边框式
    // @group 属性
    SULANG_MFC_PROPERTY(border);

    // @name 画板背景色
    // @description 注意如果“自动重画”属性为真，则不支持透明色
    // @type 整数型
    // @ptype 背景颜色
    // @group 属性
    SULANG_MFC_PROPERTY(BackColor);

    // @name 底图
    // @description 本属性指定显示在画板背景上的图片
    // @type 字节集
    // @ptype 图片文件
    // @group 属性
    SULANG_MFC_PROPERTY(pic);

    // @name 底图方式
    // @description 如果设定了底图，本属性指定画板背景上底图的显示方式
    // @type 整数型
    // @ptype 整数型(选择)
    // @choices 0:图片居左上／1:图片平铺／2:图片居中
    // @group 属性
    SULANG_MFC_PROPERTY(BackPicMode);

    // @name 自动重画
    // @description 如果本属性设置为真，系统将自动把所有在画板上进行的绘画操作的结果保存到内存中，当以后需要重画时，系统简单地把所保存的绘画结果复制到画板上，而不再产生“绘画”事件。采用本属性需要消耗一定数量的内存。另外，如果本属性为真且在底图方式为居中时调整画板尺寸，将无法保留以前的绘画数据
    // @type 逻辑型
    // @group 属性
    SULANG_MFC_PROPERTY(AutoRedraw);

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

    // @name 画板宽度
    // @description 本属性提供了目前画板的宽度，使用现行绘画单位。属性值等于“画板”窗口组件的用户区域宽度
    // @type 整数型
    // @group 属性
    SULANG_MFC_PROPERTY(width);

    // @name 画板高度
    // @description 本属性提供了目前画板的高度，使用现行绘画单位。属性值等于“画板”窗口组件的用户区域高度
    // @type 整数型
    // @group 属性
    SULANG_MFC_PROPERTY(height);

    // @name 取设备句柄
    // @method
    // @description 如当前用户程序正在处理本画板所产生的“绘画”事件，则返回画板所对应的设备句柄（即HDC），否则返回 0
    SULANG_MFC_METHOD(GetHDC);

    // @name 清除
    // @method
    // @description 清除画板上指定区域的内容并将当前文本写出位置移动到被清除区左上角
    SULANG_MFC_METHOD(cls);

    // @name 取点
    // @method
    // @description 返回画板上指定点的颜色值。如果失败，将返回 -1
    SULANG_MFC_METHOD(GetPixel);

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

    // @name 翻转矩形区
    // @method
    // @description 将画板上指定矩形区域的颜色翻转过来
    SULANG_MFC_METHOD(InvertRect);

    // @name 画多边形
    // @method
    // @description 如果所画的多边形没有闭合，将自动闭合
    SULANG_MFC_METHOD(polygon);

    // @name 置写出位置
    // @method
    // @description 设置下次使用“写文本行”或“写出”命令输出数据时的位置
    SULANG_MFC_METHOD(SetWritePos);

    // @name 写文本行
    // @method
    // @description 在当前写出位置写出指定的文本、数值、逻辑值或日期时间，并将现行写出位置调整到下行行首
    SULANG_MFC_METHOD(print);

    // @name 滚动写行
    // @method
    // @description 在当前写出位置写出指定的文本、数值、逻辑值或日期时间，并将现行写出位置调整到下行行首。如果现行画板高度无法容纳当前所要写出的行，则自动向上滚动画板内容
    SULANG_MFC_METHOD(sprint);

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
    // @description 返回指定数据的写出宽度，使用当前绘画单位
    SULANG_MFC_METHOD(GetWidth);

    // @name 取高度
    // @method
    // @description 返回指定数据的写出高度，使用当前绘画单位
    SULANG_MFC_METHOD(GetHeight);

    // @name 画图片
    // @method
    SULANG_MFC_METHOD(DrawPic);

    // @name 取图片宽度
    // @method
    // @description 返回指定图片的宽度，使用当前绘画单位
    SULANG_MFC_METHOD(GetPicWidth);

    // @name 取图片高度
    // @method
    // @description 返回指定图片的高度，使用当前绘画单位
    SULANG_MFC_METHOD(GetPicHeight);

    // @name 复制
    // @method
    // @description 将源画板（本命令的调用画板对象）中指定区域的内容快速复制到目的画板中的指定位置，使用源和目的画板各自的当前绘画单位。如果源画板当前不可视，其“自动重画”属性必须为真
    SULANG_MFC_METHOD(copy);

    // @name 取图片
    // @method
    // @description 返回画板上所有现有显示内容的图片数据。如果失败，返回空字节集
    SULANG_MFC_METHOD(GetPic);

    // @name 单位转换
    // @method
    // @description 将像素单位座标值转换到当前绘画单位，或将当前绘画单位座标值转换到像素单位
    SULANG_MFC_METHOD(UnitCnv);

    // @name 绘画
    // @group 事件
    // @description 当画板中的全部或一部分区域需要被重新绘制时即产生此事件。用户程序在响应此事件期间在画板上所进行的任何绘制操作均被限制在“需重画区”参数所指定的区域内
    SULANG_MFC_EVENT(evt);

};