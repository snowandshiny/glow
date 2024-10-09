#include "log_manager.h"
namespace glow {
LoggerManager::LoggerManager() {
    auto root = Logger::ptr(new Logger);
    m_root = root;
    m_root->addAppender(LogAppender::ptr(new StdoutLogAppender));

    m_loggers[m_root->getName()] = m_root;

    init();
}

Logger::ptr LoggerManager::getLogger(const std::string& name) {
    auto it = m_loggers.find(name);
    return it == m_loggers.end() ? m_root : it->second;
}

void LoggerManager::init() {}
}  // namespace glow