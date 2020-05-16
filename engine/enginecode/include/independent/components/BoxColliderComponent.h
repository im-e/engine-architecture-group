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

		void onAttach(GameObject* owner) override
		{	
			m_owner = owner;

			rp3d::RigidBody* rb = m_owner->getComponent<RigidBodyComponent>()->getBody();
			setParentObject(rb);
			body = Application::getInstance().getPhysics()->getWorld()->createCollisionBody(rb->getTransform()); //Create body in physics world

			proxy = rb->addCollisionShape(shape, rb->getTransform(), rb->getMass()); //Create proxyShape and attach to rigidbody
		}

		void onUpdate(float timestep) override
		{
			//Update collider position from rigid body
			body->setTransform(getParentObject()->getTransform());
		}

		void onDetach() 
		{
			auto iter = m_owner->getMap().begin();
			while (iter != m_owner->getMap().end())
			{
				if ((*iter).second == this)
				{
					iter = m_owner->getMap().erase(iter);
					--iter;
				}
				else
				{
					++iter;
				}
			}

			//Clean up body
			Application::getInstance().getPhysics()->getWorld()->destroyCollisionBody(body);
		};

		inline const std::type_info& getType() override
		{
			return typeid(decltype(*this));
		}
	};

}