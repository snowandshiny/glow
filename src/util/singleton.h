/**
 * @brief 单例模式
 * @tparam T 单例模式的类型
 * @tparam X 为了创造多个实例对应的Tag
 * @tparam N 同一个Tag创造多个实例索引
 */
#ifndef GLOW_SINGLETON_H
#define GLOW_SINGLETON_H
#include <memory>

namespace glow {
// X 为了创造多个实例对应的Tag
// N 同一个Tag创造多个实例索引
template <class T, class X = void, int N = 0>
class Singleton {
   public:
    static T* GetInstance() {
        static T v;
        return &v;
    }
};

// X 为了创造多个实例对应的Tag
// N 同一个Tag创造多个实例索引
template <class T, class X = void, int N = 0>
class SingletonPtr {
   public:
    static std::shared_ptr<T> GetInstance() {
        static std::shared_ptr<T> v(new T);
        return v;
    }
};

}  // namespace glow
#endif