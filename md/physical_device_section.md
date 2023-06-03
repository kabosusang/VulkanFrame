## VkPhysicalDevice
VkPhysicalDevice对象来存储我们选择使用的显卡信息。这一对象可以在VkInstance进行清除操作时,自动清除自己,所以不需要在cleanup函数中清除  

## isDeviceSuitable 检查函数(我们自定义)
随着我们使用的特性增多，这一函数所包含的检查也越来越多

```c++
//检查获取的设备能否满足我们的需求:
bool isDeviceSuitable(VkPhysicalDevice device){
    //获取更加详细的设备信息
    VkPhysicalDeviceProperties deviceProperties;
    vkGetPhysicalDeviceProperties(device,&deviceProperties);

    //其他特性的支持可以通过vkGetPhysicalDeviceFeature函数查询

    VkPhysicalDeviceFeatures deviceFeatures;
    vkGetPhysicalDeviceFeatures(device,&deviceFeatures);

    //假设我们显卡只支持集合着色器
    return deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU
    && deviceFeatures.geometryShader;
}
```
`除了第一个方法 更好的方法是给每一个满足需求的设备按照特性加权打分，选择分数最高的设备使用`可以这样做:
```c++
#include <map>

        ···

void pickPhysicalDevice(){

        ···
        std::multimap<int,VkPhysicalDevice> candidates;

        for(const auto& device : devices){
            int score = rateDeviceSuitability(device);
            candidates.insert(std::make_pair(source,device));
        }
        //Check if the best candidate is suitable at all
        if(candidates.rbegin()->first > 0){
            physicalDevice = candidates.rbegin()->second;
        }else{
            throw std::runtime_error("failed_to_find_a_suitable_GPU!");
        }
}

int rateDeviceSuitability(VkPhysicalDevice device){
        ···
        int source = 0;

        if(deviceProperties.deviceType == 
            VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU){
                    source += 1000;
            }
        //Maximum possible size of textures affects graphics quality
        source+=deviceProperties.Limits.maxImageDimension2D;

        //Application can't function without geomoetry shaders

        if(!deviceFeatures.geometryShader)
        return 0;
}   
```
## 队列族
Vulkan的几乎所有操作，从绘制到加载纹理都需要将操作
指令提交给一个队列，然后才能执行。  
Vulkan有多种不同类型的队列,他们属于不同的队列族,每个队列族的队列只允许执行特定的一部分指令。比如，可能存在只允许执行计算相关指令的队列族和只允许执行内存传输相关指令的队列族。

我们添加了一个叫做`findQueueFamilies`的函数 这一函数会查找出满足我们需求的队列族。
