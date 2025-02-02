#include "pzpch.hpp"

#include "VkBootstrap.h"

#include "vk_initializers.h"
#include "pzDevice.hpp"

namespace pz
{
	#ifdef PZ_DEBUG
		constexpr bool enableValidationLayers = true;
	#else
		constexpr bool enableValidationLayers = false;
	#endif

	PzDevice::PzDevice(PzWindow& window) : m_Window{window}
	{
		Init();
		CreateCommandPool();
	}

	PzDevice::~PzDevice()
	{
		Shutdown();
	}

	void PzDevice::Init()
	{
		uint32_t glfwExtensionCount = 0;
		const char** glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
		// enabling glfw extensions
		vkb::InstanceBuilder instanceBuilder;
		for (uint32_t i = 0; i < glfwExtensionCount; i++)
			instanceBuilder.enable_extension(glfwExtensions[i]);

		// Instance creation
		auto instanceBuilderReturn = instanceBuilder
			.set_engine_name("pzEngine")
			.request_validation_layers(enableValidationLayers)
			.require_api_version(1, 3, 0)
			.set_debug_callback(
				[](VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
					VkDebugUtilsMessageTypeFlagsEXT messageType,
					const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
					void* pUserData)
				-> VkBool32 {
					auto severity = vkb::to_string_message_severity(messageSeverity);
					auto type = vkb::to_string_message_type(messageType);
					printf("[%s: %s] %s\n", severity, type, pCallbackData->pMessage);
					return VK_FALSE;
				}
			)
			.build();
		if (!instanceBuilderReturn)
		{
			std::cerr << "Failed to create Vulkan instance. Error: " << instanceBuilderReturn.error().message() << "\n";
		}
		vkb::Instance vkbInstance = instanceBuilderReturn.value();

		m_Instance = vkbInstance.instance;
		m_DebugMessenger = vkbInstance.debug_messenger;

		// Surface creation
		m_Window.createWindowSurface(m_Instance, &m_Surface);

		const auto features13 = VkPhysicalDeviceVulkan13Features
		{
			.synchronization2 = true,
			.dynamicRendering = true
		};

		// Physical Device Selection
		vkb::PhysicalDeviceSelector physDevSelector(vkbInstance);
		auto physDevSelectorReturn = physDevSelector
			.set_minimum_version(1, 3)
			.set_required_features_13(features13)
			.set_surface(m_Surface)
			.select();
		if (!physDevSelectorReturn)
		{
			std::cerr << "Failed to create Select Physical Device. Error: " << physDevSelectorReturn.error().message() << "\n";
		}
		vkb::PhysicalDevice vkbPhysDevice = physDevSelectorReturn.value();
		m_PhysicalDevice = vkbPhysDevice.physical_device;
		if (!vkbPhysDevice.enable_extensions_if_present(m_RequiredDeviceExtensions))
		{
			std::cout << "Extensions not enabled!" << std::endl;
		}

		// Device creation
		vkb::DeviceBuilder deviceBuiler{ vkbPhysDevice };
		vkb::Device vkbDevice = deviceBuiler
			.build()
			.value();
		m_Device = vkbDevice.device;


		// Graphics queue
		m_GraphicsQueue = vkbDevice.get_queue(vkb::QueueType::graphics).value();
		m_GraphicsQueueFamily = vkbDevice.get_queue_index(vkb::QueueType::graphics).value();
	}

	void PzDevice::Shutdown()
	{
		vkDeviceWaitIdle(m_Device);

		for (int i = 0; i < FRAME_OVERLAP; i++)
		{
			vkDestroyCommandPool(m_Device, m_Frames[i].CommandPool, nullptr);
			m_Frames[i].CommandPool = VK_NULL_HANDLE;

			//destroy sync objects
			vkDestroyFence(m_Device, m_Frames[i].RenderFence, nullptr);
			vkDestroySemaphore(m_Device, m_Frames[i].RenderSemaphore, nullptr);
			vkDestroySemaphore(m_Device, m_Frames[i].SwapchainSemaphore, nullptr);
		}

		vkDestroySurfaceKHR(m_Instance, m_Surface, nullptr);
		vkDestroyDevice(m_Device, nullptr);
		vkb::destroy_debug_utils_messenger(m_Instance, m_DebugMessenger);
		vkDestroyInstance(m_Instance, nullptr);
	}

	void PzDevice::CreateCommandPool()
	{
		//create a command pool for commands submitted to the graphics queue.
		//we also want the pool to allow for resetting of individual command buffers
		VkCommandPoolCreateInfo commandPoolInfo = vkinit::command_pool_create_info(m_GraphicsQueueFamily, VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT);

		for (int i = 0; i < FRAME_OVERLAP; i++) {

			VK_CHECK(vkCreateCommandPool(m_Device, &commandPoolInfo, nullptr, &m_Frames[i].CommandPool));

			// allocate the default command buffer that we will use for rendering
			VkCommandBufferAllocateInfo cmdAllocInfo = vkinit::command_buffer_allocate_info(m_Frames[i].CommandPool, 1);

			VK_CHECK(vkAllocateCommandBuffers(m_Device, &cmdAllocInfo, &m_Frames[i].MainCommandBuffer));
		}
	}


} // namespace pz