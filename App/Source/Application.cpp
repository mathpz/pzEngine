#include <pzEngine.hpp>
#include "imgui/imgui.h"

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

	virtual void onImGuiRender() override
	{
	}

	void onEvent(pz::Event& event) override
	{
	}
};

class SBApplication : public pz::Application
{
public:
    SBApplication()
    {
        PushLayer(new ExampleLayer());
    }

    ~SBApplication()
    {
    }

};


pz::Application* pz::createApplication()
{
    return new SBApplication();
}