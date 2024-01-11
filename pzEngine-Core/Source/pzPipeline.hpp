#pragma once

#include <string>
#include <vector>

#include "pzDevice.hpp"

namespace pz
{
    struct PipelineConfigInfo 
    {
        PipelineConfigInfo(const PipelineConfigInfo&) = delete;
        PipelineConfigInfo &operator=(const PipelineConfigInfo&) = delete;

        VkPipelineViewportStateCreateInfo viewportInfo;             
        VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo;
        VkPipelineRasterizationStateCreateInfo rasterizationInfo;
        VkPipelineMultisampleStateCreateInfo multisampleInfo;
        VkPipelineColorBlendAttachmentState colorBlendAttachment;
        VkPipelineColorBlendStateCreateInfo colorBlendInfo;
        VkPipelineDepthStencilStateCreateInfo depthStencilInfo;
        std::vector<VkDynamicState> dynamicStateEnables;
        VkPipelineDynamicStateCreateInfo dynamicStateInfo;
        VkPipelineLayout pipelineLayout = nullptr;
        VkRenderPass renderPass = nullptr;
        uint32_t subpass = 0;
    };

    class PzPipeline
    {
        public:
            PzPipeline(PzDevice& device, const std::string& vertexShaderFilepath, const std::string& fragmentShaderFilepath, const PipelineConfigInfo& configInfo);
            ~PzPipeline();

            PzPipeline(const PzPipeline&) = delete;
            PzPipeline &operator=(const PzPipeline&) = delete;
            PzPipeline() = default;

            static void defaultPipelineConfigInfo(PipelineConfigInfo& configInfo);

            void bind(VkCommandBuffer commandBuffer);

        private:
            static std::vector<char> readFile(const std::string& filepath);

            void createGraphicsPipeline(const std::string& vertexShaderFilepath, const std::string& fragmentShaderFilepath, const PipelineConfigInfo& configInfo);
            
            void createShaderModule(const std::vector<char>& code, VkShaderModule* shaderModule);

            PzDevice& pzDevice;
            VkPipeline graphicsPipeline;
            VkShaderModule vertexShaderModule;
            VkShaderModule fragmentShaderModule;
    };
} // namespace pz