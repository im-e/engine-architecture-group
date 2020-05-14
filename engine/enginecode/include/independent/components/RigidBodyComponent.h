#pragma once
#include <components/Component.h>
#include <components/PositionComponent.h>
#include "core/application.h"

namespace Engine 
{
	class RigidBodyComponent : public Component
	{
	private:
		rp3d::RigidBody * body;

		rp3d::Transform bodyTra, curTra, prevTra, intTra;
		rp3d::Quaternion bodyOri;	
		rp3d::Vector3 bodyPos;	
		rp3d::BodyType m_type;
		float bodyDen = 1.0f;
		bool m_gravity;

		glm::mat4 m_model;
		bool safe = false;
	public:
		rp3d::Vector3 m_linear, m_angular = rp3d::Vector3(0,0,0);
		glm::vec3 linear, angular = glm::vec3(0, 0, 0);
		


		RigidBodyComponent() //default constructor
		{
			bodyTra.setPosition(m_owner->getComponent<PositionComponent>()->getRenderPosition());
			bodyTra.setOrientation(rp3d::Quaternion::identity());
		} 
		RigidBodyComponent(rp3d::BodyType type, bool gravity){

			m_type = type;
			m_gravity = gravity;

			
		}

		void onAttach(GameObject * owner) override
		{
			m_owner = owner;
			
			//Get model world position
			m_model = m_owner->getComponent<PositionComponent>()->getModel();

			
			//Get opengl mesh for model
			bodyTra.setFromOpenGL(&m_model[0][0]);
			
			//Create rigid body at position
			body = Application::getInstance().getPhysics()->getWorld()->createRigidBody(bodyTra);
			body->enableGravity(m_gravity);
			body->setType(m_type);
			
			prevTra = body->getTransform();
			for (auto& msg : m_possibleMessages)
			{
				m_owner->getMap().insert(std::pair<ComponentMessageType, Component*>(msg, this));
			}
			safe = true;
		}
		void onUpdate(float timestep) override
		{
			if(safe){
				std::pair<glm::vec3, glm::vec3> data(linear * timestep, angular * timestep);

				
				ComponentMessage msg(ComponentMessageType::RP3DPositionIntegrate, std::any(data));
				sendMessage(msg);

				curTra = body->getTransform();
				
				//Interpolate transform between physics cycles
				intTra = rp3d::Transform::interpolateTransforms(prevTra, curTra, Application::getInstance().getPhysFactor());


				

				//Match rendered body position with rigid body position
				body->getTransform().getOpenGLMatrix(&m_model[0][0]);
				m_owner->getComponent<PositionComponent>()->setModel(m_model);

				body->setTransform(intTra);
	
				prevTra = curTra;
			}
		}

		void onDetach() override
		{
		}

		inline const std::type_info& getType() override
		{
			return typeid(decltype(*this));
		}

		inline glm::vec3 &setRenderPosition()
		{
			glm::vec3 temp;
			temp.x = this->getBody()->getTransform().getPosition().x;
			temp.y = this->getBody()->getTransform().getPosition().y;
			temp.z = this->getBody()->getTransform().getPosition().z;
			return temp;
		}
		inline glm::vec3 &setRenderRotation()
		{
			glm::vec3 temp;
			temp.x = body->getTransform().getOrientation().x;
			temp.y = body->getTransform().getOrientation().y;
			temp.z = body->getTransform().getOrientation().z;
			return temp;
		}

		//######################################################################################



		void changeType(rp3d::BodyType type)
		{
			body->setType(type); //Allows changing of body type
			m_type = type;
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
