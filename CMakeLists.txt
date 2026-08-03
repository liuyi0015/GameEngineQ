cmake_minimum_required(VERSION 4.0)
set(projectname GameEngine)
project(${projectname})

set(CMAKE_CXX_STANDARD 17)

# 第1步：编译主程序源文件
file(GLOB_RECURSE GAMEENGINE_SOURCES CONFIGURE_DEPENDS
        "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
)
list(SORT GAMEENGINE_SOURCES)

add_executable(${projectname}
        ${GAMEENGINE_SOURCES}
        src/demogame/RotationSystem.cpp
        src/demogame/RotationSystem.h
)

#add_subdirectory(mineSweeper)
# 第4步：添加其他第三方库（有Cmake配置文件）
# add之前要先设好参数，即：使用ttf默认的字体库
set(SDLTTF_VENDORED ON)

add_subdirectory(third-party/SDL3-3.4.0 EXCLUDE_FROM_ALL)
add_subdirectory(third-party/SDL3_ttf-3.2.2 EXCLUDE_FROM_ALL)
add_subdirectory(third-party/SDL3_image-3.2.6 EXCLUDE_FROM_ALL)
add_subdirectory(third-party/SDL3_mixer-3.2.4 EXCLUDE_FROM_ALL)
add_subdirectory(third-party/box2d-3.1.1 EXCLUDE_FROM_ALL)
add_subdirectory(third-party/glm EXCLUDE_FROM_ALL)
# 第四步：链接所有依赖
target_link_libraries(${projectname} PUBLIC
        SDL3::SDL3
        SDL3_ttf::SDL3_ttf
        SDL3_image::SDL3_image
        SDL3_mixer::SDL3_mixer
        box2d::box2d
        glm::glm
)

# 添加 sol 头文件
#target_include_directories(${projectname} PRIVATE ${PROJECT_SOURCE_DIR}/third-party/sol-3.3.0/include)
