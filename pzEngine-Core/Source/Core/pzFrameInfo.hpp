#pragma once

#include "Core/pzCamera.hpp"

#include <vulkan/vulkan.h>

namespace pz
{
	struct FrameInfo
	{
		int frameIndex;
		float frameTime;
		VkCommandBuffer commandBuffer;
		PzCamera& camera;
		VkDescriptorSet globalDescriptorSet;
	};
} // namespace pz