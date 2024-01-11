#pragma once

#ifdef PZ_PLATFORM_WINDOWS

extern pz::Application* pz::CreateApplication();


int main(int argc, char** argv)
{
    auto app = pz::CreateApplication();
    app->Run();
    delete app;
}

#endif