#ifndef GLOW_CONFIG_H
#define GLOW_CONFIG_H
#include <yaml-cpp/yaml.h>
#include <algorithm>
#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>
#include "boost/lexical_cast.hpp"

namespace glow {
/*
* @brief 配置项基类
*/
class ConfigVarBase {
   public:
    typedef std::shared_ptr<ConfigVarBase> ptr;
    ConfigVarBase(const std::string& name, const std::string& description = "")
        : m_name(name), m_description(description) {
        // 将名称变成大小写不敏感的
        std::transform(m_name.begin(), m_name.end(), m_name.begin(), ::tolower);
    }
    // 有具体类型的子类，所以需要变成 虚析构
    virtual ~ConfigVarBase() {}

    const std::string& getName() const { return m_name; }
    const std::string& getDescription() const { return m_description; }

    virtual std::string toString() = 0;
    virtual bool fromString(const std::string& val) = 0;
    virtual std::string getTypeName() const = 0;

   protected:
    std::string m_name;               // 配置项名称
    std::string m_description;        // 配置项描述
};

// 使用模板类来实现不同值类型的子类
template <class T>
class ConfigVar : public ConfigVarBase {
   public:
    typedef std::shared_ptr<ConfigVar> ptr;

    ConfigVar(const std::string& name,
              const T& default_value,
              const std::string& description = "")
        : ConfigVarBase(name, description), m_val(default_value) {}

    std::string toString() override {
        try {
            return boost::lexical_cast<std::string>(m_val);
        } catch (std::exception& e) {
            std::cout << "ConfigVar::toString exception" << e.what()
                      << " convert: " << typeid(m_val).name() << " to string";
        }
        return "";
    }

    bool fromString(const std::string& val) override {
        try {
            m_val = boost::lexical_cast<T>(val);
        } catch (std::exception& e) {
            std::cout << "ConfigVar::toString exception" << e.what()
                      << " convert: string to " << typeid(m_val).name() << " - "
                      << val;
        }
        return false;
    }

    const T getValue() const { return m_val; }

    void setValue(const T& v) { m_val = v; }
    std::string getTypeName() const override { return typeid(T).name(); }

   private:
    T m_val;          // 配置项的值
};

class Config {
   public:
    typedef std::unordered_map<std::string, ConfigVarBase::ptr> ConfigVarMap;

    //
    template <class T>
    static typename ConfigVar<T>::ptr Lookup(
        const std::string& name,
        const T& default_value,
        const std::string& description = "") {
        auto it = GetDatas().find(name);
        if (it != GetDatas().end()) {
            // 智能指针的强转 可以查看我的 【配置系统-知识储备篇】
            auto tmp = std::dynamic_pointer_cast<ConfigVar<T> >(it->second);
            if (tmp) {
                SYLAR_LOG_INFO(SYLAR_LOG_ROOT())
                    << "Lookup name=" << name << " exists";
                return tmp;
            } else {
                SYLAR_LOG_ERROR(SYLAR_LOG_ROOT())
                    << "Lookup name=" << name << " exists but type not "
                    << typeid(T).name()
                    << " real_type=" << it->second->getTypeName() << " "
                    << it->second->toString();
                return nullptr;
            }
        }

        if (name.find_first_not_of("abcdefghikjlmnopqrstuvwxyz._0123456789") !=
            std::string::npos) {
            SYLAR_LOG_ERROR(SYLAR_LOG_ROOT()) << "Lookup name invalid " << name;
            throw std::invalid_argument(name);
        }

        typename ConfigVar<T>::ptr v(
            new ConfigVar<T>(name, default_value, description));
        GetDatas()[name] = v;
        return v;
    }

    template <class T>
    static typename ConfigVar<T>::ptr Lookup(const std::string& name) {
        auto it = GetDatas().find(name);
        if (it == GetDatas().end()) {
            return nullptr;
        }
        return std::dynamic_pointer_cast<ConfigVar<T> >(it->second);
    }

    static void LoadFromYaml(const YAML::Node& root);
    static ConfigVarBase::ptr LookupBase(const std::string& name);

   private:
    // 获取配置数据map
    static ConfigVarMap& GetDatas() {
        static ConfigVarMap s_datas;
        return s_datas;
    }
};

ConfigVarBase::ptr Config::LookupBase(const std::string& name) {
    auto it = GetDatas().find(name);
    return it == GetDatas().end() ? nullptr : it->second;
}

}  // namespace glow
#endif