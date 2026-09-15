//
// Created by abc17 on 2026/9/6.
//

#ifndef GAMEENGINEQ_RENDERCONTEXT_H
#define GAMEENGINEQ_RENDERCONTEXT_H
#include <unordered_map>


struct RenderContext {
    //Entity,offset
    std::unordered_map<unsigned long long,unsigned long long>vertOffsets;
    std::unordered_map<unsigned long long,unsigned long long>vertCounts;
    std::unordered_map<unsigned long long,unsigned long long>indexOffsets;
    std::unordered_map<unsigned long long,unsigned long long>indexCounts;
    std::unordered_map<unsigned long long,unsigned long long> uniformOffsets;
    //合成器
    unsigned long long composeVertStart;
    unsigned long long composeIndexStart;
};


#endif //GAMEENGINEQ_RENDERCONTEXT_H
