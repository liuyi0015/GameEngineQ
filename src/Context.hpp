#pragma once
#include <any>
#include<unordered_map>
#include<string>
class ApplicationContext {
private:
    std::unordered_map<std::string, std::any>_storage;
public:
    static ApplicationContext& getInstance() {
        static ApplicationContext instance;
        return instance;
    }
    template<typename T>
    void set(const std::string& key, const T& value) {
        _storage[key] = value;
    }
    template<typename T>
    T get(const std::string& key) {
        return std::any_cast<T>(_storage[key]);
    }
};
