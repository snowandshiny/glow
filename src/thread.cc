#include "thread.h"
#include <functional>
#include <string>

namespace glow {
static thread_local std::string t_thread_name = "UNKNOW";

const std::string& Thread::GetName() {
    return t_thread_name;
}

Thread::Thread(std::function<void()> cb, const std::string& name)
    : m_cb(cb), m_name(name) {
    // if (name.empty()) {
    //     m_name = "UNKNOW";
    // }
    // int rt = pthread_create(&m_thread, nullptr, &Thread::run, this);
    // if (rt) {
    //     SYLAR_LOG_ERROR(g_logger)
    //         << "pthread_create thread fail, rt=" << rt << " name=" << name;
    //     throw std::logic_error("pthread_create error");
    // }
    // m_semaphore.wait();
}

Thread::~Thread() {
    if (m_thread) {
        pthread_detach(m_thread);
    }
}

}  // namespace glow