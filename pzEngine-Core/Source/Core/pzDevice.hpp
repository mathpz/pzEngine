#pragma once

#include "pzWindow.hpp"

namespace pz
{
	struct FrameData
	{
		VkCommandPool CommandPool;
		VkCommandBuffer MainCommandBuffer;
	};

	constexpr unsigned int FRAME_OVERLAP = 2;

	class PzDevice
	{
	public:
		PzDevice(PzWindow& window);
		~PzDevice();

		VkInstance GetInstance() const { return m_Instance; }
		VkPhysicalDevice GetPhysicalDevice() const { return m_PhysicalDevice; }
		VkDevice GetDevice() const { return m_Device; }
		VkSurfaceKHR GetSurface() const { return m_Surface; }

		uint32_t GetFrameNumber() const { return m_FrameNumber; }
		FrameData& GetCurrentFrame() { return m_Frames[m_FrameNumber % FRAME_OVERLAP]; }

		VkQueue GetGraphicsQueue() const { return m_GraphicsQueue; }

		std::array<FrameData, FRAME_OVERLAP>& GetFrames() { return m_Frames; }
		uint32_t m_FrameNumber = 0;

	private:
		void Init();
		void Shutdown();

		void CreateCommandPool();

	private:
		PzWindow &m_Window;

		VkInstance m_Instance;
		VkDebugUtilsMessengerEXT m_DebugMessenger;
		VkPhysicalDevice m_PhysicalDevice;
		VkDevice m_Device;
		VkSurfaceKHR m_Surface;


		std::array<FrameData, FRAME_OVERLAP> m_Frames;

		VkQueue m_GraphicsQueue;
		uint32_t m_GraphicsQueueFamily;

		// VkQueue m_PresentQueue;

		const std::vector<const char*> m_RequiredDeviceExtensions =
		{
			VK_KHR_DYNAMIC_RENDERING_EXTENSION_NAME,
			VK_KHR_DEPTH_STENCIL_RESOLVE_EXTENSION_NAME,		// required by dynamic rendering	     validaiton errors without it
			VK_KHR_CREATE_RENDERPASS_2_EXTENSION_NAME,			// required by depth stencil resolve     validaiton errors without it
			VK_KHR_MULTIVIEW_EXTENSION_NAME,					// same
			VK_KHR_MAINTENANCE2_EXTENSION_NAME,					// same
		};

		// todo add VK_EXT_debug_utils
	};
} // namespace pz