#pragma once
#include "ColliderComponent.h"
namespace Engine {

	class CapsuleColliderComponent : public ColliderComponent
	{
	private:
	public:


		inline const std::type_info& getType() override
		{
			return typeid(decltype(*this));
		}
	};

}