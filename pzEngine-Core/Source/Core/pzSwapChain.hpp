#pragma once

#include "pzDevice.hpp"

// vulkan headers
#include <vulkan/vulkan.h>

namespace pz
{

    class PzSwapChain
    {
    public:
        PzSwapChain();

        ~PzSwapChain();

        VkSwapchainKHR GetSwapchain() const { return m_Swapchain; }
        std::vector<VkImage> GetSwapchainImages() const { return m_SwapchainImages; }

        void CreateSwapchain(VkPhysicalDevice physDevice, VkDevice device, VkSurfaceKHR surface, uint32_t width, uint32_t height);
        void DestroySwapchain(VkDevice device);
        void InitSyncStructures(VkDevice device, std::array<FrameData, FRAME_OVERLAP>& frames);

    private:
        void Init();
        void ShutDown();


    private:
        VkSwapchainKHR m_Swapchain;
        VkFormat m_SwapchainImageFormat;

        std::vector<VkImage> m_SwapchainImages;
        std::vector<VkImageView> m_SwapchainImageViews;
        VkExtent2D m_SwapchainExtent;

    };

}  // namespace pz