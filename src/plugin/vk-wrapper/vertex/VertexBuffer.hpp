/**************************************************************************
 * VkWrapper v0.0.0
 *
 * VkWrapper is a software package, part of the Engine².
 *
 * This file is part of the VkWrapper project that is under GPL-3.0 License.
 * Copyright © 2024 by @EngineSquared, All rights reserved.
 *
 * VkWrapper is a free software: you can redistribute it and/or modify
 * it under the terms of the GPL-3.0 License as published by the
 * Free Software Foundation. See the GPL-3.0 License for more details.
 *
 * @file VertexBuffer.hpp
 * @brief VertexBuffer class declaration.
 *
 *
 * @author @MasterLaplace
 * @version 0.0.0
 * @date 2024-11-03
 **************************************************************************/

#ifndef VERTEXBUFFER_HPP_
#define VERTEXBUFFER_HPP_

#include "Vertex.hpp"

namespace ES::Plugin::Wrapper {

/**
 * @brief VertexBuffer class.
 *
 * This class is used to represent a vertex buffer in the Vulkan API.
 * It contains the buffer and buffer memory.
 *
 * @example
 * @code
 * VertexBuffer vertexBuffer;
 * vertexBuffer.Create(device, physicalDevice);
 * vertexBuffer.Destroy(device);
 * @endcode
 */
class VertexBuffer {
  public:
    /**
     * @brief Create the VertexBuffer object and the IndexBuffer object.
     *
     * @param device The Vulkan device.
     * @param physicalDevice The Vulkan physical device.
     * @param commandPool The Vulkan command pool.
     * @param graphicsQueue The Vulkan graphics queue.
     */
    void Create(const VkDevice &device, const VkPhysicalDevice &physicalDevice, const VkCommandPool &commandPool,
                const VkQueue &graphicsQueue);

    /**
     * @brief Destroy the VertexBuffer object.
     *
     * @param device The Vulkan device.
     */
    void Destroy(const VkDevice &device);

    /**
     * @brief Get the buffer.
     *
     * @return const VkBuffer& The buffer.
     */
    [[nodiscard]] const VkBuffer &Get() const { return _buffer; }

    /**
     * @brief Get the index buffer.
     *
     * @return const VkBuffer& The index buffer.
     */
    [[nodiscard]] const VkBuffer &GetIndexBuffer() const { return _indexBuffer; }

  private:
    void CreateBuffer(const VkDevice &device, const VkPhysicalDevice &physicalDevice, const VkDeviceSize size,
                      const VkBufferUsageFlags usage, const VkMemoryPropertyFlags properties, VkBuffer &buffer,
                      VkDeviceMemory &bufferMemory);

    /**
     * @brief Find the memory type in the physical device.
     *
     * @param physicalDevice The Vulkan physical device.
     * @param typeFilter The type filter.
     * @param properties The properties.
     * @return uint32_t The memory type index.
     */
    uint32_t FindMemoryType(const VkPhysicalDevice &physicalDevice, const uint32_t typeFilter,
                            const VkMemoryPropertyFlags properties);

    /**
     * @brief Copy the buffer.
     *
     * @param device  The Vulkan device.
     * @param commandPool  The Vulkan command pool.
     * @param graphicsQueue  The Vulkan graphics queue.
     * @param srcBuffer  The source buffer.
     * @param dstBuffer  The destination buffer.
     * @param size  The size of the buffer.
     */
    void CopyBuffer(const VkDevice &device, const VkCommandPool &commandPool, const VkQueue &graphicsQueue,
                    const VkBuffer &srcBuffer, const VkBuffer &dstBuffer, VkDeviceSize size);

  private:
    VkBuffer _buffer;
    VkDeviceMemory _bufferMemory;
    VkBuffer _indexBuffer;
    VkDeviceMemory _indexBufferMemory;
};

} // namespace ES::Plugin::Wrapper

#endif /* !VERTEXBUFFER_HPP_ */
