#include "Buffer.hpp"

namespace ES::Plugin::Wrapper {

void Buffers::Create(const VkDevice &device, const VkPhysicalDevice &physicalDevice, const VkCommandPool &commandPool,
                     const VkQueue &graphicsQueue, const std::vector<VkImage> &swapChainImages)
{
    CreateVertexBuffer(device, physicalDevice, commandPool, graphicsQueue);
    CreateIndexBuffer(device, physicalDevice, commandPool, graphicsQueue);
}

void Buffers::CreateVertexBuffer(const VkDevice &device, const VkPhysicalDevice &physicalDevice,
                                 const VkCommandPool &commandPool, const VkQueue &graphicsQueue)
{
    VkDeviceSize bufferSize = sizeof(VERTICES[0]) * VERTICES.size();

    VkBuffer stagingBuffer{};
    VkDeviceMemory stagingBufferMemory{};

    CreateBuffer(device, physicalDevice, bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
                 VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer,
                 stagingBufferMemory);

    void *data = nullptr;
    vkMapMemory(device, stagingBufferMemory, 0, bufferSize, 0, &data);
    memcpy(data, VERTICES.data(), bufferSize);
    vkUnmapMemory(device, stagingBufferMemory);

    CreateBuffer(device, physicalDevice, bufferSize,
                 VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
                 VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, _vertexBuffer, _vertexBufferMemory);

    CopyBuffer(device, commandPool, graphicsQueue, stagingBuffer, _vertexBuffer, bufferSize);

    vkDestroyBuffer(device, stagingBuffer, nullptr);
    vkFreeMemory(device, stagingBufferMemory, nullptr);
}

void Buffers::CreateIndexBuffer(const VkDevice &device, const VkPhysicalDevice &physicalDevice,
                                const VkCommandPool &commandPool, const VkQueue &graphicsQueue)
{
    VkDeviceSize bufferSize = sizeof(INDICES[0]) * INDICES.size();

    VkBuffer stagingBuffer{};
    VkDeviceMemory stagingBufferMemory{};

    CreateBuffer(device, physicalDevice, bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
                 VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer,
                 stagingBufferMemory);

    void *data = nullptr;
    vkMapMemory(device, stagingBufferMemory, 0, bufferSize, 0, &data);
    memcpy(data, INDICES.data(), bufferSize);
    vkUnmapMemory(device, stagingBufferMemory);

    CreateBuffer(device, physicalDevice, bufferSize,
                 VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
                 VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, _indexBuffer, _indexBufferMemory);

    CopyBuffer(device, commandPool, graphicsQueue, stagingBuffer, _indexBuffer, bufferSize);

    vkDestroyBuffer(device, stagingBuffer, nullptr);
    vkFreeMemory(device, stagingBufferMemory, nullptr);
}

void Buffers::Destroy(const VkDevice &device)
{
    vkDestroyBuffer(device, _indexBuffer, nullptr);
    vkFreeMemory(device, _indexBufferMemory, nullptr);

    vkDestroyBuffer(device, _vertexBuffer, nullptr);
    vkFreeMemory(device, _vertexBufferMemory, nullptr);
}

void Buffers::CreateBuffer(const VkDevice &device, const VkPhysicalDevice &physicalDevice, const VkDeviceSize size,
                           const VkBufferUsageFlags usage, const VkMemoryPropertyFlags properties, VkBuffer &buffer,
                           VkDeviceMemory &bufferMemory)
{
    VkBufferCreateInfo bufferInfo{};
    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferInfo.size = size;
    bufferInfo.usage = usage;
    bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    if (vkCreateBuffer(device, &bufferInfo, nullptr, &buffer) != VK_SUCCESS)
        throw VkWrapperError("failed to create vertex buffer!");

    VkMemoryRequirements memRequirements{};
    vkGetBufferMemoryRequirements(device, buffer, &memRequirements);

    VkMemoryAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = FindMemoryType(physicalDevice, memRequirements.memoryTypeBits, properties);

    if (vkAllocateMemory(device, &allocInfo, nullptr, &bufferMemory) != VK_SUCCESS)
        throw VkWrapperError("failed to allocate vertex buffer memory!");

    vkBindBufferMemory(device, buffer, bufferMemory, 0);
}

uint32_t Buffers::FindMemoryType(const VkPhysicalDevice &physicalDevice, const uint32_t typeFilter,
                                 const VkMemoryPropertyFlags properties)
{
    VkPhysicalDeviceMemoryProperties memProperties{};
    vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memProperties);

    for (uint32_t i = 0; i < memProperties.memoryTypeCount; ++i)
        if (typeFilter & (1 << i) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties)
            return i;

    throw VkWrapperError("failed to find suitable memory type!");
}

void Buffers::CopyBuffer(const VkDevice &device, const VkCommandPool &commandPool, const VkQueue &graphicsQueue,
                         const VkBuffer &srcBuffer, const VkBuffer &dstBuffer, VkDeviceSize size)
{
    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandPool = commandPool;
    allocInfo.commandBufferCount = 1;

    VkCommandBuffer commandBuffer{};
    vkAllocateCommandBuffers(device, &allocInfo, &commandBuffer);

    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

    vkBeginCommandBuffer(commandBuffer, &beginInfo);

    VkBufferCopy copyRegion{};
    copyRegion.size = size;
    vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, 1, &copyRegion);

    vkEndCommandBuffer(commandBuffer);

    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &commandBuffer;

    vkQueueSubmit(graphicsQueue, 1, &submitInfo, VK_NULL_HANDLE);
    vkQueueWaitIdle(graphicsQueue);

    vkFreeCommandBuffers(device, commandPool, 1, &commandBuffer);
}

} // namespace ES::Plugin::Wrapper
