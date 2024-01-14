#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>


namespace pz
{
    class PzWindow
    {
        public:
            PzWindow(int width, int height, std::string name);
            ~PzWindow();

            PzWindow(const PzWindow&) = delete;
            PzWindow &operator=(const PzWindow&) = delete;

            bool shouldClose() { return glfwWindowShouldClose(window); }
            VkExtent2D getExtent() { return {static_cast<uint32_t>(windowWidth), static_cast<uint32_t>(windowHeight)}; }
            bool wasWindowResized() { return frameBufferResized; }
            void resetWindowResizedFlag() { frameBufferResized = false; }

            void createWindowSurface(VkInstance instance, VkSurfaceKHR* surface);
        private:
            static void framebufferResizeCallback(GLFWwindow* window, int width, int height);
             GLFWwindow* window;
             std::string windowName;

             int windowWidth;
             int windowHeight;
             bool frameBufferResized = false;
    
        void initWindow();

    };
} // namespace pz