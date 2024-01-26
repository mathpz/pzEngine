#pragma once

#include "Core/pzCamera.hpp"
#include "Core/pzGameObject.hpp"

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
		PzGameObject::Map& gameObjects;
	};
} // namespace pz