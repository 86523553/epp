#pragma once

// SuLang MFC 组件 SDK：C++ 内部使用 MFC，DLL 边界使用稳定 C ABI。
#ifndef _WIN32
#error "SuLang MFC 组件 SDK 仅支持 Windows"
#endif

#include <afxwin.h>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <string>
#include <type_traits>

#define SULANG_MFC_COMPONENT_SDK_VERSION 0x00040000u
#define SULANG_MFC_COMPONENT_ABI_VERSION 1u
#define SULANG_MFC_COMPONENT_NOTIFICATION_BASE (WM_APP + 0x530)
#define SULANG_MFC_CALL __cdecl

// 组件接口声明宏只为头文件解析器提供稳定锚点，不生成 C++ 成员。
// 元数据注释必须紧贴对应宏；支持库继续在 set_property_json、
// call_method_json 和事件发送代码中实现实际业务。
#define SULANG_MFC_PROPERTY(...)
#define SULANG_MFC_METHOD(...)
#define SULANG_MFC_EVENT(...)

#if defined(SULANG_MFC_COMPONENT_SHARED)
#if defined(SULANG_MFC_COMPONENT_BUILD)
#define SULANG_MFC_COMPONENT_API __declspec(dllexport)
#else
#define SULANG_MFC_COMPONENT_API __declspec(dllimport)
#endif
#else
#define SULANG_MFC_COMPONENT_API
#endif

// 所有跨 DLL 字符串均为 UTF-8，调用方负责保证指针在本次调用期间有效。
extern "C" {

typedef void* SuMfcComponentHandle;

typedef struct SuMfcComponentInfoV1 {
    const char* key;
    const char* display_name;
    const char* version;
} SuMfcComponentInfoV1;

typedef void (SULANG_MFC_CALL *SuMfcComponentEventSinkV1)(
    SuMfcComponentHandle instance,
    const char* event_key,
    const char* arguments_json,
    void* user_data
);

typedef struct SuMfcComponentApiV1 {
    uint32_t abi_version;
    uint32_t sdk_version;
    const char* library_id;
    const char* library_version;
    uint32_t component_count;
    const SuMfcComponentInfoV1* (SULANG_MFC_CALL *component_info)(uint32_t index);
    SuMfcComponentHandle (SULANG_MFC_CALL *create)(const char* key, HWND parent, int32_t control_id);
    HWND (SULANG_MFC_CALL *get_hwnd)(SuMfcComponentHandle instance);
    int32_t (SULANG_MFC_CALL *set_bounds)(SuMfcComponentHandle instance, int32_t x, int32_t y, int32_t width, int32_t height);
    int32_t (SULANG_MFC_CALL *set_property)(SuMfcComponentHandle instance, const char* name, const char* value_json);
    int32_t (SULANG_MFC_CALL *call_method)(SuMfcComponentHandle instance, const char* name, const char* arguments_json, char* result, size_t result_size);
    int32_t (SULANG_MFC_CALL *set_event_sink)(SuMfcComponentHandle instance, SuMfcComponentEventSinkV1 sink, void* user_data);
    void (SULANG_MFC_CALL *destroy)(SuMfcComponentHandle instance);
} SuMfcComponentApiV1;

typedef const SuMfcComponentApiV1* (SULANG_MFC_CALL *SuMfcGetComponentApiV1)(void);

SULANG_MFC_COMPONENT_API const SuMfcComponentApiV1* SULANG_MFC_CALL SuMfcGetComponentApi();

}

namespace sulang::mfc {

struct ComponentNotification final {
    NMHDR header{};
    LPARAM value{};
};

template<class BaseControl>
class ComponentControl : public BaseControl {
public:
    static_assert(std::is_base_of_v<CWnd, BaseControl>, "组件基类必须派生自 CWnd");

    // 源码和静态库组件通过同一组 JSON 接口实现属性与方法；动态库由 C ABI 转发到这里。
    virtual int set_property_json(const char* name, const char* value_json) = 0;

    virtual int call_method_json(
        const char* name,
        const char* arguments_json,
        char* result,
        size_t result_size
    ) = 0;

    // 源码、静态库与动态库组件共用同一事件协议：事件参数必须是 JSON 数组。
    void set_event_sink(SuMfcComponentEventSinkV1 sink, void* user_data) noexcept {
        event_sink_ = sink;
        event_user_data_ = user_data;
    }

protected:
    void emit_event(const char* event_key, const char* arguments_json = "[]") {
        if (event_sink_ == nullptr || event_key == nullptr) {
            return;
        }
        event_sink_(
            static_cast<SuMfcComponentHandle>(this),
            event_key,
            arguments_json != nullptr ? arguments_json : "[]",
            event_user_data_
        );
    }

