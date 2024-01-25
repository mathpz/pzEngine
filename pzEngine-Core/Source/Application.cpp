// precompiled headers
#include "pzpch.hpp"

#include "Application.hpp"

// pzEngine
#include "Core/simpleRenderSystem.hpp"
#include "Core/pzCamera.hpp"
#include "Core/IO/keyboard_controller.hpp"

// GLM
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

    void Application::Run() {
        SimpleRenderSystem simpleRenderSystem{ pzDevice, pzRenderer.getSwapChainRenderPass() };
        PzCamera camera{};
        // camera.setViewDirection(glm::vec3(0.f), glm::vec3(0.5f, 0.f, 1.f));
        //camera.setViewTarget(glm::vec3(-1.f, -2.f, 2.f), glm::vec3(0.f, 0.f, 2.5f));

        auto viewerObject = PzGameObject::createGameObject();
        KeyboardMovementController cameraController{};

        auto currentTime = std::chrono::high_resolution_clock::now();

        while (!pzWindow.shouldClose()) {
            glfwPollEvents();

            auto newTime = std::chrono::high_resolution_clock::now();
            float frameTime = std::chrono::duration<float, std::chrono::seconds::period>(newTime - currentTime).count();
            currentTime = newTime;

            // frameTime = glm::min(frameTime, MAX_FRAME_TIME);

            cameraController.moveInPlaneXZ(pzWindow.getGLFWwindow(), frameTime, viewerObject);
            camera.setViewYXZ(viewerObject.transform.translation, viewerObject.transform.rotation);


            float aspectRatio = pzRenderer.getAspectRatio();

            camera.setPerspectiveProjection(glm::radians(50.f), aspectRatio, 0.1f, 10.f);

            if (auto commandBuffer = pzRenderer.beginFrame()) {
                pzRenderer.beginSwapChainRenderPass(commandBuffer);
                simpleRenderSystem.renderGameObjects(commandBuffer, gameObjects, camera);
                pzRenderer.endSwapChainRenderPass(commandBuffer);
                pzRenderer.endFrame();
            }
        }

        vkDeviceWaitIdle(pzDevice.device());
    }


    void Application::loadGameObjects() {
        std::shared_ptr<PzModel> model = PzModel::createModelFromFile(pzDevice, "F:\\programmingProjects\\pzEngine\\pzEngine-Core\\models\\smooth_vase.obj");

        auto gameObject = PzGameObject::createGameObject();
        gameObject.model = model;
        gameObject.transform.translation = { 0.0f, 0.0f, 2.5f };
        gameObject.transform.scale = glm::vec3(3.f);

        gameObjects.push_back(std::move(gameObject));
    }

} // namespace pz