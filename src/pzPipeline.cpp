#include "pzPipeline.hpp"

// std
#include <fstream>
#include <stdexcept>
#include <iostream>

namespace pz
{
    PzPipeline::PzPipeline(PzDevice& device, const std::string& vertexShaderFilepath, const std::string& fragmentShaderFilepath, const PipelineConfigInfo& configInfo)
    : pzDevice(device)
    {
        createGraphicsPipeline(vertexShaderFilepath, fragmentShaderFilepath, configInfo);
    }

    std::vector<char> PzPipeline::readFile(const std::string& filepath)
    {
        std::ifstream file(filepath, std::ios::ate | std::ios::binary);

        if(!file.is_open())
        {
            throw std::runtime_error("failed to open file: " + filepath);
        }

        size_t fileSize = static_cast<size_t>(file.tellg());
        std::vector<char> buffer(fileSize);

        file.seekg(0);
        file.read(buffer.data(), fileSize);
        
        file.close();
        return buffer;
    }

    void PzPipeline::createGraphicsPipeline(const std::string& vertexShaderFilepath, const std::string& fragmentShaderFilepath, const PipelineConfigInfo& configInfo)
    {
        auto vertexShaderCode = readFile(vertexShaderFilepath);
        auto fragmentShaderCode = readFile(fragmentShaderFilepath);

        std::cout << "Vertex Shader Code Size: " << vertexShaderCode.size() << std::endl;
        std::cout << "Fragment Shader Code Size: " << fragmentShaderCode.size() << std::endl;
    }

    void PzPipeline::createShaderModule(const std::vector<char> &code, VkShaderModule *shaderModule)
    {
        VkShaderModuleCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
        createInfo.codeSize = code.size();
        createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

        if(vkCreateShaderModule(pzDevice.device(), &createInfo, nullptr, shaderModule) != VK_SUCCESS)
        {
            throw std::runtime_error("failed to create shader module!");
        }
    }

    PipelineConfigInfo PzPipeline::defaultPipelineConfigInfo(uint32_t width, uint32_t height)
    {
        PipelineConfigInfo configInfo{};

        return configInfo;
    }

} // namespace pz