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

		inline const std::type_info& getType() override
		{
			return typeid(decltype(*this));
		}

		/*void makeBoxMesh(RigidBodyComponent *body, rp3d::Vector3 boxSize) //Box Collider
		{
			parentObject = body->getBody();

			rp3d::BoxShape shape(boxSize);

			proxy = parentObject->addCollisionShape(&shape, parentObject->getTransform(), parentObject->getMass());
		}*/

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
		/*void makeConvexMesh(RigidBodyComponent *body, float vertices, int indices)
		{
			parentObject = body->getBody();
		}
		void makeConcaveMesh(RigidBodyComponent *body)
		{
			parentObject = body->getBody();
			parentObject->setType(rp3d::BodyType::STATIC);
		*/

		//const int& getRows() const override { return m_rows };
		//const int& getColumns() const override { return m_columns };

		void makeHeightFieldShape(RigidBodyComponent *body, const int rows, const int columns, float min, float max)
		{
			parentObject = body->getBody();
			parentObject->setType(rp3d::BodyType::STATIC);

			std::vector<float> heights;
			heights.resize(rows * columns);

			rp3d::HeightFieldShape *HFS = new rp3d::HeightFieldShape(columns, rows, min,
				max, heights.data(), rp3d::HeightFieldShape::HeightDataType::HEIGHT_FLOAT_TYPE);

			proxy = parentObject->addCollisionShape(HFS, parentObject->getTransform(), parentObject->getMass());
		}

		void setType(int type)
		{
			proxy->setCollisionCategoryBits(type);
		}
	};
}