#ifndef CLIONPROJECT_RESOURCEMANAGER_H
#define CLIONPROJECT_RESOURCEMANAGER_H

#include <unordered_map>
#include <memory>
#include <string>

#include "SDL3/SDL_render.h"
#include "SDL3_image/SDL_image.h"
#include "SDL3_ttf/SDL_ttf.h"

template<typename T>
class ResourceCache {
private:
    std::vector<T> _trashBox;
    std::unordered_map<std::string,T> m_cache;
public:
    void set(const std::string& key,const T& resource) {
        m_cache[key] = resource;
    }

    T get(const std::string& key) {
        auto it = m_cache.find(key);
        if (it != m_cache.end())  return it->second;
        return nullptr;
    }

    [[nodiscard]] bool contains(const std::string& key) const {
        return m_cache.find(key) != m_cache.end();
    }

    void erase(const std::string& key) {
        m_cache.erase(key);
    }
    void trash(const std::string& key) {
        _trashBox.push_back(m_cache[key]);
    }
    void clearTrashBox() {
        _trashBox.clear();
    }

    void clear() {
        m_cache.clear();
    }

    [[nodiscard]] size_t size() const {
        return m_cache.size();
    }

    std::unordered_map<std::string,const T&> getAll() const {
        return m_cache;
    };
    // 获取所有资源的ID列表
    [[nodiscard]] std::vector<std::string> getAllIds() const {
        std::vector<std::string> ids;
        ids.reserve(m_cache.size());
        for (const auto& pair : m_cache) {
            ids.push_back(pair.first);
        }
        return ids;
    }
};

class ResourceManager {
private:
    // 私有构造函数
    ResourceManager() = default;
    // 资源缓存成员变量
    ResourceCache<SDL_Surface*> m_surfaceCache;
    ResourceCache<SDL_Texture*> m_textureCache;
    ResourceCache<TTF_Font*> m_fontCache;
    ResourceCache<IMG_Animation*> m_animationCache;

public:
    // 禁止拷贝和赋值
    ResourceManager(const ResourceManager&) = delete;
    ResourceManager& operator=(const ResourceManager&) = delete;
    // 获取单例实例
    static ResourceManager& getInstance() {
        static ResourceManager instance; // 局部静态变量保证线程安全
        return instance;
    }

    // 访问缓存的接口，有set方法就不能返回const了
    ResourceCache<SDL_Surface*>& getSurfaceCache() { return m_surfaceCache; }
    ResourceCache<SDL_Texture*>& getTextureCache() { return m_textureCache; }
    ResourceCache<TTF_Font*>& getFontCache() { return m_fontCache; }
    ResourceCache<IMG_Animation*>& getAnimationCache() { return m_animationCache; }

};

#endif //CLIONPROJECT_RESOURCEMANAGER_H
