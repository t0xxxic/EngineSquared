/*
** EPITECH PROJECT, 2024
** Title: EngineSquared
** Author: MasterLaplace
** Created: 2024-11-03
** File description:
** VertexBuffer
*/

#include "VertexBuffer.hpp"

namespace ES::Plugin::Wrapper {

void VertexBuffer::Create(const VkDevice &device, const VkPhysicalDevice &physicalDevice)
{
    VkBufferCreateInfo bufferInfo{};
    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferInfo.size = sizeof(VERTICES[0]) * VERTICES.size();
    bufferInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
    bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    if (vkCreateBuffer(device, &bufferInfo, nullptr, &_buffer) != VK_SUCCESS)
        throw VkWrapperError("failed to create vertex buffer!");

    VkMemoryRequirements memRequirements{};
    vkGetBufferMemoryRequirements(device, _buffer, &memRequirements);

    VkMemoryAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex =
        FindMemoryType(physicalDevice, memRequirements.memoryTypeBits,
                       VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

    if (vkAllocateMemory(device, &allocInfo, nullptr, &_bufferMemory) != VK_SUCCESS)
        throw VkWrapperError("failed to allocate vertex buffer memory!");

    vkBindBufferMemory(device, _buffer, _bufferMemory, 0);

    void *data;
    vkMapMemory(device, _bufferMemory, 0, bufferInfo.size, 0, &data);
    memcpy(data, VERTICES.data(), bufferInfo.size);
    vkUnmapMemory(device, _bufferMemory);
}

void VertexBuffer::Destroy(const VkDevice &device)
{
    vkDestroyBuffer(device, _buffer, nullptr);
    vkFreeMemory(device, _bufferMemory, nullptr);
}

uint32_t VertexBuffer::FindMemoryType(const VkPhysicalDevice &physicalDevice, const uint32_t typeFilter,
                                      const VkMemoryPropertyFlags properties)
{
    VkPhysicalDeviceMemoryProperties memProperties{};
    vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memProperties);

    for (uint32_t i = 0; i < memProperties.memoryTypeCount; ++i)
        if (typeFilter & (1 << i) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties)
            return i;

    throw VkWrapperError("failed to find suitable memory type!");
}

} // namespace ES::Plugin::Wrapper
