#pragma once

#ifdef PZ_PLATFORM_WINDOWS

extern pz::Application* pz::createApplication();

int main(int argc, char** argv)
{

    auto app = pz::createApplication();
    app->Run();
    delete app;
}

#endif