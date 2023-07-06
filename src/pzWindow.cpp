#include "pzWindow.hpp"

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

    void PzWindow::initWindow()
    {
        glfwInit();

        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

        window = glfwCreateWindow(windowWidth, windowHeight, windowName.c_str(), nullptr, nullptr);
    }
} // namespace pz
