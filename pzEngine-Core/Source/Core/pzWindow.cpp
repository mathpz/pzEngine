#include "pzWindow.hpp"

// pre compiled header
#include "pzpch.hpp"

namespace pz
{
    PzWindow::PzWindow(int width, int height, std::string name)
        : windowWidth(width), windowHeight(height), windowName(name)
    {
        initWindow();
    }

    PzWindow::~PzWindow()
    {
        glfwDestroyWindow(window);
        glfwTerminate();
    }

    void PzWindow::createWindowSurface(VkInstance instance, VkSurfaceKHR *surface)
    {
        if (glfwCreateWindowSurface(instance, window, nullptr, surface) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to create window surface!");
        }
    }

    void PzWindow::initWindow()
    {
        glfwInit();

        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

        window = glfwCreateWindow(windowWidth, windowHeight, windowName.c_str(), nullptr, nullptr);
        glfwSetWindowUserPointer(window, this);
        glfwSetFramebufferSizeCallback(window, framebufferResizeCallback);
    }

    void PzWindow::framebufferResizeCallback(GLFWwindow* window, int width, int height)
    {
        auto pzWindow = reinterpret_cast<PzWindow*>(glfwGetWindowUserPointer(window));
        pzWindow->frameBufferResized = true;
        pzWindow->windowWidth = width;
        pzWindow->windowHeight = height;
    }
} // namespace pz
