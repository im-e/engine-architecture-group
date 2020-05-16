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

			shape = new rp3d::CapsuleShape(capRad, capHeight);

			
			
		};


		void onAttach(GameObject* owner)override
		{
			m_owner = owner;
			setParentObject(m_owner->getComponent<RigidBodyComponent>()->getBody());
			body = Application::getInstance().getPhysics()->getWorld()->createCollisionBody(getParentObject()->getTransform()); //Create body in physics world
			getParentObject()->addCollisionShape(shape, getParentObject()->getTransform(), getParentObject()->getMass()); //Create proxyShape
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