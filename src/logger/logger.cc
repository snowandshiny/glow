
#include "logger.h"
#include <iostream>
#include <memory>  //智能指针所需的头文件
#include <sstream>
#include <string>  //日志名称用到了字符串
#include "log_event.h"

namespace glow {
LogEventWrap::LogEventWrap(Logger::ptr logger, LogEvent::ptr e)
    : m_logger(logger), m_event(e) {}

LogEventWrap::~LogEventWrap() {
    m_logger->log(m_event);
}

std::stringstream& LogEventWrap::getSS() {
    return m_event->getSS();
}

// 头文件中已经有默认值定义了 这里就不需要了(也就是不需要
// nmae="root")，否则编译器会报错
Logger::Logger(const std::string& name)
    : m_name(name)
      // 这里指定日志器一个自身默认级别是DEBUG
      ,
      m_level(LogLevel::DEBUG) {}

// 新增一个适配器
void Logger::addAppender(LogAppender::ptr appender) {
    m_appenders.push_back(appender);
}

// 删除一个适配器
void Logger::delAppender(LogAppender::ptr appender) {
    for (auto it = m_appenders.begin(); it != m_appenders.end(); ++it) {
        if (*it == appender) {
            m_appenders.erase(it);
            break;
        }
    }
}

// 定义了一个输出日志的方法
// 这里的参数是智能指针类型 因为日志事件是对象 所以需要传递一个对象的指针
void Logger::log(LogEvent::ptr event) {
    // 如果想要查看的级别大于等于当前日志器能查看的级别，那么才进行输出
    if (event->getLevel() >= m_level) {
        for (auto& i : m_appenders) {
            i->log(event);
        }
    }
}

}  // namespace glow
