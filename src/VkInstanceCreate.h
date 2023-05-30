#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <iostream>
#include <stdexcept>
#include <functional>
#include <cstdlib>
#include <cstring>


const int WIDTH = 800;
const int HEIGHT = 600;

const std::vector<const char*> validationLayers = {
    "VK_LAYER_KHRONOS_validation"
};

#ifdef NDEBUG
const bool enableValidationLayers = false;
#else
const bool enableValidationLayers = true;
#endif


class HelloTriangleApplication{
public:
    void run()
    {
        initWinddow();
        initVulkan();
        mainLoop();
        cleanup();
    }



private:
void initWinddow(){
    glfwWindowHint(GLFW_CLIENT_API,GLFW_NO_API); //阻止自动创建OpenGL上下文
    glfwWindowHint(GLFW_RESIZABLE,GLFW_FALSE);//禁止窗口大小改变
    window = glfwCreateWindow(WIDTH,HEIGHT,"Vulkan",nullptr,nullptr);
    // 1.Width 2.Height 3.Ttile 4.which Screen

}
void initVulkan(){
    createInstance();
    
}

void mainLoop(){
    while(!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
    }
}
void cleanup(){
        vkDestroyInstance(instance,nullptr);

        glfwDestroyWindow(window);

        glfwTerminate();
}

void createInstance(){
    //检查校验层
    if(enableValidationLayers && !checkValidationLayerSupport()){
        throw std::runtime_error("validation_layers_requested,_but_not_avaliable!");
    }


    VkApplicationInfo appInfo = {};
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.pApplicationName = "Hello_Triangle";
    appInfo.applicationVersion = VK_MAKE_VERSION(1,0,0);
    appInfo.pEngineName = "No_Engine";
    appInfo.engineVersion = VK_MAKE_VERSION(1,0,0);
    appInfo.apiVersion = VK_API_VERSION_1_0;
    //必须结构体 驱动程序需要使用的全局扩展和校验层
    VkInstanceCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo = &appInfo;
    /////////Glfw/////////////////
    uint32_t glfwExtensionCount = 0;
    const char** glfwExtension;

    glfwExtension = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
    
    createInfo.enabledExtensionCount = glfwExtensionCount;
    createInfo.ppEnabledExtensionNames = glfwExtension;

    createInfo.enabledLayerCount = 0;
   
    if(enableValidationLayers){
        createInfo.enabledLayerCount = static_cast<uint32_t>(
            validationLayers.size());
        createInfo.ppEnabledLayerNames = validationLayers.data();
    }else{
        createInfo.enabledLayerCount = 0;
    }



    VkResult result = vkCreateInstance(&createInfo , nullptr,&instance);
    if(result!=VK_SUCCESS){
        throw std::runtime_error("failed_to_create_instance！");
    }

    //获取扩展
    uint32_t extensionCount = 0;
    vkEnumerateInstanceExtensionProperties(nullptr,
    &extensionCount,nullptr);
    //分配数组存储扩展信息
    std::vector<VkExtensionProperties> extensions(extensionCount);
    //获取所有扩展信息
    vkEnumerateInstanceExtensionProperties(nullptr,
    &extensionCount,extensions.data());

    std::cout << "avaliable_extensions:" <<std::endl;
    for (const auto& extension : extensions)
    {
        std::cout << "\t" <<extension.extensionName << std::endl;
    }

   


}

bool checkValidationLayerSupport(){
    uint32_t layerCount;
    vkEnumerateInstanceLayerProperties(&layerCount
    ,nullptr);
    
    std::vector<VkLayerProperties> avaliableLayers(
        layerCount);
    vkEnumerateInstanceLayerProperties(&layerCount,
    avaliableLayers.data());

    for (const char* layerName: validationLayers){
        bool layerFound = false;
        
        for (const auto& layerProperties: avaliableLayers)
        {
            if(strcmp(layerName,layerProperties.layerName) == 0)
            {
                layerFound = true;
                break;
            }
        
      }
       if(!layerFound)
            {
                return false;
            }

    }
    return true;
}

private:
    VkInstance instance;
    GLFWwindow* window;
};



