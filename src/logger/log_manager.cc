#include "log_manager.h"
namespace glow {
LoggerManager::LoggerManager() {
    // TODO m_root.reset(new Logger);
    auto m_root = Logger::ptr(new Logger);
    m_root->addAppender(LogAppender::ptr(new StdoutLogAppender));

    m_loggers[m_root->getName()] = m_root;

    init();
}

Logger::ptr LoggerManager::getLogger(const std::string& name) {
    // MutexType::Lock lock(m_mutex);
    auto it = m_loggers.find(name);
    if (it != m_loggers.end()) {
        return it->second;
    }

    Logger::ptr logger(new Logger(name));
    m_loggers[name] = logger;
    return logger;
}

/**
 * @brief 将所有的日志器配置转成YAML String
 */
std::string LoggerManager::toYamlString() {
    // TODO
    return "";
}

void LoggerManager::init() {}
}  // namespace glow