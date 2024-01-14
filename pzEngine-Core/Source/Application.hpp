#pragma once

// pzEngine
#include "Core.hpp"
#include "Events/Event.hpp"

// #include "pzWindow.hpp"
// #include "pzDevice.hpp"
// #include "pzGameObject.hpp"
// #include "pzRenderer.hpp"

// std
// #include <memory>
// #include <vector>

namespace pz
{
    class PZ_API Application
    {
    public:
        Application();
        virtual ~Application();

    //    static constexpr int WIDTH = 800;
    //    static constexpr int HEIGHT = 600;
    //
    //    Application(const Application&) = delete;
    //    Application &operator=(const Application&) = delete;

        void Run();

    private:
    //    void loadGameObjects();

    //    PzWindow pzWindow{WIDTH, HEIGHT, "pzEngine"};
    //    PzDevice pzDevice{pzWindow};
    //    PzRenderer pzRenderer{pzWindow, pzDevice};
    //    std::vector<PzGameObject> gameObjects;

    };
    
    // To be defined in CLIENT
    Application* createApplication();

} // namespace pz