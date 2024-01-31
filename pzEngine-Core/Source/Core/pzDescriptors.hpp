#pragma once

#include "pzDevice.hpp"

namespace pz {

    class PzDescriptorSetLayout {
    public:
        class Builder {
        public:
            Builder(PzDevice& pzDevice) : pzDevice{ pzDevice } {}

            Builder& addBinding(
                uint32_t binding,
                VkDescriptorType descriptorType,
                VkShaderStageFlags stageFlags,
                uint32_t count = 1);
            std::unique_ptr<PzDescriptorSetLayout> build() const;

        private:
            PzDevice& pzDevice;
            std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> bindings{};
        };

        PzDescriptorSetLayout(
            PzDevice& pzDevice, std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> bindings);
        ~PzDescriptorSetLayout();
        PzDescriptorSetLayout(const PzDescriptorSetLayout&) = delete;
        PzDescriptorSetLayout& operator=(const PzDescriptorSetLayout&) = delete;

        VkDescriptorSetLayout getDescriptorSetLayout() const { return descriptorSetLayout; }

    private:
        PzDevice& pzDevice;
        VkDescriptorSetLayout descriptorSetLayout;
        std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> bindings;

        friend class PzDescriptorWriter;
    };

    class PzDescriptorPool {
    public:
        class Builder {
        public:
            Builder(PzDevice& pzDevice) : pzDevice{ pzDevice } {}

            Builder& addPoolSize(VkDescriptorType descriptorType, uint32_t count);
            Builder& setPoolFlags(VkDescriptorPoolCreateFlags flags);
            Builder& setMaxSets(uint32_t count);
            std::unique_ptr<PzDescriptorPool> build() const;

        private:
            PzDevice& pzDevice;
            std::vector<VkDescriptorPoolSize> poolSizes{};
            uint32_t maxSets = 1000;
            VkDescriptorPoolCreateFlags poolFlags = 0;
        };

        VkDescriptorPool& getDescriptorPool() { return descriptorPool; }
        PzDescriptorPool(
            PzDevice& pzDevice,
            uint32_t maxSets,
            VkDescriptorPoolCreateFlags poolFlags,
            const std::vector<VkDescriptorPoolSize>& poolSizes);
        ~PzDescriptorPool();
        PzDescriptorPool(const PzDescriptorPool&) = delete;
        PzDescriptorPool& operator=(const PzDescriptorPool&) = delete;

        bool allocateDescriptor(
            const VkDescriptorSetLayout descriptorSetLayout, VkDescriptorSet& descriptor) const;

        void freeDescriptors(std::vector<VkDescriptorSet>& descriptors) const;

        void resetPool();

    private:
        PzDevice& pzDevice;
        VkDescriptorPool descriptorPool;

        friend class PzDescriptorWriter;
    };

    class PzDescriptorWriter {
    public:
        PzDescriptorWriter(PzDescriptorSetLayout& setLayout, PzDescriptorPool& pool);

        PzDescriptorWriter& writeBuffer(uint32_t binding, VkDescriptorBufferInfo* bufferInfo);
        PzDescriptorWriter& writeImage(uint32_t binding, VkDescriptorImageInfo* imageInfo);

        bool build(VkDescriptorSet& set);
        void overwrite(VkDescriptorSet& set);

    private:
        PzDescriptorSetLayout& setLayout;
        PzDescriptorPool& pool;
        std::vector<VkWriteDescriptorSet> writes;
    };

}  // namespace pz