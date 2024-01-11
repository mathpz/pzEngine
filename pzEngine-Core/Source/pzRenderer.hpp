#pragma once

#include "pzWindow.hpp"
#include "pzDevice.hpp"
#include "pzSwapChain.hpp"

// std
#include <memory>
#include <vector>
#include <cassert>

namespace pz
{
    class PzRenderer
    {
        public:
            PzRenderer(PzWindow& pzWindow, PzDevice& pzDevice);
            ~PzRenderer();

            PzRenderer(const PzRenderer&) = delete;
            PzRenderer &operator=(const PzRenderer&) = delete;

            VkRenderPass getSwapChainRenderPass() const { return pzSwapChain->getRenderPass(); }
            bool isFrameInProgress() const { return m_isFrameStarted; }

            VkCommandBuffer getCurrentCommandBuffer() const 
            { 
                assert(m_isFrameStarted && "Cannot get command buffer when frame not in progress");
                return commandBuffers[m_currentFrameIndex]; 
            }

            int getFrameIndex() const 
            { 
                assert(m_isFrameStarted && "Cannot get frame index when frame not in progress");    
                return m_currentFrameIndex; 
            }

            VkCommandBuffer beginFrame();
            void endFrame();
            void beginSwapChainRenderPass(VkCommandBuffer commandBuffer);
            void endSwapChainRenderPass(VkCommandBuffer commandBuffer);

        private:
            void createCommandBuffers();
            void freeCommandBuffers();
            void recreateSwapChain();

            PzWindow& pzWindow;
            PzDevice& pzDevice;
            std::unique_ptr<PzSwapChain> pzSwapChain;
            std::vector<VkCommandBuffer> commandBuffers;

            uint32_t m_currentImageIndex;
            int m_currentFrameIndex = 0;
            bool m_isFrameStarted = false;
    };

} // namespace pz