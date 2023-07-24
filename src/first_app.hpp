#pragma once

#include "pzWindow.hpp"
#include "pzPipeline.hpp"
#include "pzDevice.hpp"

namespace pz
{
    class FirstApp
    {
        public:
            FirstApp();
            ~FirstApp();
            static constexpr int WIDTH = 800;
            static constexpr int HEIGHT = 600;

            void run();

        private:
            PzWindow pzWindow{WIDTH, HEIGHT, "pzEngine"};
            PzDevice pzDevice{pzWindow};
            PzPipeline pzPipeline{pzDevice, "shaders/simple_shader.vert.spv", "shaders/simple_shader.frag.spv", PzPipeline::defaultPipelineConfigInfo(WIDTH, HEIGHT)};
    };

} // namespace pz