// precompiled headers
#include "pzpch.hpp"

#include "Application.hpp"

// pzEngine
#include "Core/systems/simpleRenderSystem.hpp"
#include "Core/systems/pointLightSystem.hpp"
#include "Core/pzCamera.hpp"
#include "Core/IO/keyboard_controller.hpp"
#include "Core/pzInput.hpp"

// GLM
#define  GLM_FORCE_RADIANS
#define  GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

// imgui
#include "imgui.h"

namespace pz
{
#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

    // singleton
    Application* Application::s_Instance = nullptr;

    Application::Application()
    {
        PZ_CORE_ASSERT(!s_Instance, "Application already exists!");
        s_Instance = this;

        globalPool = PzDescriptorPool::Builder(pzDevice)
            .setMaxSets(PzSwapChain::MAX_FRAMES_IN_FLIGHT)
			.addPoolSize(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, PzSwapChain::MAX_FRAMES_IN_FLIGHT)
			.build();

        pzWindow.SetEventCallback(BIND_EVENT_FN(OnEvent));

        for (int i = 0; i < uboBuffers.size(); i++)
        {
            uboBuffers[i] = std::make_unique<PzBuffer>(
                pzDevice,
                sizeof(GlobalUbo),
                1,
                VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
                VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);
            uboBuffers[i]->map();
        }

        globalSetLayout = PzDescriptorSetLayout::Builder(pzDevice)
            .addBinding(0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_ALL_GRAPHICS, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER)
            .build();

        for (int i = 0; i < globlaDescriptorSets.size(); i++)
        {
            auto bufferInfo = uboBuffers[i]->descriptorInfo();
            PzDescriptorWriter(*globalSetLayout, *globalPool)
                .writeBuffer(0, &bufferInfo)
                .build(globlaDescriptorSets[i]);
        }

        m_ImGuiLayer = new ImGuiLayer();
        PushOverlay(m_ImGuiLayer);

        loadGameObjects();
    }

    Application::~Application()
    {
    }

