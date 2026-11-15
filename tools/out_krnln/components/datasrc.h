#pragma once

#include <component_support.h>
#include "公共事件.h"
#include "公共方法.h"

// @component datasrc
// @name 数据源
// @description 数据源（由 krnln 5.7.64 生成，属性/方法/事件全量）
// @name_prefix 数据源
// @flags 窗口组件
class CSuDatasrc final : public sulang::controls::BasicControl<CWnd> {
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

    // @name 数据提供者
    // @description 指定本数据源所基于的数据提供者组件名，所有数据存取操作都将通过数据提供者实际完成
    // @type 属性类型(1019)
    // @group 属性
    SULANG_MFC_PROPERTY(DataProvider);

    // @name 只读
    // @description 指定是否允许操作者修改数据源内的数据，注意本属性并不限制在程序中直接修改数据
    // @type 逻辑型
    // @group 属性
    SULANG_MFC_PROPERTY(ReadOnly);

    // @name 允许添加
    // @description 指定是否允许操作者添加记录到数据源内，注意本属性并不限制在程序中直接添加记录
    // @type 逻辑型
    // @group 属性
    SULANG_MFC_PROPERTY(AllowAppend);

    // @name 允许删除
    // @description 指定是否允许操作者删除数据源内的记录，注意本属性并不限制在程序中直接删除记录
    // @type 逻辑型
    // @group 属性
    SULANG_MFC_PROPERTY(AllowDelete);

    // @name 到首记录
    // @method
    // @description 将数据源中当前记录指针移动到第一条记录上
    SULANG_MFC_METHOD(GoTop);

    // @name 到尾记录
    // @method
    // @description 将数据源中当前记录指针移动到最后一条记录上
    SULANG_MFC_METHOD(GoBottom);

    // @name 跳过
    // @method
    // @description 将数据源中当前记录指针向前或者向后移动数条记录
    SULANG_MFC_METHOD(skip);

    // @name 跳到
    // @method
    // @description 改变数据源中的当前记录指针到指定的记录号
    SULANG_MFC_METHOD(goto);

    // @name 取记录号
    // @method
    // @description 返回数据源当前记录指针所指向记录的记录号。记录号从1开始。如果当前记录指针无效，所返回记录号为0
    SULANG_MFC_METHOD(RecNO);

    // @name 取表头行数
    // @method
    // @description 返回当以表格的形式表现数据源中的数据时表头所占的行数。表头行在表格中显示时不会滚动，在打印时会自动打印在每一页的顶部
    SULANG_MFC_METHOD(GetFixedRowCount);

    // @name 置表头行数
    // @method
    // @description 设置当以表格的形式表现数据源中的数据时表头所占的行数。表头行在表格中显示时不会滚动，在打印时会自动打印在每一页的顶部。注意如果数据源所使用的数据提供者不支持此特性，本命令将被忽略
    SULANG_MFC_METHOD(SetFixedRowCount);

    // @name 取表头列数
    // @method
    // @description 返回当以表格的形式表现数据源中的数据时表头所占的列数。表头列在表格中显示时不会滚动
    SULANG_MFC_METHOD(GetFixedColCount);

    // @name 置表头列数
    // @method
    // @description 设置当以表格的形式表现数据源中的数据时表头所占的列数。表头列在表格中显示时不会滚动。注意如果数据源所使用的数据提供者不支持此特性，本命令将被忽略
    SULANG_MFC_METHOD(SetFixedColCount);

    // @name 取行高
    // @method
    // @description 返回数据源中指定行在表现时的高度，单位为0.1毫米
    SULANG_MFC_METHOD(GetRowHeight);

    // @name 置行高
    // @method
    // @description 设置数据源中数据行在表现时的高度，单位为0.1毫米。注意如果数据源所使用的数据提供者不支持此特性，本命令将被忽略
    SULANG_MFC_METHOD(SetRowHeight);

    // @name 取列宽
    // @method
    // @description 返回数据源中指定列在表现时的宽度，单位为0.1毫米
    SULANG_MFC_METHOD(GetColWidth);

    // @name 置列宽
    // @method
    // @description 设置数据源中数据列在表现时的宽度，单位为0.1毫米。注意如果数据源所使用的数据提供者不支持此特性，本命令将被忽略
    SULANG_MFC_METHOD(SetColWidth);

    // @name 取类型
    // @method
    // @description 返回数据源中指定单元格的数据类型。返回值为以下常量值之一： 1、#文本； 2、#图片文件名； 3、#图片数据； 4、#字节集数据
    SULANG_MFC_METHOD(GetType);

