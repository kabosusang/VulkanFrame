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
#Vulkan
include_directories(${PROJECT_SOURCE_DIR}/vendor/Vulkan/include)

if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/vendor/Vulkan")
  message(STATUS "Vulkan exists")
else()
  message(STATUS "Vulkan does not exist")
endif()

#lib  
# 包含静态库路径
link_directories(${PROJECT_SOURCE_DIR}/vendor/glfw/build/src)
#Vulkan lib
link_directories(${PROJECT_SOURCE_DIR}/vendor/Vulkan/Lib)
# glfw
link_libraries(glfw3)
link_libraries(vulkan-1)



#源文件src
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/src SRC_LIST)
add_executable(app  ${SRC_LIST})

