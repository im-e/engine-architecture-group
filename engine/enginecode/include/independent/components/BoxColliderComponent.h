#pragma once

#include "ColliderComponent.h"
namespace Engine {

	class BoxColliderComponent : public ColliderComponent
	{
	private:
		rp3d::Vector3 boxDim;
		rp3d::CollisionBody *body;

	public:

		BoxColliderComponent() {};//default constructor
		BoxColliderComponent(rp3d::Vector3 boxSize)
		{
			boxDim = boxSize;
			rp3d::BoxShape shape(boxDim); //Set collision shape dimensions
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