#pragma once
#include "ColliderComponent.h"
namespace Engine {

	class BoxColliderComponent : public Component
	{
	private:
		rp3d::RigidBody* body;
		rp3d::ProxyShape* proxy;

	public:

		BoxColliderComponent() {};
		BoxColliderComponent(rp3d::Vector3 boxSize)
		{

		};
		void onAttach(GameObject* owner)override
		{
			m_owner = owner;

			rp3d::BoxShape shape(boxSize);

			proxy = parentObject->addCollisionShape(&shape, parentObject->getTransform(), parentObject->getMass());
		}

		void onDetach() {};
		inline const std::type_info& getType() override
		{
			return typeid(decltype(*this));
		}
	};

}