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

    /**
     * @brief 构造函数
     * @param[in] logger_name 日志器名称
     * @param[in] level 日志级别
     * @param[in] file 文件名
     * @param[in] line 文件行号
     * @param[in] elapse 程序启动依赖的耗时(毫秒)
     * @param[in] thread_id 线程id
     * @param[in] fiber_id 协程id
     * @param[in] time 日志事件(秒)
     * @param[in] thread_name 线程名称
     */
    LogEvent(const std::string& logger_name,
             LogLevel::Level level,
             const char* file,
             int32_t m_line,
             uint32_t elapse,
             uint32_t thread_id,
             uint32_t fiber_id,
             uint64_t time,
             const std::string& thread_name);

    const char* getFile() const { return m_file; }  // 返回文件名
    int32_t getLine() const { return m_line; }      // 返回行号
    uint32_t getElapse() const {
        return m_elapse;
    }  // 返回程序启动开始到现在的毫秒数
    uint32_t getThreadId() const { return m_threadId; }  // 返回线程id
    uint32_t getFiberId() const { return m_fiberId; }    // 返回协程id
    uint64_t getTime() const { return m_time; }          // 返回时间戳
    const std::string& getThreadName() const {
        return m_threadName;
    }  // 返回线程名称
    LogLevel::Level getLevel() const { return m_level; }  // 返回日志级别

    const std::string& getLogName() const {
        return m_loggerName;
    }  // 返回日志器名称

    std::string getContent() const { return m_ss.str(); }  // 返回日志内容
    std::stringstream& getSS() {
        return m_ss;
    }  // 返回日志内容字符串流【此处增加流对象get方法提供流式调用！！！】

    /**
     * @brief 格式化写入日志内容
     */
    void format(const char* fmt, ...);

    /**
     * @brief 格式化写入日志内容
     */
    void format(const char* fmt, va_list al);

   private:
    const char* m_file = nullptr;  // 文件名
    int32_t m_line = 0;            // 行号
    uint32_t m_elapse = 0;         // 程序启动开始到现在的毫秒数
    uint32_t m_threadId = 0;       // 线程id
    std::string m_threadName;      // 线程名称
    uint32_t m_fiberId = 0;        // 协程id
    uint64_t m_time = 0;           // 时间戳
    LogLevel::Level m_level;       // 日志级别
    std::string m_loggerName;      // 日志器名称
    std::stringstream m_ss;  // 字符流【此处增加流对象！！！】
};

}  // namespace glow

#endif