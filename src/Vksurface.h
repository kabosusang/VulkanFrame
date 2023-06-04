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

VkResult CreateDebugUtilsMessengerEXT(VkInstance instance,
const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo,
const VkAllocationCallbacks* pAllocator,
VkDebugUtilsMessengerEXT* pCallback
){
    auto func =(PFN_vkCreateDebugUtilsMessengerEXT)
    vkGetInstanceProcAddr(instance,"vkCreateDebugUtilsMessengerEXT");

    if(func!=nullptr){
        return func(instance,pCreateInfo,pAllocator,pCallback);
    }else{  
        return VK_ERROR_EXTENSION_NOT_PRESENT;
    }

}
void DestoryDebugUtilsMessengerEXT(VkInstance instance,
    VkDebugUtilsMessengerEXT callback,
    const VkAllocationCallbacks* pAllocator
){
    auto func =(PFN_vkDestroyDebugUtilsMessengerEXT)
    vkGetInstanceProcAddr(instance,"vkDestroyDebugUtilsMessengerEXT");
    
    if(func!=nullptr){
        func(instance,callback,pAllocator);
    }

}


//该结构体作为函数返回结果的类型
struct QueueFamilyIndices{
    int graphicsFamily = -1;
    int presentFamily = -1;

    bool isComplete(){
        return graphicsFamily>=0 && presentFamily >=0;
    }

};


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
    glfwInit();
    
    glfwWindowHint(GLFW_CLIENT_API,GLFW_NO_API); //阻止自动创建OpenGL上下文
    glfwWindowHint(GLFW_RESIZABLE,GLFW_FALSE);//禁止窗口大小改变
    window = glfwCreateWindow(WIDTH,HEIGHT,"Vulkan",nullptr,nullptr);
    // 1.Width 2.Height 3.Ttile 4.which Screen

}
void initVulkan(){
    createInstance();
    setupDebugCallback();
    createSurface();
    pickPhysicalDevice();
    createLogicalDevice();

}

