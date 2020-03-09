#pragma once
#include "RigidBodyComponent.h"
#include "ColliderComponent.h"

namespace Engine
{
	class TerrainColliderComponent : public ColliderComponent
	{
	public:
		TerrainColliderComponent() {};

		TerrainColliderComponent(RigidBodyComponent *body, const int rows, const int columns, float min, float max)
		{
			parentObject = body->getBody();
			parentObject->setType(rp3d::BodyType::STATIC);

			std::vector<float> heights;
			heights.resize(rows * columns);

			rp3d::HeightFieldShape *HFS = new rp3d::HeightFieldShape(columns, rows, min,
				max, heights.data(), rp3d::HeightFieldShape::HeightDataType::HEIGHT_FLOAT_TYPE);

			proxy = parentObject->addCollisionShape(HFS, parentObject->getTransform(), parentObject->getMass());
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