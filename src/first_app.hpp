#pragma once

#include "pzWindow.hpp"
#include "pzPipeline.hpp"
#include "pzDevice.hpp"
#include "pzSwapChain.hpp"
#include "pzModel.hpp"

// std
#include <memory>
#include <vector>

namespace pz
{
    class FirstApp
    {
        public:
            FirstApp();
            ~FirstApp();
            static constexpr int WIDTH = 800;
            static constexpr int HEIGHT = 600;

            FirstApp(const FirstApp&) = delete;
            FirstApp &operator=(const FirstApp&) = delete;

            void run();

        private:
            void loadModels();
            void createPipelineLayout();
            void createPipeline();
            void createCommandBuffers();
            void freeCommandBuffers();
            void drawFrame();
            void recreateSwapChain();
            void recordCommandBuffer(int imageIndex);

            PzWindow pzWindow{WIDTH, HEIGHT, "pzEngine"};
            PzDevice pzDevice{pzWindow};
            std::unique_ptr<PzSwapChain> pzSwapChain;
            std::unique_ptr<PzPipeline> pzPipeline;
            VkPipelineLayout pipelineLayout;
            std::vector<VkCommandBuffer> commandBuffers;
            std::unique_ptr<PzModel> pzModel;

    };

} // namespace pz