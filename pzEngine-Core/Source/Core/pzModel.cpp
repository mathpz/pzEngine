#include "pzpch.hpp"

#include "pzModel.hpp"
#include "pzUtils.hpp"

#define TINYOBJLOADER_IMPLEMENTATION
#include <../vendor/tinyobjloader/tiny_obj_loader.h> // ! TODO refactor this
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/hash.hpp>

namespace std
{
    template <>
    struct hash<pz::PzModel::Vertex>
    {
        size_t operator()(pz::PzModel::Vertex const& vertex) const
        {
            size_t seed = 0;
            pz::hashCombine(seed, vertex.position, vertex.color, vertex.normal, vertex.uv);
            return seed;
        }
    };
} // namespace std

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

    std::unique_ptr<PzModel> PzModel::createModelFromFile(PzDevice& pzDevice, const std::string& filepath)
    {
        Data data{};
        data.loadModel(filepath);

        std::cout << "Model has " << data.vertices.size() << " vertices\n";
        return std::make_unique<PzModel>(pzDevice, data);
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
        std::vector<VkVertexInputAttributeDescription> attributeDescriptions{};

        attributeDescriptions.push_back({0,0, VK_FORMAT_R32G32B32_SFLOAT, offsetof(Vertex, position)});
        attributeDescriptions.push_back({1,0, VK_FORMAT_R32G32B32_SFLOAT, offsetof(Vertex, color)});
        attributeDescriptions.push_back({2,0, VK_FORMAT_R32G32B32_SFLOAT, offsetof(Vertex, normal)});
        attributeDescriptions.push_back({3,0, VK_FORMAT_R32G32_SFLOAT, offsetof(Vertex, uv)});

        return attributeDescriptions;

    }

    void PzModel::Data::loadModel(const std::string& filepath)
    {
        tinyobj::attrib_t attrib;
        std::vector<tinyobj::shape_t> shapes;
        std::vector<tinyobj::material_t> materials;
        std::string warn, err;

        if(!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, filepath.c_str()))
        {
            std::cout << warn + err << std::endl;
            throw std::runtime_error(warn + err);
        }
        
        vertices.clear();
        indices.clear();


        std::unordered_map<Vertex, uint32_t> uniqueVertices{};
        for (const auto& shape : shapes)
        {
            for (const auto& index : shape.mesh.indices)
            {
                Vertex vertex{};

                if (index.vertex_index >= 0)
                {
                    vertex.position = { attrib.vertices[3 * index.vertex_index + 0],
                                        attrib.vertices[3 * index.vertex_index + 1],
                                        attrib.vertices[3 * index.vertex_index + 2],
                    };
                
                    vertex.color = {    attrib.colors[3 * index.vertex_index + 0],
                                        attrib.colors[3 * index.vertex_index + 1],
                                        attrib.colors[3 * index.vertex_index + 2],
                    };
                }

                if (index.normal_index >= 0)
                {
                    vertex.normal = {   attrib.normals[3 * index.normal_index + 0],
                                        attrib.normals[3 * index.normal_index + 1],
                                        attrib.normals[3 * index.normal_index + 2],
                    };
                }

                if (index.texcoord_index >= 0)
                {
                    vertex.uv = {       attrib.texcoords[2 * index.texcoord_index + 0],
                                        attrib.texcoords[2 * index.texcoord_index + 1]
                    };
                }
            
                if (uniqueVertices.count(vertex) == 0)
                {
                    uniqueVertices[vertex] = static_cast<uint32_t>(vertices.size());
                    vertices.push_back(vertex);
                }
                
                indices.push_back(uniqueVertices[vertex]);
            }
        }
    }
} // namespace pz