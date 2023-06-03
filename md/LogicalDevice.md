选择物理设备后 们还需要一个逻辑设备来作为和物理设备交互的
接口。逻辑设备的创建过程类似于我们之前描述的`Vulkan`实例的创建过程。

Vulkan 需要我们赋予队列一个戰戮戰到戱戮戰之间的浮点数作为优先级来控制
指令缓冲的执行顺序。即使只有一个队列，我们也要显式地赋予队列优先
级

vkGetDeviceQueue函数可以获取指定队列族的队列句柄
```c++
VkQueue graphicsQueue;
///////////////////////////
vkGetDeviceQueue(device,indices.graphicFamily,0,&graphi0csQueue);
```

