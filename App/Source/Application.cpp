#include <pzEngine.hpp>

class ExampleLayer : public pz::Layer
{
public:
    ExampleLayer()
        : Layer("Example")
    {
	}

    void onUpdate() override
    {

	}

    void onEvent(pz::Event& event) override
    {
    	PZ_TRACE("{0}", event);
	}

};

class SBApplication : public pz::Application
{
public:
    SBApplication()
    {
        PushLayer(new ExampleLayer());
        PushOverlay(new pz::ImGuiLayer());
    }

    ~SBApplication()
    {
    }

};


pz::Application* pz::createApplication()
{
    return new SBApplication();
}