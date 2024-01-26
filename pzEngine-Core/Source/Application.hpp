#pragma once

// pzEngine
#include "Core/Core.hpp"
#include "Core/Events/Event.hpp"

#include "Core/pzWindow.hpp"
#include "Core/pzDevice.hpp"
#include "Core/pzGameObject.hpp"
#include "Core/pzRenderer.hpp"
#include "Core/pzUniformBuffer.hpp"

namespace pz
{
    struct GlobalUbo
    {
        glm::mat4 projectionView{1.0f};
        glm::vec3 lightDirection = glm::normalize( glm::vec3{1.0f, -3.0f, -1.0f} );
	};

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
        std::vector<PzGameObject> gameObjects;

    };
    
    // To be defined in CLIENT
    Application* createApplication();

} // namespace pz