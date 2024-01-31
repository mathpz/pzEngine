#include "pzpch.hpp"

#include "pzInput.hpp"
#include "Application.hpp"

#include <GLFW/glfw3.h>

namespace pz
{
    Input* Input::s_Instance = new Input();

    bool pz::Input::isKeyPressedImpl(int keycode)
    {
        auto window = Application::Get().GetWindow().getGLFWwindow();
        auto state = glfwGetKey(window, keycode);

        return state == GLFW_PRESS || state == GLFW_REPEAT;
    }
    bool Input::isMouseButtonPressedImpl(int button)
    {
        auto window = Application::Get().GetWindow().getGLFWwindow();
        auto state = glfwGetMouseButton(window, button);

        return state == GLFW_PRESS;
    }
    std::pair<float, float> Input::getMousePositionImpl()
    {
        auto window = Application::Get().GetWindow().getGLFWwindow();
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);

        return { (float)xpos, (float)ypos };
    }
    float Input::getMouseXImpl()
    {
        auto[x, y] = getMousePositionImpl();

        return (float)x;
    }
    float Input::getMouseYImpl()
    {
        auto [x, y] = getMousePositionImpl();

        return (float)y;
    }
} // namespace pz
