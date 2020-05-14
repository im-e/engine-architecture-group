#pragma once
#include "ColliderComponent.h"
namespace Engine {

	class CapsuleColliderComponent : public ColliderComponent
	{
	private:
		rp3d::decimal capRad, capHeight;
		rp3d::CollisionBody *body;

	public:

		CapsuleColliderComponent() { };
		CapsuleColliderComponent( rp3d::decimal radius, rp3d::decimal height)
		{
			capRad = radius;
			capHeight = height;

			rp3d::CapsuleShape shape(capRad, capHeight);

			body = Application::getInstance().getPhysics()->getWorld()->createCollisionBody(getParentObject()->getTransform()); //Create body in physics world
			getParentObject()->addCollisionShape(&shape, getParentObject()->getTransform(), getParentObject()->getMass()); //Create proxyShape
		};


		void onAttach(GameObject* owner)override
		{
			m_owner = owner;
		}
		void onUpdate(float timestep) override
		{
			//Update render position from rigid body
			body->setTransform(getParentObject()->getTransform());
		}

		void onDetach()
		{
			//Clean up body
			Application::getInstance().getPhysics()->getWorld()->destroyCollisionBody(body);
		}

		inline const std::type_info& getType() override
		{
			return typeid(decltype(*this));
		}
	};

}