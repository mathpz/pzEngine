#pragma once

#include <vulkan/vulkan.h>
#include "Core/vk_types.h"

namespace pz
{
	struct DescriptorlayoutBuilder
	{
		std::vector<VkDescriptorSetLayoutBinding> bindings;

		void AddBinding(uint32_t binding, VkDescriptorType type);
		void Clear();
		VkDescriptorSetLayout Build(VkDevice device, VkShaderStageFlags shaderStages, void* pNext = nullptr, VkDescriptorSetLayoutCreateFlags flags = 0);
	};

	// todo: make descriptor allocator growable
	struct DescriptorAllocator
	{
		struct PoolSizeRatio
		{
			VkDescriptorType Type;
			float Ratio;
		};

		VkDescriptorPool Pool;
		void InitPool(VkDevice device, uint32_t maxSets, std::span<PoolSizeRatio> poolRatios);
		void ClearDescriptors(VkDevice device);
		void DestroyPool(VkDevice device);

		VkDescriptorSet Allocate(VkDevice device, VkDescriptorSetLayout layout);
	};

}  // namespace pz