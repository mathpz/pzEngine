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

			virtual void onAttach() override;
			virtual void onDetach() override;
			virtual void onImGuiRender() override;

			void Begin();
			void End();

	private:


	};
} // namespace pz

