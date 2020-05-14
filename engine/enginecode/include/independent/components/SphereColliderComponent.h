#pragma once
#include "ColliderComponent.h"
namespace Engine {
	class SphereColliderComponent : public ColliderComponent
	{
	private:
		rp3d::decimal sphereRad;
		rp3d::CollisionBody *body;

	public:
		SphereColliderComponent() {};
		SphereColliderComponent(rp3d::decimal radius) 
		{
			sphereRad = radius;
			
			rp3d::SphereShape shape(sphereRad);
			
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
		};

		inline const std::type_info& getType() override
		{
			return typeid(decltype(*this));
		}

		
	};

}