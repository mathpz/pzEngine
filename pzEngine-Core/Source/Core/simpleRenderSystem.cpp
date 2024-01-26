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
        glm::mat4 normalMatrix{1.f};
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

    void SimpleRenderSystem::renderGameObjects(FrameInfo &frameInfo, std::vector<PzGameObject>& gameObjects)
    {
        pzPipeline->bind(frameInfo.commandBuffer);
        auto projectionView = frameInfo.camera.getProjectionMatrix() * frameInfo.camera.getView();


        for (auto &obj : gameObjects)
        {
            SimplePushConstantsData push{};
            auto modelMatrix = obj.transform.mat4();
            
            push.transform = projectionView * modelMatrix;   // Temporary, send both to vertex shaders
            push.normalMatrix = obj.transform.normalMatrix();


            vkCmdPushConstants(frameInfo.commandBuffer, pipelineLayout, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, 0, sizeof(SimplePushConstantsData), &push);
            obj.model->bind(frameInfo.commandBuffer);
            obj.model->draw(frameInfo.commandBuffer);
        }
    }

} // namespace pz