    void notify_parent(UINT code, LPARAM value = 0) {
        CWnd* parent = this->GetParent();
        if (parent == nullptr || !::IsWindow(parent->GetSafeHwnd())) {
            return;
        }
        ComponentNotification notification{};
        notification.header.hwndFrom = this->GetSafeHwnd();
        notification.header.idFrom = static_cast<UINT_PTR>(this->GetDlgCtrlID());
        notification.header.code = code;
        notification.value = value;
        parent->SendMessage(WM_NOTIFY, notification.header.idFrom, reinterpret_cast<LPARAM>(&notification));
    }

private:
    SuMfcComponentEventSinkV1 event_sink_ = nullptr;
    void* event_user_data_ = nullptr;
};

// 自定义组件可继承此类，把 MFC 消息转换成 SDK 事件。
template<class BaseControl>
class ComponentAdapter : public ComponentControl<BaseControl> {
public:
    using ComponentControl<BaseControl>::ComponentControl;
    virtual const char* component_key() const noexcept = 0;
};

// 宿主只按清单参数顺序读取支持库上报的 JSON 数组，不理解具体事件业务。
class EventArguments final {
public:
    explicit EventArguments(const char* json) noexcept : json_(json != nullptr ? json : "[]") {}

    std::string string_value(size_t index) const {
        const char* cursor = value_at(index);
        std::string result;
        if (cursor == nullptr || *cursor != '"') {
            return result;
        }
        ++cursor;
        while (*cursor != '\0' && *cursor != '"') {
            if (*cursor != '\\') {
                result.push_back(*cursor++);
                continue;
            }
            ++cursor;
            if (*cursor == '\0') {
                return {};
            }
            switch (*cursor) {
            case 'n': result.push_back('\n'); break;
            case 'r': result.push_back('\r'); break;
            case 't': result.push_back('\t'); break;
            case '"': result.push_back('"'); break;
            case '\\': result.push_back('\\'); break;
            case '/': result.push_back('/'); break;
            default: return {};
            }
            ++cursor;
        }
        return *cursor == '"' ? result : std::string{};
    }

    long long integer_value(size_t index) const noexcept {
        const char* cursor = value_at(index);
        if (cursor == nullptr) {
            return 0;
        }
        char* end = nullptr;
        const long long result = std::strtoll(cursor, &end, 10);
        return end != cursor ? result : 0;
    }

    long double number_value(size_t index) const noexcept {
        const char* cursor = value_at(index);
        if (cursor == nullptr) {
            return 0.0L;
        }
        char* end = nullptr;
        const long double result = std::strtold(cursor, &end);
        return end != cursor ? result : 0.0L;
    }

    bool boolean_value(size_t index) const noexcept {
        const char* cursor = value_at(index);
        return cursor != nullptr && std::strncmp(cursor, "true", 4) == 0;
    }

private:
    static void skip_spaces(const char*& cursor) noexcept {
        while (*cursor == ' ' || *cursor == '\t' || *cursor == '\r' || *cursor == '\n') {
            ++cursor;
        }
    }

    static bool skip_value(const char*& cursor) noexcept {
        skip_spaces(cursor);
        if (*cursor == '"') {
            ++cursor;
            while (*cursor != '\0' && *cursor != '"') {
                if (*cursor++ == '\\' && *cursor != '\0') {
                    ++cursor;
                }
            }
            if (*cursor != '"') {
                return false;
            }
            ++cursor;
            return true;
        }
        const char* start = cursor;
        while (*cursor != '\0' && *cursor != ',' && *cursor != ']') {
            ++cursor;
        }
        return cursor != start;
    }

    const char* value_at(size_t index) const noexcept {
        const char* cursor = json_;
        skip_spaces(cursor);
        if (*cursor++ != '[') {
            return nullptr;
        }
        skip_spaces(cursor);
        for (size_t current = 0; current < index; ++current) {
            if (!skip_value(cursor)) {
                return nullptr;
            }
            skip_spaces(cursor);
            if (*cursor++ != ',') {
                return nullptr;
            }
            skip_spaces(cursor);
        }
        return *cursor == ']' || *cursor == '\0' ? nullptr : cursor;
    }

    const char* json_;
};

}  // namespace sulang::mfc

