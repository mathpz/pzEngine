#pragma once

#include "Core/Events/KeyEvent.hpp"
#include "Core/Events/MouseEvent.hpp"
#include "Core/Events/ApplicationEvent.hpp"
#include "Core/Layer.hpp"

namespace pz
{
	class ImGuiLayer : public Layer
	{
	public:
			ImGuiLayer();
			~ImGuiLayer();

			void onAttach();
			void onDetach();
			void onEvent(Event& event);
			void onUpdate();

	private:
			bool onMouseButtonPressedEvent(MouseButtonPressedEvent& event);
			bool onMouseButtonReleasedEvent(MouseButtonReleasedEvent& event);
			bool onMouseMovedEvent(MouseMovedEvent& event);
			bool onMouseScrolledEvent(MouseScrolledEvent& event);
			bool onKeyPressedEvent(KeyPressedEvent& event);
			bool onKeyReleasedEvent(KeyReleasedEvent& event);
			bool onKeyTypedEvent(KeyTypedEvent& event);
			bool onWindowResizeEvent(WindowResizeEvent& event);


	};
} // namespace pz

