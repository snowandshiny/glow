#ifndef GLOW_LOG_LEVEL_H
#define GLOW_LOG_LEVEL_H

#include <string>

namespace glow {
// 一般来说就是用枚举的方式来定义
class LogLevel {
   public:
    enum Level {
        UNKNOW = 0,  // 起手先来个未知级别兜底
        DEBUG = 1,   // 调试级别
        INFO = 2,    // 普通信息级别
        WARN = 3,    // 警告信息
        ERROR = 4,   // 错误信息
        FATAL = 5    // 灾难级信息
    };

    /**
     * @brief 将日志级别转成文本输出
     * @param[in] level 日志级别
     */
    static const char* ToString(LogLevel::Level level);

    /**
     * @brief 将文本转换成日志级别
     * @param[in] str 日志级别文本
     */
    static LogLevel::Level FromString(const std::string& str);
};
}  // namespace glow

#endif