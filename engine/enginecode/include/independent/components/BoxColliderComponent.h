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
			shape = new rp3d::BoxShape(boxDim); //Set collision shape dimensions
			
			
			
		};
		void onAttach(GameObject* owner)override
		{	
			m_owner = owner;
			setParentObject(m_owner->getComponent<RigidBodyComponent>()->getBody());
			body = Application::getInstance().getPhysics()->getWorld()->createCollisionBody(getParentObject()->getTransform()); //Create body in physics world
			getParentObject()->addCollisionShape(shape, getParentObject()->getTransform(), getParentObject()->getMass()); //Create proxyShape and attach to rigidbody
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