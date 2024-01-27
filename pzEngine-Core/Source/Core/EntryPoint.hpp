#pragma once

#ifdef PZ_PLATFORM_WINDOWS

#define BIT(x) (1 << x)

extern pz::Application* pz::createApplication();

int main(int argc, char** argv)
{
    pz::Log::Init();
    PZ_CORE_INFO("Initialized Log!");
    auto app = pz::createApplication();
    app->Run();
    delete app;
}

#endif
