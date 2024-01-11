#include <pzEngine.hpp>

class Application : public pz::Application
{
public:
    Application()
    {
    }

    ~Application()
    {
    }

};


pz::Application* pz::createApplication()
{
    return new Application();
}