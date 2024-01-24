#include "pzpch.hpp"

#include "pzModel.hpp"


namespace pz
{
    PzModel::PzModel(PzDevice &pzDevice, const PzModel::Data& data) : pzDevice{pzDevice}
    {
        createVertexBuffers(data.vertices);
        createIndexBuffer(data.indices);
    }

    PzModel::~PzModel()
    {
        vkDestroyBuffer(pzDevice.device(), vertexBuffer, nullptr);
        vkFreeMemory(pzDevice.device(), vertexBufferMemory, nullptr);

        if(hasIndexBuffer)
		{
			vkDestroyBuffer(pzDevice.device(), indexBuffer, nullptr);
			vkFreeMemory(pzDevice.device(), indexBufferMemory, nullptr);
		}
    }

    void PzModel::createVertexBuffers(const std::vector<Vertex> &vertices)
    {
        vertexCount = static_cast<uint32_t>(vertices.size());
        assert(vertexCount >= 3 && "Vertex count must be at least 3");

        VkDeviceSize bufferSize = sizeof(vertices[0]) * vertexCount;

        VkBuffer staginfBuffer;
        VkDeviceMemory stagingBufferMemory;

        pzDevice.createBuffer(
            bufferSize,
            VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
            staginfBuffer,
            stagingBufferMemory);

        // this maps the vertex buffer memory to the CPU | Host -> Device
        void *data;
        vkMapMemory(pzDevice.device(), stagingBufferMemory, 0, bufferSize, 0, &data);
        memcpy(data, vertices.data(), (size_t) bufferSize);
        vkUnmapMemory(pzDevice.device(), stagingBufferMemory);

        pzDevice.createBuffer(
            bufferSize,
            VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
            VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
            vertexBuffer,
            vertexBufferMemory);
            
        pzDevice.copyBuffer(staginfBuffer, vertexBuffer, bufferSize);

        vkDestroyBuffer(pzDevice.device(), staginfBuffer, nullptr);
        vkFreeMemory(pzDevice.device(), stagingBufferMemory, nullptr);
    
    }

    void PzModel::createIndexBuffer(const std::vector<uint32_t>& indices)
    {
        indexCount = static_cast<uint32_t>(indices.size());
        hasIndexBuffer = indexCount > 0;

        if(!hasIndexBuffer)
			return;

        VkDeviceSize bufferSize = sizeof(indices[0]) * indexCount;

        VkBuffer staginfBuffer;
        VkDeviceMemory stagingBufferMemory;

        pzDevice.createBuffer(
            bufferSize,
            VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
            staginfBuffer,
            stagingBufferMemory);

        // this maps the index buffer memory to the CPU | Host -> Device
        void* data;
        vkMapMemory(pzDevice.device(), stagingBufferMemory, 0, bufferSize, 0, &data);
        memcpy(data, indices.data(), (size_t)bufferSize);
        vkUnmapMemory(pzDevice.device(), stagingBufferMemory);

        pzDevice.createBuffer(
            bufferSize,
            VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
            VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
            indexBuffer,
            indexBufferMemory);

        pzDevice.copyBuffer(staginfBuffer, indexBuffer, bufferSize);

        vkDestroyBuffer(pzDevice.device(), staginfBuffer, nullptr);
        vkFreeMemory(pzDevice.device(), stagingBufferMemory, nullptr);
    }

    void PzModel::draw(VkCommandBuffer commandBuffer)
    {
        if (hasIndexBuffer)
        {
			vkCmdDrawIndexed(commandBuffer, indexCount, 1, 0, 0, 0);
        }
        else
        {
            vkCmdDraw(commandBuffer, vertexCount, 1, 0, 0);
        }
    }

    void PzModel::bind(VkCommandBuffer commandBuffer)
    {
        VkBuffer buffers[] = {vertexBuffer};
        VkDeviceSize offsets[] = {0};
        vkCmdBindVertexBuffers(commandBuffer, 0, 1, buffers, offsets);
        
        if (hasIndexBuffer)
        {
            vkCmdBindIndexBuffer(commandBuffer, indexBuffer, 0, VK_INDEX_TYPE_UINT32);
        }

    }

    std::vector<VkVertexInputBindingDescription> PzModel::Vertex::getBindingDescriptions()
    {
        std::vector<VkVertexInputBindingDescription> bindingDescriptions(1);

        bindingDescriptions[0].binding = 0;
        bindingDescriptions[0].stride = sizeof(Vertex);
        bindingDescriptions[0].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

        return bindingDescriptions;
    }

    std::vector<VkVertexInputAttributeDescription> PzModel::Vertex::getAttributeDescriptions()
    {
        std::vector<VkVertexInputAttributeDescription> attributeDescriptions(2);

        // position
        attributeDescriptions[0].binding = 0;
        attributeDescriptions[0].location = 0;
        attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
        attributeDescriptions[0].offset = offsetof(Vertex, position);

        // colors
        attributeDescriptions[1].binding = 0;
        attributeDescriptions[1].location = 1;
        attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
        attributeDescriptions[1].offset = offsetof(Vertex, color);

        return attributeDescriptions;

    }
} // namespace pz