#include "engine_pch.h"
#include "components/GameObject.h"

namespace Engine
{
	void GameObject::onUpdate(float timestep)
	{
		for (auto& comp : m_components)
		{
			comp->onUpdate(timestep);
		}
	}

	void GameObject::onEvent(Event & e)
	{
		for (auto& comp : m_components)
		{
			comp->onEvent(e);
		}
	}

	void GameObject::addComponent(const std::shared_ptr<Component>& comp)
	{
		comp->onAttach(this);
		m_components.push_back(comp);
	}

	void GameObject::removeComponent(std::shared_ptr<Component> comp)
	{
		auto iter = std::find(begin(), end(), comp);

		if (iter != end())
		{
			comp->onDetach();
			m_components.erase(iter);
		}
	}
}