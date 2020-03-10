#pragma once
#include <components/Component.h>
#include <components/PositionComponent.h>
#include "core/application.h"

namespace Engine 
{
	class RigidBody : public Component
	{
	private:
		rp3d::RigidBody * body;
	public:

		RigidBody(){}
		RigidBody(){}
		void onAttach(GameObject *m_owner) override
		{
			Application::getInstance().getPhysics()->getWorld()->createRigidBody(body->getTransform());
		}
		void onUpdate(float timestep) override
		{

		}

		void onDetach() override
		{
			Application::getInstance().getPhysics()->getWorld()->destroyRigidBody(body);
		}
	};
}
