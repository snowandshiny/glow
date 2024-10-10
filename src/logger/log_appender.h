#ifndef GLOW_LOG_APPENDER_H
#define GLOW_LOG_APPENDER_H
#include <fstream>
#include <memory>
#include <string>
#include "log_event.h"
#include "log_formatter.h"

namespace glow {
class LogAppender {
    // friend class Logger;
   public:
    // typedef Spinlock MutexType;
    // 定义智能指针
    typedef std::shared_ptr<LogAppender> ptr;
    LogAppender();
    // 虚析构 空函数没有复杂逻辑所以 直接定义掉
    virtual ~LogAppender() {}
    // 输出函数为纯虚函数，因为具体实现各个子类不一样，由各个子类自己决定
    virtual void log(LogEvent::ptr event) = 0;

    /**
     * @brief 更改日志格式器
     */
    void setFormatter(LogFormatter::ptr val);

    /**
     * @brief 获取日志格式器
     */
    LogFormatter::ptr getFormatter() const {
        // MutexType::Lock lock(m_mutex);
        return m_formatter;
    }

    // /**
    //  * @brief 将日志输出目标的配置转成YAML String
    //  */
    // virtual std::string toYamlString() = 0;
    // /**
    //  * @brief 获取日志级别
    //  */
    // LogLevel::Level getLevel() const { return m_level; }

    // /**
    //  * @brief 设置日志级别
    //  */
    // void setLevel(LogLevel::Level val) { m_level = val; }

   protected:
    // MutexType m_mutex;
    LogFormatter::ptr m_formatter;

    /// 是否有自己的日志格式器
    bool m_hasFormatter = false;
    // /// 日志级别
    // LogLevel::Level m_level = LogLevel::DEBUG;
};

// 输出到控制台的Appender
class StdoutLogAppender : public LogAppender {
   public:
    typedef std::shared_ptr<StdoutLogAppender> ptr;
    // 这里的override用于表示是重写父类方法的
    void log(LogEvent::ptr event) override;

    // std::string toYamlString() override;
};

// 定义输出到文件的Appender
class FileLogAppender : public LogAppender {
   public:
    typedef std::shared_ptr<FileLogAppender> ptr;
    FileLogAppender(const std::string& filename);
    void log(LogEvent::ptr event) override;

    // std::string toYamlString() override;

    /**
     * @brief 重新打开日志文件
     * @return 成功返回true
     */
    bool reopen();

   private:
    std::string m_filename;
    /// 文件流
    std::ofstream m_filestream;
    /// 上次重新打开时间
    uint64_t m_lastTime = 0;
};

}  // namespace glow

#endif