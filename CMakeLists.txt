cmake_minimum_required(VERSION 3.0.0)

project(vulkanlearn VERSION 0.0.1)
set(SRC main.cpp)

#增加-std=c++17
set(CMAKE_CXX_STANDARD 17)

# 输出目录
set(output_bin ${CMAKE_CURRENT_SOURCE_DIR}/bin)
set(EXECUTABLE_OUTPUT_PATH ${output_bin})

#头文件
include_directories(${PROJECT_SOURCE_DIR}/vendor/glfw/include)
include_directories(${PROJECT_SOURCE_DIR}/vendor/glm)
include_directories(${PROJECT_SOURCE_DIR}/vendor/stb)
include_directories(${PROJECT_SOURCE_DIR}/vendor/tinyobjloader)
include_directories(${PROJECT_SOURCE_DIR}/vendor/imgui)
include_directories(${PROJECT_SOURCE_DIR}/src/Imgui)

#add subdirectory
add_subdirectory(vendor/glfw)
add_subdirectory(vendor/imgui)

#lib  
# 包含静态库路径
link_directories(${PROJECT_SOURCE_DIR}/vendor/glfw/build/src)

#Vulkan
set(VULKAN_PATH $ENV{VULKAN_SDK})

message("VULKAN_SDK: ${VULKAN_PATH}")
include_directories(${VULKAN_PATH}/Include)
link_directories(${VULKAN_PATH}/Lib)

# glfw
link_libraries(glfw3)
link_libraries(vulkan-1)

aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/src/imgui IMGUI_LIST)

list(APPEND SRC_LIST ${IMGUI_LIST})

#源文件src
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/src SRC_LIST)
add_executable(app  ${SRC_LIST})

target_link_libraries(app PRIVATE imgui glfw3)
