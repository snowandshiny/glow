#ifndef GLOW_LOG_FORMATTER_H
#define GLOW_LOG_FORMATTER_H
#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include "log_event.h"

namespace glow {
class LogFormatter {
   public:
    typedef std::shared_ptr<LogFormatter> ptr;
    LogFormatter(const std::string& pattern);

    /**
     * @brief 初始化,解析日志模板
     */
    void init();

    /**
     * @brief 返回日志模板
     */
    const std::string getPattern() const { return m_pattern; }

    std::string format(LogEvent::ptr event);
    // std::ostream &format(std::ostream &ofs, std::shared_ptr<Logger> logger,
    // LogLevel::Level level, LogEvent::ptr event);

    // /**
    //  * @brief 是否有错误
    //  */
    // bool isError() const { return m_error; }

    // 定义一个格式化类 用于解析模板字符串
    // 这里的FormatItem是一个抽象类 需要子类实现format方法
    class FormatItem {
       public:
        typedef std::shared_ptr<FormatItem> ptr;
        // 有子类 需要虚析构
        virtual ~FormatItem() {}
        virtual void format(std::ostream& os, LogEvent::ptr event) = 0;
    };

   private:
    std::string m_pattern;
    std::vector<FormatItem::ptr> m_items;
    // /// 是否有错误
    // bool m_error = false;
};
}  // namespace glow

#endif