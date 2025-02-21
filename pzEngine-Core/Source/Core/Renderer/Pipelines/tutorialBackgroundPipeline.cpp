#include "pzpch.hpp"

#include "Core/Renderer/Vulkan/GFXDevice.hpp"
#include "Core/Renderer/Vulkan/Pipelines.hpp"
#include "Core/Renderer/Pipelines/tutorialBackgroundPipeline.hpp"
#include "Core/pzUtils.hpp"

namespace pz
{
	void BackgroundPipeline::Init(GFXDevice& gfxdevice)
	{
		PZ_CORE_TRACE("Initializing background pipeline.");
		VkDevice device = gfxdevice.GetDevice();

		const auto setLayouts = std::array{ gfxdevice.m_DrawImageDescriptorSetLayout };
		m_BackgroundPipelineLayout = vkutil::CreatePipelineLayout(device, setLayouts, {});

		VkShaderModule shader = vkutil::LoadShaderModule("E:/programmingProjects/pzEngine/pzEngine-Core/Assets/Shaders/gradient.comp.spv", device);
		/// TODO: addDebugLabel

		m_BackgroundPipeline = ComputePipelineBuilder{ m_BackgroundPipelineLayout }.SetShader(shader).build(device);

		vkDestroyShaderModule(device, shader, nullptr);

		PZ_CORE_TRACE("Background pipeline initialized.");
	}

	void BackgroundPipeline::Cleanup(VkDevice device)
	{
		PZ_CORE_TRACE("Cleaning background pipelines.");

		vkDestroyPipelineLayout(device, m_BackgroundPipelineLayout, nullptr);
		vkDestroyPipeline(device, m_BackgroundPipeline, nullptr);

		PZ_CORE_TRACE("Background pipelines cleaned.");
	}

	void BackgroundPipeline::Draw(VkCommandBuffer cmd, GFXDevice& gfxDevice, VkExtent2D extent)
	{
		vkCmdBindPipeline(cmd, VK_PIPELINE_BIND_POINT_COMPUTE, m_BackgroundPipeline);

		vkCmdBindDescriptorSets(cmd, VK_PIPELINE_BIND_POINT_COMPUTE, m_BackgroundPipelineLayout, 0, 1, &gfxDevice.m_DrawImageDescriptors, 0, nullptr);

		static const auto workgroupSize = 16.0;

		// execute the compute pipeline dispatch. We are using 16x16 workgroup size so we need to divide by it
		vkCmdDispatch(cmd, std::ceil(extent.width  / workgroupSize), std::ceil(extent.height / workgroupSize), 1);
	}

} // namespace pz