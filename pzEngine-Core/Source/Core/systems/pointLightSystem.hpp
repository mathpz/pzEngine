#pragma once

#include "pzpch.hpp"

#include "Core/pzPipeline.hpp"
#include "Core/pzDevice.hpp"
#include "Core/pzGameObject.hpp"
#include "Core/pzCamera.hpp"
#include "Core/pzFrameInfo.hpp"

namespace pz
{
    struct PointLightPushConstants
    {
		glm::vec4 position;
		glm::vec4 color;
        float radius;
	};

    class PointLightSystem
    {
        public:
            PointLightSystem(PzDevice& pzDevice, VkRenderPass renderPass, VkDescriptorSetLayout globalSetLayout);
            ~PointLightSystem();

            PointLightSystem(const PointLightSystem&) = delete;
            PointLightSystem &operator=(const PointLightSystem&) = delete;

            void update(FrameInfo &frameInfo, GlobalUbo &ubo);
            void render(FrameInfo &frameInfo);

        private:
            void createPipelineLayout(VkDescriptorSetLayout globalSetLayout);
            void createPipeline(VkRenderPass renderPass);

            PzDevice& pzDevice;
            std::unique_ptr<PzPipeline> pzPipeline;
            VkPipelineLayout pipelineLayout;
    };

} // namespace pz