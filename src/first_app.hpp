#pragma once

#include "pzWindow.hpp"
#include "pzDevice.hpp"
#include "pzGameObject.hpp"
#include "pzRenderer.hpp"

// std
#include <memory>
#include <vector>

namespace pz
{
    class FirstApp
    {
        public:
            FirstApp();
            ~FirstApp();
            static constexpr int WIDTH = 800;
            static constexpr int HEIGHT = 600;

            FirstApp(const FirstApp&) = delete;
            FirstApp &operator=(const FirstApp&) = delete;

            void run();

        private:
            void loadGameObjects();

            PzWindow pzWindow{WIDTH, HEIGHT, "pzEngine"};
            PzDevice pzDevice{pzWindow};
            PzRenderer pzRenderer{pzWindow, pzDevice};
            std::vector<PzGameObject> gameObjects;

    };

} // namespace pz