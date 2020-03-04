#pragma once

/*! \file RigidBodyComponent.h
\brief Implementation of a component responsible for handling ReactPhysics3D functions.
*/

#include "Component.h"


namespace Engine
{
	class RigidBodyComponent : public Component
	{
	private:
		rp3d::DynamicsWorld *m_world; //RP3D world
		rp3d::RigidBody * body; //RP3D Rigid body
		rp3d::Transform bodyTra;	//Transform component
		rp3d::Quaternion bodyOri;	//Orientation
		rp3d::Vector3 bodyPos;	//RP3D position
		float bodyDen = 1.0f;
	public:

		RigidBodyComponent()
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

			m_possibleMessages = { ComponentMessageType::RigidBodySet };
		}


		void onDetach() override
		{
			m_world->destroyRigidBody(body); //Destroys body component when detached from owner
		}


		void setWorld(rp3d::DynamicsWorld *world)
		{
			m_world = world;
		}

		void changeType(rp3d::BodyType type)
		{
			body->setType(type); //Allows changing of body type
		}
		void setPosition(rp3d::Vector3 position)
		{
			bodyPos = position;
			body->setTransform(rp3d::Transform(bodyPos, bodyOri)); //Update position
		}
		void setOrientation(rp3d::Quaternion orientation)
		{
			bodyOri = orientation;
			body->setTransform(rp3d::Transform(bodyPos, bodyOri)); //Update orientation
		}


		void setMass(rp3d::decimal mass) //Set mass
		{
			body->setMass(mass);
		}
		rp3d::decimal getMass()
		{
			return body->getMass();
		}

		void setDensity(float density) //Set density
		{
			bodyDen = density;
		}
		float getDensity()
		{
			return bodyDen;
		}

		void toggleGravity(bool gravity)
		{
			body->enableGravity(gravity);
		}

		rp3d::RigidBody* getBody()
		{
			return body;
		}
	};
}