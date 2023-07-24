#pragma once

#include <string>
#include <vector>

#include "pzDevice.hpp"

namespace pz
{
    struct PipelineConfigInfo {};

    class PzPipeline
    {
        public:
            PzPipeline(PzDevice& device, const std::string& vertexShaderFilepath, const std::string& fragmentShaderFilepath, const PipelineConfigInfo& configInfo);
            ~PzPipeline() {}

            PzPipeline(const PzPipeline&) = delete;
            PzPipeline &operator=(const PzPipeline&) = delete;

            static PipelineConfigInfo defaultPipelineConfigInfo(uint32_t width, uint32_t height);

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