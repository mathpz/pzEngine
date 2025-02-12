#pragma once

#include "Core/Renderer/Vulkan/GPUImage.hpp"
#include "Core/Renderer/Vulkan/Common.hpp"

// vulkan headers
#include <vulkan/vulkan.h>

#include "../Third_Party/vk-bootstrap/src/VkBootstrap.h"

namespace pz
{
    static constexpr int TIMEOUT = 1000000000; // 1 second

    class Swapchain
    {
    public:
        Swapchain();
        ~Swapchain();

        VkSwapchainKHR GetSwapchain() const { return m_Swapchain.swapchain; }
        std::vector<VkImage> GetSwapchainImages() const { return m_SwapchainImages; }

        void CreateSwapchain(vkb::Device& device, VkFormat format, uint32_t width, uint32_t height, bool vsync);
        void DestroySwapchain(VkDevice device);
        void InitSyncStructures(VkDevice device);

        // draw related
        void BeginFrame(VkDevice device, uint32_t frameIndex);
        void ResetFences(VkDevice device, uint32_t frameIndex);
        std::pair<VkImage, uint32_t> AcquireImage(VkDevice device, uint32_t frameIndex);
        void SubmitAndPresent(VkCommandBuffer cmd, VkQueue graphicsQueue, uint32_t frameIndex, uint32_t swapchainImageIndex);



    private:
        vkb::Swapchain m_Swapchain;
        VkFormat m_SwapchainImageFormat;

        std::vector<VkImage> m_SwapchainImages;
        std::vector<VkImageView> m_SwapchainImageViews;
        VkExtent2D m_SwapchainExtent;

        struct FrameData
        {
            VkSemaphore SwapchainSemaphore, RenderSemaphore;
            VkFence RenderFence;
        };

        std::array<FrameData, graphics::FRAME_OVERLAP> m_Frames{};
        std::uint32_t frameNumber{ 0 };

        bool m_Dirty = false;
    };

}  // namespace pz