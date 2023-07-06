#pragma once

#include "pzWindow.hpp"

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

    };

} // namespace pz