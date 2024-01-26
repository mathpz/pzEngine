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
            SimpleRenderSystem(PzDevice& pzDevice, VkRenderPass renderPass);
            ~SimpleRenderSystem();

            SimpleRenderSystem(const SimpleRenderSystem&) = delete;
            SimpleRenderSystem &operator=(const SimpleRenderSystem&) = delete;

            void renderGameObjects(FrameInfo &frameInfo, std::vector<PzGameObject>& gameObjects);

        private:
            void createPipelineLayout();
            void createPipeline(VkRenderPass renderPass);

            PzDevice& pzDevice;
            std::unique_ptr<PzPipeline> pzPipeline;
            VkPipelineLayout pipelineLayout;
    };

} // namespace pz