#pragma once

#include <array>

#include <vulkan/vulkan.h>

#include "Core/Renderer/Vulkan/Common.hpp"

namespace pz
{
	class GFXDevice;

	class BackgroundPipeline
	{
	public:
		void Init(GFXDevice& gfxdevice);
		void Cleanup(VkDevice gfxdevice);

		void Draw(VkCommandBuffer cmd, GFXDevice& gfxDevice, VkExtent2D extent);

	private:
		VkPipelineLayout m_BackgroundPipelineLayout;
		VkPipeline m_BackgroundPipeline;


	};
} // namespace pz