#pragma once

/*! \file ColliderComponent.h
\brief Base class for all colliders. Currently not working.
*/

#include "Component.h"
#include "RigidbodyComponent.h"

namespace Engine {
	
	//! \enum Category \brief Collider categories
	enum Category
	{
		TYPE1 = 0x0001,
		TYPE2 = 0x0002,
		TYPE3 = 0x0004
	};

	/*! \class ColliderComponent
	\brief Base class for colliders. Currently not working.
	
		Inherits from Component
	*/
	class ColliderComponent : public Component
	{
	protected:
		rp3d::RigidBody *parentObject; //!< Parent physics body
		rp3d::ProxyShape *proxy; //!< Proxy shape
		rp3d::CollisionShape *shape; //!< Collision shape
		std::string m_type; //!< Collider type
	public:

		//! Default constructor
		ColliderComponent() {}; //Default Constructor

		void onAttach(GameObject* owner) override
		{
			m_owner = owner;
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
		}

		void onUpdate(float timestep) override
		{
		}

		inline const std::type_info& getType() override
		{
			return typeid(decltype(*this));
		}

		//! Gets a proxy shape \return proxy shape
		rp3d::ProxyShape *getProxyShape()
		{
			return proxy;
		}

		//! Sets collider type \param t new type
		void setColliderType(std::string t)
		{
			m_type = t;
		}

		//! Gets collider type \return collider type
		std::string getColliderType()
		{
			return m_type;
		}

		//! Sets a proxy shape \param shape new proxy shape
		void setProxyshape(rp3d::ProxyShape *shape)
		{
			proxy = shape;
		}

		//! Sets parent body \param parent new parent physics body
		void setParentObject(rp3d::RigidBody *parent)
		{
			parentObject = parent;
		}

		//! Gets parent body \return parent physics body
		rp3d::RigidBody *getParentObject()
		{
			return parentObject;
		}

		//! Sets collider type \param type new type
		void setType(int type)
		{
			proxy->setCollisionCategoryBits(type);
		}
	};
}