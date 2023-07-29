#pragma once

#include "pzDevice.hpp"

// libs
#define  GLM_FORCE_RADIANS
#define  GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

namespace pz
{
    class PzModel
    {
        public:
        
        struct Vertex
        {
            glm::vec2 position;

            static std::vector<VkVertexInputBindingDescription> getBindingDescriptions();
            static std::vector<VkVertexInputAttributeDescription> getAttributeDescriptions();
        };

        PzModel(PzDevice &pzDevice, const std::vector<Vertex> &vertices);
        ~PzModel();	

        PzModel(const PzModel&) = delete;
        PzModel& operator=(const PzModel&) = delete;

        void bind(VkCommandBuffer commandBuffer);
        void draw(VkCommandBuffer commandBuffer);

        private:
            void createVertexBuffers(const std::vector<Vertex> &vertices);

            PzDevice &pzDevice;
            VkBuffer vertexBuffer;
            VkDeviceMemory vertexBufferMemory;
            uint32_t vertexCount;
    };

} // namespace pz