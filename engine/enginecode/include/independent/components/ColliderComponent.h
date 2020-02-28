#pragma once

#include "Component.h"
namespace Engine {
	class ColliderComponent : public Component
	{
	private:
	public:
		void onAttach(GameObject* owner) override
		{
			m_owner = owner; //Sets owner 
		}
	};
}