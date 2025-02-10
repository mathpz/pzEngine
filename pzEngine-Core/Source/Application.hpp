#pragma once

// pzEngine
#include "Core/Core.hpp"
#include "Core/LayerStack.hpp"
#include "Core/Events/Event.hpp"
#include "Core/Events/ApplicationEvent.hpp"

#include "Core/pzWindow.hpp"
#include "Core/Renderer/Vulkan/GFXDevice.hpp"
// #include "Core/pzGameObject.hpp"
#include "Core/pzRenderer.hpp"
// #include "Core/pzUniformBuffer.hpp"
// #include "Core/pzDescriptors.hpp"
#include "Core/ImGui/ImGuiLayer.hpp"

namespace pz
{
    class PZ_API Application
    {
    public:
        Application();
        virtual ~Application();

        Application(const Application&) = delete;
        Application &operator=(const Application&) = delete;

        void Run();

        void OnEvent(Event& e);

        void PushLayer(Layer* layer);
        void PushOverlay(Layer* overlay);

        inline static Application& Get() { return *s_Instance; }
        inline pz::PzWindow& GetWindow() { return pzWindow; }
        // inline pz::PzDevice& GetDevice() { return pzDevice; }
        // inline pz::PzDescriptorPool& GetDescriptorPool() { return *globalPool; }
        inline pz::PzRenderer& GetRenderer() { return pzRenderer; }
        // inline PzGameObject::Map& GetGameObjects() { return gameObjects; }

    private:
        void Init();
        void ShutDown();

        // void loadGameObjects();
        bool OnWindowClose(WindowCloseEvent& e);

        pz::WindowProperties m_WindowProperties {};
        PzWindow pzWindow{m_WindowProperties};
        GFXDevice pzDevice{pzWindow};
        PzRenderer pzRenderer{pzWindow, pzDevice};
        // note: order of declarations matters

        // std::unique_ptr<PzDescriptorPool> globalPool{};
        //
        // std::vector<std::unique_ptr<PzBuffer>> uboBuffers{ PzSwapChain::MAX_FRAMES_IN_FLIGHT };
        // std::unique_ptr<PzDescriptorSetLayout> globalSetLayout{};
        // std::vector<VkDescriptorSet> globlaDescriptorSets{ PzSwapChain::MAX_FRAMES_IN_FLIGHT };

        // PzGameObject::Map gameObjects;

        LayerStack m_LayerStack;
        ImGuiLayer* m_ImGuiLayer;
        bool m_Running = true;

    private:
        static Application* s_Instance;
    };

    // To be defined in CLIENT
    Application* createApplication();

} // namespace pz