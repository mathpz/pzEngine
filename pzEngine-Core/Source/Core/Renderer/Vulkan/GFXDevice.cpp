#include "pzpch.hpp"

#include "Core/vk_initializers.h"
#include "GFXDevice.hpp"

namespace pz
{
	#ifdef PZ_DEBUG
		constexpr bool enableValidationLayers = true;
	#else
		constexpr bool enableValidationLayers = false;
	#endif

	GFXDevice::GFXDevice(PzWindow& window) : m_Window{window}
	{
		Init();
		CreateCommandPool();
	}

	GFXDevice::~GFXDevice()
	{
		Shutdown();
	}

	void GFXDevice::Init()
	{
		PZ_CORE_TRACE("Initializing GFXDevice.");

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
		PZ_CORE_TRACE("Vulkan instance created.");
		vkb::Instance vkbInstance = instanceBuilderReturn.value();

		m_Instance = vkbInstance;
		m_DebugMessenger = vkbInstance.debug_messenger;

		// Surface creation
		m_Window.createWindowSurface(m_Instance, &m_Surface);
		PZ_CORE_TRACE("Vulkan surface created.");
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
		m_PhysicalDevice = vkbPhysDevice;
		if (!vkbPhysDevice.enable_extensions_if_present(m_RequiredDeviceExtensions))
		{
			std::cout << "Extensions not enabled!" << std::endl;
		}
		PZ_CORE_TRACE("Vulkan physical device created.");
		// Device creation
		vkb::DeviceBuilder deviceBuiler{ vkbPhysDevice };
		vkb::Device vkbDevice = deviceBuiler
			.build()
			.value();
		m_Device = vkbDevice;
		PZ_CORE_TRACE("Vulkan logical device created.");

		// Graphics queue
		m_GraphicsQueue = vkbDevice.get_queue(vkb::QueueType::graphics).value();
		m_GraphicsQueueFamily = vkbDevice.get_queue_index(vkb::QueueType::graphics).value();

		PZ_CORE_TRACE("GFXDevice fully initiated.");
	}

	void GFXDevice::Shutdown()
	{
		PZ_CORE_TRACE("Shutting down GFXDevice.");
		vkDeviceWaitIdle(m_Device.device);

		for (int i = 0; i < FRAME_OVERLAP; i++)
		{
			vkDestroyCommandPool(m_Device, m_Frames[i].CommandPool, nullptr);
			m_Frames[i].CommandPool = VK_NULL_HANDLE;

			//destroy sync objects
			vkDestroyFence(m_Device, m_Frames[i].RenderFence, nullptr);
			vkDestroySemaphore(m_Device, m_Frames[i].RenderSemaphore, nullptr);
			vkDestroySemaphore(m_Device, m_Frames[i].SwapchainSemaphore, nullptr);
		}
		PZ_CORE_TRACE("Command pools destroyed.");
		PZ_CORE_TRACE("Fences destroyed.");
		PZ_CORE_TRACE("Semaphores destroyed.");

		vkDestroySurfaceKHR(m_Instance, m_Surface, nullptr);
		PZ_CORE_TRACE("Vulkan surface destroyed.");
		vkDestroyDevice(m_Device, nullptr);
		PZ_CORE_TRACE("Vulkan device destroyed.");
		vkb::destroy_debug_utils_messenger(m_Instance, m_DebugMessenger);
		PZ_CORE_TRACE("Debug messenger destroyed.");
		vkDestroyInstance(m_Instance, nullptr);
		PZ_CORE_TRACE("Vulkan instance destroyed.");
		PZ_CORE_TRACE("GFXDevice fully shutdown.");
	}

	void GFXDevice::CreateCommandPool()
	{
		PZ_CORE_TRACE("Creating command pool.");
		//create a command pool for commands submitted to the graphics queue.
		//we also want the pool to allow for resetting of individual command buffers
		VkCommandPoolCreateInfo commandPoolInfo = vkinit::command_pool_create_info(m_GraphicsQueueFamily, VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT);

		for (int i = 0; i < FRAME_OVERLAP; i++) {

			VK_CHECK(vkCreateCommandPool(m_Device, &commandPoolInfo, nullptr, &m_Frames[i].CommandPool));

			// allocate the default command buffer that we will use for rendering
			VkCommandBufferAllocateInfo cmdAllocInfo = vkinit::command_buffer_allocate_info(m_Frames[i].CommandPool, 1);

			VK_CHECK(vkAllocateCommandBuffers(m_Device, &cmdAllocInfo, &m_Frames[i].MainCommandBuffer));
		}
		PZ_CORE_TRACE("Command pool created.");
	}


} // namespace pz