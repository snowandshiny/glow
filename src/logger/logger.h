#ifndef GLOW_LOGGER_H
#define GLOW_LOGGER_H
#include <stdint.h>
#include <iostream>
#include <list>
#include <memory>  //智能指针所需的头文件
#include <sstream>
#include <string>  //日志名称用到了字符串
#include "log_appender.h"
#include "log_event.h"
#include "log_level.h"

namespace glow {
class Logger {
   public:
    // 定义智能指针
    typedef std::shared_ptr<Logger> ptr;
    // 日志名称有且仅有构造时可以指定，如果未指定则给个默认名称 root
    // 这里传引用类型是为了避免不必要的拷贝操作
    // 使用const是为了规定这个名称在内部不再会被改变
    Logger(const std::string& name = "root");

    // 希望名称能被获取 但不应该被改变所以用const
    // 为避免无用的拷贝所以返回引用类型
    const std::string& getName() const { return m_name; }
    LogLevel::Level getLevel() const { return m_level; }
    void setLevel(LogLevel::Level val) { m_level = val; }

    // 定义了一个输出日志的方法 传入想要查看的最大日志级别
    void log(LogEvent::ptr event);

    // 新增一个适配器
    void addAppender(LogAppender::ptr appender);
    // 删除一个适配器
    void delAppender(LogAppender::ptr appender);

   private:
    // 这里使用 m_ 开头是一个私有变量的规范
    std::string m_name;
    // 日志器能输出的最大日志级别
    LogLevel::Level m_level;
    // 日志器的适配器列表
    std::list<LogAppender::ptr> m_appenders;
};

// 定义一个 LogEventWarp类 进行RAII的方式调用输出
class LogEventWrap {
   public:
    LogEventWrap(Logger::ptr logger, LogEvent::ptr e);
    ~LogEventWrap();
    LogEvent::ptr getEvent() const { return m_event; }
    std::stringstream& getSS();

   private:
    Logger::ptr m_logger;
    LogEvent::ptr m_event;
};

}  // namespace glow

#endif