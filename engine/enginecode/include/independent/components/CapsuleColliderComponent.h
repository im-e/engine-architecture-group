#pragma once
#include "ColliderComponent.h"
namespace Engine {

	class CapsuleColliderComponent : public ColliderComponent
	{
	private:
		rp3d::decimal capRad, capHeight;
	public:

		CapsuleColliderComponent() { };
		CapsuleColliderComponent( rp3d::decimal radius, rp3d::decimal height)
		{
			capRad = radius;
			capHeight = height;

			rp3d::CapsuleShape shape(capRad, capHeight);

			getParentObject()->addCollisionShape(&shape, getParentObject()->getTransform(), getParentObject()->getMass());

		};


		void onAttach(GameObject* owner)override
		{
			m_owner = owner;
		}
		void onUpdate(float timestep) override
		{
		}

		void onDetach()
		{
		}

		inline const std::type_info& getType() override
		{
			return typeid(decltype(*this));
		}
	};

}