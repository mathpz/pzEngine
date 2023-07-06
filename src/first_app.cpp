#include "first_app.hpp"

namespace pz
{
    FirstApp::FirstApp()
    {
    }

    FirstApp::~FirstApp()
    {
    }

    void pz::FirstApp::run()
    {
        while(!pzWindow.shouldClose())
        {
            glfwPollEvents();
        }
    }
} // namespace pz
