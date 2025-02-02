#include "pzpch.hpp"

#include "pzSwapChain.hpp"
#include "vk_initializers.h"
#include "VkBootstrap.h"

namespace pz {

    PzSwapChain::PzSwapChain()
    {
        Init();
    }

    PzSwapChain::~PzSwapChain()
    {
        ShutDown();
    }

    void PzSwapChain::Init()
    {

    }

    void PzSwapChain::ShutDown()
    {

    }

    void PzSwapChain::CreateSwapchain(VkPhysicalDevice physDevice, VkDevice device, VkSurfaceKHR surface, uint32_t width, uint32_t height)
    {
        vkb::SwapchainBuilder swapchainBuilder{ physDevice, device, surface };
        m_SwapchainImageFormat = VK_FORMAT_B8G8R8A8_UNORM;

        auto swapchainBuilderReturn = swapchainBuilder
            .set_desired_format(VkSurfaceFormatKHR{ .format = m_SwapchainImageFormat, .colorSpace = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR })
            .set_desired_present_mode(VK_PRESENT_MODE_FIFO_KHR) // vsync
            .set_desired_extent(width, height)
            .add_image_usage_flags(VK_IMAGE_USAGE_TRANSFER_DST_BIT)
            .build();

        if (!swapchainBuilderReturn)
        {
            std::cerr << "Failed to create Swap Chain. Error: " << swapchainBuilderReturn.error().message() << "\n";
        }

        vkb::Swapchain vkbSwapchain = swapchainBuilderReturn.value();
        m_SwapchainExtent = vkbSwapchain.extent;
        m_Swapchain = vkbSwapchain.swapchain;
        m_SwapchainImages = vkbSwapchain.get_images().value();
        m_SwapchainImageViews = vkbSwapchain.get_image_views().value();
    }

    void PzSwapChain::DestroySwapchain(VkDevice device)
    {
        vkDestroySwapchainKHR(device, m_Swapchain, nullptr);

        // destroy swapchain resources
        for (int i = 0; i < m_SwapchainImageViews.size(); i++)
        {
            vkDestroyImageView(device, m_SwapchainImageViews[i], nullptr);
        }
    }

    void PzSwapChain::InitSyncStructures(VkDevice device, std::array<FrameData, FRAME_OVERLAP>& frames)
    {
        //create syncronization structures
        //one fence to control when the gpu has finished rendering the frame,
        //and 2 semaphores to syncronize rendering with swapchain
        //we want the fence to start signalled so we can wait on it on the first frame
        VkFenceCreateInfo fenceCreateInfo = vkinit::fence_create_info(VK_FENCE_CREATE_SIGNALED_BIT);
        VkSemaphoreCreateInfo semaphoreCreateInfo = vkinit::semaphore_create_info();

        for (int i = 0; i < FRAME_OVERLAP; i++)
        {
            VK_CHECK(vkCreateFence(device, &fenceCreateInfo, nullptr, &frames[i].RenderFence));
            VK_CHECK(vkCreateSemaphore(device, &semaphoreCreateInfo, nullptr, &frames[i].SwapchainSemaphore));
            VK_CHECK(vkCreateSemaphore(device, &semaphoreCreateInfo, nullptr, &frames[i].RenderSemaphore));
        }
    }


}  // namespace pz