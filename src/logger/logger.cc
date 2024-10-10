
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
// name="root")，否则编译器会报错
// 这里指定日志器一个自身默认级别是DEBUG
Logger::Logger(const std::string& name)
    : m_name(name), m_level(LogLevel::DEBUG) {
    m_formatter = LogFormatter::ptr(new LogFormatter(
        "%d{%Y-%m-%d %H:%M:%S}%T%t%T%F%T[%p]%T[%c]%T%f:%l%T%m%n"));
}

void Logger::setFormatter(LogFormatter::ptr val) {
    // MutexType::Lock lock(m_mutex);
    m_formatter = val;

    // for (auto& i : m_appenders) {
    //     // MutexType::Lock ll(i->m_mutex);
    //     if (!i->m_hasFormatter) {
    //         i->m_formatter = m_formatter;
    //     }
    // }
}

void Logger::setFormatter(const std::string& val) {
    LogFormatter::ptr new_val(new LogFormatter(val));
    // if (new_val->isError()) {
    //     std::cout << "Logger setFormatter name=" << m_name << " value=" <<
    //     val
    //               << " invalid formatter" << std::endl;
    //     return;
    // }
    setFormatter(new_val);
}

LogFormatter::ptr Logger::getFormatter() {
    // MutexType::Lock lock(m_mutex);
    return m_formatter;
}

std::string Logger::toYamlString() {
    // MutexType::Lock lock(m_mutex);
    // YAML::Node node;
    // node["name"] = m_name;
    // if (m_level != LogLevel::UNKNOW) {
    //     node["level"] = LogLevel::ToString(m_level);
    // }
    // if (m_formatter) {
    //     node["formatter"] = m_formatter->getPattern();
    // }

    // for (auto& i : m_appenders) {
    //     node["appenders"].push_back(YAML::Load(i->toYamlString()));
    // }
    // std::stringstream ss;
    // ss << node;
    // return ss.str();
    return "";
}

// 新增一个适配器
void Logger::addAppender(LogAppender::ptr appender) {
    // MutexType::Lock lock(m_mutex);
    if (!appender->getFormatter()) {
        // MutexType::Lock ll(appender->m_mutex);
        appender->setFormatter(m_formatter);
    }
    m_appenders.push_back(appender);
}

// 删除一个适配器
void Logger::delAppender(LogAppender::ptr appender) {
    // MutexType::Lock lock(m_mutex);
    for (auto it = m_appenders.begin(); it != m_appenders.end(); ++it) {
        if (*it == appender) {
            m_appenders.erase(it);
            break;
        }
    }
}

void Logger::clearAppenders() {
    // MutexType::Lock lock(m_mutex);
    m_appenders.clear();
}

// 定义了一个输出日志的方法
// 这里的参数是智能指针类型 因为日志事件是对象 所以需要传递一个对象的指针
void Logger::log(LogEvent::ptr event) {
    // 如果想要查看的级别大于等于当前日志器能查看的级别，那么才进行输出
    if (event->getLevel() >= m_level) {
        if (!m_appenders.empty()) {
            for (auto& i : m_appenders) {
                i->log(event);
            }
        }
    }
}

}  // namespace glow
