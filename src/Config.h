//
// Created by abc17 on 2026/3/3.
//

#ifndef CLIONPROJECT_CONFIG_H
#define CLIONPROJECT_CONFIG_H
#include <string>
struct Config {
    // window
    std::string WINDOW_TITLE="MyGame";
    std::string WINDOW_ICON="assets/1.ico";
    int WINDOW_WIDTH=1200;
    int WINDOW_HEIGHT=900;
    bool FULL_SCREEN=false;
    bool RESIZEABLE= true;
    //更新率
    double UPDATE_INTERVAL=0.0016;
    // renderer
    int VSYNC=0;
    //windows平台配置
    bool SHOW_CONSOLE;
};

#endif //CLIONPROJECT_CONFIG_H