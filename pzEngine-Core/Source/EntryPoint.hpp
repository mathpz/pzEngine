#pragma once

#ifdef PZ_PLATFORM_WINDOWS
#include <iostream>

extern pz::Application* pz::createApplication();

int main(int argc, char** argv)
{
    std::cout << "pzEngine" << std::endl;
    auto app = pz::createApplication();
    app->Run();
    delete app;
}

#endif