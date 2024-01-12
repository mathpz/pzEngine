#pragma once

#ifdef PZ_PLATFORM_WINDOWS

extern pz::Application* pz::createApplication();

int main(int argc, char** argv)
{
    pz::Log::Init();
    PZ_CORE_WARN("Initialized Engine Log!");
    int a = 5;
    PZ_INFO("Initialized Client Log!  Var={0}", a);

    auto app = pz::createApplication();
    app->Run();
    delete app;
}

#endif