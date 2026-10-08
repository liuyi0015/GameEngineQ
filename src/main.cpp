#include <functional>
#include <iostream>
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>

#include "core/AudioPlayer.h"
#include "Config.h"
#include "core/Application.h"
#include "SDL3_image/SDL_image.h"

#include "core/Context.hpp"
#include "core/EventBus.h"
#include "demo/DemogameApplication.h"
#include "input/MouseInputMapping.h"
#include "soft-render/GpuSimulator.h"

#if _WIN32
	#include <windows.h>
#endif
static void init() {
	Config config=Config();
	ApplicationContext::getInstance().set<Config>("config",config);
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
	ApplicationContext::getInstance().set<Application*>("app", app);
	app->init();
	std::cout<<"init success"<<std::endl;
}
static void fixed_update(double deltaTime) {
	ApplicationContext::getInstance().get<Application*>("app")->fixed_update(deltaTime);
}
static void update(double deltaTime) {
	ApplicationContext::getInstance().get<Application*>("app")->update(deltaTime);
}
static void draw() {
	ApplicationContext::getInstance().get<Application*>("app")->draw();
}
void handleInput(const SDL_Event& event) {

	if (event.type==SDL_EVENT_MOUSE_BUTTON_DOWN) {
		if (event.button.button==SDL_BUTTON_LEFT) {
			MouseInputMapping::leftMousePressed(event.button);
		}else if (event.button.button==SDL_BUTTON_RIGHT) {
			MouseInputMapping::rightMousePressed(event.button);
		}else if (event.button.button==SDL_BUTTON_MIDDLE) {
			MouseInputMapping::middleMousePressed(event.button);
		}
	}else if (event.type==SDL_EVENT_MOUSE_BUTTON_UP) {
		if (event.button.button==SDL_BUTTON_LEFT) {
			MouseInputMapping::leftMouseReleased(event.button);
		}else if (event.button.button==SDL_BUTTON_RIGHT) {
			MouseInputMapping::rightMouseReleased(event.button);
		}else if (event.button.button==SDL_BUTTON_MIDDLE) {
			MouseInputMapping::middleMouseReleased(event.button);
		}
	}else if (event.type==SDL_EVENT_MOUSE_MOTION) {
		MouseInputMapping::mouseMoved(event.motion);
	}else if (event.type==SDL_EVENT_MOUSE_WHEEL){
		if (event.wheel.y > 0) {
			MouseInputMapping::wheelScrollUp(event.wheel);
		} else if (event.wheel.y < 0) {
			MouseInputMapping::wheelScrollDown(event.wheel);
		}
	}else if (event.type==SDL_EVENT_KEY_DOWN) {
	}else if (event.type==SDL_EVENT_KEY_UP) {
	}
}

static int main_loop() {
	auto config=ApplicationContext::getInstance().get<Config>("config");
	//帧间隔
	Uint32 lastTick = SDL_GetTicks(); // 毫秒
	// FPS计数相关
	Uint32 lastSecondTick = SDL_GetTicks();
	//用于计算FPS
	int frameCount =0;
	//用于逻辑更新
	double frameTimeAccumulator = 0.0;
	// 主循环
	std::cout<<"Main loop started after"<<SDL_GetTicks()<<std::endl;
	bool shouldStop = false;
    while (!shouldStop) {
    	// 计算帧间隔
        Uint32 now = SDL_GetTicks();
    	double deltaTime = static_cast<double>(now - lastTick) / 1000.0;
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
    	//帧率
        // 每渲染一帧计数
        frameCount++;
    	//每秒打印
        if (now-lastSecondTick >= 1000) {
            std::cout<<"FPS: "<<frameCount<<std::endl;
            frameCount = 0;
    		lastSecondTick=now;
        }
    	// 事件处理
    	SDL_Event event;
    	while (SDL_PollEvent(&event)) {
    		if (event.type==SDL_EVENT_QUIT) {
    			shouldStop=true;
    		}

    		if (event.type==SDL_EVENT_WINDOW_RESIZED) {
    			auto* window=ApplicationContext::getInstance().get<SDL_Window*>("window");
    			SDL_GetWindowSize(window,&config.WINDOW_WIDTH,&config.WINDOW_HEIGHT);
    			ApplicationContext::getInstance().set<Config>("config",config);
    		}
    		handleInput(event);
    	}
    	EventBus::getInstance().consumeEvents();
    	lastTick=now;
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
