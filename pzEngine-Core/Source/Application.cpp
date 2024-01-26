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
    struct GlobalUbo
    {
        glm::mat4 projectionView{ 1.0f };
        glm::vec3 lightDirection = glm::normalize(glm::vec3{ 1.0f, -3.0f, -1.0f });
    };

    Application::Application()
    {
        globalPool = PzDescriptorPool::Builder(pzDevice)
            .setMaxSets(PzSwapChain::MAX_FRAMES_IN_FLIGHT)
			.addPoolSize(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, PzSwapChain::MAX_FRAMES_IN_FLIGHT)
			.build();
        loadGameObjects();
    }

    Application::~Application()
    {
    }

    void Application::Run() {
        std::vector<std::unique_ptr<PzBuffer>> uboBuffers{ PzSwapChain::MAX_FRAMES_IN_FLIGHT };
        for (int i = 0; i < uboBuffers.size(); i++)
        {
            uboBuffers[i] = std::make_unique<PzBuffer>(
                pzDevice,
                sizeof(GlobalUbo),
                1,
                VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
                VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT );
            uboBuffers[i]->map();
        }

        auto globalSetLayout = PzDescriptorSetLayout::Builder(pzDevice)
			.addBinding(0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_VERTEX_BIT)
			.build();

        std::vector<VkDescriptorSet> globlaDescriptorSets{ PzSwapChain::MAX_FRAMES_IN_FLIGHT };
        for (int i = 0; i < globlaDescriptorSets.size(); i++)
        {
            auto bufferInfo = uboBuffers[i]->descriptorInfo();
            PzDescriptorWriter(*globalSetLayout, *globalPool)
                .writeBuffer(0, &bufferInfo)
                .build(globlaDescriptorSets[i]);
        }

        SimpleRenderSystem simpleRenderSystem{ pzDevice, pzRenderer.getSwapChainRenderPass(), globalSetLayout->getDescriptorSetLayout()};
        PzCamera camera{};

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
                int frameIndex = pzRenderer.getFrameIndex();
                FrameInfo frameInfo{ frameIndex, frameTime, commandBuffer, camera, globlaDescriptorSets[frameIndex]};

                // update
                GlobalUbo ubo{};
                ubo.projectionView = camera.getProjectionMatrix() * camera.getView();
                uboBuffers[frameIndex]->writeToBuffer(&ubo);
                uboBuffers[frameIndex]->flush();

                // render
                pzRenderer.beginSwapChainRenderPass(commandBuffer);
                simpleRenderSystem.renderGameObjects(frameInfo, gameObjects);
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