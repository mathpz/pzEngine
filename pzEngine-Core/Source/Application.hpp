#pragma once

#include "Core.hpp"

namespace pz
{
    class PZ_API Application
    {
    public:
        Application();
        virtual ~Application();

        void Run();
    };

    // To be defined in CLIENT
    Application* createApplication();
    int test;
} // namespace pz