    void Application::OnEvent(Event& e)
    {
        EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClose));

        for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();)
        {
            (*--it)->onEvent(e);
            if (e.Handled)
            {
                break;
            }
        }
    }

    void Application::Run() {


        SimpleRenderSystem simpleRenderSystem{ pzDevice, pzRenderer.getSwapChainRenderPass(), globalSetLayout->getDescriptorSetLayout()};
        PointLightSystem pointLightSystem{ pzDevice, pzRenderer.getSwapChainRenderPass(), globalSetLayout->getDescriptorSetLayout() };
        PzCamera camera{};

        auto viewerObject = PzGameObject::createGameObject();
        viewerObject.transform.translation.z = -2.5f;
        KeyboardMovementController cameraController{};

        auto currentTime = std::chrono::high_resolution_clock::now();


        while (m_Running)
        {
            for (Layer* layer : m_LayerStack)
            {
                layer->onUpdate();
            }


            pzWindow.onUpdate();

            auto newTime = std::chrono::high_resolution_clock::now();
            float frameTime = std::chrono::duration<float, std::chrono::seconds::period>(newTime - currentTime).count();
            currentTime = newTime;

            // frameTime = glm::min(frameTime, MAX_FRAME_TIME);

            cameraController.moveInPlaneXZ(pzWindow.getGLFWwindow(), frameTime, viewerObject);
            camera.setViewYXZ(viewerObject.transform.translation, viewerObject.transform.rotation);


            float aspectRatio = pzRenderer.getAspectRatio();

            camera.setPerspectiveProjection(glm::radians(50.f), aspectRatio, 0.1f, 100.f);

            if (auto commandBuffer = pzRenderer.beginFrame())
            {
                int frameIndex = pzRenderer.getFrameIndex();
                FrameInfo frameInfo{ frameIndex, frameTime, commandBuffer, camera, globlaDescriptorSets[frameIndex], gameObjects};


                // update
                GlobalUbo ubo{};
                ubo.projection = camera.getProjectionMatrix();
                ubo.view = camera.getView();
                ubo.inverseView = camera.getInverseView();
                pointLightSystem.update(frameInfo, ubo);
                uboBuffers[frameIndex]->writeToBuffer(&ubo);
                uboBuffers[frameIndex]->flush();

                // render
                pzRenderer.beginSwapChainRenderPass(commandBuffer);

                // order here matters
                simpleRenderSystem.renderGameObjects(frameInfo);
                pointLightSystem.render(frameInfo);

                m_ImGuiLayer->Begin();
                for (Layer* layer : m_LayerStack)
                {
                    layer->onImGuiRender();
                }
                m_ImGuiLayer->End();

                pzRenderer.endSwapChainRenderPass(commandBuffer);
                pzRenderer.endFrame();

            }


        }

        vkDeviceWaitIdle(pzDevice.device());
    }


    void Application::loadGameObjects() {
        std::shared_ptr<PzModel> model = PzModel::createModelFromFile(pzDevice, "E:\\programmingProjects\\pzEngine\\pzEngine-Core\\Assets\\Models\\flat_vase.obj");

        auto flatVase = PzGameObject::createGameObject();
        flatVase.model = model;
        flatVase.transform.translation = { -.5f, 0.5f, 0.f };
        flatVase.transform.scale = { 3.f, 1.5f, 3.f };
        gameObjects.emplace(flatVase.getId(), std::move(flatVase));


        model = PzModel::createModelFromFile(pzDevice, "E:\\programmingProjects\\pzEngine\\pzEngine-Core\\Assets\\Models\\among_us.obj");
        auto smoothVase = PzGameObject::createGameObject();
        smoothVase.model = model;
        smoothVase.transform.translation = { .5f, 0.5f, 0.f };
        smoothVase.transform.rotation = { 0.f, glm::pi<float>(), glm::pi<float>() };
        smoothVase.transform.scale = { 0.005f,0.005f, 0.005f };
        gameObjects.emplace(smoothVase.getId(), std::move(smoothVase));

        model = PzModel::createModelFromFile(pzDevice, "E:\\programmingProjects\\pzEngine\\pzEngine-Core\\Assets\\Models\\quad.obj");
        auto floor = PzGameObject::createGameObject();
        floor.model = model;
        floor.transform.translation = { 0.f, 0.5f, 0.f };
        floor.transform.scale = { 3.f, 1.f, 3.f };
        gameObjects.emplace(floor.getId(), std::move(floor));

        {
  //          auto pointLight = PzGameObject::makePointLight(0.2f);
  //          gameObjects.emplace(pointLight.getId(), std::move(pointLight));
        }

        std::vector<glm::vec3> lightColors{
            {1.f, .1f, .1f},
            {.1f, .1f, 1.f},
            {.1f, 1.f, .1f},
            {1.f, 1.f, .1f},
            {.1f, 1.f, 1.f},
            {1.f, 1.f, 1.f}  //
        };

        for (int i = 0; i < lightColors.size(); i++)
        {
            auto pointLight = PzGameObject::makePointLight(0.2f);
            pointLight.color = lightColors[i];
            auto rotateLight = glm::rotate(
                glm::mat4(1.f),
                (i * glm::two_pi<float>()) / lightColors.size(),
				{0.f, -11.f, 0.f});

            pointLight.transform.translation = glm::vec3(rotateLight * glm::vec4(-1.0f, -1.f, -1.f, 1.f));
            gameObjects.emplace(pointLight.getId(), std::move(pointLight));
        }
    }


    void Application::PushLayer(Layer* layer)
    {
		m_LayerStack.pushLayer(layer);

	}

    void Application::PushOverlay(Layer* overlay)
    {
		m_LayerStack.pushOverlay(overlay);

	}

    bool Application::OnWindowClose(WindowCloseEvent& e)
    {
        m_Running = false;
        return true;
    }
} // namespace pz