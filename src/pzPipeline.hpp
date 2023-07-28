#pragma once

#include <string>
#include <vector>

#include "pzDevice.hpp"

namespace pz
{
    struct PipelineConfigInfo 
    {
        VkViewport viewport;                                        // The viewport describes the region of the framebuffer that the output will be rendered to
        VkRect2D scissor;                                           // The scissor describes the region of the framebuffer that the output will be rendered to
        VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo;
        VkPipelineRasterizationStateCreateInfo rasterizationInfo;
        VkPipelineMultisampleStateCreateInfo multisampleInfo;
        VkPipelineColorBlendAttachmentState colorBlendAttachment;
        VkPipelineColorBlendStateCreateInfo colorBlendInfo;
        VkPipelineDepthStencilStateCreateInfo depthStencilInfo;
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

            static PipelineConfigInfo defaultPipelineConfigInfo(uint32_t width, uint32_t height);

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