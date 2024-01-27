#include <pzEngine.hpp>

class ExampleLayer : public pz::Layer
{
public:
    ExampleLayer()
        : Layer("Example")
    {
	}

    void OnUpdate() override
    {
		PZ_INFO("ExampleLayer::Update");
	}

    void OnEvent(pz::Event& event) override
    {
		PZ_TRACE("{0}", event);
	}

};

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