#include "engine_pch.h"
#include "rendering/layers/LayerStack.h"

namespace Engine
{
	void LayerStack::start(SystemSignal init, ...)
	{
		
	}

	void LayerStack::stop(SystemSignal close, ...)
	{
		for (auto& it = m_layers.begin(); it != m_layers.end(); it++)
		{
			(*it)->onDetach();
		}
	}

	void LayerStack::push(std::shared_ptr<Layer> layer)
	{
		m_layers.push_back(layer);
		layer->onAttach();
	}

	void LayerStack::pop(std::shared_ptr<Layer> layer)
	{
		auto iterator = std::find(begin(), end(), layer);

		if (iterator != end())
		{
			layer->onDetach();
			m_layers.erase(iterator);
		}
	}
}