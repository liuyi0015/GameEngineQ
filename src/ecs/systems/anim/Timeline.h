//
// Created by XL0002 on 2026/9/17.
//

#ifndef GAMEENGINEQ_TIMELINE_H
#define GAMEENGINEQ_TIMELINE_H
#include <functional>
#include <queue>
#include <vector>

struct  TimerFunc {
    double time;
    std::function<void()>func;
};
class TimerFuncComparer {
    public:
    bool operator()(const TimerFunc& a,const TimerFunc& b) const {
        return a.time < b.time;
    }
};
//一条轨道
class Timeline {
public:
    virtual ~Timeline() = default;
    virtual void updateValue(double timer,double duration)=0;
};


#endif //GAMEENGINEQ_TIMELINE_H
