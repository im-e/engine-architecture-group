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

		rp3d::Vector3 m_linear; //Linear force
		rp3d::Vector3 m_angular; //Angular torque

		RigidBodyComponent()
		{
			//bodyPos = rp3d::Vector3(0.f, 0.f, 0.f); //defaults physics body poisition
			//bodyOri = rp3d::Quaternion::identity();	//defaults orientation
			//bodyTra = rp3d::Transform(bodyPos, bodyOri);//Creates default transform
			
			bodyTra.setPosition.getOpenGLMatrix(m_owner->getComponent<PositionComponent>()->getCurrentPosition());
			body = m_world->createRigidBody(bodyTra);//Assigns default transform
			body->setType(rp3d::BodyType::DYNAMIC); //defaults body type to dynamic

			
		}

		void onAttach(GameObject* owner) override
		{
			m_owner = owner; //Sets owner 	

			m_possibleMessages = { ComponentMessageType::RigidBodySet };

			
			rp3d::Vector3 temp = m_linear;

			for (auto& msg : m_possibleMessages)
			{
				m_owner->getMap().insert(std::pair<ComponentMessageType, Component*>(msg, this));
				switch (msg)
				{
				case ComponentMessageType::VelocitySetLinear:
					m_msgsMap[msg] = [this](std::any data)
					{
						rp3d::Vector3 linear = std::any_cast<rp3d::Vector3>(data);
						m_linear = linear;
						return true;
					};
					break;

				case ComponentMessageType::VelocitySetAngular:
					m_msgsMap[msg] = [this](std::any data)
					{
						rp3d::Vector3 angular = std::any_cast<rp3d::Vector3>(data);
						m_angular = angular;
						return true;
					};
					break;

				case ComponentMessageType::LinearForceApply:
					m_msgsMap[msg] = [this](std::any data)
					{
						rp3d::Vector3 linear = std::any_cast<rp3d::Vector3>(data);
						body->applyForceToCenterOfMass(linear);
						return true;
					};
					break;

				case ComponentMessageType::AngularForceApply:
					m_msgsMap[msg] = [this](std::any data)
					{
						rp3d::Vector3 angular = std::any_cast<rp3d::Vector3>(data);
						body->applyTorque(angular);
						return true;
					};
					break;

				case ComponentMessageType::AIPositionSet:
					m_msgsMap[msg] = [temp, owner, this](std::any data)
					{
						m_linear = temp;
						rp3d::Vector3 desired = std::any_cast<rp3d::Vector3>(data);
						rp3d::Vector3 current = bodyTra.setPosition.getOpenGLMatrix(owner->getComponent<PositionComponent>()->getCurrentPosition());

						rp3d::Vector3 diff = current - desired;

						if (diff.x > 0.001f || diff.x < -0.001f)
						{
							if (diff.x > 0.0f)
							{
								if (m_linear.x > 0.0f)
								{
									m_linear.x *= -1;
								}
								else
								{
									m_linear.x = m_linear.x;
								}
							}
							else
							{
								if (m_linear.x < 0.0f)
								{
									m_linear.x *= -1; // reverse
								}
								else
								{
									m_linear.x = m_linear.x;
								}
							}
						}
						else
						{
							m_linear.x = 0.0f;
						}

						if (diff.y > 0.001f || diff.y < -0.001f)
						{
							if (diff.y > 0.0f)
							{
								if (m_linear.y > 0.0f)
								{
									m_linear.y *= -1;
								}
								else
								{
									m_linear.y = m_linear.y;
								}
							}
							else
							{
								if (m_linear.y < 0.0f)
								{
									m_linear.y *= -1; // reverse
								}
								else
								{
									m_linear.y = m_linear.y;
								}
							}
						}
						else
						{
							m_linear.y = 0.0f;
						}

						if (diff.z > 0.001f || diff.z < -0.001f)
						{
							if (diff.z > 0.0f)
							{
								if (m_linear.z > 0.0f)
								{
									m_linear.z *= -1;
								}
								else
								{
									m_linear.z = m_linear.z;
								}
							}
							else
							{
								if (m_linear.z < 0.0f)
								{
									m_linear.z *= -1; // reverse
								}
								else
								{
									m_linear.z = m_linear.z;
								}
							}
						}
						else
						{
							m_linear.z = 0.0f;
						}

						//m_linear = glm::normalize(m_linear);

						return true;
					};
					break;
		}

		void onUpdate(float timestep) override
		{
			std::pair<rp3d::Vector3, rp3d::Vector3> data(m_linear * timestep, m_angular * timestep);
			sendMessage(ComponentMessage(ComponentMessageType::PositionIntegrate, std::any(data)));
		}
		void onDetach() override
		{
			m_world->destroyRigidBody(body); //Destroys body component when detached from owner
		}

		inline const std::type_info& getType() override
		{
			return typeid(decltype(*this));
		}


		//In world parameters
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

		//Rigid body parameters
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

		void toggleSleep(bool sleep)
		{
			body->setIsAllowedToSleep(sleep);
		}

		void toggleGravity(bool gravity)
		{
			body->enableGravity(gravity);
		}

		rp3d::RigidBody* getBody()
		{
			return body;
		}

		//Phys materials
		rp3d::Material getMaterial()
		{
			return body->getMaterial();
		}

		void setRestitution(rp3d::decimal restitution)
		{
			body->getMaterial().setBounciness(restitution);
		}
		rp3d::decimal getRestitution()
		{
			return body->getMaterial().getBounciness();
		}

		void setFriction(rp3d::decimal friction)
		{
			body->getMaterial().setFrictionCoefficient(friction);
		}
		rp3d::decimal getFriction()
		{
			return body->getMaterial().getFrictionCoefficient();
		}

		void setRollingRes(rp3d::decimal rollingRes)
		{
			body->getMaterial().setRollingResistance(rollingRes);
		}
		rp3d::decimal getRollingRes()
		{
			return body->getMaterial().getRollingResistance();
		}
	};
}