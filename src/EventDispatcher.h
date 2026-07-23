//
// Created by abc17 on 2026/7/17.
//

#ifndef GAMEENGINE_EVENTSYSTEM_H
#define GAMEENGINE_EVENTSYSTEM_H
#include <any>
#include <functional>
#include <optional>
#include <queue>
#include <string>
#include <variant>

struct EventCallback{
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
    std::vector<EventCallback> subscribed_events;
    //延迟执行队列
    std::queue< EventCallback> published_events;
    EventDispatcher()=default;
public:
    static EventDispatcher& getInstance() {
        static EventDispatcher instance;
        return instance;
    }
    void consumeEvents();//主循环调用
    void publish(const std::string &event_name, std::any param);
    void subscribe(std::string event_name,std::function<void(std::any)>callback,
        bool isOnce=false,bool nextFrame=true);
};


#endif //GAMEENGINE_EVENTSYSTEM_H
