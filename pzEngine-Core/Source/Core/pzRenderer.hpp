#pragma once

// pre compiled header
#include "pzpch.hpp"

#include "pzWindow.hpp"
#include "Core/Renderer/Vulkan/GFXDevice.hpp"
#include "Core/Renderer/Vulkan/GPUImage.hpp"

namespace pz
{
    class PzRenderer
    {
        public:
            PzRenderer(PzWindow& pzWindow, GFXDevice& device);
            ~PzRenderer();

            bool isFrameInProgress() const { return m_isFrameStarted; }

            void Draw();
    private:

            PzWindow& pzWindow;
            GFXDevice& m_Device;
            std::vector<VkCommandBuffer> commandBuffers;

            bool m_isFrameStarted = false;
    };

} // namespace pz