#pragma once

/// Inspired a lot by Edbr's implementation

#include <vulkan/vulkan.h>

#include <span>

namespace vkutil
{
	VkShaderModule LoadShaderModule(const char* filePath, VkDevice device);
	VkPipelineLayout CreatePipelineLayout(VkDevice device, std::span<const VkDescriptorSetLayout> layouts = {}, std::span<const VkPushConstantRange> pushConstantRanges = {});
} // namespace vkutil

namespace pz
{
	class PipelineBuilder
	{
	public:
		PipelineBuilder(VkPipelineLayout pipelineLayout);
		VkPipeline Build(VkDevice device);

		PipelineBuilder& SetShaders(VkShaderModule vertexShader, VkShaderModule fragmentShader);
		PipelineBuilder& SetShaders(VkShaderModule vertexShader, VkShaderModule geometryShader, VkShaderModule fragmentShader);

		PipelineBuilder& SetInputTopology(VkPrimitiveTopology topology);
		PipelineBuilder& SetPolygonMode(VkPolygonMode mode);
		PipelineBuilder& SetCullMode(VkPolygonMode mode);
		PipelineBuilder& EnableCulling();	// defaults for culling
		PipelineBuilder& DisableCulling();

		PipelineBuilder& SetMultisamplingNone();
		PipelineBuilder& SetMultisampling(VkSampleCountFlagBits samples);
		PipelineBuilder& DisableBlending();
		PipelineBuilder& EnableBlending(
			VkBlendOp blendOp = VK_BLEND_OP_ADD,
			VkBlendFactor srcBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA,
			VkBlendFactor dstBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA,
			VkBlendFactor srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE,
			VkBlendFactor dstAlphaBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA);

		PipelineBuilder& SetColorAttachmentFormat(VkFormat format);
		PipelineBuilder& SetDepthFormat(VkFormat format);
		PipelineBuilder& EnableDepthTest(bool depthWriteEnable, VkCompareOp op);
		PipelineBuilder& EnableDepthClamp();
		PipelineBuilder& DisableDepthTest();
		PipelineBuilder& EnableDynamicDepth();
		PipelineBuilder& EnableDepthBias(float constantFactor, float slopeFactor);

	private:
		void Clear();
		std::vector<VkPipelineShaderStageCreateInfo> m_ShaderStages;
		VkPipelineInputAssemblyStateCreateInfo m_InputAssembly;
		VkPipelineRasterizationStateCreateInfo m_Rasterizer;
		VkPipelineColorBlendAttachmentState m_ColorBlendAttachment;
		VkPipelineMultisampleStateCreateInfo m_Multisampling;
		VkPipelineDepthStencilStateCreateInfo m_DepthStencil;
		VkPipelineRenderingCreateInfo m_RenderInfo;
		VkFormat m_ColorAttachmentformat;
		VkPipelineLayout m_PipelineLayout;

		bool m_DynamicDepth;
	};

	class ComputePipelineBuilder
	{
	public:
		ComputePipelineBuilder(VkPipelineLayout pipelineLayout);
		ComputePipelineBuilder& SetShader(VkShaderModule shaderModule);

		VkPipeline build(VkDevice device);

	private:
		VkPipelineLayout m_PipelineLayout;
		VkShaderModule m_ShaderModule;
	};
} // namespace pz

	// VkPipelineLayout CreatePipelineLayout(VkDevice device, std::span<const VkDescriptorSetLayout> layouts = {},
	// 	std::span<const VkPushConstantRange> pushConstantRanges = {});