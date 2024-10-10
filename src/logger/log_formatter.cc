#include "log_formatter.h"
#include <functional>
#include <iostream>
#include <map>
#include <memory>
#include <sstream>
#include <string>
#include <tuple>
#include <vector>
#include "log_event.h"

namespace glow {
LogFormatter::LogFormatter(const std::string& pattern) : m_pattern(pattern) {
    // 在初始化时就将pattern解析好
    init();
}

// 格式化日志消息
class MessageFormatItem : public LogFormatter::FormatItem {
   public:
    MessageFormatItem(const std::string& str = "") {}
    void format(std::ostream& os, LogEvent::ptr event) override {
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

// 格式化线程名
class ThreadNameFormatItem : public LogFormatter::FormatItem {
   public:
    ThreadNameFormatItem(const std::string& str = "") {}
    void format(std::ostream& os, LogEvent::ptr event) override {
        os << event->getThreadName();
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

// 我们需要将模板字符串解析成 符号：子串：解析方式 的结构
// 例如这个模板 "%d{%Y-%m-%d %H:%M:%S}%T%t%T%F%T[%p]%T[%c]%T%f:%l%T%m%n"
// 可以解析成
// 符号    子串                   解析方式  注释
//"d"    "%Y-%m-%d %H:%M:%S"    1 		#当前时间
//"T"    ""                     1  		#制表（4空格）
//"t"	 ""						1	    #线程ID
//"T"    ""                     1 		#制表（4空格）
//"F"    ""                     1		#协程ID
//"T"    ""                     1 		#制表（4空格）
//"["    ""                     0		#普通字符
//"p"    ""                     1		#日志级别
//"]"    ""                     0		#普通字符
//"T"    ""                     1  		#制表（4空格）
//"["    ""                     0		#普通字符
//"c"    ""                     1		#日志器名称
//"]"    ""                     0		#普通字符
//"T"    ""                     1 		#制表（4空格）
//"f"    ""                     1		#文件名称
//":"    ""                     0		#普通字符
//"l"    ""                     1		#行号
//"T"    ""                     1 		#制表（4空格）
//"m"    ""                     1		#消息
//"n"    ""                     1 		#换行
void LogFormatter::init() {
    // 我们粗略的把上面的解析对象分成两类 一类是普通字符串 另一类是可被解析的
    // 可以用 tuple来定义 需要的格式 std::tuple<std::string,std::string,int>
    //<符号,子串,类型>  类型0-普通字符串 类型1-可被解析的字符串
    // 可以用一个 vector来存储
    // std::vector<std::tuple<std::string,std::string,int> > vec;
    std::vector<std::tuple<std::string, std::string, int> > vec;
    // 解析后的字符串
    std::string nstr;
    // 循环中解析
    for (size_t i = 0; i < m_pattern.size(); ++i) {
        // 如果不是%号
        // nstr字符串后添加1个字符m_pattern[i]
        if (m_pattern[i] != '%') {
            nstr.append(1, m_pattern[i]);
            continue;
        }
        // m_pattern[i]是% && m_pattern[i + 1] == '%' ==>
        // 两个%,第二个%当作普通字符
        if ((i + 1) < m_pattern.size()) {
            if (m_pattern[i + 1] == '%') {
                nstr.append(1, '%');
                continue;
            }
        }

        // m_pattern[i]是% && m_pattern[i + 1] != '%', 需要进行解析
        size_t n = i + 1;  // 跳过'%',从'%'的下一个字符开始解析
        int fmt_status =
            0;  // 是否解析大括号内的内容: 已经遇到'{',但是还没有遇到'}' 值为1
        size_t fmt_begin = 0;  // 大括号开始的位置

        std::string str;
        std::string fmt;  // 存放'{}'中间截取的字符
        // 从m_pattern[i+1]开始遍历
        while (n < m_pattern.size()) {
            // m_pattern[n]不是字母 & m_pattern[n]不是'{' & m_pattern[n]不是'}'
            if (!fmt_status && (!isalpha(m_pattern[n]) && m_pattern[n] != '{' &&
                                m_pattern[n] != '}')) {
                str = m_pattern.substr(i + 1, n - i - 1);
                break;
            }
            if (fmt_status == 0) {
                if (m_pattern[n] == '{') {
                    // 遇到'{',将前面的字符截取
                    str = m_pattern.substr(i + 1, n - i - 1);
                    // std::cout << "*" << str << std::endl;
                    fmt_status = 1;  // 标志进入'{'
                    fmt_begin = n;   // 标志进入'{'的位置
                    ++n;
                    continue;
                }
            } else if (fmt_status == 1) {
                if (m_pattern[n] == '}') {
                    // 遇到'}',将和'{'之间的字符截存入fmt
                    fmt = m_pattern.substr(fmt_begin + 1, n - fmt_begin - 1);
                    // std::cout << "#" << fmt << std::endl;
                    fmt_status = 0;
                    ++n;
                    // 找完一组大括号就退出循环
                    break;
                }
            }
            ++n;
            // 判断是否遍历结束
            if (n == m_pattern.size()) {
                if (str.empty()) {
                    str = m_pattern.substr(i + 1);
                }
            }
        }

        if (fmt_status == 0) {
            if (!nstr.empty()) {
                // 保存其他字符 '['  ']'  ':'
                vec.push_back(std::make_tuple(nstr, std::string(), 0));
                nstr.clear();
            }
            // fmt:寻找到的格式
            vec.push_back(std::make_tuple(str, fmt, 1));
            // 调整i的位置继续向后遍历
            i = n - 1;
        } else if (fmt_status == 1) {
            // 没有找到与'{'相对应的'}' 所以解析报错，格式错误
            std::cout << "pattern parse error: " << m_pattern << " - "
                      << m_pattern.substr(i) << std::endl;
            vec.push_back(std::make_tuple("<<pattern_error>>", fmt, 0));
        }
    }

    if (!nstr.empty()) {
        vec.push_back(std::make_tuple(nstr, "", 0));
    }

    // 以下的编写方式绝对堪称经典！！！
    static std::map<std::string,
                    std::function<FormatItem::ptr(const std::string& str)> >
        s_format_items = {
#define XX(str, C) \
    {#str, [](const std::string& fmt) { return FormatItem::ptr(new C(fmt)); }}

            XX(m, MessageFormatItem),     // m:消息
            XX(p, LevelFormatItem),       // p:日志级别
            XX(r, ElapseFormatItem),      // r:累计毫秒数
            XX(c, NameFormatItem),        // c:日志名称
            XX(t, ThreadIdFormatItem),    // t:线程id
            XX(n, NewLineFormatItem),     // n:换行
            XX(d, DateTimeFormatItem),    // d:时间
            XX(f, FilenameFormatItem),    // f:文件名
            XX(l, LineFormatItem),        // l:行号
            XX(T, TabFormatItem),         // T:Tab
            XX(F, FiberIdFormatItem),     // F:协程id
            XX(N, ThreadNameFormatItem),  // N:线程名称
#undef XX
        };

    for (auto& i : vec) {
        if (std::get<2>(i) == 0) {
            m_items.push_back(
                FormatItem::ptr(new StringFormatItem(std::get<0>(i))));
        } else {
            auto it = s_format_items.find(std::get<0>(i));
            if (it == s_format_items.end()) {
                m_items.push_back(FormatItem::ptr(new StringFormatItem(
                    "<<error_format %" + std::get<0>(i) + ">>")));
            } else {
                m_items.push_back(it->second(std::get<1>(i)));
            }
        }
    }
}

std::string LogFormatter::format(LogEvent::ptr event) {
    std::stringstream ss;
    for (auto& i : m_items) {
        i->format(ss, event);
    }
    return ss.str();
}

// std::ostream &LogFormatter::format(std::ostream &ofs, std::shared_ptr<Logger>
// logger, LogLevel::Level level, LogEvent::ptr event)
// {
//     for (auto &i : m_items)
//     {
//         i->format(ofs, logger, level, event);
//     }
//     return ofs;
// }

}  // namespace glow