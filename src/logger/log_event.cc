
#include "log_event.h"
#include <stdint.h>
#include <memory>

namespace glow {
LogEvent::LogEvent(const std::string& logName,
                   LogLevel::Level level,
                   const char* file,
                   int32_t m_line,
                   uint32_t elapse,
                   uint32_t thread_id,
                   uint32_t fiber_id,
                   uint64_t time)
    : m_logName(logName),
      m_level(level),
      m_file(file),
      m_line(m_line),
      m_elapse(elapse),
      m_threadId(thread_id),
      m_fiberId(fiber_id),
      m_time(time) {};

}  // namespace glow
