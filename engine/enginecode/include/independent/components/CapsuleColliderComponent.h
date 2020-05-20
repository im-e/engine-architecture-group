#pragma once

/*! \file CapsuleColliderComponent.h
\brief Defines a capsule collider. Currently not working.
*/

#include "ColliderComponent.h"

namespace Engine 
{
	/*! \class CapsuleColliderComponent
	\brief Defines a capsule collider. Currently not working.
	*/
	class CapsuleColliderComponent : public ColliderComponent
	{
	private:
		rp3d::decimal capRad; //!< Capsule radius
		rp3d::decimal capHeight; //!< Capsule height
		rp3d::CollisionBody *body; //!< Collision body

	public:
		//! Default constructor
		CapsuleColliderComponent() { };

		//! Custom constructor \param radius capsule radius \param height capsule height
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
		}

		inline const std::type_info& getType() override
		{
			return typeid(decltype(*this));
		}
	};

}