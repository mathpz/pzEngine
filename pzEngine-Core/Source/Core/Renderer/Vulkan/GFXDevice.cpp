#include "pzpch.hpp"

#include "GFXDevice.hpp"
#include "Core/vk_initializers.h"
#include "Core/vk_images.h"

#define VMA_IMPLEMENTATION
#include "vma/vk_mem_alloc.h"

namespace pz
{
	#ifdef PZ_DEBUG
		constexpr bool enableValidationLayers = true;
	#else
		constexpr bool enableValidationLayers = false;
	#endif

	GFXDevice::GFXDevice(PzWindow& window) : m_Window{window}
	{
		Init(true);
	}

	GFXDevice::~GFXDevice()
	{
		Shutdown();
	}

	void GFXDevice::Init(bool vSync)
	{
		InitGFXDevice();

		m_Swapchain.InitSyncStructures(m_Device.device);

		CreateDrawImage();
		InitDescriptors();
		m_BackgroundPipeline.Init(*this);

		m_VSync = vSync;

		uint32_t width = m_Window.GetWidth();
		uint32_t height = m_Window.GetHeight();
		m_SwapchainFormat = VK_FORMAT_B8G8R8A8_SRGB;
		m_Swapchain.CreateSwapchain(m_Device, m_SwapchainFormat, width, height, m_VSync);


		CreateCommandPool();
	}

	VkCommandBuffer GFXDevice::BeginFrame()
	{
		m_Swapchain.BeginFrame(m_Device.device, GetCurrentFrameIndex());

		FrameData& frame = GetCurrentFrame();
		VkCommandBuffer& cmd = frame.MainCommandBuffer;
		VkCommandBufferBeginInfo cmdBeginInfo = VkCommandBufferBeginInfo{
			.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
			.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT,
		};

		VK_CHECK(vkBeginCommandBuffer(cmd, &cmdBeginInfo));

		return cmd;
	}

