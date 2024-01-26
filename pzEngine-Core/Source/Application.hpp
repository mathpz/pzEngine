#pragma once

// pzEngine
#include "Core/Core.hpp"
#include "Core/Events/Event.hpp"

#include "Core/pzWindow.hpp"
#include "Core/pzDevice.hpp"
#include "Core/pzGameObject.hpp"
#include "Core/pzRenderer.hpp"
#include "Core/pzUniformBuffer.hpp"
#include "Core/pzDescriptors.hpp"

namespace pz
{

    class PZ_API Application
    {
    public:
        Application();
        virtual ~Application();

        static constexpr int WIDTH = 800;
        static constexpr int HEIGHT = 600;
     
        Application(const Application&) = delete;
        Application &operator=(const Application&) = delete;

        void Run();

    private:
        void loadGameObjects();
     
        PzWindow pzWindow{WIDTH, HEIGHT, "pzEngine"};
        PzDevice pzDevice{pzWindow};
        PzRenderer pzRenderer{pzWindow, pzDevice};

        // note: order of declarations matters
        std::unique_ptr<PzDescriptorPool> globalPool{};
        std::vector<PzGameObject> gameObjects;

    };
    
    // To be defined in CLIENT
    Application* createApplication();

} // namespace pz