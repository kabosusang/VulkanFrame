#include "VkInstanceCreate.h"




int main(){

    HelloTriangleApplication app;

    const std::vector<const char*> validationLayers={
        "VK LAYER KHRONOS validation"
    };

    try{
        app.run();

    }
    catch(const std::exception&e){
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
