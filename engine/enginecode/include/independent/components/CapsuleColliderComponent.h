#pragma once
#include "ColliderComponent.h"
namespace Engine {

	class CapsuleColliderComponent : public Component
	{
	private:
		rp3d::RigidBody* parentObject;
		rp3d::ProxyShape* proxy;
	public:

		CapsuleColliderComponent() { };
		CapsuleColliderComponent(RigidBodyComponent *body, rp3d::decimal radius, rp3d::decimal height)
		{
			parentObject = body->getBody();

			rp3d::CapsuleShape shape(radius, height);

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