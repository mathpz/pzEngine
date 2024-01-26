#pragma once

#include "pzpch.hpp"

#include "pzPipeline.hpp"
#include "pzDevice.hpp"
#include "pzGameObject.hpp"
#include "pzCamera.hpp"
#include "pzFrameInfo.hpp"

namespace pz
{
    class SimpleRenderSystem
    {
        public:
            SimpleRenderSystem(PzDevice& pzDevice, VkRenderPass renderPass, VkDescriptorSetLayout globalSetLayout);
            ~SimpleRenderSystem();

            SimpleRenderSystem(const SimpleRenderSystem&) = delete;
            SimpleRenderSystem &operator=(const SimpleRenderSystem&) = delete;

            void renderGameObjects(FrameInfo &frameInfo);

        private:
            void createPipelineLayout(VkDescriptorSetLayout globalSetLayout);
            void createPipeline(VkRenderPass renderPass);

            PzDevice& pzDevice;
            std::unique_ptr<PzPipeline> pzPipeline;
            VkPipelineLayout pipelineLayout;
    };

} // namespace pz