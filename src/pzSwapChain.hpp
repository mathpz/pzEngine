#pragma once

#include "pzDevice.hpp"

// vulkan headers
#include <vulkan/vulkan.h>

// std lib headers
#include <memory>
#include <string>
#include <vector>

namespace pz {

class PzSwapChain {
  public:
      static constexpr int MAX_FRAMES_IN_FLIGHT = 2;

      PzSwapChain(PzDevice &deviceRef, VkExtent2D windowExtent);
      PzSwapChain(PzDevice &deviceRef, VkExtent2D windowExtent, std::shared_ptr<PzSwapChain> previous);
      ~PzSwapChain();

      PzSwapChain(const PzSwapChain &) = delete;
      PzSwapChain &operator=(const PzSwapChain &) = delete;

      VkFramebuffer getFrameBuffer(int index) { return swapChainFramebuffers[index]; }
      VkRenderPass getRenderPass() { return renderPass; }
      VkImageView getImageView(int index) { return swapChainImageViews[index]; }
      size_t imageCount() { return swapChainImages.size(); }
      VkFormat getSwapChainImageFormat() { return m_swapChainImageFormat; }
      VkExtent2D getSwapChainExtent() { return swapChainExtent; }
      uint32_t width() { return swapChainExtent.width; }
      uint32_t height() { return swapChainExtent.height; }

      float extentAspectRatio() {
        return static_cast<float>(swapChainExtent.width) / static_cast<float>(swapChainExtent.height);
      }
      VkFormat findDepthFormat();

      VkResult acquireNextImage(uint32_t *imageIndex);
      VkResult submitCommandBuffers(const VkCommandBuffer *buffers, uint32_t *imageIndex);

      bool compareSwapFormats(const PzSwapChain &swapChain) const {
        return swapChain.m_swapChainDepthFormat == m_swapChainDepthFormat &&
               swapChain.m_swapChainImageFormat == m_swapChainImageFormat;
      }

  private:
      void init();
      void createSwapChain();
      void createImageViews();
      void createDepthResources();
      void createRenderPass();
      void createFramebuffers();
      void createSyncObjects();

      // Helper functions
      VkSurfaceFormatKHR chooseSwapSurfaceFormat(
          const std::vector<VkSurfaceFormatKHR> &availableFormats);
      VkPresentModeKHR chooseSwapPresentMode(
          const std::vector<VkPresentModeKHR> &availablePresentModes);
      VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR &capabilities);

      VkFormat m_swapChainImageFormat;
      VkFormat m_swapChainDepthFormat;
      VkExtent2D swapChainExtent;

      std::vector<VkFramebuffer> swapChainFramebuffers;
      VkRenderPass renderPass;

      std::vector<VkImage> depthImages;
      std::vector<VkDeviceMemory> depthImageMemorys;
      std::vector<VkImageView> depthImageViews;
      std::vector<VkImage> swapChainImages;
      std::vector<VkImageView> swapChainImageViews;

      PzDevice &device;
      VkExtent2D windowExtent;

      VkSwapchainKHR swapChain;
      std::shared_ptr<PzSwapChain> oldSwapChain;

      std::vector<VkSemaphore> imageAvailableSemaphores;
      std::vector<VkSemaphore> renderFinishedSemaphores;
      std::vector<VkFence> inFlightFences;
      std::vector<VkFence> imagesInFlight;
      size_t currentFrame = 0;
};

}  // namespace pz