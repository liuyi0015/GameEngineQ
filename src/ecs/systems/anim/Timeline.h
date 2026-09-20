//
// Created by XL0002 on 2026/9/17.
//

#ifndef GAMEENGINEQ_TIMELINE_H
#define GAMEENGINEQ_TIMELINE_H
#include <functional>
#include <vector>

class Timeline {
    std::vector<std::function<void>>funcs;
    std::vector<double>times;
};


#endif //GAMEENGINEQ_TIMELINE_H
