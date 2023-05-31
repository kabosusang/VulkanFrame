## 获取扩展的信息
```c++
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
```
---

## 通过比较符过滤处理一定级别以上的调试信息:
```c++
static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(
    //VKAPI_ATTR 和 VKAPI_CALL 确保被Vulkan库调用
    VkDebugUtilsMessageSeverityFlagBitsEXT 
    messageSeverity,
    //指定消息的级别
    VkDebugUtilsMessageTypeFlagsEXT messageType,
    //messageType 可能是如下的值
    const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
    //
    void* pUserData 
    //
    ){
        if(messageSeverity >= VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT)
        {
            //Message is important enough to show
        }
        return VK_FALSE;
        
    }
```
### 1. **messageSeverity** 参数是如下的值
这些值经过一定设计，可以使用比较运算符来过滤处理一定级别以上
的调试信息：
```c++
 if(messageSeverity >= VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT)
        {
            //Message is important enough to show
        }
```
`VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT`: 诊断信息

`VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT`: 资源信息创建之类的信息

`VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT`: 警告信息

`VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT`: 不合法和可能造成的崩溃的信息操作  

### 2. **messageType** 可能是如下的值
`VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT`:发生了一些规范与性能无关的事件  

`VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT`:出现了违反规范的情况或发生了一个可能的错误

`VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT`:进行了可能影响Vulkan性能的行为

### 3. pCallbackData指向一个结构体指针 结构体包含成员如下:

- pMessage: 一个以null结尾的包含调试信息的字符串
- pObjects: 存储有和信息相关的Vulkan对象句柄的数组
- objectCount: 数组中的对象个数

### 4. pUserData指向我们设置回调函数时,传递的数据的指针

  
## **VkDebugUtilsMessengerCreateInfoEXT**
填写`VkDebugUtilsMessengerCreateInfoEXT`信息
```c++
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
}
```
`messageSeverity`: 指定回调函数处理的级别

`messageType`:指定回调函数处理的消息类型

## **vkCreateDebugUtilsMessengerEXT**
完成结构体后将它作为一个参数调用`vkCreateDebugUtilsMessengerEXT`
由于该函数是一个扩展函数 不会被Vulkan库自动加载 所以我们
要用`vkGetInstanceProcAddr`函数来加载
```c++
 VkResult CreateDebugUtilsMessengerEXT(VkInstance isntance,
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
```
校验层除了VkDebugUtilsMessengerCreateInfoEXT结构体的指顶的标志外还有大量可以决定校验层行为的设置  
Vulkan SDK `Config`目录 `vk_layer_settings.txt`
