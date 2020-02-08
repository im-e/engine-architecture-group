#pragma once

#include "rendering/layers/Layer.h"
#include "data/json/JsonLayer.h"

namespace Engine
{
	class ImGuiLayer : public Layer
	{
	private:
		JsonLayer* m_layer;
		bool m_gameObjectWindow;
		bool m_removeGameObjectWindow;
	public:
		ImGuiLayer(const std::string& name, std::shared_ptr<Layer> layer) : Layer(name)
		{
			m_layer = dynamic_cast<Engine::JsonLayer*>(layer.get());
		};

		// Inherited via Layer
		void onAttach() override;
		void onDetach() override;
		void onUpdate(float timestep) override;
		void onEvent(Event & e) override;
	};
}