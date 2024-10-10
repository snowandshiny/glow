#include "log_appender.h"
#include <iostream>
#include <memory>
#include "log_event.h"
#include "util.h"

namespace glow {
LogAppender::LogAppender() {
    LogFormatter::ptr formatter(new LogFormatter(
        "%d{%Y-%m-%d %H:%M:%S}%T%t%T%F%T[%p]%T[%c]%T%f:%l%T%m%n"));
    m_formatter = formatter;
}

void LogAppender::setFormatter(LogFormatter::ptr val) {
    // MutexType::Lock lock(m_mutex);
    m_formatter = val;
    if (m_formatter) {
        m_hasFormatter = true;
    } else {
        m_hasFormatter = false;
    }
}

void StdoutLogAppender::log(LogEvent::ptr event) {
    // MutexType::Lock lock(m_mutex);
    // 这里需要使用m_formatter来进行格式化
    std::cout << m_formatter->format(event);
}

// std::string StdoutLogAppender::toYamlString() {
//     MutexType::Lock lock(m_mutex);
//     YAML::Node node;
//     node["type"] = "StdoutLogAppender";
//     if (m_level != LogLevel::UNKNOW) {
//         node["level"] = LogLevel::ToString(m_level);
//     }
//     if (m_hasFormatter && m_formatter) {
//         node["formatter"] = m_formatter->getPattern();
//     }
//     std::stringstream ss;
//     ss << node;
//     return ss.str();
// }

FileLogAppender::FileLogAppender(const std::string& filename)
    : m_filename(filename) {
    reopen();
}

bool FileLogAppender::reopen() {
    // MutexType::Lock lock(m_mutex);
    if (m_filestream) {
        m_filestream.close();
    }
    return FSUtil::OpenForWrite(m_filestream, m_filename, std::ios::app);
}

void FileLogAppender::log(LogEvent::ptr event) {
    // if (event->getLevel() >= m_level) {
    uint64_t now = event->getTime();
    if (now >= (m_lastTime + 3)) {
        reopen();
        m_lastTime = now;
    }

    // MutexType::Lock lock(m_mutex);
    m_filestream << m_formatter->format(event);
}

// std::string FileLogAppender::toYamlString() {
//     MutexType::Lock lock(m_mutex);
//     YAML::Node node;
//     node["type"] = "FileLogAppender";
//     node["file"] = m_filename;
//     if (m_level != LogLevel::UNKNOW) {
//         node["level"] = LogLevel::ToString(m_level);
//     }
//     if (m_hasFormatter && m_formatter) {
//         node["formatter"] = m_formatter->getPattern();
//     }
//     std::stringstream ss;
//     ss << node;
//     return ss.str();
// }

}  // namespace glow
