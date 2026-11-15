#pragma once

#include <component_support.h>
#include "公共事件.h"
#include "公共方法.h"

// @component odbcdb
// @name 外部数据库
// @description 外部数据库（由 krnln 5.7.64 生成，属性/方法/事件全量）
// @name_prefix 外部数据库
// @flags 窗口组件|功能提供者
class CSuOdbcdb final : public sulang::controls::BasicControl<CWnd> {
public:

    // @name 左边
    // @type 整数型
    // @group 属性
    SULANG_MFC_PROPERTY(p_16);

    // @name 顶边
    // @type 整数型
    // @group 属性
    SULANG_MFC_PROPERTY(p_21);

    // @name 宽度
    // @type 整数型
    // @group 属性
    SULANG_MFC_PROPERTY(p_26);

    // @name 高度
    // @type 整数型
    // @group 属性
    SULANG_MFC_PROPERTY(p_31);

    // @name 标记
    // @type 文本型
    // @group 属性
    SULANG_MFC_PROPERTY(p_36);

    // @name 可视
    // @type 逻辑型
    // @group 属性
    SULANG_MFC_PROPERTY(p_41);

    // @name 禁止
    // @type 逻辑型
    // @group 属性
    SULANG_MFC_PROPERTY(p_46);

    // @name 鼠标指针
    // @type 光标
    // @group 属性
    SULANG_MFC_PROPERTY(p_51);

    // @name 打开
    // @method
    // @description 打开指定的ODBC数据源以供以后操作。成功返回真，失败返回假
    SULANG_MFC_METHOD(open);

    // @name 关闭
    // @method
    // @description 关闭当前被打开的数据库
    SULANG_MFC_METHOD(close);

    // @name 取连接文本
    // @method
    // @description 返回当前被打开数据库的ODBC数据源连接文本。失败返回空文本
    SULANG_MFC_METHOD(GetConnect);

    // @name 启动事务
    // @method
    // @description 启动当前被打开数据库的事务，成功返回真，失败返回假
    SULANG_MFC_METHOD(BeginTrans);

    // @name 回滚事务
    // @method
    // @description 回滚当前被打开数据库的事务，所有在上一次事务提交前对数据库所作的修改均被取消
    SULANG_MFC_METHOD(RollbackTrans);

    // @name 提交事务
    // @method
    // @description 提交当前被打开数据库的事务
    SULANG_MFC_METHOD(CommitTrans);

    // @name 查询
    // @method
    // @description 对当前被打开数据库进行数据查询，成功返回结果记录集句柄，失败返回0。注意当不再使用此记录集时，必须使用“关闭记录集”命令将其关闭
    SULANG_MFC_METHOD(query);

    // @name 重新查询
    // @method
    // @description 对指定的记录集进行重新查询，并将其当前记录指针重定位到首记录。成功返回真，失败返回假
    SULANG_MFC_METHOD(Requery);

    // @name 关闭记录集
    // @method
    // @description 关闭指定的记录集
    SULANG_MFC_METHOD(CloseRecordset);

    // @name 首记录前
    // @method
    // @description 如果指定记录集的当前记录指针已在首记录的前面，返回真，否则返回假
    SULANG_MFC_METHOD(bof);

    // @name 尾记录后
    // @method
    // @description 如果指定记录集的当前记录指针已在尾记录的后面，返回真，否则返回假
    SULANG_MFC_METHOD(eof);

    // @name 到首记录
    // @method
    // @description 将指定记录集的当前记录指针移动到第一条记录上
    SULANG_MFC_METHOD(GoTop);

    // @name 到尾记录
    // @method
    // @description 将指定记录集的当前记录指针移动到最后一条记录上
    SULANG_MFC_METHOD(GoBottom);

    // @name 到前一记录
    // @method
    // @description 将指定记录集的当前记录指针向前移动一条记录
    SULANG_MFC_METHOD(GoPrev);

    // @name 到后一记录
    // @method
    // @description 将指定记录集的当前记录指针向后移动一条记录
    SULANG_MFC_METHOD(GoNext);

    // @name 读
    // @method
    // @description 返回指定记录集的当前记录处指定字段的数据内容，所返回数据的类型与字段SQL类型对照表如下：
    SULANG_MFC_METHOD(r);

    // @name 执行
    // @method
    // @description 执行指定的非查询类SQL语句，SQL语句中的列名（尤其是中文列名）可用中括号括住。如果SQL语句中涉及到备注或者字节集型字段，请在相应位置加上问号，然后再加上对应的文本型或字节集型数据参数，参数数目必须与问号的数目一致。例如，修改数据表student中编号为100的学生像片的语句为：  外部数据库1.执行 (“update student set picture = ? where no = 100”, 像片数据)。 成功返回真，失败返回假
    SULANG_MFC_METHOD(ExecuteSql);

    // @name 打开MDB数据库
    // @method
    // @description 本命令是针对 ACCESS MDB 数据库的特定打开方法，通过自建 ODBC 连接文本来打开 MDB 数据库。调用本命令后无需再调用“打开”命令。成功返回真，失败返回假
    SULANG_MFC_METHOD(OpenMDB);

    // @name 打开SQL数据库
    // @method
    // @description 本命令是针对 SQL SERVER 数据库的特定打开方法，通过自建 ODBC 连接文本来打开 SQL SERVER 数据库。调用本命令后无需再调用“打开”命令。成功返回真，失败返回假
    SULANG_MFC_METHOD(OpenSqlServerDB);

};