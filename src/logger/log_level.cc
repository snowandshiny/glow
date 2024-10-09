#include "log_level.h"
namespace glow {

// 格式化日志级别
// 使用宏定义简化switch语句
const char* LogLevel::ToString(LogLevel::Level level) {
    switch (level) {
#define XX(name)         \
    case LogLevel::name: \
        return #name;    \
        break;

        XX(DEBUG);
        XX(INFO);
        XX(WARN);
        XX(ERROR);
        XX(FATAL);

#undef XX
        default:
            return "UNKNOW";
    }
    return "UNKNOW";
}
}  // namespace glow
