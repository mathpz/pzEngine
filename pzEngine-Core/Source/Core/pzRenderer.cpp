#include "pzpch.hpp"

#include "pzRenderer.hpp"

// std
#include <array>
#include <cassert>
#include <stdexcept>

namespace pz
{

    PzRenderer::PzRenderer(PzWindow& pzWindow, PzDevice& pzDevice)
        : pzWindow{pzWindow}, pzDevice{pzDevice}
    {
        recreateSwapChain();
        createCommandBuffers();
    }

    PzRenderer::~PzRenderer() { freeCommandBuffers(); }

    void PzRenderer::recreateSwapChain()
    {
        auto extent = pzWindow.getExtent();
        while (extent.width == 0 || extent.height == 0)
        {
            extent = pzWindow.getExtent();
            glfwWaitEvents();
        }
        vkDeviceWaitIdle(pzDevice.device());

        if (pzSwapChain == nullptr)
        {
            pzSwapChain = std::make_unique<PzSwapChain>(pzDevice, extent);
        }
        else
        {
            std::shared_ptr<PzSwapChain> oldSwapChain = std::move(pzSwapChain);
            pzSwapChain = std::make_unique<PzSwapChain>(pzDevice, extent, oldSwapChain);

            if(!oldSwapChain->compareSwapFormats(*pzSwapChain.get()))
            {
                throw std::runtime_error("Swap chain image(or depth) format has changed!");
            }


        }

    }

    void PzRenderer::createCommandBuffers()
    {
        commandBuffers.resize(PzSwapChain::MAX_FRAMES_IN_FLIGHT);

        VkCommandBufferAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandPool = pzDevice.getCommandPool();
        allocInfo.commandBufferCount = static_cast<uint32_t>(commandBuffers.size());

        if (vkAllocateCommandBuffers(pzDevice.device(), &allocInfo, commandBuffers.data()) !=
            VK_SUCCESS)
        {
            throw std::runtime_error("failed to allocate command buffers!");
        }
    }

    void PzRenderer::freeCommandBuffers()
    {
        vkFreeCommandBuffers(pzDevice.device(), pzDevice.getCommandPool(), static_cast<uint32_t>(commandBuffers.size()), commandBuffers.data());
        commandBuffers.clear();
    }

    VkCommandBuffer PzRenderer::beginFrame()
    {
        assert(!m_isFrameStarted && "Can't call beginFrame while already in progress");
        
        auto result = pzSwapChain->acquireNextImage(&m_currentImageIndex);

        if (result == VK_ERROR_OUT_OF_DATE_KHR)
        {
            recreateSwapChain();
            return nullptr;
        }

        if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
        {
            throw std::runtime_error("failed to acquire swap chain image!");
        }

        m_isFrameStarted = true;

        auto commandBuffer = getCurrentCommandBuffer();
        VkCommandBufferBeginInfo beginInfo{};
        beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

        if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to begin recording command buffer!");
        }

        return commandBuffer;
    }

    void PzRenderer::endFrame()
    {
        assert(m_isFrameStarted && "Can't call endFrame while frame is not in progress");
        auto commandBuffer = getCurrentCommandBuffer();

        if (vkEndCommandBuffer(commandBuffer) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to record command buffer!");
        }

        auto result = pzSwapChain->submitCommandBuffers(&commandBuffer, &m_currentImageIndex);
        if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || pzWindow.wasWindowResized())
        {
            pzWindow.resetWindowResizedFlag();
            recreateSwapChain();
        }
        else if (result != VK_SUCCESS)
        {
            throw std::runtime_error("failed to present swap chain image!");
        }

        m_isFrameStarted = false;
        m_currentFrameIndex = (m_currentFrameIndex + 1) % PzSwapChain::MAX_FRAMES_IN_FLIGHT;
    }

    void PzRenderer::beginSwapChainRenderPass(VkCommandBuffer commandBuffer)
    {
        assert(m_isFrameStarted && "Can't call beginSwapChainRenderPass if frame is not in progress");
        assert(commandBuffer == getCurrentCommandBuffer() && "Can't begin render pass on command buffer from a different frame");

        VkRenderPassBeginInfo renderPassInfo{};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        renderPassInfo.renderPass = pzSwapChain->getRenderPass();
        renderPassInfo.framebuffer = pzSwapChain->getFrameBuffer(m_currentImageIndex);

        renderPassInfo.renderArea.offset = {0, 0};
        renderPassInfo.renderArea.extent = pzSwapChain->getSwapChainExtent();

        std::array<VkClearValue, 2> clearValues{};
        clearValues[0].color = {0.01f, 0.01f, 0.01f, 1.0f};
        clearValues[1].depthStencil = {1.0f, 0};
        renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
        renderPassInfo.pClearValues = clearValues.data();

        vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

        VkViewport viewport{};
        viewport.x = 0.0f;
        viewport.y = 0.0f;
        viewport.width = static_cast<float>(pzSwapChain->getSwapChainExtent().width);
        viewport.height = static_cast<float>(pzSwapChain->getSwapChainExtent().height);
        viewport.minDepth = 0.0f;
        viewport.maxDepth = 1.0f;
        VkRect2D scissor{{0, 0}, pzSwapChain->getSwapChainExtent()};
        vkCmdSetViewport(commandBuffer, 0, 1, &viewport);
        vkCmdSetScissor(commandBuffer, 0, 1, &scissor);

    }

    void PzRenderer::endSwapChainRenderPass(VkCommandBuffer commandBuffer)
    {
        assert(m_isFrameStarted && "Can't call endSwapChainRenderPass if frame is not in progress");
        assert(commandBuffer == getCurrentCommandBuffer() && "Can't end render pass on command buffer from a different frame");

        vkCmdEndRenderPass(commandBuffer);
    }

} // namespace pz