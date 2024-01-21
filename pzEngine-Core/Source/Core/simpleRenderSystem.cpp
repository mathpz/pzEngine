// pre compiled headers
#include "pzpch.hpp"

#include "simpleRenderSystem.hpp"


// libs
#define  GLM_FORCE_RADIANS
#define  GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

namespace pz
{

    struct SimplePushConstantsData
    {
        glm::mat4 transform{1.f};
        glm::quat rotation{ 1.0f, 0.0f, 0.0f, 0.0f };  // Quaternion for rotation
        alignas(16) glm::vec3 color;
    };

    SimpleRenderSystem::SimpleRenderSystem(PzDevice& pzDevice, VkRenderPass renderPass)
        : pzDevice{pzDevice}
    {
        createPipelineLayout();
        createPipeline(renderPass);
    }

    SimpleRenderSystem::~SimpleRenderSystem() { vkDestroyPipelineLayout(pzDevice.device(), pipelineLayout, nullptr); }

    void SimpleRenderSystem::createPipelineLayout()
    {
        VkPushConstantRange pushConstantRange{};
        pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
        pushConstantRange.offset = 0;
        pushConstantRange.size = sizeof(SimplePushConstantsData);

        VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
        pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        pipelineLayoutInfo.setLayoutCount = 0;
        pipelineLayoutInfo.pSetLayouts = nullptr;
        pipelineLayoutInfo.pushConstantRangeCount = 1;
        pipelineLayoutInfo.pPushConstantRanges = &pushConstantRange;
        if (vkCreatePipelineLayout(pzDevice.device(), &pipelineLayoutInfo, nullptr, &pipelineLayout) !=
            VK_SUCCESS)
        {
            throw std::runtime_error("failed to create pipeline layout!");
        }
    }

    void SimpleRenderSystem::createPipeline(VkRenderPass renderPass)
    {
        assert(pipelineLayout != nullptr && "Cannot create pipeline before pipeline layout");

        PipelineConfigInfo pipelineConfig{};
        PzPipeline::defaultPipelineConfigInfo(pipelineConfig);
        pipelineConfig.renderPass = renderPass;
        pipelineConfig.pipelineLayout = pipelineLayout;
        pzPipeline = std::make_unique<PzPipeline>(pzDevice, "F:/programmingProjects/pzEngine/pzEngine-Core/Shaders/simple_shader.vert.spv", "F:/programmingProjects/pzEngine/pzEngine-Core/Shaders/simple_shader.frag.spv", pipelineConfig);
    }   // TODO : change absolute path to relative

    void SimpleRenderSystem::renderGameObjects(VkCommandBuffer commandBuffer, std::vector<PzGameObject>& gameObjects, const PzCamera& camera)
    {
        pzPipeline->bind(commandBuffer);

        for (auto &obj : gameObjects)
        {
            obj.transform.rotation = glm::rotate(obj.transform.rotation, 0.01f, glm::vec3(0.0f, 1.0f, 0.0f));
            obj.transform.rotation = glm::rotate(obj.transform.rotation, 0.005f, glm::vec3(1.0f, 0.0f, 0.0f));
            
            SimplePushConstantsData push{};
            push.color = obj.color;
            push.transform = camera.getProjectionMatrix() * obj.transform.mat4();   // Temporary, send both to vertex shaders

            vkCmdPushConstants(commandBuffer, pipelineLayout, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, 0, sizeof(SimplePushConstantsData), &push);
            obj.model->bind(commandBuffer);
            obj.model->draw(commandBuffer);
        }
    }

} // namespace pz