    // @name 置类型
    // @method
    // @description 设置数据源中指定单元格的数据类型。注意如果数据源所使用的数据提供者不支持此特性，本命令将被忽略
    SULANG_MFC_METHOD(SetType);

    // @name 取文本色
    // @method
    // @description 返回数据源中指定单元格在表现时的文本颜色
    SULANG_MFC_METHOD(GetTextColor);

    // @name 置文本色
    // @method
    // @description 设置数据源中指定单元格在表现时的文本颜色。注意如果数据源所使用的数据提供者不支持此特性，本命令将被忽略
    SULANG_MFC_METHOD(SetTextColor);

    // @name 取背景色
    // @method
    // @description 返回数据源中指定单元格在表现时的背景颜色
    SULANG_MFC_METHOD(GetBackground);

    // @name 置背景色
    // @method
    // @description 设置数据源中指定单元格在表现时的背景颜色。注意如果数据源所使用的数据提供者不支持此特性，本命令将被忽略
    SULANG_MFC_METHOD(SetBackground);

    // @name 取字体名
    // @method
    // @description 返回数据源中指定单元格在表现时所使用字体的名称
    SULANG_MFC_METHOD(GetFontName);

    // @name 置字体名
    // @method
    // @description 设置数据源中指定单元格在表现时所使用字体的名称。注意如果数据源所使用的数据提供者不支持此特性，本命令将被忽略
    SULANG_MFC_METHOD(SetFontName);

    // @name 取字体尺寸
    // @method
    // @description 返回数据源中指定单元格在表现时所使用字体的尺寸，单位为0.1毫米
    SULANG_MFC_METHOD(GetFontSize);

    // @name 置字体尺寸
    // @method
    // @description 设置数据源中指定单元格在表现时所使用字体的尺寸，单位为0.1毫米。注意如果数据源所使用的数据提供者不支持此特性，本命令将被忽略
    SULANG_MFC_METHOD(SetFontSize);

    // @name 取字体属性
    // @method
    // @description 返回数据源中指定单元格在表现时所使用字体的属性。返回值为以下常量值之一或之和： 1、#粗体； 2、#斜体； 4、#下划线； 8、#删除线
    SULANG_MFC_METHOD(GetFontAttr);

    // @name 置字体属性
    // @method
    // @description 设置数据源中指定单元格在表现时所使用字体的属性。注意如果数据源所使用的数据提供者不支持此特性，本命令将被忽略
    SULANG_MFC_METHOD(SetFontAttr);

    // @name 取边距
    // @method
    // @description 返回数据源中指定单元格在表现其中数据时至其单元格边框之间的空白距离，单位为0.1毫米
    SULANG_MFC_METHOD(GetExtra);

    // @name 置边距
    // @method
    // @description 设置数据源中指定单元格在表现其中数据时至其单元格边框之间的空白距离，单位为0.1毫米。注意如果数据源所使用的数据提供者不支持此特性，本命令将被忽略
    SULANG_MFC_METHOD(SetExtra);

    // @name 取文本输入格式
    // @method
    // @description 返回数据源中某文本型单元格的输入格式。当新的数据通过输入方式更新到该单元格内之前，将首先自动根据此格式转换。返回值为以下常量值之一： 0、#通常型； 1、#字节型； 2、#短整数型； 3、#整数型； 4、#长整数型； 5、#小数型； 6、#双精度小数型； 7、#逻辑型； 8、#日期时间型
    SULANG_MFC_METHOD(GetInputType);

    // @name 置文本输入格式
    // @method
    // @description 设置数据源中某文本型单元格的输入格式。当新的数据通过输入方式更新到该单元格内之前，将首先自动根据此格式转换。注意如果数据源所使用的数据提供者不支持此特性，本命令将被忽略
    SULANG_MFC_METHOD(SetInputType);

    // @name 取对齐方式
    // @method
    // @description 返回数据源中指定单元格在表现时所使用的对齐方式。返回值为以下常量值之一： 1、#上左； 2、#上中； 3、#上右； 4、#中左； 5、#中中； 6、#中右； 7、#下左； 8、#下中； 9、#下右；  10、#缩放图片； 11、#居中图片； 12、#缩放居中图片； 13、#平铺图片； 14、#缩放平铺图片
    SULANG_MFC_METHOD(GetAlignMode);

    // @name 置对齐方式
    // @method
    // @description 设置数据源中指定单元格在表现时所使用的对齐方式。注意如果数据源所使用的数据提供者不支持某些属性，该属性将被忽略
    SULANG_MFC_METHOD(SetAlignMode);

