#pragma once
#include "RigidBodyComponent.h"
namespace Engine {
	class SphereColliderComponent : public Component
	{
	private:
		rp3d::RigidBody* parentObject;
		rp3d::ProxyShape* proxy;
	public:
		SphereColliderComponent() {};
		SphereColliderComponent(rp3d::decimal radius, RigidBodyComponent *body) 
		{
			parentObject = body->getBody();

			rp3d::SphereShape shape(radius);

			proxy = parentObject->addCollisionShape(&shape, parentObject->getTransform(), parentObject->getMass());
		};


		void onAttach(GameObject* owner)override
		{
			m_owner = owner;

		}

		void onDetach()
		{
			parentObject->removeCollisionShape(proxy);
		};

		inline const std::type_info& getType() override
		{
			return typeid(decltype(*this));
		}
	};

}