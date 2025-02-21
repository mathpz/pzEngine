// pre compiled header
#include "pzpch.hpp"

#include "Pipelines.hpp"

#include <Core/vk_initializers.h>

#include <fstream>

namespace vkutil
{
	VkShaderModule LoadShaderModule(const char* filePath, VkDevice device)
	{
        // open the file. With cursor at the end
        std::ifstream file(filePath, std::ios::ate | std::ios::binary);

        if (!file.is_open())
        {
            PZ_CORE_ERROR("Shader file did not open");
            std::exit(1);
        }

        // find what the size of the file is by looking up the location of the cursor
        // because the cursor is at the end, it gives the size directly in bytes
        size_t fileSize = (size_t)file.tellg();

        // spirv expects the buffer to be on uint32, so make sure to reserve a int
        // vector big enough for the entire file
        std::vector<uint32_t> buffer(fileSize / sizeof(uint32_t));

        file.seekg(0);
        file.read((char*)buffer.data(), fileSize);
        file.close();

        // create a new shader module, using the buffer we loaded
        VkShaderModuleCreateInfo createInfo = {};
        createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        createInfo.pNext = nullptr;

        // codeSize has to be in bytes, so multply the ints in the buffer by size of
        // int to know the real size of the buffer
        createInfo.codeSize = buffer.size() * sizeof(uint32_t);
        createInfo.pCode = buffer.data();

        // check that the creation goes well.
        VkShaderModule shaderModule;
        if (vkCreateShaderModule(device, &createInfo, nullptr, &shaderModule) != VK_SUCCESS)
        {
            PZ_CORE_ERROR("Failed to create shader module.");
            std::exit(1);
        }

        return shaderModule;
	}

    VkPipelineLayout CreatePipelineLayout(VkDevice device, std::span<const VkDescriptorSetLayout> layouts, std::span<const VkPushConstantRange> pushConstantRanges)
    {
        VkPipelineLayoutCreateInfo pipeLayoutCI = VkPipelineLayoutCreateInfo{
            .sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
            .setLayoutCount = (uint32_t)layouts.size(),
            .pSetLayouts = layouts.data(),
            .pushConstantRangeCount = (uint32_t)pushConstantRanges.size(),
            .pPushConstantRanges = pushConstantRanges.data(),
        };

        VkPipelineLayout layout;
        VK_CHECK(vkCreatePipelineLayout(device, &pipeLayoutCI, nullptr, &layout));
        return layout;
    }
}


namespace pz
{
    PipelineBuilder::PipelineBuilder(VkPipelineLayout pipelineLayout)
    {
        /// TODO
        m_PipelineLayout = pipelineLayout;
        Clear();
    }

    VkPipeline PipelineBuilder::Build(VkDevice device)
    {
        /// TODO
        VkPipeline pipeline;
        return pipeline;
    }

    PipelineBuilder& PipelineBuilder::SetShaders(VkShaderModule vertexShader, VkShaderModule fragmentShader)
    {
        /// TODO
        return *this;
    }

    PipelineBuilder& PipelineBuilder::SetShaders(VkShaderModule vertexShader, VkShaderModule geometryShader, VkShaderModule fragmentShader)
    {
        /// TODO
        return *this;
    }

    PipelineBuilder& PipelineBuilder::SetInputTopology(VkPrimitiveTopology topology)
    {
        /// TODO
        return *this;
    }

    PipelineBuilder& PipelineBuilder::SetPolygonMode(VkPolygonMode mode)
    {
        /// TODO
        return *this;
    }

    PipelineBuilder& PipelineBuilder::SetCullMode(VkPolygonMode mode)
    {
        /// TODO
        return *this;
    }

    PipelineBuilder& PipelineBuilder::EnableCulling()
    {
        /// TODO
        return *this;
    }

    PipelineBuilder& PipelineBuilder::DisableCulling()
    {
        /// TODO
        return *this;
    }

    PipelineBuilder& PipelineBuilder::SetMultisamplingNone()
    {
        /// TODO
        return *this;
    }

    PipelineBuilder& PipelineBuilder::SetMultisampling(VkSampleCountFlagBits samples)
    {
        /// TODO
        return *this;
    }

    PipelineBuilder& PipelineBuilder::DisableBlending()
    {
        /// TODO
        return *this;
    }

    PipelineBuilder& PipelineBuilder::EnableBlending(VkBlendOp blendOp, VkBlendFactor srcBlendFactor, VkBlendFactor dstBlendFactor, VkBlendFactor srcAlphaBlendFactor, VkBlendFactor dstAlphaBlendFactor)
    {
        /// TODO
        return *this;
    }

    PipelineBuilder& PipelineBuilder::SetColorAttachmentFormat(VkFormat format)
    {
        /// TODO
        return *this;
    }

    PipelineBuilder& PipelineBuilder::SetDepthFormat(VkFormat format)
    {
        /// TODO
        return *this;
    }

    PipelineBuilder& PipelineBuilder::EnableDepthTest(bool depthWriteEnable, VkCompareOp op)
    {
        /// TODO
        return *this;
    }

    PipelineBuilder& PipelineBuilder::EnableDepthClamp()
    {
        /// TODO
        return *this;
    }

    PipelineBuilder& PipelineBuilder::DisableDepthTest()
    {
        /// TODO
        return *this;
    }

    PipelineBuilder& PipelineBuilder::EnableDynamicDepth()
    {
        /// TODO
        return *this;
    }

    PipelineBuilder& PipelineBuilder::EnableDepthBias(float constantFactor, float slopeFactor)
    {
        /// TODO
        return *this;
    }

    void PipelineBuilder::Clear()
    {
        /// TODO
    }

    ComputePipelineBuilder::ComputePipelineBuilder(VkPipelineLayout pipelineLayout)
        :m_PipelineLayout(pipelineLayout)
    {
    }

    ComputePipelineBuilder& ComputePipelineBuilder::SetShader(VkShaderModule shaderModule)
    {
        m_ShaderModule = shaderModule;
        return *this;
    }

    VkPipeline ComputePipelineBuilder::build(VkDevice device)
    {
        VkComputePipelineCreateInfo pipeCI = VkComputePipelineCreateInfo{
            .sType = VK_STRUCTURE_TYPE_COMPUTE_PIPELINE_CREATE_INFO,
            .stage = {
                .sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
                .stage = VK_SHADER_STAGE_COMPUTE_BIT,
                .module = m_ShaderModule,
                .pName = "main",
            },
            .layout = m_PipelineLayout,
        };

        VkPipeline pipeline;
        VK_CHECK(vkCreateComputePipelines(device, VK_NULL_HANDLE, 1, &pipeCI, 0, &pipeline));
        PZ_CORE_TRACE("Compute pipeline  built.");

        return pipeline;
    }
} // namespace pz