    // @name 取密码方式
    // @method
    // @description 返回数据源中指定单元格在表现时是否使用密码方式
    SULANG_MFC_METHOD(GetPwdMode);

    // @name 置密码方式
    // @method
    // @description 如设置值为真，则数据源中指定单元格在表现时以密码字符呈现。注意如果数据源所使用的数据提供者不支持此特性，本命令将被忽略
    SULANG_MFC_METHOD(SetPwdMode);

    // @name 取文本
    // @method
    // @description 返回数据源中指定单元格的文本内容
    SULANG_MFC_METHOD(GetText);

    // @name 置文本
    // @method
    // @description 设置数据源中指定单元格的文本内容，注意该单元格类型必须为“#文本”或“#图片文件名”
    SULANG_MFC_METHOD(SetText);

    // @name 取数据
    // @method
    // @description 返回数据源中指定单元格的图片或字节集数据内容
    SULANG_MFC_METHOD(GetData);

    // @name 置数据
    // @method
    // @description 设置数据源中指定单元格的图片或字节集数据内容，注意该单元格类型必须为“#图片数据”或“#字节集数据”
    SULANG_MFC_METHOD(SetData);

    // @name 合并
    // @method
    // @description 组合数据源中指定范围内的单元格，使之以一个单元格的形式表现。注意如果数据源所使用的数据提供者不支持此特性，本命令将被忽略
    SULANG_MFC_METHOD(Cmb);

    // @name 分解
    // @method
    // @description 分解数据源中指定的已经组合的单元格，行列参数指向被组合单元格内的任何一个单元格即可。注意如果数据源所使用的数据提供者不支持此特性，本命令将被忽略
    SULANG_MFC_METHOD(Split);

    // @name 是否被合并
    // @method
    // @description 如果数据源中指定单元格与周边指定单元格有组合关系，返回真，否则返回假
    SULANG_MFC_METHOD(HasCmb);

    // @name 加线条
    // @method
    // @description 为数据源中指定范围内单元格添加线条。注意如果数据源所使用的数据提供者不支持此特性，本命令将被忽略
    SULANG_MFC_METHOD(AddLine);

    // @name 删线条
    // @method
    // @description 将数据源中指定范围内单元格内的线条去除。注意如果数据源所使用的数据提供者不支持此特性，本命令将被忽略
    SULANG_MFC_METHOD(RemoveLine);

    // @name 是否有线条
    // @method
    // @description 如果数据源中指定单元格有指定线条，返回真，否则返回假
    SULANG_MFC_METHOD(HasLine);

    // @name 清除
    // @method
    // @description 清除数据源中指定范围内单元格内容为空文本
    SULANG_MFC_METHOD(EmptyCell);

    // @name 置初始属性
    // @method
    // @description 设置在数据源中初始或者添加新单元格时默认的单元格初始属性
    SULANG_MFC_METHOD(SetInitData);

    // @name 初始尺寸
    // @method
    // @description 初始数据源中数据的行列数，注意数据源中所有原有数据将被清除。注意某些数据提供者可能不支持此方法。成功返回真，失败返回假
    SULANG_MFC_METHOD(InitSize);

    // @name 取行数
    // @method
    // @description 返回数据源中现行数据行数
    SULANG_MFC_METHOD(GetRowCount);

    // @name 取列数
    // @method
    // @description 返回数据源中现行数据列数
    SULANG_MFC_METHOD(GetColCount);

    // @name 插入行
    // @method
    // @description 在数据源中指定位置处插入新数据行。注意某些数据提供者可能不支持在中间插入行。成功返回真，失败返回假
    SULANG_MFC_METHOD(InsertRow);

    // @name 添加行
    // @method
    // @description 在数据源中最后一行后插入新数据行。成功返回真，失败返回假
    SULANG_MFC_METHOD(AppendRow);

    // @name 删除行
    // @method
    // @description 在数据源中指定位置处删除数据行。成功返回真，失败返回假
    SULANG_MFC_METHOD(RemoveRow);

    // @name 插入列
    // @method
    // @description 在数据源中指定位置处插入新数据列。成功返回真，失败返回假
    SULANG_MFC_METHOD(InsertCol);

    // @name 删除列
    // @method
    // @description 在数据源中指定位置处删除数据列。成功返回真，失败返回假
    SULANG_MFC_METHOD(RemoveCol);