void mainLoop(){
    while(!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
    }
}
void cleanup(){
         vkDestroyDevice(device,nullptr);

        if(enableValidationLayers){
            DestoryDebugUtilsMessengerEXT(instance,callback,nullptr);
             } 
        
        vkDestroySurfaceKHR(instance,surface,nullptr);
        vkDestroyInstance(instance,nullptr);
       
        glfwDestroyWindow(window);

        glfwTerminate();
}
void setupDebugCallback()
{
    if(!enableValidationLayers) return;
    VkDebugUtilsMessengerCreateInfoEXT createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    
    createInfo.messageSeverity =
    VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
    VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
    VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;

    createInfo.messageType = 
    VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
    VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
    VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;

    createInfo.pfnUserCallback = debugCallback;//设置回调函数
    createInfo.pUserData = nullptr; //用户自定义数据的指针

    //检测是否能加载
    if(CreateDebugUtilsMessengerEXT(instance,&createInfo,nullptr,
    &callback)!=VK_SUCCESS){
        throw std::runtime_error("failed_to_set_up_debug_callback");
    }
}
void createLogicalDevice()
{
    //指定要创建的队列
    QueueFamilyIndices indices = findQueueFamilies(physicalDevice);

    VkDeviceQueueCreateInfo queueCreateInfo = {};
    queueCreateInfo.sType =
    VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queueCreateInfo.queueFamilyIndex = indices.graphicsFamily;
    queueCreateInfo.queueCount = 1;
    //指定队列0.0~1.0 作为优先级来控制指令缓冲的执行顺序
    float queuePriority = 1.0f;
    queueCreateInfo.pQueuePriorities = &queuePriority;

    //指定应用程序使用的设备特性
    VkPhysicalDeviceFeatures deviceFeatures = {};

    //创建逻辑设备
    VkDeviceCreateInfo createinfo = {};
    createinfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    //将VkDeviceCreateInfo结构体的pQueueCreateInfos指针指向queueCreateInfo的地址
    //pEnabledFeatures指向deviceFeatures的地址
    createinfo.pQueueCreateInfos = &queueCreateInfo;
    createinfo.queueCreateInfoCount = 1;
    createinfo.pEnabledFeatures = &deviceFeatures;

    //对设备使用校验层 不需要额外的扩展支持。
    createinfo.enabledExtensionCount = 0;
    if(enableValidationLayers){
        createinfo.enabledLayerCount = static_cast<uint32_t>(
        validationLayers.size());
        createinfo.ppEnabledExtensionNames = validationLayers.data();

    }else{
        createinfo.enabledLayerCount = 0;
    }
    //可以调用vkCreateDevice函数创建逻辑设备了
    if(vkCreateDevice(physicalDevice,&createinfo,nullptr
    ,&device)!=VK_SUCCESS){
        throw std::runtime_error("failed_to_create_logical_device!");
    }

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


 //使用VK_EXT_debug_utils 扩展
    auto extensions = getRequiredExtensions();
    createInfo.enabledExtensionCount = static_cast<uint32_t>(
        extensions.size());
    createInfo.ppEnabledExtensionNames = extensions.data();


    if(enableValidationLayers){
        createInfo.enabledLayerCount = static_cast<uint32_t>(
            validationLayers.size());
        createInfo.ppEnabledLayerNames = validationLayers.data();
    }else{
        createInfo.enabledLayerCount = 0;
    }
    
    //最后创建实例
    VkResult result = vkCreateInstance(&createInfo , nullptr,&instance);
    if(result!=VK_SUCCESS){
        throw std::runtime_error("failed_to_create_instance！");
    }
}
//创建表面
void createSurface()
{
    if(glfwCreateWindowSurface(instance,window,nullptr
    ,&surface)!= VK_SUCCESS)
    {
        throw std::runtime_error("failed_to_create_window_surface!");
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

void pickPhysicalDevice()
{
  
    //请求显卡的数量
    uint32_t deviceCount = 0;
    vkEnumeratePhysicalDevices(instance,&deviceCount,nullptr);
    //如果可用显卡设备数量为0 阻止程序运行
        if(deviceCount) 
        throw std::runtime_error("failed_to_find_GPUs_with_Vulkan_support!");

    //分配数组存储VkPhysicallDevice
    std::vector<VkPhysicalDevice> devices(deviceCount);
    vkEnumeratePhysicalDevices(instance,&deviceCount,devices.data());

    for(const auto& device : devices){
        if(isDeviceSuitable(device)){
            physicalDevice = device;
            break;
        }
    }

    if(physicalDevice == VK_NULL_HANDLE){
        throw std::runtime_error("failed_to_find_a_suitable_GPU!");
    }
    
}
//检查获取的设备能否满足我们的需求:
bool isDeviceSuitable(VkPhysicalDevice device){

    //自定义函数确保选择的设备
    QueueFamilyIndices indices = findQueueFamilies(device);
    return indices.isComplete();
}

//查询需要的队列族
QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device)
{
    QueueFamilyIndices indices;

    uint32_t queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(device,&queueFamilyCount
    ,nullptr);

    std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(device,&queueFamilyCount
    ,queueFamilies.data());
    //VkQueueFamilyProperties结构包含很多信息 比如支持的操作类型
    int i =0;
   
    for(const auto& queueFamily: queueFamilies){
        if(queueFamily.queueCount & VK_QUEUE_GRAPHICS_BIT){
                indices.graphicsFamily = i;
            }
    VkBool32 presentSupport = false;
    vkGetPhysicalDeviceSurfaceSupportKHR(device,i,surface,&
    presentSupport);
    if(queueFamily.queueCount>0 && presentSupport){
        indices.presentFamily = i;
    }

        if(indices.isComplete()) {break;}
        i++;
    }
    
    return indices;
}

std::vector<const char*> getRequiredExtensions(){
    //需要VK_EXT_debug_utils 扩展

    uint32_t glfwExtensionCount = 0;
    const char** glfwExtensions;
    glfwExtensions = glfwGetRequiredInstanceExtensions(
        &glfwExtensionCount);

    std::vector<const char*> extensions(glfwExtensions,
    glfwExtensions + glfwExtensionCount);

    if(enableValidationLayers){
        extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
    }
    return extensions;
}

static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
    //VKAPI_ATTR 和 VKAPI_CALL 确保被Vulkan库调用
    VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
    //指定消息的级别
    VkDebugUtilsMessageTypeFlagsEXT messageType,
    //
    const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
    //
    void* pUserData 
    //
    ){
      
        std::cerr << "validation_layer:_" <<pCallbackData->pMessage 
        << std::endl;
        return VK_FALSE;
    }

private:
    VkInstance instance;
    GLFWwindow* window;
    VkDebugUtilsMessengerEXT callback; //设置回调函数

    VkSurfaceKHR surface;
    VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
    VkDevice device;
    VkQueue graphicsQueue;

};


