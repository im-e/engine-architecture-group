#pragma once
/*! \file SphereColliderComponent.h
\brief Defines a sphere collider. Currently not working.
*/

#include "ColliderComponent.h"

namespace Engine 
{
	/*! \class SphereColliderComponent
	\brief Defines a sphere collider. Currently not working.
	
		Inherits from ColliderComponent
	*/
	class SphereColliderComponent : public ColliderComponent
	{
	private:
		rp3d::decimal sphereRad; //!< Radius
		rp3d::CollisionBody *body; //!< Collision body

	public:
		//! Default constructor
		SphereColliderComponent() {};
		//! Custom constructor \param radius sphere radius
		SphereColliderComponent(rp3d::decimal radius) 
		{
			sphereRad = radius;
			
			shape = new rp3d::SphereShape(sphereRad);
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
		};

		inline const std::type_info& getType() override
		{
			return typeid(decltype(*this));
		}

		//! Sets sphere radius \param newRad new radius
		void setRadius(float newRad)
		{
			sphereRad = newRad;
		}

		//! Gets sphere radius \return sphere collider radius
		float getRadius()
		{
			return sphereRad;
		}
	};

}