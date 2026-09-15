//
// Created by XL0002 on 2026/7/22.
//

#include "Render2DProcess.h"

#include <algorithm>
#include <cassert>

#include "Pipelines.h"
#include "Render2DComponents.h"
#include "../../core/ResourceManager.hpp"
#include "../../core/ecs/Util.h"
#include "../../soft-render/GpuSimulator.h"
#include "../transform2d/Transform2dComponents.h"
#include "../transform2d/TransformUtil.h"

void Render2DProcess::draw() {
    //目前只拿第一个摄像机
    Entity camera=ecs::searchEntity<Camera2DComp>(scene)[0];
    ColorBuffer* target=ecs::getComponent<Camera2DComp>(scene,camera).value().target;
    //按pipeline分组排序
    //或者说先整体排序再分组
    std::vector<std::pair<int,Entity>> drawableEntities;
    for (const auto entity:ecs::getEntities<Drawable2DFlag>(scene)) {
        if (ecs::getComponent<Drawable2DFlag>(scene, entity).has_value()) {
            auto zOrder = ecs::getComponent<Drawable2DFlag>(scene, entity).value().z_order;
            drawableEntities.push_back({zOrder,entity});
        }
    }
    // sort by zOrder (ascending), stable to preserve insertion order for equal z
    std::stable_sort(drawableEntities.begin(), drawableEntities.end(),
                     [](const auto &a, const auto &b){ return a.first < b.first; });
    //绑定渲染目标
    RenderPass render_pass(target);
    //绑定渲染管线 todo 未分组
    render_pass.cur_pipeline=gpu->pipelines["default_pipeline"];

    target->clear({0,0,0,1});//黑屏时调成红色用来debug
    //顶点分组
    for (int i=0;i<drawableEntities.size();i++) {
        Entity entity=drawableEntities[i].second;
        auto indexOffset=renderContext->indexOffsets[entity];
        auto indexCount=renderContext->indexCounts[entity];
        auto vertOffset=renderContext->vertOffsets[entity];
        auto uniformOffset=renderContext->uniformOffsets[entity];
        gpu->drawcall(render_pass,indexOffset,indexCount,vertOffset,uniformOffset);
    }
}
