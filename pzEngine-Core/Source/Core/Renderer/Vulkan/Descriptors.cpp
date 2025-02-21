#include "pzpch.hpp"

#include "Descriptors.hpp"

namespace pz
{
	void DescriptorlayoutBuilder::AddBinding(uint32_t binding, VkDescriptorType type)
	{
		PZ_CORE_TRACE("Descriptor layout builder: adding bindind.");
		VkDescriptorSetLayoutBinding newBind{
			.binding = binding,
			.descriptorType = type,
			.descriptorCount = 1,
		};

		bindings.push_back(newBind);
	}

	void DescriptorlayoutBuilder::Clear()
	{
		bindings.clear();
	}

	VkDescriptorSetLayout DescriptorlayoutBuilder::Build(VkDevice device, VkShaderStageFlags shaderStages, void* pNext, VkDescriptorSetLayoutCreateFlags flags)
	{
		PZ_CORE_TRACE("Building descriptor set layout.");
		for (auto& b : bindings)
		{
			b.stageFlags |= shaderStages;
		}

		VkDescriptorSetLayoutCreateInfo info = {
			.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO
		};
		info.pNext = pNext;

		info.pBindings = bindings.data();
		info.bindingCount = (uint32_t)bindings.size();
		info.flags = flags;

		VkDescriptorSetLayout set;
		VK_CHECK(vkCreateDescriptorSetLayout(device, &info, nullptr, &set));

		return set;
	}

	void DescriptorAllocator::InitPool(VkDevice device, uint32_t maxSets, std::span<PoolSizeRatio> poolRatios)
	{
		PZ_CORE_TRACE("Initializing descriptor pool.");
		std::vector<VkDescriptorPoolSize> poolSizes;
		for (PoolSizeRatio ratio : poolRatios)
		{
			poolSizes.push_back(VkDescriptorPoolSize{
					.type = ratio.Type,
					.descriptorCount = (uint32_t)(ratio.Ratio * maxSets)
				});
		}

		VkDescriptorPoolCreateInfo poolInfo = { .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO };
		poolInfo.flags = 0;
		poolInfo.maxSets = maxSets;
		poolInfo.poolSizeCount = (uint32_t)poolSizes.size();
		poolInfo.pPoolSizes = poolSizes.data();

		vkCreateDescriptorPool(device, &poolInfo, nullptr, &Pool);
	}

	void DescriptorAllocator::ClearDescriptors(VkDevice device)
	{
		PZ_CORE_TRACE("Reseting descriptor pool.");
		vkResetDescriptorPool(device, Pool, 0);
	}

	void DescriptorAllocator::DestroyPool(VkDevice device)
	{
		PZ_CORE_TRACE("Destroying descriptor pool.");
		vkDestroyDescriptorPool(device, Pool, nullptr);
	}

	VkDescriptorSet DescriptorAllocator::Allocate(VkDevice device, VkDescriptorSetLayout layout)
	{
		PZ_CORE_TRACE("Allocating descriptor set.");
		VkDescriptorSetAllocateInfo allocInfo = { .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO };
		allocInfo.pNext = nullptr;
		allocInfo.descriptorPool = Pool;
		allocInfo.descriptorSetCount = 1;
		allocInfo.pSetLayouts = &layout;

		VkDescriptorSet ds;
		VK_CHECK(vkAllocateDescriptorSets(device, &allocInfo, &ds));

		return ds;
	}

}  // namespace pz