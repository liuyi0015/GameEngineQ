//
// Created by XL0002 on 2026/8/26.
//

#include "GeometryUtil.h"

static bool inRect(const float mx, const float my, const float x, const float y, const float w, const float h) {
    return mx >= x and mx <= x + w&&my >= y and my <= y + h;
}

//
/**
 * 判断点在多边形内，目前仅UI，因为没考虑取景和投影和视图变换
 * @param mousePos 鼠标屏幕坐标
 * @param worldTransform UI物体的世界坐标，就是屏幕坐标
 * @param vertices  UI物体内每个顶点的模型坐标
 * @return
 */
static bool pointInPolygon(){

    return false;
}