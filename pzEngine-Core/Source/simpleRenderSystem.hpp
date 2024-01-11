#pragma once

#include "pzPipeline.hpp"
#include "pzDevice.hpp"
#include "pzGameObject.hpp"

// std
#include <memory>
#include <vector>

namespace pz
{
    class SimpleRenderSystem
    {
        public:
            SimpleRenderSystem(PzDevice& pzDevice, VkRenderPass renderPass);
            ~SimpleRenderSystem();

            SimpleRenderSystem(const SimpleRenderSystem&) = delete;
            SimpleRenderSystem &operator=(const SimpleRenderSystem&) = delete;

            void renderGameObjects(VkCommandBuffer commandBuffer, std::vector<PzGameObject>& gameObjects);

        private:
            void createPipelineLayout();
            void createPipeline(VkRenderPass renderPass);

            PzDevice& pzDevice;
            std::unique_ptr<PzPipeline> pzPipeline;
            VkPipelineLayout pipelineLayout;
    };

} // namespace pz