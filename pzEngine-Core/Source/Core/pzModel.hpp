#pragma once

#include "pzDevice.hpp"
#include "pzUniformBuffer.hpp"

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
            glm::vec3 position{};
            glm::vec3 color{};
            glm::vec3 normal{};
            glm::vec2 uv{};

            static std::vector<VkVertexInputBindingDescription> getBindingDescriptions();
            static std::vector<VkVertexInputAttributeDescription> getAttributeDescriptions();

            bool operator==(const Vertex& other) const
            {
                return position == other.position && color == other.color && normal == other.normal && uv == other.uv;
            }
        };

        struct Data
        {
            std::vector<Vertex> vertices{};
            std::vector<uint32_t> indices{};

            void loadModel(const std::string &filepath);
        };

        PzModel(PzDevice &pzDevice, const PzModel::Data &data);
        ~PzModel();

        PzModel(const PzModel&) = delete;
        PzModel& operator=(const PzModel&) = delete;

        static std::unique_ptr<PzModel> createModelFromFile(PzDevice &pzDevice, const std::string &filepath);

        void bind(VkCommandBuffer commandBuffer);
        void draw(VkCommandBuffer commandBuffer);

        private:
            void createVertexBuffers(const std::vector<Vertex> &vertices);
            void createIndexBuffer(const std::vector<uint32_t> &indices);

            PzDevice &pzDevice;

            std::unique_ptr<PzBuffer> vertexBuffer;
            uint32_t vertexCount;

            bool hasIndexBuffer = false;
            std::unique_ptr<PzBuffer> indexBuffer;
            uint32_t indexCount;
    };

} // namespace pz