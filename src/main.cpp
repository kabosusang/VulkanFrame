#include "VKimgui.h"
#include "VulkanRenderer.h"

int main(){
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit())
        return 1;

    // Create window with Vulkan context
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    GLFWwindow* window = glfwCreateWindow(1280, 720, "Dear ImGui GLFW+Vulkan example", nullptr, nullptr);
    if (!glfwVulkanSupported())
    {
        printf("GLFW: Vulkan Not Supported\n");
        return 1;
    }

    // Main loop
    //Vulkan Renderer
    HelloTriangleApplication app;

    app.InputWindow(window);
    
    //Imugi Renderer
    
    try{
        app.run();
    }catch(const std::exception&e){
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    } 
    
    glfwDestroyWindow(window);
    glfwTerminate();
    return EXIT_SUCCESS;


}
