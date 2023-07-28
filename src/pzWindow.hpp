#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <string>

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

            void createWindowSurface(VkInstance instance, VkSurfaceKHR* surface);
        private:
             GLFWwindow* window;
             std::string windowName;

             const int windowWidth;
             const int windowHeight;
    
        void initWindow();

    };
} // namespace pz