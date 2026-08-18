//
// Created by abc17 on 2026/7/17.
//

#ifndef GAMEENGINE_EVENTSYSTEM_H
#define GAMEENGINE_EVENTSYSTEM_H
#include <any>
#include <functional>
#include <queue>
#include <string>

struct SubscribeInfo{
    std::string name;
    std::function<void(std::any)> callback;
    std::any param=nullptr;
    //是否触发后取消订阅
    bool isOnce;
    //是否下一帧执行
    bool nextFrame;
};
class EventDispatcher {
private:
    size_t subscribeId=0;
    std::unordered_map<std::string,std::vector<size_t> >id_group;
    std::unordered_map<size_t,SubscribeInfo> subscribed_events{};
    //延迟执行队列
    std::queue< SubscribeInfo> published_events{};
    EventDispatcher()=default;
public:
    static EventDispatcher& getInstance() {
        static EventDispatcher instance;
        return instance;
    }
    void consumeEvents();//主循环调用
    void publish(const std::string &event_name, std::any param);
    size_t subscribe(std::string event_name,std::function<void(std::any)>callback,
        bool isOnce=false,bool nextFrame=true);
    bool unsubscribe(size_t id);
};

class EventUtil {
};
#endif //GAMEENGINE_EVENTSYSTEM_H
