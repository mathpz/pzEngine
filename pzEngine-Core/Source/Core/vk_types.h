// vulkan_guide.h : Include file for standard system include files,
// or project specific include files.
#pragma once

#include <memory>
#include <optional>
#include <string>
#include <vector>
#include <span>
#include <array>
#include <functional>
#include <deque>

#include <vulkan/vulkan.h>
#include <vulkan/vk_enum_string_helper.h>
#include <vma/vk_mem_alloc.h>

// #include <fmt/core.h>

#include <glm/mat4x4.hpp>
#include <glm/vec4.hpp>

// struct AllocatedImage
// {
//     VkImage image;
//     VkImageView imageView;
//     VmaAllocation allocation;
//     VkExtent3D imageExtent;
//     VkFormat imageFormat;
// };

// struct AllocatedBuffer
// {
//     VkBuffer buffer;
//     VmaAllocation allocation;
//     VmaAllocationInfo info;
// };

// struct Vertex
// {
//     glm::vec3 position;
//     float uv_x;
//     glm::vec3 normal;
//     float uv_y;
//     glm::vec4 color;
// };

// holds the resources needed for a mesh
// struct GPUMeshBuffers
// {
//     AllocatedBuffer indexBuffer;
//     AllocatedBuffer vertexBuffer;
//     VkDeviceAddress vertexBufferAddress;
// };
//
// // push constants for out mesh object draws
// struct GPUDrawPushConstants
// {
//     glm::mat4 worldMatrix;
//     VkDeviceAddress vertexBuffer;
// };

namespace vkb
{
	inline const std::string to_string(VkResult result)
	{
		switch (result)
		{
#define STR(r)   \
	case VK_##r: \
		return #r
			STR(NOT_READY);
			STR(TIMEOUT);
			STR(EVENT_SET);
			STR(EVENT_RESET);
			STR(INCOMPLETE);
			STR(ERROR_OUT_OF_HOST_MEMORY);
			STR(ERROR_OUT_OF_DEVICE_MEMORY);
			STR(ERROR_INITIALIZATION_FAILED);
			STR(ERROR_DEVICE_LOST);
			STR(ERROR_MEMORY_MAP_FAILED);
			STR(ERROR_LAYER_NOT_PRESENT);
			STR(ERROR_EXTENSION_NOT_PRESENT);
			STR(ERROR_FEATURE_NOT_PRESENT);
			STR(ERROR_INCOMPATIBLE_DRIVER);
			STR(ERROR_TOO_MANY_OBJECTS);
			STR(ERROR_FORMAT_NOT_SUPPORTED);
			STR(ERROR_FRAGMENTED_POOL);
			STR(ERROR_UNKNOWN);
			STR(ERROR_OUT_OF_POOL_MEMORY);
			STR(ERROR_INVALID_EXTERNAL_HANDLE);
			STR(ERROR_FRAGMENTATION);
			STR(ERROR_INVALID_OPAQUE_CAPTURE_ADDRESS);
			STR(PIPELINE_COMPILE_REQUIRED);
			STR(ERROR_SURFACE_LOST_KHR);
			STR(ERROR_NATIVE_WINDOW_IN_USE_KHR);
			STR(SUBOPTIMAL_KHR);
			STR(ERROR_OUT_OF_DATE_KHR);
			STR(ERROR_INCOMPATIBLE_DISPLAY_KHR);
			STR(ERROR_VALIDATION_FAILED_EXT);
			STR(ERROR_INVALID_SHADER_NV);
#undef STR
		default:
			return "UNKNOWN_ERROR";
		}
	}
}

#define VK_CHECK(x)                                                                    \
	do                                                                                 \
	{                                                                                  \
		VkResult err = x;                                                              \
		if (err)                                                                       \
		{                                                                              \
			std::cout << "Vulkan Error: " << vkb::to_string(err) << std::endl;        \
		}                                                                              \
	} while (0)