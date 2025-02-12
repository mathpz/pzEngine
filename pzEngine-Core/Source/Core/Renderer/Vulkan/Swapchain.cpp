#include "pzpch.hpp"

#include "Core/Renderer/Vulkan/Swapchain.hpp"
#include "Core/vk_initializers.h"
// #include "VkBootstrap.h"

namespace pz {

    Swapchain::Swapchain()
    {

    }

    Swapchain::~Swapchain()
    {

    }

    void Swapchain::CreateSwapchain(vkb::Device& device, VkFormat format, uint32_t width, uint32_t height, bool vsync)
    {
        PZ_CORE_TRACE("Creating swapchain.");

        vkb::SwapchainBuilder swapchainBuilder{ device };
        m_SwapchainImageFormat = format;

        auto swapchainBuilderReturn = swapchainBuilder
            .set_desired_format(VkSurfaceFormatKHR{ .format = m_SwapchainImageFormat, .colorSpace = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR })
            .set_desired_present_mode(vsync ? VK_PRESENT_MODE_FIFO_KHR : VK_PRESENT_MODE_IMMEDIATE_KHR) // vsync
            .set_desired_extent(width, height)
            .add_image_usage_flags(VK_IMAGE_USAGE_TRANSFER_DST_BIT)
            .build();

        if (!swapchainBuilderReturn)
        {
            std::cerr << "Failed to create Swap Chain. Error: " << swapchainBuilderReturn.error().message() << "\n";
        }
        PZ_CORE_TRACE("Swapchain created.");

        vkb::Swapchain vkbSwapchain = swapchainBuilderReturn.value();
        m_SwapchainExtent = vkbSwapchain.extent;
        m_Swapchain = vkbSwapchain;
        m_SwapchainImages = vkbSwapchain.get_images().value();
        m_SwapchainImageViews = vkbSwapchain.get_image_views().value();

    }

    void Swapchain::DestroySwapchain(VkDevice device)
    {
        PZ_CORE_TRACE("Destroying swapchain.");

        for (auto& frame : m_Frames)
        {
            vkDestroyFence(device, frame.RenderFence, nullptr);
            vkDestroySemaphore(device, frame.SwapchainSemaphore, nullptr);
            vkDestroySemaphore(device, frame.RenderSemaphore, nullptr);
        }
        PZ_CORE_TRACE("Fences destroyed.");
        PZ_CORE_TRACE("Semaphores destroyed.");

        // destroy swapchain resources
        for (int i = 0; i < m_SwapchainImageViews.size(); i++)
        {
            vkDestroyImageView(device, m_SwapchainImageViews[i], nullptr);
        }
        PZ_CORE_TRACE("Swapchain image views destroyed.");

        vkb::destroy_swapchain(m_Swapchain);
        PZ_CORE_TRACE("Swapchain destroyed.");
    }

    void Swapchain::InitSyncStructures(VkDevice device)
    {
        PZ_CORE_TRACE("Initializing sync structures.");
        //create syncronization structures
        //one fence to control when the gpu has finished rendering the frame,
        //and 2 semaphores to syncronize rendering with swapchain
        //we want the fence to start signalled so we can wait on it on the first frame
        VkFenceCreateInfo fenceCreateInfo = vkinit::fence_create_info(VK_FENCE_CREATE_SIGNALED_BIT);
        VkSemaphoreCreateInfo semaphoreCreateInfo = vkinit::semaphore_create_info();

        for (int i = 0; i < graphics::FRAME_OVERLAP; i++)
        {
            VK_CHECK(vkCreateFence(device, &fenceCreateInfo, nullptr, &m_Frames[i].RenderFence));
            VK_CHECK(vkCreateSemaphore(device, &semaphoreCreateInfo, nullptr, &m_Frames[i].SwapchainSemaphore));
            VK_CHECK(vkCreateSemaphore(device, &semaphoreCreateInfo, nullptr, &m_Frames[i].RenderSemaphore));
        }
        PZ_CORE_TRACE("Fences created.");
        PZ_CORE_TRACE("Semaphores created.");
        PZ_CORE_TRACE("Sync structures initialized.");
    }

    void Swapchain::BeginFrame(VkDevice device, uint32_t frameIndex)
    {
        auto& frame = m_Frames[frameIndex]; // m_frames being
        // vector of frame data { Fences and semaphores }

        VK_CHECK(vkWaitForFences(device, 1, &frame.RenderFence, true, TIMEOUT));
        // VK_CHECK(vkWaitForFences(device, 1, &m_frame.RenderFence, true, TIMEOUT)); // how it need be at the end
    }

    void Swapchain::ResetFences(VkDevice device, uint32_t frameIndex)
    {
        auto& frame = m_Frames[frameIndex];
        VK_CHECK(vkResetFences(device, 1, &frame.RenderFence));
    }

    std::pair<VkImage, uint32_t> Swapchain::AcquireImage(VkDevice device, uint32_t frameIndex)
    {
        uint32_t swapchainImageIndex{};
        const auto result = vkAcquireNextImageKHR(device, m_Swapchain, TIMEOUT, m_Frames[frameIndex].SwapchainSemaphore, VK_NULL_HANDLE, &swapchainImageIndex);
        if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR)
        {
            m_Dirty = true;
            return { m_SwapchainImages[swapchainImageIndex], swapchainImageIndex };
        }
        else if (result != VK_SUCCESS)
        {
            PZ_CORE_ERROR("Failed to acquire swapchain image!");
            throw std::runtime_error("\n");
        }

        return { m_SwapchainImages[swapchainImageIndex], swapchainImageIndex};
    }

    void Swapchain::SubmitAndPresent(VkCommandBuffer cmd, VkQueue graphicsQueue, uint32_t frameIndex, uint32_t swapchainImageIndex)
    {
        const auto& frame = m_Frames[frameIndex];

        // submit
        VkCommandBufferSubmitInfo submitInfo = VkCommandBufferSubmitInfo{
            .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_SUBMIT_INFO,
            .commandBuffer = cmd,
        };
        VkSemaphoreSubmitInfo waitInfo = vkinit::semaphore_submit_info(VK_PIPELINE_STAGE_2_COLOR_ATTACHMENT_OUTPUT_BIT_KHR, frame.SwapchainSemaphore);
        VkSemaphoreSubmitInfo signalInfo = vkinit::semaphore_submit_info(VK_PIPELINE_STAGE_2_ALL_GRAPHICS_BIT, frame.RenderSemaphore);
        VkSubmitInfo2 submit = vkinit::submit_info(&submitInfo, &signalInfo, &waitInfo);
        VK_CHECK(vkQueueSubmit2(graphicsQueue, 1, &submit, frame.RenderFence));

        // present
        VkPresentInfoKHR presentInfo = VkPresentInfoKHR{
            .sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,
            .waitSemaphoreCount = 1,
            .pWaitSemaphores = &frame.RenderSemaphore,
            .swapchainCount = 1,
            .pSwapchains = &m_Swapchain.swapchain,
            .pImageIndices = &swapchainImageIndex,
        };

        auto result = vkQueuePresentKHR(graphicsQueue, &presentInfo);
        if (result != VK_SUCCESS)
        {
            if (result != VK_SUBOPTIMAL_KHR)
            {
                PZ_INFO("Failed to present: {}", string_VkResult(result));
            }
            m_Dirty = true;
        }
    }


}  // namespace pz