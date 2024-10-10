#include "log_level.h"
#include <string>
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

/**
 * @brief 将文本转换成日志级别
 * @param[in] str 日志级别文本
 */
LogLevel::Level LogLevel::FromString(const std::string& str) {
#define XX(level, v)            \
    if (str == #v) {            \
        return LogLevel::level; \
    }
    XX(DEBUG, debug);
    XX(INFO, info);
    XX(WARN, warn);
    XX(ERROR, error);
    XX(FATAL, fatal);

    XX(DEBUG, DEBUG);
    XX(INFO, INFO);
    XX(WARN, WARN);
    XX(ERROR, ERROR);
    XX(FATAL, FATAL);
    return LogLevel::UNKNOW;
#undef XX
}

}  // namespace glow
