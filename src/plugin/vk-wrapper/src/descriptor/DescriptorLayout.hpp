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
 * @file DescriptorLayout.hpp
 * @brief DescriptorLayout class declaration.
 *
 *
 * @author @MasterLaplace
 * @version 0.0.0
 * @date 2024-11-04
 **************************************************************************/

#ifndef DESCRIPTORLAYOUT_HPP_
#define DESCRIPTORLAYOUT_HPP_

#include "DebugMessenger.hpp"

namespace ES::Plugin::Wrapper {

/**
 * @brief DescriptorLayout class.
 *
 *
 * @example
 * @code
 * DescriptorLayout descriptorLayout;
 * descriptorLayout.Create(device);
 * descriptorLayout.Destroy(device);
 * @endcode
 */
class DescriptorLayout {
  public:
    void Create(const VkDevice &device);
    void Destroy(const VkDevice &device);

  private:
    VkDescriptorSetLayout _descriptorSetLayout;
    VkPipelineLayout _pipelineLayout;
};

} // namespace ES::Plugin::Wrapper

#endif /* !DESCRIPTORLAYOUT_HPP_ */
