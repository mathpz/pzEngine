#pragma once

#include "pzpch.hpp"

#include "Core/pzPipeline.hpp"
#include "Core/pzDevice.hpp"
#include "Core/pzGameObject.hpp"
#include "Core/pzCamera.hpp"
#include "Core/pzFrameInfo.hpp"

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