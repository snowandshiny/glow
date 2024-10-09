#include <stdint.h>
#include <time.h>
#include <unistd.h>
#include <iostream>
#include <list>
#include <map>
#include <memory>  //智能指针所需的头文件
#include <sstream>
#include <string>  //日志名称用到了字符串
#include <thread>
#include <tuple>
#include <vector>
#include "log.h"

using namespace glow;

void test_log_formatter() {
    // 获取线程ID
    std::stringstream ss;
    ss << std::this_thread::get_id();
    uint32_t tid = std::stoull(ss.str());

    // 创建一个日志事件(这里的内容随便定义，因为我们没有真正用到它)
    LogEvent::ptr event(new LogEvent("XXX",
                                     LogLevel::INFO,  // 日志级别
                                     __FILE__,        // 文件名称
                                     __LINE__,        // 行号
                                     1234567,         // 运行时间
                                     tid,             // 线程ID
                                     0,               // 协程ID
                                     time(0)          // 当前时间
                                     ));

    LogFormatter::ptr formatter(new LogFormatter(
        "%d{%Y-%m-%d %H:%M:%S}%T%t%T%F%T[%p]%T[%c]%T%f:%l%T%m%n"));
    std::cout << formatter->format(event);
}

// 此时我们可以写一个测试类试试
int main(int argc, char** argv) {
    // // 测试日志格式化器
    // test_log_formatter();

    // 获取线程ID
    std::stringstream ss;
    ss << std::this_thread::get_id();
    uint32_t tid = std::stoull(ss.str());

    std::cout << "======START======" << std::endl;
    Logger::ptr lg(new Logger("XYZ"));
    LogEvent::ptr event(new LogEvent(lg->getName(),
                                     LogLevel::INFO,  // 日志级别
                                     __FILE__,        // 文件名称
                                     __LINE__,        // 行号
                                     1234567,         // 运行时间
                                     tid,             // 线程ID
                                     0,               // 协程ID
                                     time(0)          // 当前时间
                                     ));

    LogFormatter::ptr formatter(new LogFormatter(
        "%d{%Y-%m-%d %H:%M:%S}%T%t%T%F%T[%p]%T[%c]%T%f:%l%T%m%n"));
    // 添加控制台输出适配器
    StdoutLogAppender::ptr stdApd(new StdoutLogAppender());
    stdApd->setFormatter(formatter);
    lg->addAppender(stdApd);
    event->getSS() << "hello sylar";
    //   lg->log(event);

    LogEventWrap(lg, event).getSS() << " 追加内容";
    std::cout << "=======END=======" << std::endl;
    return 0;
}