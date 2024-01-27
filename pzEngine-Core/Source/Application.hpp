#pragma once

// pzEngine
#include "Core/Core.hpp"
#include "Core/LayerStack.hpp"
#include "Core/Events/Event.hpp"
#include "Core/Events/ApplicationEvent.hpp"

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

        void PushLayer(Layer* layer);
        void PushOverlay(Layer* overlay);

    private:
        void loadGameObjects();

        PzWindow pzWindow{WIDTH, HEIGHT, "pzEngine"};
        PzDevice pzDevice{pzWindow};
        PzRenderer pzRenderer{pzWindow, pzDevice};

        // note: order of declarations matters
        std::unique_ptr<PzDescriptorPool> globalPool{};
        PzGameObject::Map gameObjects;

        LayerStack m_LayerStack;
    };

    // To be defined in CLIENT
    Application* createApplication();

} // namespace pz