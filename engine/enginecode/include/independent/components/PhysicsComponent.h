#pragma once

/*! \file PhysicsComponent.h
\brief Implementation of a component responsible for handling ReactPhysics3D functions.
*/

#include <reactphysics3d.h>
#include "Component.h"


namespace Engine
{
	class PhysicsComponent : public Component
	{
	private:
		rp3d::RigidBody * body; //RP3D Rigid body
		rp3d::Transform bodyTra;	//Transform component
		rp3d::Quaternion bodyOri;	//Orientation
		rp3d::Vector3 bodyPos;	//RP3D position
	public:
		std::shared_ptr<rp3d::DynamicsWorld> m_world; //Shared pointer to physics world
		
		
		PhysicsComponent()
		{
			bodyPos = rp3d::Vector3(0.f, 0.f, 0.f); //defaults physics body poisition
			bodyOri = rp3d::Quaternion::identity();	//defaults orientation
			bodyTra = rp3d::Transform(bodyPos, bodyOri);//Creates default transform

			body = m_world->createRigidBody(bodyTra);//Assigns default transform
			body->setType(rp3d::BodyType::DYNAMIC); //defaults body type to dynamic
		}

		void onAttach(GameObject* owner) override
		{
			m_owner = owner; //Sets owner 	
		}
		
		
		void onDetach() override
		{
			m_world->destroyRigidBody(body); //Destroys body component when detached from owner
		}
		void changeType(rp3d::BodyType type)
		{
			body->setType(type); //Allows changing of body type
		}
		void setTransform(rp3d::Vector3 position)
		{
			body->setTransform(rp3d::Transform(position, bodyOri)); //Update position
		}
		void setOrientation(rp3d::Quaternion orientation)
		{
			body->setTransform(rp3d::Transform(bodyPos,orientation)); //Update orientation
		}

	};
}