#pragma once

#include "Component.h"
#include "RigidbodyComponent.h"

namespace Engine {
	class ColliderComponent : public Component
	{
	private:
		rp3d::RigidBody *parentObject;
		rp3d::ProxyShape *proxy;
	public:
		void onAttach(GameObject* owner) override
		{
			m_owner = owner; //Sets owner 
		}

		void onDetach()
		{
			parentObject->removeCollisionShape(proxy);
		}

		void makeBoxMesh(RigidBodyComponent *body, rp3d::Vector3 boxSize) //Box Collider
		{
			parentObject = body->getBody();

			rp3d::BoxShape shape(boxSize);

			proxy = parentObject->addCollisionShape(&shape, parentObject->getTransform(), parentObject->getMass());
		}
		void makeSphereMesh(RigidBodyComponent *body, rp3d::decimal radius)
		{
			parentObject = body->getBody();

			rp3d::SphereShape shape(radius);

			proxy = parentObject->addCollisionShape(&shape, parentObject->getTransform(), parentObject->getMass());
		}
		void makeCapsuleMesh(RigidBodyComponent *body, rp3d::decimal radius, rp3d::decimal height)
		{
			parentObject = body->getBody();

			rp3d::CapsuleShape shape(radius, height);

			proxy = parentObject->addCollisionShape(&shape, parentObject->getTransform(), parentObject->getMass());
		}
		void makeConvexMesh(RigidBodyComponent *body, float vertices, int indices)
		{
			parentObject = body->getBody();
		}
		void makeConcaveMesh(RigidBodyComponent *body)
		{
			parentObject = body->getBody();
			parentObject->setType(rp3d::BodyType::STATIC);
		}


		void makeHeightFieldShape(RigidBodyComponent *body, int rows, int columns, float max, float min)
		{
			parentObject = body->getBody();
			parentObject->setType(rp3d::BodyType::STATIC);
		}
	};
}