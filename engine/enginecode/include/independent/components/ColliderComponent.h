#pragma once

#include "Component.h"
#include "RigidbodyComponent.h"

namespace Engine {

	enum Category
	{

		TYPE1 = 0x0001,
		TYPE2 = 0x0002,
		TYPE3 = 0x0004
	};

	class ColliderComponent : public Component
	{
	protected:
		rp3d::RigidBody *parentObject;
		rp3d::ProxyShape *proxy;
		rp3d::CollisionShape *shape;

	public:

		ColliderComponent() {}; //Default Constructor

		void onAttach(GameObject* owner) override
		{
			m_owner = owner;
		}

		void onDetach()
		{
		}

		void onUpdate(float timestep) override
		{
		}

		inline const std::type_info& getType() override
		{
			return typeid(decltype(*this));
		}

		rp3d::ProxyShape *getProxyShape()
		{
			return proxy;
		}

		void setProxyshape(rp3d::ProxyShape *shape)
		{
			proxy = shape;
		}

		void setParentObject(rp3d::RigidBody *parent)
		{
			parentObject = parent;
		}

		//Find object's rigid body
		rp3d::RigidBody *getParentObject()
		{
			return parentObject;
		}

		void setType(int type)
		{
			proxy->setCollisionCategoryBits(type);
		}
	};
}