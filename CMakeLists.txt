cmake_minimum_required(VERSION 4.0)
set(projectname GameEngineQ)
project(${projectname})

set(CMAKE_CXX_STANDARD 17)

# 编译主程序源文件
file(GLOB_RECURSE GAMEENGINE_SOURCES CONFIGURE_DEPENDS
        "${CMAKE_CURRENT_SOURCE_DIR}/src/**.cpp"
)
#list(SORT GAMEENGINE_SOURCES)

add_executable(${projectname}
        ${GAMEENGINE_SOURCES}
)

# 添加其他第三方库（有Cmake配置文件）
# 记得要先执行库里的ps1脚本
# add之前要先设好参数，即：使用ttf默认的字体库
set(SDLTTF_VENDORED ON)

add_subdirectory(third-party/SDL3-3.4.0 EXCLUDE_FROM_ALL)
add_subdirectory(third-party/SDL3_ttf-3.2.2 EXCLUDE_FROM_ALL)
add_subdirectory(third-party/SDL3_image-3.2.6 EXCLUDE_FROM_ALL)
add_subdirectory(third-party/SDL3_mixer-3.2.4 EXCLUDE_FROM_ALL)
add_subdirectory(third-party/glm-master EXCLUDE_FROM_ALL)
#静态库
#link_libraries()
#link_directories()
# 添加 sol 头文件
#target_include_directories(${projectname} PRIVATE ${PROJECT_SOURCE_DIR}/third-party/sol-3.3.0/include)

# 添加从网络获取的库
include(FetchContent)

FetchContent_Declare(box3d
        GIT_REPOSITORY https://github.com/erincatto/box3d.git
        GIT_TAG v0.1.0)
FetchContent_MakeAvailable(box3d)
FetchContent_Declare(box2d
        GIT_REPOSITORY https://github.com/erincatto/box2d.git
        GIT_TAG v3.1.1)
FetchContent_MakeAvailable(box2d)
FetchContent_Declare(tinyobjloader
        GIT_REPOSITORY https://github.com/tinyobjloader/tinyobjloader.git
        GIT_TAG release
)
FetchContent_MakeAvailable(tinyobjloader)
#
#target_link_libraries(${projectname} PRIVATE box3d::box3d)
# 链接所有依赖（动态）
target_link_libraries(${projectname} PUBLIC
        SDL3::SDL3
        SDL3_ttf::SDL3_ttf
        SDL3_image::SDL3_image
        SDL3_mixer::SDL3_mixer
        box2d::box2d
        glm::glm
        tinyobjloader_c
)
