#pragma once

#include "rendering/layers/Layer.h"

namespace Engine
{
	class ImGuiLayer : public Layer
	{
	private:

	public:
		ImGuiLayer(const std::string& name) : Layer(name) {};

		// Inherited via Layer
		void onAttach() override;
		void onDetach() override;
		void onUpdate(float timestep) override;
		void onEvent(Event & e) override;

	};
}