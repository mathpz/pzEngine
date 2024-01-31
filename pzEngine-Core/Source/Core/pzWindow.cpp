// pre compiled header
#include "pzpch.hpp"

#include "pzWindow.hpp"

#include "Core/Events/ApplicationEvent.hpp"
#include "Core/Events/MouseEvent.hpp"
#include "Core/Events/KeyEvent.hpp"


namespace pz
{

    static bool s_GLFWInitialized = false;

    static void GLFWErrorCallback(int error, const char* description)
    {
		PZ_CORE_ERROR("GLFW Error ({0}): {1}", error, description);
	}

    PzWindow::PzWindow(const WindowProperties& props)
    {
        initWindow(props);
    }

    PzWindow::~PzWindow()
    {
        Shutdown();
    }

    void PzWindow::createWindowSurface(VkInstance instance, VkSurfaceKHR *surface)
    {
        if (glfwCreateWindowSurface(instance, m_Window, nullptr, surface) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to create window surface!");
        }
    }

    void PzWindow::initWindow(const WindowProperties& props)
    {
        m_Data.title = props.title;
        m_Data.width = props.width;
        m_Data.height = props.height;

        PZ_CORE_INFO("Creating window {0} ({1},{2})", props.title, props.width, props.height);

        if (!s_GLFWInitialized)
        {
            int success = glfwInit();
            glfwInit();
            PZ_CORE_ASSERT(success, "Could not initialize GLFW!");

            glfwSetErrorCallback(GLFWErrorCallback);
            s_GLFWInitialized = true;
        }

        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

        m_Window = glfwCreateWindow((int)props.width, (int)props.height, props.title.c_str(), nullptr, nullptr);
        glfwSetWindowUserPointer(m_Window, &m_Data);
        glfwSetFramebufferSizeCallback(m_Window, framebufferResizeCallback);

        // Set GLFW callbacks

        glfwSetWindowCloseCallback(m_Window, [](GLFWwindow * window)
        {
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
			WindowCloseEvent event;
			data.eventCallback(event);
		});

        glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
        {
            WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

            switch (action)
            {
                case GLFW_PRESS:
                {
                    KeyPressedEvent event(key, 0);
                    data.eventCallback(event);
                    break;
                }
                case GLFW_RELEASE:
                {
                    KeyReleasedEvent event(key);
                    data.eventCallback(event);
                    break;
                }
                case GLFW_REPEAT:
                {
                    KeyPressedEvent event(key, 1);
                    data.eventCallback(event);
                    break;
                }
            }
        });

        glfwSetCharCallback(m_Window, [](GLFWwindow* window, unsigned int keycode)
        {
            WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

            KeyTypedEvent event(keycode);
            data.eventCallback(event);
        });

        glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double xOffset, double yOffset)
        {
            WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

            MouseScrolledEvent event((float)xOffset, (float)yOffset);
            data.eventCallback(event);
        });

        glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double xPos, double yPos)
        {
            WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

            MouseMovedEvent event((float)xPos, (float)yPos);
            data.eventCallback(event);
        });

    }

    void PzWindow::onUpdate()
    {
        glfwPollEvents();
    }

    void PzWindow::framebufferResizeCallback(GLFWwindow* window, int width, int height)
    {
        // ! TODO verify properly this function

        auto pzWindow = reinterpret_cast<PzWindow*>(glfwGetWindowUserPointer(window));
        pzWindow->frameBufferResized = true;
        //pzWindow->m_Data.width = width;
        // pzWindow->m_Data.height = height;

        WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
        data.width = width;
        data.height = height;
        WindowResizeEvent event(width, height);
        data.eventCallback(event);
    }

    void PzWindow::Shutdown()
    {
        glfwDestroyWindow(m_Window);
		glfwTerminate();
    }

    void PzWindow::SetVSync(bool enabled)
    {
        // not implemented yet
        m_Data.VSync = enabled;
	}

    bool PzWindow::IsVSync() const
    {
		return m_Data.VSync;
	}

    bool PzWindow::shouldClose()
    {
		return glfwWindowShouldClose(m_Window);
	}
} // namespace pz
