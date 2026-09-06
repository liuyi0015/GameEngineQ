//
// Created by abc17 on 2026/9/6.
//

#ifndef GAMEENGINEQ_RENDERCONTEXT_H
#define GAMEENGINEQ_RENDERCONTEXT_H
#include <unordered_map>


struct RenderContext {
    //Entity,offset
    std::unordered_map<unsigned long long,unsigned long long>vertStarts;
    std::unordered_map<unsigned long long,unsigned long long>vertCounts;
    std::unordered_map<unsigned long long,unsigned long long>indexStarts;
    std::unordered_map<unsigned long long,unsigned long long>indexCounts;
    unsigned long long composeVertStart;
    unsigned long long composeIndexStart;
};


#endif //GAMEENGINEQ_RENDERCONTEXT_H
