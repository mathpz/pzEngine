#pragma once

// pre compiled header
#include "pzpch.hpp"

#include "pzWindow.hpp"
#include "pzDevice.hpp"
#include "pzSwapChain.hpp"

namespace pz
{
    class PzRenderer
    {
        public:
            PzRenderer(PzWindow& pzWindow, PzDevice& device);
            ~PzRenderer();

            bool isFrameInProgress() const { return m_isFrameStarted; }

            void Draw();
        private:

            PzWindow& pzWindow;
            PzDevice& m_Device;
            PzSwapChain m_Swapchain;
            std::vector<VkCommandBuffer> commandBuffers;

            bool m_isFrameStarted = false;
    };

} // namespace pz