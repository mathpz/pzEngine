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
			void onImGuiRender();

			void Begin();
			void End();

	private:


	};
} // namespace pz

