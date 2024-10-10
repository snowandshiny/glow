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

    /**
     * @brief 构造函数
     */
    LoggerManager();

    // 获取名称为name的日志器
    // 如果name不存在，则创建一个，并使用root配置
    Logger::ptr getLogger(const std::string& name);

    /**
     * @brief 初始化
     */
    void init();

    /**
     * @brief 返回主日志器
     */
    Logger::ptr getRoot() const { return m_root; }

    /**
     * @brief 将所有的日志器配置转成YAML String
     */
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