#ifndef GLOW_LOG_FORMATTER_H
#define GLOW_LOG_FORMATTER_H
#include <iostream>
#include <memory>
#include <vector>
#include "log_event.h"

namespace glow {
class LogFormatter {
   public:
    typedef std::shared_ptr<LogFormatter> ptr;
    LogFormatter(const std::string& pattern);

    void init();  // 在初始化时就将pattern解析好

    std::string format(LogEvent::ptr event);

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
};

// 格式化日志消息
class MessageFormatItem : public LogFormatter::FormatItem {
   public:
    MessageFormatItem(const std::string& str = "") {}
    void format(std::ostream& os, LogEvent::ptr event) override {
        // TODO
        os << event->getContent();
    }
};

// 格式化日志级别
class LevelFormatItem : public LogFormatter::FormatItem {
   public:
    LevelFormatItem(const std::string& str = "") {}
    void format(std::ostream& os, LogEvent::ptr event) override {
        os << LogLevel::ToString(event->getLevel());
    }
};

// 格式化时间
class ElapseFormatItem : public LogFormatter::FormatItem {
   public:
    ElapseFormatItem(const std::string& str = "") {}
    void format(std::ostream& os, LogEvent::ptr event) override {
        os << event->getElapse();
    }
};

// 格式化日志器名
class NameFormatItem : public LogFormatter::FormatItem {
   public:
    NameFormatItem(const std::string& str = "") {}
    void format(std::ostream& os, LogEvent::ptr event) override {
        os << event->getLogName();
    }
};

// 格式化线程id
class ThreadIdFormatItem : public LogFormatter::FormatItem {
   public:
    ThreadIdFormatItem(const std::string& str = "") {}
    void format(std::ostream& os, LogEvent::ptr event) override {
        os << event->getThreadId();
    }
};

// 格式化协程id
class FiberIdFormatItem : public LogFormatter::FormatItem {
   public:
    FiberIdFormatItem(const std::string& str = "") {}
    void format(std::ostream& os, LogEvent::ptr event) override {
        os << event->getFiberId();
    }
};

// 格式化时间戳
// 这里使用了时间戳 但是没有使用时间日期格式化
class DateTimeFormatItem : public LogFormatter::FormatItem {
   public:
    DateTimeFormatItem(const std::string& format = "%Y-%m-%d %H:%M:%S")
        : m_format(format) {
        if (m_format.empty()) {
            m_format = "%Y-%m-%d %H:%M:%S";
        }
    }

    void format(std::ostream& os, LogEvent::ptr event) override {
        struct tm tm;
        time_t time = event->getTime();
        localtime_r(&time, &tm);
        char buf[64];
        strftime(buf, sizeof(buf), m_format.c_str(), &tm);
        os << buf;
    }

   private:
    std::string m_format;
};

// 格式化文件名
class FilenameFormatItem : public LogFormatter::FormatItem {
   public:
    FilenameFormatItem(const std::string& str = "") {}
    void format(std::ostream& os, LogEvent::ptr event) override {
        os << event->getFile();
    }
};

// 格式化行号
class LineFormatItem : public LogFormatter::FormatItem {
   public:
    LineFormatItem(const std::string& str = "") {}
    void format(std::ostream& os, LogEvent::ptr event) override {
        os << event->getLine();
    }
};

// 格式化换行
class NewLineFormatItem : public LogFormatter::FormatItem {
   public:
    NewLineFormatItem(const std::string& str = "") {}
    void format(std::ostream& os, LogEvent::ptr event) override {
        os << std::endl;
    }
};

// 格式化字符串
// 这里的字符串可以是任意字符串
class StringFormatItem : public LogFormatter::FormatItem {
   public:
    StringFormatItem(const std::string& str) : m_string(str) {}
    void format(std::ostream& os, LogEvent::ptr event) override {
        os << m_string;
    }

   private:
    std::string m_string;
};

// 格式化Tab
class TabFormatItem : public LogFormatter::FormatItem {
   public:
    TabFormatItem(const std::string& str = "") {}
    void format(std::ostream& os, LogEvent::ptr event) override { os << "\t"; }

   private:
    std::string m_string;
};

}  // namespace glow

#endif