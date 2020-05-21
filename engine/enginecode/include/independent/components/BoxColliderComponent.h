#pragma once

/*! \file BoxColliderComponent.h
\brief Defines box collider. Currently not working
*/

#include "ColliderComponent.h"

namespace Engine 
{
	/*! \class BoxColliderComponent
	\brief Defines a box collider. Currently not working.

		Inherits from collider component.
	*/
	class BoxColliderComponent : public ColliderComponent
	{
	private:
		rp3d::Vector3 boxDim; //!< Box dimensions 
		rp3d::CollisionBody *body; //!< Collision body

	public:
		//! Default constructor
		BoxColliderComponent() {};
		//! Custom constructor \param boxSize collider size
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

		//! Sets size of the collider box \param newSize new collider size
		void setSize(rp3d::Vector3 newSize)
		{
			boxDim = newSize;
		}

		//! Sets size of the collider on X axis \param value size on given axis
		void setXSize(float value)
		{
			boxDim.x = value;
		}

		//! Sets size of the collider on Y axis \param value size on given axis
		void setYSize(float value)
		{
			boxDim.y = value;
		}

		//! Sets size of the collider on Z axis \param value size on given axis
		void setZSize(float value)
		{
			boxDim.z = value;
		}

		//! Gets size of the collider \return size of the collider box
		rp3d::Vector3 getSize()
		{
			return boxDim;
		}
	};

}