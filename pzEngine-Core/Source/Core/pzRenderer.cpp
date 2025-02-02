#include "pzpch.hpp"

#include "pzRenderer.hpp"
#include "vk_types.h"
#include "vk_initializers.h"
#include "vk_images.h"

// std
#include <array>
#include <cassert>
#include <stdexcept>

namespace pz
{

    PzRenderer::PzRenderer(PzWindow& pzWindow, PzDevice& device)
        : pzWindow{pzWindow}, m_Device{device}
    {
        m_Swapchain.CreateSwapchain(
            m_Device.GetPhysicalDevice(),
            m_Device.GetDevice(),
            m_Device.GetSurface(),
            pzWindow.GetWidth(),
            pzWindow.GetHeight());
        m_Swapchain.InitSyncStructures(m_Device.GetDevice(), m_Device.GetFrames());
    }

    PzRenderer::~PzRenderer()
    {
        m_Swapchain.DestroySwapchain(m_Device.GetDevice());
    }

    void PzRenderer::Draw()
    {
        // wait until the gpu has finished rendering the last frame. Timeout of 1
        // second
        VK_CHECK(vkWaitForFences(m_Device.GetDevice(), 1, &m_Device.GetCurrentFrame().RenderFence, true, 1000000000));
        VK_CHECK(vkResetFences(m_Device.GetDevice(), 1, &m_Device.GetCurrentFrame().RenderFence));

        uint32_t swapchainImageIndex;
        VK_CHECK(vkAcquireNextImageKHR(m_Device.GetDevice(), m_Swapchain.GetSwapchain(), 1000000000, m_Device.GetCurrentFrame().SwapchainSemaphore, nullptr, &swapchainImageIndex));

        VkCommandBuffer cmd = m_Device.GetCurrentFrame().MainCommandBuffer;
        // now that we are sure that the commands finished executing, we can safely
        // reset the command buffer to begin recording again.
        VK_CHECK(vkResetCommandBuffer(cmd, 0));
        //begin the command buffer recording. We will use this command buffer exactly once, so we want to let vulkan know that
        VkCommandBufferBeginInfo cmdBeginInfo = vkinit::command_buffer_begin_info(VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT);
        //start the command buffer recording
        VK_CHECK(vkBeginCommandBuffer(cmd, &cmdBeginInfo));

        //make the swapchain image into writeable mode before rendering
        vkutil::transition_image(cmd, m_Swapchain.GetSwapchainImages()[swapchainImageIndex], VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_GENERAL);
        //make a clear-color from frame number. This will flash with a 120 frame period.
        VkClearColorValue clearValue;
        float flash = std::abs(std::sin(m_Device.GetFrameNumber() / 120.f));
        clearValue = { { 0.0f, 0.0f, flash, 1.0f } };
        VkImageSubresourceRange clearRange = vkinit::image_subresource_range(VK_IMAGE_ASPECT_COLOR_BIT);
        //clear image
        vkCmdClearColorImage(cmd, m_Swapchain.GetSwapchainImages()[swapchainImageIndex], VK_IMAGE_LAYOUT_GENERAL, &clearValue, 1, &clearRange);
        //make the swapchain image into presentable mode
        vkutil::transition_image(cmd, m_Swapchain.GetSwapchainImages()[swapchainImageIndex], VK_IMAGE_LAYOUT_GENERAL, VK_IMAGE_LAYOUT_PRESENT_SRC_KHR);
        //finalize the command buffer (we can no longer add commands, but it can now be executed)
        VK_CHECK(vkEndCommandBuffer(cmd));

        //prepare the submission to the queue.
            //we want to wait on the _presentSemaphore, as that semaphore is signaled when the swapchain is ready
            //we will signal the _renderSemaphore, to signal that rendering has finished
        VkCommandBufferSubmitInfo cmdinfo = vkinit::command_buffer_submit_info(cmd);
        VkSemaphoreSubmitInfo waitInfo = vkinit::semaphore_submit_info(VK_PIPELINE_STAGE_2_COLOR_ATTACHMENT_OUTPUT_BIT_KHR, m_Device.GetCurrentFrame().SwapchainSemaphore);
        VkSemaphoreSubmitInfo signalInfo = vkinit::semaphore_submit_info(VK_PIPELINE_STAGE_2_ALL_GRAPHICS_BIT, m_Device.GetCurrentFrame().RenderSemaphore);
        VkSubmitInfo2 submit = vkinit::submit_info(&cmdinfo, &signalInfo, &waitInfo);
        //submit command buffer to the queue and execute it.
        // _renderFence will now block until the graphic commands finish execution
        VK_CHECK(vkQueueSubmit2(m_Device.GetGraphicsQueue(), 1, &submit, m_Device.GetCurrentFrame().RenderFence));

        VkSwapchainKHR swapchain = m_Swapchain.GetSwapchain();
        //prepare present
        // this will put the image we just rendered to into the visible window.
        // we want to wait on the _renderSemaphore for that,
        // as its necessary that drawing commands have finished before the image is displayed to the user
        VkPresentInfoKHR presentInfo = {};
        presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
        presentInfo.pNext = nullptr;
        presentInfo.pSwapchains = &swapchain;
        presentInfo.swapchainCount = 1;

        presentInfo.pWaitSemaphores = &m_Device.GetCurrentFrame().RenderSemaphore;
        presentInfo.waitSemaphoreCount = 1;

        presentInfo.pImageIndices = &swapchainImageIndex;

        VK_CHECK(vkQueuePresentKHR(m_Device.GetGraphicsQueue(), &presentInfo));

        //increase the number of frames drawn
        m_Device.m_FrameNumber++;

    }


} // namespace pz