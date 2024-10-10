/**
 * @file log.h
 * @author snowtiger
 * @brief 日志系统的头文件
 * @version 0.1
 */
#ifndef LOG_H
#define LOG_H

#include "log_appender.h"
#include "log_event.h"
#include "log_formatter.h"
#include "log_level.h"
#include "logger.h"
#include "util.h"

/**
 * @brief 使用流式方式将日志级别level的日志写入到logger
 */
#define GLOW_LOG_LEVEL(logger, level)                                        \
    if (logger->getLevel() <= level)                                         \
    glow::LogEventWrap(logger,                                               \
                       glow::LogEvent::ptr(new glow::LogEvent(               \
                           logger->getName(), level, __FILE__, __LINE__, 0,  \
                           glow::GetThreadId(), glow::GetFiberId(), time(0), \
                           glow::Thread::GetName())))                        \
        .getSS()

/**
 * @brief 使用流式方式将日志级别debug的日志写入到logger
 */
#define GLOW_LOG_DEBUG(logger) GLOW_LOG_LEVEL(logger, glow::LogLevel::DEBUG)

/**
 * @brief 使用流式方式将日志级别info的日志写入到logger
 */
#define GLOW_LOG_INFO(logger) GLOW_LOG_LEVEL(logger, glow::LogLevel::INFO)

/**
 * @brief 使用流式方式将日志级别warn的日志写入到logger
 */
#define GLOW_LOG_WARN(logger) GLOW_LOG_LEVEL(logger, glow::LogLevel::WARN)

/**
 * @brief 使用流式方式将日志级别error的日志写入到logger
 */
#define GLOW_LOG_ERROR(logger) GLOW_LOG_LEVEL(logger, glow::LogLevel::ERROR)

/**
 * @brief 使用流式方式将日志级别fatal的日志写入到logger
 */
#define GLOW_LOG_FATAL(logger) GLOW_LOG_LEVEL(logger, glow::LogLevel::FATAL)

/**
 * @brief 使用格式化方式将日志级别level的日志写入到logger
 */
#define GLOW_LOG_FMT_LEVEL(logger, level, fmt, ...)                          \
    if (logger->getLevel() <= level)                                         \
    glow::LogEventWrap(logger,                                               \
                       glow::LogEvent::ptr(new glow::LogEvent(               \
                           logger->getName(), level, __FILE__, __LINE__, 0,  \
                           glow::GetThreadId(), glow::GetFiberId(), time(0), \
                           glow::Thread::GetName())))                        \
        .getEvent()                                                          \
        ->format(fmt, __VA_ARGS__)

/**
 * @brief 使用格式化方式将日志级别debug的日志写入到logger
 */
#define GLOW_LOG_FMT_DEBUG(logger, fmt, ...) \
    GLOW_LOG_FMT_LEVEL(logger, glow::LogLevel::DEBUG, fmt, __VA_ARGS__)

/**
 * @brief 使用格式化方式将日志级别info的日志写入到logger
 */
#define GLOW_LOG_FMT_INFO(logger, fmt, ...) \
    GLOW_LOG_FMT_LEVEL(logger, glow::LogLevel::INFO, fmt, __VA_ARGS__)

/**
 * @brief 使用格式化方式将日志级别warn的日志写入到logger
 */
#define GLOW_LOG_FMT_WARN(logger, fmt, ...) \
    GLOW_LOG_FMT_LEVEL(logger, glow::LogLevel::WARN, fmt, __VA_ARGS__)

/**
 * @brief 使用格式化方式将日志级别error的日志写入到logger
 */
#define GLOW_LOG_FMT_ERROR(logger, fmt, ...) \
    GLOW_LOG_FMT_LEVEL(logger, glow::LogLevel::ERROR, fmt, __VA_ARGS__)

/**
 * @brief 使用格式化方式将日志级别fatal的日志写入到logger
 */
#define GLOW_LOG_FMT_FATAL(logger, fmt, ...) \
    GLOW_LOG_FMT_LEVEL(logger, glow::LogLevel::FATAL, fmt, __VA_ARGS__)

/**
 * @brief 获取主日志器
 */
#define GLOW_LOG_ROOT() glow::LoggerMgr::GetInstance()->getRoot()

/**
 * @brief 获取name的日志器
 */
#define GLOW_LOG_NAME(name) glow::LoggerMgr::GetInstance()->getLogger(name)

#endif