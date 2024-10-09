#include "log_appender.h"
#include <iostream>
#include <memory>
#include "log_event.h"

namespace glow {
LogAppender::LogAppender() {
    LogFormatter::ptr formatter(new LogFormatter(
        "%d{%Y-%m-%d %H:%M:%S}%T%t%T%F%T[%p]%T[%c]%T%f:%l%T%m%n"));
    m_formatter = formatter;
}

void StdoutLogAppender::log(LogEvent::ptr event) {
    // 这里需要使用m_formatter来进行格式化
    std::cout << m_formatter->format(event);
}

FileLogAppender::FileLogAppender(const std::string& filename)
    : m_filename(filename) {}

void FileLogAppender::log(LogEvent::ptr event) {
    // // TODO
    // std::cout << m_formatter->format(event);
}

}  // namespace glow
