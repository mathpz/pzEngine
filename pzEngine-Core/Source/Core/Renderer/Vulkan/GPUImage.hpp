#pragma once

#include <vulkan/vulkan.h>
#include <vma/vk_mem_alloc.h>

namespace pz
{
	struct GPUImage
	{
	    VkImage image;
	    VkImageView imageView;
	    VmaAllocation allocation;
	    VkExtent3D imageExtent;
	    VkFormat imageFormat;
	};
} // namespace pz