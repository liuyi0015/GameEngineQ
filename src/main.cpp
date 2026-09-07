#include <functional>
#include <iostream>
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>

#include "core/AudioPlayer.h"
#include "core/EcsApplication.h"
#include "Config.h"
#include "core/ResourceManager.hpp"
#include "SDL3_image/SDL_image.h"

// #include "DemoGame3d/DemoGameApplication.h"
#include "demo/DemogameApplication.h"
#include "soft-render/SoftRenderer2D.h"

#if _WIN32
	#include <windows.h>
#endif
static void init() {
	Config config=Config();
	ApplicationContext::getInstance().set("config",config);
	//初始化SDL，没有的额外参数，用到会自动初始化
	SDL_Init(SDL_INIT_VIDEO|SDL_INIT_EVENTS|SDL_INIT_AUDIO);
	AudioPlayer::init();
	TTF_Init();
	// 创建窗口
	SDL_Window* window = SDL_CreateWindow(config.WINDOW_TITLE.c_str(), config.WINDOW_WIDTH, config.WINDOW_HEIGHT,0);
	SDL_SetWindowResizable(window,config.RESIZEABLE);
	SDL_SetWindowFullscreen(window, config.FULL_SCREEN);
	SDL_Surface *icon = IMG_Load(config.WINDOW_ICON.c_str());
	SDL_SetWindowIcon(window,  icon);
	// 创建渲染器
	auto mygpu=new SoftGPU(window);
	ApplicationContext::getInstance().set("window", window);
	ApplicationContext::getInstance().set<SoftGPU*>("mygpu",mygpu);
	auto* app=new DemogameApplication();//可替换
	ApplicationContext::getInstance().set<EcsApplication*>("app", app);
	app->init();
	std::cout<<"init success"<<std::endl;
}
static void start() {
	// AudioPlayer::loadAndPlay("assets/2.mp3");
	ApplicationContext::getInstance().get<EcsApplication*>("app")->start();
}
static void update(double deltaTime) {
	ApplicationContext::getInstance().get<EcsApplication*>("app")->update(deltaTime);
}
static void fixed_update(double deltaTime) {
	ApplicationContext::getInstance().get<EcsApplication*>("app")->fixed_update(deltaTime);
}
static void draw() {
	ApplicationContext::getInstance().get<EcsApplication*>("app")->draw();
}
bool handleEvents() {
	SDL_Event event;
	// 事件循环
	while (SDL_PollEvent(&event)) {
		if (event.type==SDL_EVENT_QUIT) {
			return false;
		}
		if (event.type==SDL_EVENT_MOUSE_BUTTON_DOWN) {
			EventBus::getInstance().publish("input mouse button",event.button);
		}else if (event.type==SDL_EVENT_MOUSE_BUTTON_UP) {
			EventBus::getInstance().publish("input mouse button",event.button);
		}else if (event.type==SDL_EVENT_MOUSE_MOTION) {
			EventBus::getInstance().publish("input mouse moved",event.motion);
		}else if (event.type==SDL_EVENT_MOUSE_WHEEL){
			EventBus::getInstance().publish("input mouse wheeled",event.wheel);
		}else if (event.type==SDL_EVENT_KEY_DOWN) {
			EventBus::getInstance().publish("input key",event.key);
		}else if (event.type==SDL_EVENT_KEY_UP) {
			EventBus::getInstance().publish("input key",event.key);
		}
	}
	EventBus::getInstance().consumeEvents();
	return true;
}

static int main_loop() {
	start();
	auto* mygpu=ApplicationContext::getInstance().get<SoftGPU*>("mygpu");
	// FPS计数相关（使用 SDL_GetTicks 返回 Uint32）
	Uint32 fpsLastTick = SDL_GetTicks(); // 毫秒
	int frameCount =0;
	Uint64 lastPerformanceCounter = SDL_GetPerformanceCounter();
	Uint64 performanceFrequency = SDL_GetPerformanceFrequency();
	auto config=ApplicationContext::getInstance().get<Config>("config");
	double frameTimeAccumulator = 0.0;
	// 主循环
	std::cout<<"Main loop started after"<<SDL_GetTicks()<<std::endl;
    while (true) {
    	// 计算帧间隔
    	Uint64 currentCounter = SDL_GetPerformanceCounter();
    	double deltaTime = static_cast<double>(currentCounter - lastPerformanceCounter) / static_cast<double>(performanceFrequency);
    	lastPerformanceCounter = currentCounter;

    	// 将帧间隔存入全局上下文？
    	ApplicationContext::getInstance().set("deltaTime", deltaTime);
    	//逻辑更新
    	frameTimeAccumulator += deltaTime;
    	while (frameTimeAccumulator >= config.UPDATE_INTERVAL) {
    		fixed_update(config.UPDATE_INTERVAL);
    		frameTimeAccumulator -= config.UPDATE_INTERVAL;
    	}
    	//帧更新
    	update(deltaTime);
    	draw();
    	//后面可以加一些调试信息
        // 每渲染一帧计数
        frameCount++;
    	//每秒打印
        Uint32 now = SDL_GetTicks();
        if (now - fpsLastTick >= 1000) {
            std::cout<<"FPS: "<<frameCount<<std::endl;
            frameCount = 0;
            fpsLastTick = now;
        }
    	//提交渲染
    	mygpu->present();
    	if (!handleEvents()) break;
	}
	SDL_Quit();
    return 0;
}
static void testEvents() {
	std::string s="successful!";
	EventBus::getInstance().subscribe("testEvent", [s](std::any param) {
		auto* paramPtr=std::any_cast<std::string*>(param);
		if(!paramPtr) {
			std::cerr<<"paramPtr is nullptr, cannot modify the parameter."<<std::endl;
			return;
		}
		const auto paramStr=*paramPtr;
		*paramPtr="changed "+paramStr;
		std::cout<<paramStr<<s<<*paramPtr<<std::endl;
	},false,false);
	EventBus::getInstance().subscribe("testRefEvent",[](std::any param) {
		auto& str = std::any_cast<std::reference_wrapper<std::string>>(param).get();
		str = "changed "+str;
	},false,false);
	EventBus::getInstance().subscribe("testEvent", [s](std::any param) {
		std::cout<<"testEventSystem:OnceSubscribe. "<<s<<std::endl;
	},true);
	for (int i=0;i<2;i++) {
		std::string param="param"+std::to_string(i);
		// 可能为空时应使用指针
		std::string* paramPtr=nullptr;
		EventBus::getInstance().publish("testEvent", paramPtr);
		//使用引用
		EventBus::getInstance().publish("testRefEvent", std::ref(param));
		EventBus::getInstance().consumeEvents();//debug 立即触发
		std::cout<<"函数外str是："<<param<<std::endl;
	}
}
int main() {
#if _WIN32
	SetConsoleOutputCP(65001); // Set console to CP_UTF8
#endif
	init();
	testEvents();
	main_loop();
	return 0;
}
