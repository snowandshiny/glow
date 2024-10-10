
#include "log_event.h"
#include <stdint.h>
#include <cstdarg>
#include <memory>
#include <string>

namespace glow {
LogEvent::LogEvent(const std::string& logger_name,
                   LogLevel::Level level,
                   const char* file,
                   int32_t m_line,
                   uint32_t elapse,
                   uint32_t thread_id,
                   uint32_t fiber_id,
                   uint64_t time,
                   const std::string& thread_name)
    : m_loggerName(logger_name),
      m_level(level),
      m_file(file),
      m_line(m_line),
      m_elapse(elapse),
      m_threadId(thread_id),
      m_fiberId(fiber_id),
      m_time(time),
      m_threadName(thread_name) {};

void LogEvent::format(const char* fmt, ...) {
    va_list al;
    va_start(al, fmt);
    format(fmt, al);
    va_end(al);
}

void LogEvent::format(const char* fmt, va_list al) {
    char* buf = nullptr;
    int len = vasprintf(&buf, fmt, al);
    if (len != -1) {
        m_ss << std::string(buf, static_cast<std::string::size_type>(len));
        free(buf);
    }
}
}  // namespace glow
