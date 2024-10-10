#ifndef GLOW_THREAD_H
#define GLOW_THREAD_H
#include <functional>
#include <memory>
#include <string>

namespace glow {

/**
 * @brief 线程类
 */
class Thread {
   public:
    /// 线程智能指针类型
    typedef std::shared_ptr<Thread> ptr;

    /**
     * @brief 构造函数
     * @param[in] cb 线程执行函数
     * @param[in] name 线程名称
     */
    Thread(std::function<void()> cb, const std::string& name);

    /**
     * @brief 析构函数
     */
    ~Thread();

    /**
     * @brief 线程名称
     */
    const std::string& getName() const { return m_name; }

    /**
     * @brief 获取当前的线程名称
     */
    static const std::string& GetName();

   private:
    /// 线程id
    pid_t m_id = -1;
    /// 线程结构
    pthread_t m_thread = 0;
    /// 线程执行函数
    std::function<void()> m_cb;
    /// 线程名称
    std::string m_name;
    // /// 信号量
    // Semaphore m_semaphore;
};

}  // namespace glow
#endif