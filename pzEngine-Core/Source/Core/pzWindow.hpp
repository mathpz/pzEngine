#pragma once


#include "Core/Core.hpp"
#include "Core/Events/Event.hpp"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

namespace pz
{
        struct WindowProperties
        {
			std::string title;
			unsigned int width;
			unsigned int height;

            WindowProperties(const std::string& title = "pzEngine",
                unsigned int width = 1280,
                unsigned int height = 720)
                : title(title), width(width), height(height)
            {
			}
		};

    // Interface representing a desktop system based Window
    class PzWindow
    {

        public:
            using EventCallBackFn = std::function<void(Event&)>;

            PzWindow(const WindowProperties& props);
            ~PzWindow();

            PzWindow(const PzWindow&) = delete;
            PzWindow &operator=(const PzWindow&) = delete;

            // window specific
            void onUpdate();

            bool shouldClose() { return glfwWindowShouldClose(m_Window); }
            bool wasWindowResized() const { return frameBufferResized; }
            void resetWindowResizedFlag() { frameBufferResized = false; }

            inline unsigned int GetWidth() const  { return m_Data.width; }
            inline unsigned int GetHeight() const  { return m_Data.height; }

                // Window attributes
            inline void SetEventCallback(const EventCallBackFn& callback) { m_Data.eventCallback = callback; }
            void SetVSync(bool enabled);
            bool IsVSync() const;

            // glfw specific
            GLFWwindow* getGLFWwindow() const { return m_Window; }

            // vulkan specific
            void createWindowSurface(VkInstance instance, VkSurfaceKHR* surface);
            VkExtent2D getExtent() { return {static_cast<uint32_t>(m_Data.width), static_cast<uint32_t>(m_Data.height)}; }

        private:
            static void framebufferResizeCallback(GLFWwindow* window, int width, int height);

            void initWindow(const WindowProperties& props);
            void Shutdown();

            GLFWwindow* m_Window;
            bool frameBufferResized = false;

            struct WindowData
            {
				 std::string title;
				 unsigned int width, height;
				 bool VSync;

				 EventCallBackFn eventCallback;
			};

            WindowData m_Data;


    };
} // namespace pz