    // @name 添加
    // @method
    // @description 将指定其它数据源或者数据提供者中的数据添加到本数据源的尾部。成功返回真，失败返回假
    SULANG_MFC_METHOD(Append);

    // @name 存到字节集
    // @method
    // @description 将数据源中所有现有数据保存在字节集中返回。如果失败，将返回空字节集。
    SULANG_MFC_METHOD(SaveDS);

    // @name 从字节集读
    // @method
    // @description 将指定字节集内的数据源数据写入到数据源中，数据源内所有原有数据将被清除。成功返回真，失败返回假
    SULANG_MFC_METHOD(LoadDS);

    // @name 存到文件
    // @method
    // @description 将数据源中所有现有数据保存到指定文件中。成功返回真，失败返回假
    SULANG_MFC_METHOD(SaveDSFile);

    // @name 从文件读
    // @method
    // @description 将指定文件内的数据源数据写入到数据源中，数据源内所有原有数据将被清除。成功返回真，失败返回假
    SULANG_MFC_METHOD(LoadDSFile);

    // @name 单元格到字节集
    // @method
    // @description 将数据源中指定范围内单元格数据保存在字节集中返回。如果失败，将返回空字节集
    SULANG_MFC_METHOD(SaveDSCell);

    // @name 字节集到单元格
    // @method
    // @description 将指定字节集内的单元格数据写入到数据源中指定位置。成功返回真，失败返回假
    SULANG_MFC_METHOD(LoadDSCell);

    // @name 单元格到文件
    // @method
    // @description 将数据源中指定范围内单元格数据保存到指定文件中。成功返回真，失败返回假
    SULANG_MFC_METHOD(SaveDSCellFile);

    // @name 文件到单元格
    // @method
    // @description 将指定文件内的单元格数据写入到数据源中指定位置。成功返回真，失败返回假
    SULANG_MFC_METHOD(LoadDSCellFile);

    // @name 刷新
    // @method
    // @description 通知所有基于本数据源的数据处理者，使其自动从数据源重新获取并显示数据
    SULANG_MFC_METHOD(Refrush);

    // @name 保存更改
    // @method
    // @description 通知所有基于本数据源的数据处理者，使其自动将处理或更改后的数据写回到数据源中
    SULANG_MFC_METHOD(SaveChange);

    // @name 打印设置
    // @method
    // @description 调用对话框设置数据的打印配置信息。当操作者按确认按钮退出对话框时返回真，否则返回假
    SULANG_MFC_METHOD(SetupPrinter);

    // @name 取打印设置
    // @method
    // @description 返回打印数据源数据时所将使用的设置信息
    SULANG_MFC_METHOD(GetPrintInf);

    // @name 置打印设置
    // @method
    // @description 设置打印数据源数据时所将使用的设置信息。成功返回真，失败返回假
    SULANG_MFC_METHOD(SetPrintInf);

    // @name 取打印页宽
    // @method
    // @description 返回打印数据源数据时所将使用打印纸的正文区域宽度，单位为0.1毫米。注意：1、此宽度不包含页边距；2、“打印设置”命令执行后，如果操作者改变了纸张类型，此值将自动改变
    SULANG_MFC_METHOD(GetPageWidth);

    // @name 取打印页高
    // @method
    // @description 返回打印数据源数据时所将使用打印纸的正文区域高度，单位为0.1毫米。注意：1、此宽度不包含页边距；2、“打印设置”命令执行后，如果操作者改变了纸张类型，此值将自动改变
    SULANG_MFC_METHOD(GetPageHeight);

    // @name 取只读方式
    // @method
    // @description 返回数据源中指定单元格在表现时是否只读
    SULANG_MFC_METHOD(GetReadOnly);

    // @name 置只读方式
    // @method
    // @description 如设置值为真，则数据源中指定单元格在表现时只能读取而不能写入。注意： 1、如果数据处理者不支持此特性，则本属性无效； 2、如果数据提供者不支持此特性，本命令将被忽略
    SULANG_MFC_METHOD(SetReadOnly);

    // @name 当前记录改变
    // @group 事件
    // @description 当操作者按下移动记录按钮移动了当前记录指针后，会产生本事件
    SULANG_MFC_EVENT(evt);

    // @name 添加记录
    // @group 事件
    // @description 当操作者按下添加记录按钮添加了新记录后，会产生本事件
    SULANG_MFC_EVENT(evt);

    // @name 删除记录
    // @group 事件
    // @description 当操作者按下删除记录按钮删除了当前记录后，会产生本事件
    SULANG_MFC_EVENT(evt);

};