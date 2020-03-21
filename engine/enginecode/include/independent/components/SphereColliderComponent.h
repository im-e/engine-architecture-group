#pragma once
#include "ColliderComponent.h"
namespace Engine {
	class SphereColliderComponent : public ColliderComponent
	{
	private:
		rp3d::decimal sphereRad;

	public:
		SphereColliderComponent() {};
		SphereColliderComponent(rp3d::decimal radius) 
		{
			sphereRad = radius;
			//parentObject = body->getBody();
			//
			rp3d::SphereShape shape(sphereRad);
			//
			//proxy = parentObject->addCollisionShape(&shape, parentObject->getTransform(), parentObject->getMass());
			getParentObject()->addCollisionShape(&shape, getParentObject()->getTransform(), getParentObject()->getMass());
		};


		void onAttach(GameObject* owner)override
		{
			m_owner = owner;
			
		}
		void onUpdate(float timestep) override
		{
		}

		void onDetach()
		{
			
		};

		inline const std::type_info& getType() override
		{
			return typeid(decltype(*this));
		}

		
	};

}