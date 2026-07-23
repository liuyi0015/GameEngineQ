#include <functional>
#include <iostream>
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>

#include "AudioPlayer.h"
#include "EcsApplication.h"
#include "Config.h"
#include "Context.hpp"
#include "EventDispatcher.h"
#include "ResourceManager.hpp"
#include "demogame/DemogameApplication.h"
#include "SDL3_image/SDL_image.h"


#if _WIN32
	#include <windows.h>
#endif
void init() {
	Config config=Config();
	ApplicationContext::getInstance().set("config",config);
	//初始化SDL，没有的额外参数，用到会自动初始化
	SDL_Init(SDL_INIT_VIDEO|SDL_INIT_EVENTS|SDL_INIT_AUDIO);
	AudioPlayer::getInstance().init();
	// 创建窗口
	SDL_Surface *icon = IMG_Load(config.WINDOW_ICON.c_str());
	SDL_Window* window = SDL_CreateWindow(config.WINDOW_TITLE.c_str(), config.WINDOW_WIDTH, config.WINDOW_HEIGHT,0);
	SDL_SetWindowResizable(window,config.RESIZEABLE);
	SDL_SetWindowFullscreen(window, config.FULL_SCREEN);

	SDL_SetWindowIcon(window,  icon);
	// 创建渲染器
	SDL_Renderer* renderer = SDL_CreateRenderer(window, "opengl");
	SDL_SetRenderVSync(renderer, config.VSYNC);// 垂直同步
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	SDL_SetRenderLogicalPresentation(renderer, config.LOGIC_WIDTH, config.LOGIC_HEIGHT, SDL_LOGICAL_PRESENTATION_INTEGER_SCALE);
	SDL_PropertiesID props=SDL_GetRendererProperties(renderer);
	const char* rendererName = SDL_GetStringProperty(props, SDL_PROP_RENDERER_NAME_STRING, nullptr);
	std::cout<<"current graphics api name:"<<rendererName<<std::endl;

	TTF_Init();
	ApplicationContext::getInstance().set("window", window);
	ApplicationContext::getInstance().set("renderer", renderer);
	SDL_Log("init success");
	auto app=std::make_shared<DemogameApplication>();//可替换
	ApplicationContext::getInstance().set<std::shared_ptr< EcsApplication>>("app", app);
	app->init();
}
void start() {
	AudioPlayer::loadAndPlay("assets/1.mp3");
	ApplicationContext::getInstance().get<std::shared_ptr<EcsApplication>>("app")->start();
}
void update(double deltaTime) {
	ApplicationContext::getInstance().get<std::shared_ptr<EcsApplication>>("app")->update(deltaTime);
}
void fixed_update(double deltaTime) {
	ApplicationContext::getInstance().get<std::shared_ptr<EcsApplication>>("app")->fixed_update(deltaTime);
}
void draw() {
	ApplicationContext::getInstance().get<std::shared_ptr<EcsApplication>>("app")->draw();
}
int main_loop() {
	start();
	auto renderer = ApplicationContext::getInstance().get<SDL_Renderer*>("renderer");
	// FPS计数相关（使用 SDL_GetTicks 返回 Uint32）
	Uint32 fpsLastTick = SDL_GetTicks(); // 毫秒
	int frameCount =0;
	Uint64 lastPerformanceCounter = SDL_GetPerformanceCounter();
	Uint64 performanceFrequency = SDL_GetPerformanceFrequency();
	auto config=ApplicationContext::getInstance().get<Config>("config");
	double frameTimeAccumulator = 0.0;
	// 主循环
	bool isRunning = true;
    while (isRunning) {
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
    	//清屏
    	SDL_SetRenderDrawColor(renderer, 0, 100, 100, 255);//test
    	SDL_RenderClear(renderer);
    	draw();
    	//提交渲染
    	SDL_RenderPresent(renderer);

        // 每渲染一帧计数
        frameCount++;
        Uint32 now = SDL_GetTicks();
        if (now - fpsLastTick >= 1000) {
            SDL_Log("FPS: %d", frameCount);
            frameCount = 0;
            fpsLastTick = now;
        }

    	SDL_Event event;
    	// 事件循环
    	while (SDL_PollEvent(&event)) {
    		SDL_ConvertEventToRenderCoordinates(renderer, &event);
    		if (event.type==SDL_EVENT_QUIT) {
    			isRunning=false;
    			break;
    		}
		    if (event.type==SDL_EVENT_MOUSE_BUTTON_DOWN) {
			    if (event.button.button==SDL_BUTTON_LEFT) {
				    SDL_Log("Mouse left button down at (%f, %f)", event.button.x, event.button.y);
			    	MouseEventParam param={event.button.x,event.button.y};
			    	EventDispatcher::getInstance().publish("left mouse pressed",param);
			    	AudioPlayer::loadAndPlay("assets/2.mp3");
			    }else if (event.button.button==SDL_BUTTON_RIGHT) {
				    SDL_Log("Mouse right button down at (%f, %f)", event.button.x, event.button.y);
				    EventDispatcher::getInstance().publish("switch_scene",std::string("scene1"));
		    		AudioPlayer::loadAndPlay("assets/1.mp3");
			    }
		    }
	    }
    	EventDispatcher::getInstance().consumeEvents();
	}
	SDL_Quit();
    return 0;
}
void testEvents() {
	std::string s="successful!";
	EventDispatcher::getInstance().subscribe("testEventSystem", [s](std::any param) {
		auto* paramPtr=std::any_cast<std::string*>(param);
		if(!paramPtr) {
			std::cerr<<"paramPtr is nullptr, cannot modify the parameter."<<std::endl;
			return;
		}
		const auto paramStr=*paramPtr;
		*paramPtr="changed "+paramStr;
		std::cout<<paramStr<<s<<*paramPtr<<std::endl;
	},false,false);
	EventDispatcher::getInstance().subscribe("testRefEventSystem",[](std::any param) {
		auto& str = std::any_cast<std::reference_wrapper<std::string>>(param).get();
		str = "changed "+str;
	},false,false);
	EventDispatcher::getInstance().subscribe("testEventSystem", [s](std::any param) {
		std::cout<<"testEventSystem:OnceSubscribe. "<<s<<std::endl;
	},true);
	for (int i=0;i<2;i++) {
		std::string param="param"+std::to_string(i);
		// 可能为空时使用指针，最好是智能指针
		std::string* paramPtr=nullptr;
		EventDispatcher::getInstance().publish("testEventSystem", paramPtr);
		//使用引用
		EventDispatcher::getInstance().publish("testRefEventSystem", std::ref(param));
		EventDispatcher::getInstance().consumeEvents();//debug 立即触发
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
