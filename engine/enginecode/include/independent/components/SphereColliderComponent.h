#pragma once
#include "ColliderComponent.h"
namespace Engine {
	class SphereColliderComponent : public ColliderComponent
	{
	private:
		rp3d::decimal sphereRad;
		rp3d::CollisionBody *body;
		rp3d::SphereShape *shape;
	public:
		SphereColliderComponent() {};
		SphereColliderComponent(rp3d::decimal radius) 
		{
			sphereRad = radius;
			
			shape = new rp3d::SphereShape(sphereRad);
			
			body = Application::getInstance().getPhysics()->getWorld()->createCollisionBody(getParentObject()->getTransform()); //Create body in physics world
			
		};


		void onAttach(GameObject* owner)override
		{
			m_owner = owner;
			setParentObject(m_owner->getComponent<RigidBodyComponent>()->getBody());
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
		};

		inline const std::type_info& getType() override
		{
			return typeid(decltype(*this));
		}

		
	};

}