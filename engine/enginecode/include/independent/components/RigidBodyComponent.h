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

		float bodyDen = 1.0f;


	public:
		rp3d::Vector3 m_linear, m_angular = rp3d::Vector3(0,0,0);
		glm::vec3 linear, angular = glm::vec3(0, 0, 0);
		//rp3d::Vector3 m_angular = 0;


		RigidBodyComponent() //default constructor
		{
			bodyTra.setPosition(m_owner->getComponent<PositionComponent>()->getRenderPosition());
			bodyTra.setOrientation(rp3d::Quaternion::identity());
		} 
		RigidBodyComponent(rp3d::BodyType type, bool gravity){
			bodyTra.setPosition(rp3d::Vector3(0.0, 0.0, 0.0));
			bodyTra.setOrientation(rp3d::Quaternion::identity());
		}

		void onAttach(GameObject * owner) override
		{
			m_owner = owner;
			//bodyTra.setPosition.getOpenGLMatrix(m_owner->getComponent<PositionComponent>()->getCurrentPosition());
			//bodyTra.setOrientation.getOpenGLMatrix(m_owner->getComponent<PositionComponent>()->getCurrentRotation());
		
			bodyTra.setPosition(m_owner->getComponent<PositionComponent>()->getRenderPosition());
			Application::getInstance().getPhysics()->getWorld()->createRigidBody(body->getTransform());
			
			for (auto& msg : m_possibleMessages)
			{
				m_owner->getMap().insert(std::pair<ComponentMessageType, Component*>(msg, this));
			}
		}
		void onUpdate(float timestep) override
		{
			std::pair<glm::vec3, glm::vec3> data(linear * timestep, angular * timestep);
			
			ComponentMessage msg(ComponentMessageType::RP3DPositionIntegrate, std::any(data));
			sendMessage(msg);


			

			
				curTra = body->getTransform();

				intTra = rp3d::Transform::interpolateTransforms(prevTra, curTra, timestep);

				// Now you can render your body using the interpolated transform here 

				m_owner->getComponent<PositionComponent>()->setPosition(setRenderPosition());
				m_owner->getComponent<PositionComponent>()->setRotation(setRenderRotation());
				
				// Update the previous transform 
				prevTra = curTra;
			
		}

		void onDetach() override
		{
			Application::getInstance().getPhysics()->getWorld()->destroyRigidBody(body);
		}

		inline const std::type_info& getType() override
		{
			return typeid(decltype(*this));
		}

		inline glm::vec3 setRenderPosition()
		{
			glm::vec3 temp;
			temp.x = this->getBody()->getTransform().getPosition().x;
			temp.y = this->getBody()->getTransform().getPosition().y;
			temp.z = this->getBody()->getTransform().getPosition().z;
			return temp;
		}
		inline glm::vec3 setRenderRotation()
		{
			glm::vec3 temp;
			temp.x = this->getBody()->getTransform().getOrientation().x;
			temp.y = this->getBody()->getTransform().getOrientation().y;
			temp.z = this->getBody()->getTransform().getOrientation().z;
			return temp;
		}

		//######################################################################################



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
