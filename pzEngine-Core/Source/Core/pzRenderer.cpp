#include "pzpch.hpp"

#include "pzRenderer.hpp"
#include "vk_types.h"
#include "vk_initializers.h"
#include "vk_images.h"

// std
#include <array>
#include <cassert>
#include <stdexcept>

namespace pz
{

    PzRenderer::PzRenderer(PzWindow& pzWindow, GFXDevice& device)
        : pzWindow{pzWindow}, m_Device{device}
    {
    }

    PzRenderer::~PzRenderer()
    {

    }

    void PzRenderer::Draw()
    {
        VkCommandBuffer cmd;
        cmd = m_Device.BeginFrame();
        m_Device.EndFrame(cmd, m_Device.GetDrawImage());

    }


} // namespace pz