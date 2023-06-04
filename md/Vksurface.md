为了将Vulkan渲染的图像显示在窗口上，我们需要使用**WSI(Window Systen Integration)**扩展。  
先介绍`VK_KHR_surface`扩展,它通过VksurfaceKHR对象抽象出可供Vulkan渲染的表面   
VK_KHR_surface和WSI扩展被包含在`glfwGetRequiredInstanceExtensions`函数获取的扩展列表中  
由于窗口表面对物理设备的选择有一定影响，它的创建只能在Vulkan例创建之后进行。   
Windows系统创建表面:  
```c++
VkSurfaceKHR surface;

VkWin32SurfaceCreateInfoKHR createInfo = {};
createInfo.sType = 
    VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
createInfo.hwnd = glfwGetWin32Window(window);
createInfo.hinstance = GetMouduleHandle(nullptr);
```
`glfwGetWin32Window`函数可以获取GLFW窗口对象的Windows平台窗
口句柄。`GetMouduleHandle`函数可以获取当前进程的实例句柄。
`vkCreateWin32SurfaceKHR`函数需要我们自己加载。加载后使用Vulkan实例,要创建的表面信息,自定义内存分配器和要存储表面对象的内存为调用参数:
```c++
auto CreateWin32SurfaceKHR = (PFN_vkCreateWin32SurfaceKHR)
vkGetInstanceProcAddr(instance,"vk_CreateWin32SurfaceKHR");

if(!CreateWin32SurfaceKHR || CreateWin32SurfaceKHR(
instance,&createInfo,nullptr,&surface)!=VK_SUCCESS){
throw std::runtime_error("failed_to_create_window_surface!");
}
```
Linux:vkCreateXcbSurfaceKHR函数完成表面的创建  

---
### 创建呈现队列
> VkQueue presentQueue;
```c++
#include <set>
        ···
        




