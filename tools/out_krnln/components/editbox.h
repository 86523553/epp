#pragma once

#include <component_support.h>
#include "公共事件.h"
#include "公共方法.h"

// @component editbox
// @name 编辑框
// @description 编辑框（由 krnln 5.7.64 生成，属性/方法/事件全量）
// @name_prefix 编辑框
// @flags 窗口组件
class CSuEditbox final : public sulang::controls::BasicControl<CWnd> {
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

    // @name 内容
    // @type 文本型
    // @group 属性
    SULANG_MFC_PROPERTY(context);

    // @name 边框
    // @type 整数型
    // @ptype 整数型(选择)
    // @choices 0:无边框／1:凹入式／2:凸出式／3:浅凹入式／4:镜框式／5:单线边框式
    // @group 属性
    SULANG_MFC_PROPERTY(border);

    // @name 文本颜色
    // @type 整数型
    // @ptype 颜色
    // @group 属性
    SULANG_MFC_PROPERTY(TextColor);

    // @name 背景颜色
    // @type 整数型
    // @ptype 背景颜色
    // @group 属性
    SULANG_MFC_PROPERTY(BackColor);

    // @name 字体
    // @type 字体
    // @group 属性
    SULANG_MFC_PROPERTY(font);

    // @name 隐藏选择
    // @description 本属性指定失去输入焦点后是否还显示当前被选择的区域
    // @type 逻辑型
    // @group 属性
    SULANG_MFC_PROPERTY(HideSel);

    // @name 最大允许长度
    // @description 本属性设置最大允许输入到本组件中的字符数目，如为0则输入字符数目不受限制。
    // @type 整数型
    // @group 属性
    SULANG_MFC_PROPERTY(MaxAllowLength);

    // @name 是否允许多行
    // @description 本属性设置本组件是否允许输入多行文本。
    // @type 逻辑型
    // @group 属性
    SULANG_MFC_PROPERTY(AllowMultiLines);

    // @name 滚动条
    // @description 滚动条只有在多行方式下才有效
    // @type 整数型
    // @ptype 整数型(选择)
    // @choices 0:无／1:横向滚动条／2:纵向滚动条／3:横向及纵向滚动条
    // @group 属性
    SULANG_MFC_PROPERTY(ScrollBar);

    // @name 对齐方式
    // @type 整数型
    // @ptype 整数型(选择)
    // @choices 0:左对齐／1:居中／2:右对齐
    // @group 属性
    SULANG_MFC_PROPERTY(AlignMode);

    // @name 输入方式
    // @type 整数型
    // @ptype 整数型(选择)
    // @choices 0:通常方式／1:只读方式／2:密码输入／3:整数文本输入／4:小数文本输入／5:输入字节／6:输入短整数／7:输入整数／8:输入长整数／9:输入小数／10:输入双精度小数／11:输入日期时间
    // @group 属性
    SULANG_MFC_PROPERTY(InputMode);

    // @name 密码遮盖字符
    // @description 本属性设置当输入方式为“密码输入”时输入字符后显示在输入框中的替代字符。
    // @type 文本型
    // @group 属性
    SULANG_MFC_PROPERTY(PasswordChar);

    // @name 转换方式
    // @type 整数型
    // @ptype 整数型(选择)
    // @choices 0:无／1:大写->小写／2:小写->大写
    // @group 属性
    SULANG_MFC_PROPERTY(ConvertMode);

    // @name 调节器方式
    // @description 调节器只有在单行方式下才有效
    // @type 整数型
    // @ptype 整数型(选择)
    // @choices 0:无调节器／1:自动调节器／2:手动调节器
    // @group 属性
    SULANG_MFC_PROPERTY(spin);

    // @name 调节器底限值
    // @description 只有在自动调节器方式下才有效，最小不能小于-32767
    // @type 整数型
    // @group 属性
    SULANG_MFC_PROPERTY(SpinMin);

    // @name 调节器上限值
    // @description 只有在自动调节器方式下才有效，最大不能超过32767
    // @type 整数型
    // @group 属性
    SULANG_MFC_PROPERTY(SpinMax);

    // @name 起始选择位置
    // @description 返回或设置所选择文本的起始点；0 为位置 1 ，1 为位置 2，如此类推。如果没有文本被选中，则指出光标位置。如果设置位置时使用值 -1，则将当前光标位置移动到文本尾部
    // @type 整数型
    // @group 属性
    SULANG_MFC_PROPERTY(SelStart);

    // @name 被选择字符数
    // @description 返回或设置所选择的字符数。如果设置字符数时使用值 -1，则选择文本框内的所有字符
    // @type 整数型
    // @group 属性
    SULANG_MFC_PROPERTY(SelLength);

    // @name 被选择文本
    // @description 返回或替换当前所选择的文本
    // @type 文本型
    // @group 属性
    SULANG_MFC_PROPERTY(SelText);

    // @name 数据源
    // @description 指定提供数据的数据源组件名
    // @type 属性类型(1018)
    // @group 属性
    SULANG_MFC_PROPERTY(DataSource);

    // @name 数据列
    // @description 指定数据源中的数据列，可以是列名或以1开始的列序号文本
    // @type 属性类型(1020)
    // @group 属性
    SULANG_MFC_PROPERTY(DataCol);

    // @name 加入文本
    // @method
    // @description 将指定文本加入到编辑框内容的尾部
    SULANG_MFC_METHOD(AddText);

    // @name 内容被改变
    // @group 事件
    // @description 当编辑框的内容被修改后即产生此事件
    SULANG_MFC_EVENT(evt);

    // @name 调节钮被按下
    // @group 事件
    // @description 只有调节器方式为“手动调节器”时才会产生此事件
    SULANG_MFC_EVENT(evt);

};