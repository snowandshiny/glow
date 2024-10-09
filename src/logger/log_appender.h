#ifndef GLOW_LOG_APPENDER_H
#define GLOW_LOG_APPENDER_H
#include <memory>
#include <string>
#include "log_event.h"
#include "log_formatter.h"

namespace glow {
class LogAppender {
   public:
    // 定义智能指针
    typedef std::shared_ptr<LogAppender> ptr;
    LogAppender();
    // 虚析构 空函数没有复杂逻辑所以 直接定义掉
    virtual ~LogAppender() {}
    // 输出函数为纯虚函数，因为具体实现各个子类不一样，由各个子类自己决定
    virtual void log(LogEvent::ptr event) = 0;

    void setFormatter(LogFormatter::ptr val) { m_formatter = val; }
    LogFormatter::ptr getFormatter() const { return m_formatter; }

   protected:
    LogFormatter::ptr m_formatter;
};

// 输出到控制台的Appender
class StdoutLogAppender : public LogAppender {
   public:
    typedef std::shared_ptr<StdoutLogAppender> ptr;
    // 这里的override用于表示是重写父类方法的
    void log(LogEvent::ptr event) override;
};

// 定义输出到文件的Appender
class FileLogAppender : public LogAppender {
   public:
    typedef std::shared_ptr<FileLogAppender> ptr;
    FileLogAppender(const std::string& filename);
    void log(LogEvent::ptr event) override;

   private:
    std::string m_filename;
};

}  // namespace glow

#endif