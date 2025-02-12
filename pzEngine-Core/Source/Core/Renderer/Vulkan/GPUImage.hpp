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

		VkExtent2D GetExtent2D() const { return VkExtent2D{ imageExtent.width, imageExtent.height }; }
	};
} // namespace pz