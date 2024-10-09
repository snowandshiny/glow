#ifndef GLOW_LOG_MANAGER_H
#define GLOW_LOG_MANAGER_H
#include <map>
#include <memory>
#include <string>
#include <unordered_map>
#include "log_appender.h"
#include "logger.h"
#include "singleton.h"

namespace glow {
class LoggerManager {
   public:
    // TODO
    // typedef Spinlock MutexType;
    LoggerManager();
    // 获取名称为name的日志器
    // 如果name不存在，则创建一个，并使用root配置
    Logger::ptr getLogger(const std::string& name);

    void init();
    Logger::ptr getRoot() const { return m_root; }

    // 转成yaml格式的配置
    std::string toYamlString();

   private:
    // TODO
    // MutexType m_mutex;
    std::map<std::string, Logger::ptr> m_loggers;  // 所有日志器
    Logger::ptr m_root;  // 主日志器（默认日志器
};

// 日志器管理器的单例模式
typedef Singleton<LoggerManager> LoggerMgr;

}  // namespace glow
#endif