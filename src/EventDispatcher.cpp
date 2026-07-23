//
// Created by abc17 on 2026/7/17.
//

#include "EventDispatcher.h"

#include <utility>


void EventDispatcher::consumeEvents() {
    //为避免事件处理函数中又发布新事件打乱顺序，published_events用队列存储
    //本帧执行和下一帧执行分类，不能直接复制。
    //下一帧事件也不方便直接塞原队列后面，不然还要额外判断当前事件是不是等着下一帧的，很麻烦
    //正式地说，这叫防止事件循环中的重入问题
    std::queue< EventCallback> nextFrameEvents;
    while (!published_events.empty()){
        //复制出来
        EventCallback eventCallback=published_events.front();
        published_events.pop();

        if (eventCallback.nextFrame) {
            eventCallback.nextFrame=false;
            nextFrameEvents.push(std::move(eventCallback));
        }else {
            eventCallback.callback(eventCallback.param);
        }
    }
    //交换地址比复制高效，或者用std::swap
    published_events=std::move(nextFrameEvents);
}

void EventDispatcher::publish(const std::string& event_name,std::any param) {

    //倒序删除
    for (int i=subscribed_events.size()-1;i>=0;i--){
        //复制，不能用&，不然std::move会移走subscribed_events的元素，相当于总是isOnce
        EventCallback eventCallback=subscribed_events[i];
        if (eventCallback.name==event_name){
            if (eventCallback.isOnce){
                //这里的自动转换应该是转换成const类型
                subscribed_events.erase(subscribed_events.begin()+i);
            }
            eventCallback.param=param;
            //传引用，subscribed->published
            published_events.push(std::move(eventCallback));
        }
    }
}

void EventDispatcher::subscribe(std::string event_name,std::function<void(std::any)>callback,bool isOnce,bool nextFrame) {
    EventCallback  eventCallback;
    eventCallback.name=std::move(event_name);
    eventCallback.callback=std::move(callback);
    eventCallback.isOnce=isOnce;
    eventCallback.nextFrame=nextFrame;
    subscribed_events.push_back(std::move(eventCallback));
}
