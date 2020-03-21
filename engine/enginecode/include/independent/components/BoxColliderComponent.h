#pragma once

#include "ColliderComponent.h"
namespace Engine {

	class BoxColliderComponent : public ColliderComponent
	{
	private:
		rp3d::Vector3 boxDim;

	public:

		BoxColliderComponent() {};//default constructor
		BoxColliderComponent(rp3d::Vector3 boxSize)
		{
			//rp3d::BoxShape shape(boxSize); //Set size of collision box
			//parentObject->addCollisionShape(&shape, parentObject->getTransform(), parentObject->getMass()); //Defaults poisition to match origin of the parent's rigid body
			boxDim = boxSize;
			rp3d::BoxShape shape(boxDim);
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
		};

		inline const std::type_info& getType() override
		{
			return typeid(decltype(*this));
		}
	};

}