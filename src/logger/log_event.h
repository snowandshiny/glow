#ifndef GLOW_LOG_EVENT_H
#define GLOW_LOG_EVENT_H
#include <stdint.h>
#include <memory>
#include <sstream>
#include <string>
#include "log_level.h"

namespace glow {

class LogEvent {
   public:
    typedef std::shared_ptr<LogEvent> ptr;

    LogEvent(const std::string& logName,
             LogLevel::Level level,
             const char* file,
             int32_t m_line,
             uint32_t elapse,
             uint32_t thread_id,
             uint32_t fiber_id,
             uint64_t time);

    const char* getFile() const { return m_file; }
    int32_t getLine() const { return m_line; }
    uint32_t getElapse() const { return m_elapse; }
    uint32_t getThreadId() const { return m_threadId; }
    uint32_t getFiberId() const { return m_fiberId; }
    uint64_t getTime() const { return m_time; }
    LogLevel::Level getLevel() const { return m_level; }

    const std::string& getLogName() const { return m_logName; }

    std::string getContent() const {
        return m_ss.str();
    }  // 【此处增加流对象转字符串！！！】
    std::stringstream& getSS() {
        return m_ss;
    }  // 【此处增加流对象get方法提供流式调用！！！】

   private:
    const char* m_file = nullptr;  // 文件名
    int32_t m_line = 0;            // 行号
    uint32_t m_elapse = 0;         // 程序启动开始到现在的毫秒数
    uint32_t m_threadId = 0;       // 线程id
    uint32_t m_fiberId = 0;        // 协程id
    uint64_t m_time = 0;           // 时间戳
    LogLevel::Level m_level;       // 日志级别
    std::string m_logName;         // 日志器名称
    std::stringstream m_ss;  // 字符流【此处增加流对象！！！】
};

}  // namespace glow

#endif