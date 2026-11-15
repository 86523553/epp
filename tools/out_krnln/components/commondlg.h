#pragma once

#include <component_support.h>
#include "公共事件.h"
#include "公共方法.h"

// @component commondlg
// @name 通用对话框
// @description 通用对话框（由 krnln 5.7.64 生成，属性/方法/事件全量）
// @name_prefix 通用对话框
// @flags 窗口组件|功能提供者
class CSuCommondlg final : public sulang::controls::BasicControl<CWnd> {
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

    // @name 类型
    // @type 整数型
    // @ptype 整数型(选择)
    // @choices 0:打开文件／1:保存文件／2:字体选择／3:打开帮助
    // @group 属性
    SULANG_MFC_PROPERTY(style);

    // @name 标题
    // @description 仅适用于打开文件和保存文件对话框
    // @type 文本型
    // @group 属性
    SULANG_MFC_PROPERTY(caption);

    // @name 文件名
    // @description 从本属性中读取的文件名包括驱动器符和全路径目录，同时本属性也用作初始对话框中的文件名编辑框
    // @type 文本型
    // @group 属性
    SULANG_MFC_PROPERTY(FileName);

    // @name 过滤器
    // @description 过滤器文本由单个或多个成对的文本串组成，每对文本串的第一个描述显示形式，如：“文本文件（*.txt）”；第二个指定实际的过滤匹配符，如：“*.txt”，所有各文本串之间用“／”号隔开。
    // @type 文本型
    // @group 属性
    SULANG_MFC_PROPERTY(filter);

    // @name 初始过滤器
    // @description 指定初始的过滤器，0为第一个过滤器
    // @type 整数型
    // @group 属性
    SULANG_MFC_PROPERTY(FilterIndex);

    // @name 初始目录
    // @description 指定当打开对话框时所自动跳转到的目录
    // @type 文本型
    // @group 属性
    SULANG_MFC_PROPERTY(InitialDir);

    // @name 默认文件后缀
    // @description 指定当用户没有输入文件的后缀名称时所自动使用的文件后缀名称
    // @type 文本型
    // @group 属性
    SULANG_MFC_PROPERTY(DefExt);

    // @name 创建时提示
    // @description 当类型为打开文件对话框时，如果用户指定了一个不存在的文件名称，是否提示用户即将创建它
    // @type 逻辑型
    // @group 属性
    SULANG_MFC_PROPERTY(CreatePrompt);

    // @name 文件必须存在
    // @description 当类型为打开文件对话框时，是否允许用户指定一个不存在的文件
    // @type 逻辑型
    // @group 属性
    SULANG_MFC_PROPERTY(FileMustExist);

    // @name 文件覆盖提示
    // @description 当类型为保存文件对话框时，如果用户指定了一个已经存在的文件，是否询问用户确定覆盖此文件
    // @type 逻辑型
    // @group 属性
    SULANG_MFC_PROPERTY(OverWritePrompt);

    // @name 目录必须存在
    // @description 是否允许用户指定一个不存在的目录
    // @type 逻辑型
    // @group 属性
    SULANG_MFC_PROPERTY(PathMustExist);

    // @name 不改变目录
    // @description 在对话框关闭后是否自动返回到进入对话框时的文件目录
    // @type 逻辑型
    // @group 属性
    SULANG_MFC_PROPERTY(NoChangeDir);

    // @name 字体颜色
    // @type 整数型
    // @ptype 颜色
    // @group 属性
    SULANG_MFC_PROPERTY(FontColor);

    // @name 加粗
    // @type 逻辑型
    // @group 属性
    SULANG_MFC_PROPERTY(FontBold);

    // @name 倾斜
    // @type 逻辑型
    // @group 属性
    SULANG_MFC_PROPERTY(FontItalic);

    // @name 删除线
    // @type 逻辑型
    // @group 属性
    SULANG_MFC_PROPERTY(StrikeOut);

    // @name 下划线
    // @type 逻辑型
    // @group 属性
    SULANG_MFC_PROPERTY(UnderLine);

    // @name 字体名称
    // @description 如果不指定字体名称和字体大小，将自动使用标准的Windows图形字体。如果指定了字体大小但没有指定字体名称，将自动使用“宋体”
    // @type 文本型
    // @group 属性
    SULANG_MFC_PROPERTY(FontName);

    // @name 字体大小
    // @description 单位为1/72英寸，如果不指定字体名称和字体大小，将自动使用标准的Windows图形字体
    // @type 整数型
    // @group 属性
    SULANG_MFC_PROPERTY(FontSize);

    // @name 帮助文件名
    // @description 指定欲打开帮助文件的名称，如果没有指定帮助文件，将使用默认帮助文件名称，即本易程序的文件名称（不含其后缀）＋“.hlp”后缀
    // @type 文本型
    // @group 属性
    SULANG_MFC_PROPERTY(HelpFileName);

    // @name 帮助命令
    // @type 整数型
    // @ptype 整数型(选择)
    // @choices 0:显示帮助主题／1:显示指定标志信息／2:弹出指定标志信息／3:确定帮助已显示／4:显示帮助软件使用方法／5:退出帮助
    // @group 属性
    SULANG_MFC_PROPERTY(HelpCommand);

    // @name 帮助标志值
    // @description 仅在帮助命令为“显示指定标志信息”和“弹出指定标志信息”时才有效，用于指定相应的帮助标志数值
    // @type 整数型
    // @group 属性
    SULANG_MFC_PROPERTY(HelpContext);

    // @name 打开
    // @method
    // @description 打开当前类型的对话框，返回一个逻辑值。对于类型为“打开文件”、“保存文件”、“字体选择”的对话框，如果为真，表示用户已通过该对话框输入了有效数据，否则表示用户取消了该对话框，且没有输入任何有效数据。对于类型为“打开帮助”的对话框，如打开帮助成功则返回真，否则返回假
    SULANG_MFC_METHOD(open);

};