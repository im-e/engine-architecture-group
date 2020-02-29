#pragma once

#include "Component.h"
#include "RigidbodyComponent.h"

namespace Engine {
	class ColliderComponent : public Component
	{
	private:
		rp3d::RigidBody *parentObject;
	public:
		void onAttach(GameObject* owner) override
		{
			m_owner = owner; //Sets owner 
		}

		void makeBoxMesh(RigidBodyComponent *body, rp3d::Vector3 boxSize) //Box Collider
		{
			parentObject = body->getBody();

			rp3d::BoxShape shape(boxSize);

			rp3d::ProxyShape* proxy = parentObject->addCollisionShape(&shape, parentObject->getTransform(), parentObject->getMass());
		}
		void makeSphereMesh(RigidBodyComponent *body, rp3d::decimal radius)
		{
			parentObject = body->getBody();

			rp3d::SphereShape shape(radius);

			rp3d::ProxyShape* proxy = parentObject->addCollisionShape(&shape, parentObject->getTransform(), parentObject->getMass());
		}
		void makeCapsuleMesh(RigidBodyComponent *body, rp3d::decimal radius, rp3d::decimal height)
		{
			parentObject = body->getBody();

			rp3d::CapsuleShape shape(radius, height);

			rp3d::ProxyShape* proxy = parentObject->addCollisionShape(&shape, parentObject->getTransform(), parentObject->getMass());
		}
		void makeConvexMesh(RigidBodyComponent *body)
		{
			parentObject = body->getBody();
		}
		void makeConcaveMesh(RigidBodyComponent *body)
		{
			parentObject = body->getBody();
			parentObject->setType(rp3d::BodyType::STATIC);
		}


		void makeHeightFieldShape(RigidBodyComponent *body)
		{
			parentObject = body->getBody();
			parentObject->setType(rp3d::BodyType::STATIC);
		}
	};
}