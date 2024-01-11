#include "Application.hpp"
#include "simpleRenderSystem.hpp"

// libs
#define  GLM_FORCE_RADIANS
#define  GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>


namespace pz
{
    Application::Application()
    {
        loadGameObjects();
    }

    Application::~Application()
    {
    }

    void Application::Run()
    {
        SimpleRenderSystem simpleRenderSystem{pzDevice, pzRenderer.getSwapChainRenderPass()};

        while (!pzWindow.shouldClose())
        {
            glfwPollEvents();
            if(auto commandBuffer = pzRenderer.beginFrame())
            {

                pzRenderer.beginSwapChainRenderPass(commandBuffer);
                simpleRenderSystem.renderGameObjects(commandBuffer, gameObjects);
                pzRenderer.endSwapChainRenderPass(commandBuffer);
                pzRenderer.endFrame();
            }
        }

        vkDeviceWaitIdle(pzDevice.device());
    }
    
    void Application::loadGameObjects()
    {
        std::vector<PzModel::Vertex> vertices{
            {{0.0f, -0.5f}, {1.0f, 0.0f, 0.0f}},
            {{0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}},
            {{-0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}}};
        auto pzModel = std::make_shared<PzModel>(pzDevice, vertices);

        auto triangle  = PzGameObject::createGameObject();
        triangle.model = pzModel;
        triangle.color = {.1f, .8f, .1f};
        triangle.transform2d.translation.x = .2f;
        triangle.transform2d.scale = {2.f, .5f};
        triangle.transform2d.rotation = .25f * glm::two_pi<float>();

        gameObjects.push_back(std::move(triangle));
    }

} // namespace pz