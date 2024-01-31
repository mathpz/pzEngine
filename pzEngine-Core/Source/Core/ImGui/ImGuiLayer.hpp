#pragma once

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

	};
} // namespace pz