	void GFXDevice::EndFrame(VkCommandBuffer cmd, const GPUImage& drawImage)
	{
		// get swapchain image
		const auto [swapchainImage, swapchainImageIndex] = m_Swapchain.AcquireImage(m_Device.device, GetCurrentFrameIndex());
		if (swapchainImage == VK_NULL_HANDLE)
		{
			PZ_CORE_TRACE("swapchainImage == VK_NULL_HANDLE");
			return;
		}

		m_DrawExtent.height = std::min(m_Swapchain.GetSwapchainExtent().height, m_DrawImage.imageExtent.height);
		m_DrawExtent.width = std::min(m_Swapchain.GetSwapchainExtent().width, m_DrawImage.imageExtent.width);

		// Fences are reset here to prevent the deadlock in case swapchain becomes dirty
		m_Swapchain.ResetFences(m_Device.device, GetCurrentFrameIndex());

		VkImageLayout swapchainLayout = VK_IMAGE_LAYOUT_UNDEFINED;

		/// clear swapchain image
		{
			VkImageSubresourceRange clearRange = vkinit::image_subresource_range(VK_IMAGE_ASPECT_COLOR_BIT);
			vkutil::transition_image(cmd, swapchainImage, swapchainLayout, VK_IMAGE_LAYOUT_GENERAL);
			swapchainLayout = VK_IMAGE_LAYOUT_GENERAL;

			VkClearColorValue clearValue;
			clearValue = { { 0.53f, 0.81f, 0.92f, 1.0f } };	// light blue
			vkCmdClearColorImage(cmd, swapchainImage, VK_IMAGE_LAYOUT_GENERAL, &clearValue, 1, &clearRange);
		}

		// -- copy image to swapchain
		// if needed this can be made option by implementing EndFrameProperties

		bool tmp_CopyImgToSwapchain = true;
		if (tmp_CopyImgToSwapchain)
		{
			VkImageLayout imageLayout = VK_IMAGE_LAYOUT_UNDEFINED;
			VkImageSubresourceRange clearRange = vkinit::image_subresource_range(VK_IMAGE_ASPECT_COLOR_BIT);
			vkutil::transition_image(cmd, m_DrawImage.image, imageLayout, VK_IMAGE_LAYOUT_GENERAL);
			imageLayout = VK_IMAGE_LAYOUT_GENERAL;

			m_BackgroundPipeline.Draw(cmd, *this, m_DrawExtent);

			// Transition draw image to TRANSFER_SRC_OPTIMAL for copying
			vkutil::transition_image(cmd, m_DrawImage.image, imageLayout, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL);
			imageLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;

			// Transition swapchain image to TRANSFER_DST_OPTIMAL for copying
			vkutil::transition_image(cmd, swapchainImage, swapchainLayout, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
			swapchainLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;

			// copy image to swapchain image
			vkutil::copy_image_to_image(cmd, m_DrawImage.image, swapchainImage, m_DrawImage.GetExtent2D(), m_Swapchain.GetSwapchainExtent());
			// todo if/else for copy or blit
		}

		// prepare for present
		vkutil::transition_image(cmd, swapchainImage, swapchainLayout, VK_IMAGE_LAYOUT_PRESENT_SRC_KHR);
		swapchainLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

		VK_CHECK(vkEndCommandBuffer(cmd));

		m_Swapchain.SubmitAndPresent(cmd, m_GraphicsQueue, GetCurrentFrameIndex(), swapchainImageIndex);

		m_FrameNumber++;
	}

	//VkFormat format, glm::ivec2 size)
	void GFXDevice::CreateDrawImage()
	{
		PZ_CORE_TRACE("Creating GPUImage.");
		//draw image size will match the window
		VkExtent3D drawImageExtent = {
			m_Window.GetWidth(),
			m_Window.GetHeight(),
			1
		};

		m_DrawImage.imageFormat = VK_FORMAT_R16G16B16A16_SFLOAT;
		m_DrawImage.imageExtent = drawImageExtent;

		VkImageUsageFlags drawImageUsages{};
		drawImageUsages |= VK_IMAGE_USAGE_TRANSFER_SRC_BIT;
		drawImageUsages |= VK_IMAGE_USAGE_TRANSFER_DST_BIT;
		drawImageUsages |= VK_IMAGE_USAGE_STORAGE_BIT;
		drawImageUsages |= VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

		VkImageCreateInfo rimgInfo = vkinit::image_create_info(m_DrawImage.imageFormat, drawImageUsages, drawImageExtent);

		// for the draw image, we want to allocate it from the gpu local memory
		VmaAllocationCreateInfo rimgAllocInfo = {};
		rimgAllocInfo.usage = VMA_MEMORY_USAGE_GPU_ONLY;
		rimgAllocInfo.requiredFlags = VkMemoryPropertyFlags(VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

		// allocate and create the image
		vmaCreateImage(m_Allocator, &rimgInfo, &rimgAllocInfo, &m_DrawImage.image, &m_DrawImage.allocation, nullptr);

		//build a image-view for the draw image to use for rendering
		VkImageViewCreateInfo rViewInfo = vkinit::imageview_create_info(m_DrawImage.imageFormat, m_DrawImage.image, VK_IMAGE_ASPECT_COLOR_BIT);

		VK_CHECK(vkCreateImageView(m_Device.device, &rViewInfo, nullptr, &m_DrawImage.imageView));
		PZ_CORE_TRACE("GPUImage created.");
	}

	void GFXDevice::DestroyImage(const GPUImage& image)
	{
		PZ_CORE_TRACE("Destroying GPUImage.");
		vkDestroyImageView(m_Device.device, image.imageView, nullptr);
		vmaDestroyImage(m_Allocator, image.image, image.allocation);
		PZ_CORE_TRACE("GPUImage destroyed.");
	}


	// GPUImage GFXDevice::CreateImageRaw(VkImageCreateInfo imgCreateInfo)
	// {
	// 	// todo
	// }

	void GFXDevice::InitGFXDevice()
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

		const auto features12 = VkPhysicalDeviceVulkan12Features
		{
			.bufferDeviceAddress = true,
		};

		const auto features13 = VkPhysicalDeviceVulkan13Features
		{
			.synchronization2 = true,
			.dynamicRendering = true
		};

		// Physical Device Selection
		vkb::PhysicalDeviceSelector physDevSelector(vkbInstance);
		auto physDevSelectorReturn = physDevSelector
			.set_minimum_version(1, 3)
			.set_required_features_12(features12)
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

		// initialize the memory allocator
		VmaAllocatorCreateInfo allocatorInfo = {};
		allocatorInfo.physicalDevice = m_PhysicalDevice.physical_device;
		allocatorInfo.device = m_Device.device;
		allocatorInfo.instance = m_Instance.instance;
		allocatorInfo.flags = VMA_ALLOCATOR_CREATE_BUFFER_DEVICE_ADDRESS_BIT;
		vmaCreateAllocator(&allocatorInfo, &m_Allocator);
		PZ_CORE_TRACE("VMA created.");

		PZ_CORE_TRACE("GFXDevice fully initiated.");
	}

	void GFXDevice::InitDescriptors()
	{
		// create a descriptor pool that will hold 10 sets with 1 image each
		std::vector<DescriptorAllocator::PoolSizeRatio> sizes =
		{
			{VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 1}
		};

		globalDescriptorAllocator.InitPool(m_Device.device, 10, sizes);

		// make the descriptor set layout for our compute draw
		DescriptorlayoutBuilder builder;
		builder.AddBinding(0, VK_DESCRIPTOR_TYPE_STORAGE_IMAGE);
		m_DrawImageDescriptorSetLayout = builder.Build(m_Device.device, VK_SHADER_STAGE_COMPUTE_BIT);

		// allocate a descriptor set for our draw image
		m_DrawImageDescriptors = globalDescriptorAllocator.Allocate(m_Device.device, m_DrawImageDescriptorSetLayout);

		VkDescriptorImageInfo imgInfo{};
		imgInfo.imageLayout = VK_IMAGE_LAYOUT_GENERAL;
		imgInfo.imageView = m_DrawImage.imageView;

		VkWriteDescriptorSet drawImageWrite = {};
		drawImageWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		drawImageWrite.pNext = nullptr;

		drawImageWrite.dstBinding = 0;
		drawImageWrite.dstSet = m_DrawImageDescriptors;
		drawImageWrite.descriptorCount = 1;
		drawImageWrite.descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_IMAGE;
		drawImageWrite.pImageInfo = &imgInfo;

		vkUpdateDescriptorSets(m_Device.device, 1, &drawImageWrite, 0, nullptr);

	}

	void GFXDevice::Shutdown()
	{
		PZ_CORE_TRACE("Shutting down GFXDevice.");
		vkDeviceWaitIdle(m_Device.device);

		for (int i = 0; i < graphics::FRAME_OVERLAP; i++)
		{
			vkDestroyCommandPool(m_Device, m_Frames[i].CommandPool, nullptr);
			m_Frames[i].CommandPool = VK_NULL_HANDLE;
		}
		PZ_CORE_TRACE("Command pools destroyed.");

		m_BackgroundPipeline.Cleanup(m_Device.device);

		globalDescriptorAllocator.DestroyPool(m_Device.device);
		vkDestroyDescriptorSetLayout(m_Device.device, m_DrawImageDescriptorSetLayout, nullptr);

		DestroyImage(m_DrawImage);

		m_Swapchain.DestroySwapchain(m_Device.device);
		vkDestroySurfaceKHR(m_Instance, m_Surface, nullptr);
		PZ_CORE_TRACE("Vulkan surface destroyed.");
		vmaDestroyAllocator(m_Allocator);
		PZ_CORE_TRACE("VMA destroyed.");
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

		for (int i = 0; i < graphics::FRAME_OVERLAP; i++) {

			VK_CHECK(vkCreateCommandPool(m_Device, &commandPoolInfo, nullptr, &m_Frames[i].CommandPool));

			// allocate the default command buffer that we will use for rendering
			VkCommandBufferAllocateInfo cmdAllocInfo = vkinit::command_buffer_allocate_info(m_Frames[i].CommandPool, 1);

			VK_CHECK(vkAllocateCommandBuffers(m_Device, &cmdAllocInfo, &m_Frames[i].MainCommandBuffer));
		}
		PZ_CORE_TRACE("Command pool created.");
	}

} // namespace pz