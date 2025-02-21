#pragma once

#include "Core/pzWindow.hpp"
#include "Core/Renderer/Vulkan/Swapchain.hpp"
#include "Core/Renderer/Vulkan/GPUImage.hpp"
#include "Core/Renderer/Vulkan/Common.hpp"
#include "Core/Renderer/Vulkan/Descriptors.hpp"

// --- tmp
#include "Core/Renderer/Pipelines/tutorialBackgroundPipeline.hpp"
// --- tmp

#include "../Third_Party/vk-bootstrap/src/VkBootstrap.h"
#include "vma/vk_mem_alloc.h"

#include "glm/glm.hpp"

namespace pz
{
	struct FrameData
	{
		VkCommandPool CommandPool;
		VkCommandBuffer MainCommandBuffer;
	};

	class GFXDevice
	{
	public:
		DescriptorAllocator globalDescriptorAllocator;
		VkDescriptorSet m_DrawImageDescriptors;
		VkDescriptorSetLayout m_DrawImageDescriptorSetLayout;

		BackgroundPipeline m_BackgroundPipeline;

	public:
		GFXDevice(PzWindow& window);
		~GFXDevice();

		VkInstance GetInstance() const { return m_Instance.instance; }
		VkPhysicalDevice GetPhysicalDevice() const { return m_PhysicalDevice.physical_device; }
		VkDevice GetDevice() const { return m_Device.device; }
		VkSurfaceKHR GetSurface() const { return m_Surface; }

		uint32_t GetFrameNumber() const { return m_FrameNumber; }
		FrameData& GetCurrentFrame() { return m_Frames[m_FrameNumber % graphics::FRAME_OVERLAP]; }
		uint32_t GetCurrentFrameIndex() const { return m_FrameNumber % graphics::FRAME_OVERLAP; }

		VkQueue GetGraphicsQueue() const { return m_GraphicsQueue; }

		std::array<FrameData, graphics::FRAME_OVERLAP>& GetFrames() { return m_Frames; }

		// render
		VkCommandBuffer BeginFrame();
		// todo check this later
		struct EndFrameProperties {
			// const LinearColor clearColor{ 0.f, 0.f, 0.f, 1.f };
			bool copyImageIntoSwapchain{ true };
			// glm::ivec4 drawImageBlitRect{}; // where to blit draw image to
			// bool drawImageLinearBlit{ true }; // if false - nearest filter will be used
			// bool drawImGui{ true };
		};
		void EndFrame(VkCommandBuffer cmd, const GPUImage& drawImage);
		GPUImage& GetDrawImage() { return m_DrawImage; }

		// later on do image caching
		void CreateDrawImage(); // VkFormat format, glm::ivec2 size);
		// GPUImage CreateImageRaw(VkImageCreateInfo imgCreateInfo);
		void DestroyImage(const GPUImage& image);


	private:
		void Init(bool vsync);
		void InitGFXDevice();
		void InitDescriptors();
		void Shutdown();

		void CreateCommandPool();

	private:
		PzWindow &m_Window;

		vkb::Instance m_Instance;
		vkb::PhysicalDevice m_PhysicalDevice;
		vkb::Device m_Device;
		VmaAllocator m_Allocator;

		VkDebugUtilsMessengerEXT m_DebugMessenger;

		VkSurfaceKHR m_Surface;
		VkFormat m_SwapchainFormat;
		Swapchain m_Swapchain;

		std::array<FrameData, graphics::FRAME_OVERLAP> m_Frames;
		uint32_t m_FrameNumber = 0;

		VkQueue m_GraphicsQueue;
		uint32_t m_GraphicsQueueFamily;

		// VkQueue m_PresentQueue;

		GPUImage m_DrawImage;
		VkExtent2D m_DrawExtent;


		bool m_VSync = true;

		const std::vector<const char*> m_RequiredDeviceExtensions =
		{
			VK_KHR_DYNAMIC_RENDERING_EXTENSION_NAME,
			VK_KHR_DEPTH_STENCIL_RESOLVE_EXTENSION_NAME,		// required by dynamic rendering	     validaiton errors without it
			VK_KHR_CREATE_RENDERPASS_2_EXTENSION_NAME,			// required by depth stencil resolve     validaiton errors without it
			VK_KHR_MULTIVIEW_EXTENSION_NAME,					// same
			VK_KHR_MAINTENANCE2_EXTENSION_NAME,					// same
		};

	};
} // namespace pz