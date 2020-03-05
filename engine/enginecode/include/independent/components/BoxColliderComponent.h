#pragma once

#include "RigidBodyComponent.h"
namespace Engine {

	class BoxColliderComponent : public Component
	{
	private:
		rp3d::RigidBody* parentObject;
		rp3d::ProxyShape* proxy;

	public:

		BoxColliderComponent() {};
		BoxColliderComponent(rp3d::Vector3 boxSize, RigidBodyComponent *body)
		{
			parentObject = body->getBody();
			rp3d::BoxShape shape(boxSize);
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