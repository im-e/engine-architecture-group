#pragma once

/*! \file RigidBodyComponent.h
\brief Defines a rigidbody.
*/

#include <components/Component.h>
#include <components/PositionComponent.h>
#include "core/application.h"

namespace Engine 
{
	/*! \class RigidBodyComponent
	\brief Defines a physics body
	
		Inherits from component
	*/
	class RigidBodyComponent : public Component
	{
	private:
		rp3d::RigidBody * body; //!< RP3D body

		rp3d::Transform bodyTra; //!< Body transform 
		rp3d::Transform curTra; //!< Current body transform 
		rp3d::Transform prevTra; //!< Previous body transform 
		rp3d::Transform intTra; //!< Internal body transform

		rp3d::Quaternion bodyOri; //!< Body orientation
		rp3d::Vector3 bodyPos; //!< Body position
		rp3d::BodyType m_type; //!< Body type
		float bodyDen = 1.0f; //!< Density
		bool m_gravity; //!< Is gravity affecting the body?

		glm::mat4 m_model; //!< Rendering model position

	public:
		rp3d::Vector3 m_linear; //!< Body linear velocity
		rp3d::Vector3 m_angular = rp3d::Vector3(0,0,0); //!< Body angular velocity
		glm::vec3 linear; //!< GLM linear velocity
		glm::vec3 angular = glm::vec3(0, 0, 0); //!< GLM angular velocity
		
		//! Default constructor
		RigidBodyComponent() 
		{
			bodyTra.setPosition(m_owner->getComponent<PositionComponent>()->getRenderPosition());
			bodyTra.setOrientation(rp3d::Quaternion::identity());
		} 

		//! Custom constructor \param type body type \param gravity enables gravity
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
			body->setTransform(bodyTra);

			prevTra = body->getTransform();

			body->setMass(1.0f);
		}

		void onUpdate(float timestep) override
		{
			if(m_type == rp3d::BodyType::DYNAMIC)
			{
				std::pair<glm::vec3, glm::vec3> data(linear * timestep, angular * timestep);

				curTra = body->getTransform();
				
				//Interpolate transform between physics cycles
				intTra = rp3d::Transform::interpolateTransforms(prevTra, curTra, Application::getInstance().getPhysFactor());		

				//Match rendered body position with rigid body position
				body->getTransform().getOpenGLMatrix(&m_model[0][0]);
				m_owner->getComponent<PositionComponent>()->setModel(m_model);
	
				prevTra = curTra;
			}
		}

		void onDetach() override
		{
			auto iter = m_owner->getMap().begin();
			while (iter != m_owner->getMap().end())
			{
				if ((*iter).second == this)
				{
					iter = m_owner->getMap().erase(iter);
					--iter;
				}
				else
				{
					++iter;
				}
			}
		}

		inline const std::type_info& getType() override
		{
			return typeid(decltype(*this));
		}

		//! Sets rendering position \return rendering position
		inline glm::vec3 &setRenderPosition()
		{
			glm::vec3 temp;
			temp.x = this->getBody()->getTransform().getPosition().x;
			temp.y = this->getBody()->getTransform().getPosition().y;
			temp.z = this->getBody()->getTransform().getPosition().z;
			return temp;
		}

		//! Sets rendering rotation \return rendering rotation
		inline glm::vec3 &setRenderRotation()
		{
			glm::vec3 temp;
			temp.x = body->getTransform().getOrientation().x;
			temp.y = body->getTransform().getOrientation().y;
			temp.z = body->getTransform().getOrientation().z;
			return temp;
		}

		//! Changes body type \param type new type
		void changeType(rp3d::BodyType type)
		{
			body->setType(type); //Allows changing of body type
			m_type = type;
		}

		//! Sets body position \param position new body position
		void setPosition(rp3d::Vector3 position)
		{
			bodyPos = position;
			body->setTransform(rp3d::Transform(bodyPos, bodyOri)); //Update position
		}

		//! Sets body orientation \param orientation new body orientation
		void setOrientation(rp3d::Quaternion orientation)
		{
			bodyOri = orientation;
			body->setTransform(rp3d::Transform(bodyPos, bodyOri)); //Update orientation
		}

		//! Sets body mass \param mass new mass
		void setMass(rp3d::decimal mass) //Set mass
		{
			body->setMass(mass);
		}

		//! Gets current body mass \return body mass
		rp3d::decimal getMass()
		{
			return body->getMass();
		}

		//! Sets body density \param density new density
		void setDensity(float density) //Set density
		{
			bodyDen = density;
		}
		
		//! Gets current body density \return current density
		float getDensity()
		{
			return bodyDen;
		}

		//! Temporarily disables rigidbody \param sleep should be disabled
		void toggleSleep(bool sleep)
		{
			body->setIsAllowedToSleep(sleep);
		}

		//! Toggles gravity \param gravity enables gravity
		void toggleGravity(bool gravity)
		{
			body->enableGravity(gravity);
		}

		//! Gets physics body \return RP3D physics body
		rp3d::RigidBody* getBody()
		{
			return body;
		}

		//! Gets physics material \return RP3D physics material
		rp3d::Material getMaterial()
		{
			return body->getMaterial();
		}

		//! Sets restituion \param restitution new body restitution
		void setRestitution(rp3d::decimal restitution)
		{
			body->getMaterial().setBounciness(restitution);
		}

		//! Gets restitution \return current body restitution
		rp3d::decimal getRestitution()
		{
			return body->getMaterial().getBounciness();
		}

		//! Sets friction \param friction new body friction
		void setFriction(rp3d::decimal friction)
		{
			body->getMaterial().setFrictionCoefficient(friction);
		}

		//! Gets friction \return current body friction
		rp3d::decimal getFriction()
		{
			return body->getMaterial().getFrictionCoefficient();
		}

		//! Sets rolling resistance \param rollingRes new rolling resistance
		void setRollingRes(rp3d::decimal rollingRes)
		{
			body->getMaterial().setRollingResistance(rollingRes);
		}

		//! Gets rolling resistance \return rolling resistance
		rp3d::decimal getRollingRes()
		{
			return body->getMaterial().getRollingResistance();
		}
	};
}
