#pragma once

/*! \file Component.h
\brief Base class for implementation of components.
*/
#include <any>
#include <map>
#include <memory>
#include <string>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/transform.hpp>
#include <reactphysics3d.h>

namespace Engine
{
	// Classes forward declarations
	class Event;
	class GameObject;

	//! Set of all message types
	enum class ComponentMessageType
	{
		None = 0,
		PositionIntegrate, PositionSet, RelativePositionSet,
		VelocityIntegrate, VelocitySetLinear, VelocitySetAngular,
		AccelerationSet,
		AIPositionSet, AIRotationSet,
		KeyPressed, KeyReleased, MouseButton, MouseScroll, MouseMoved,
		UniformSet, TextureSet, RigidBodySet, LinearForceApply, AngularForceApply,
		ColourSet
	};

	/*! \class ComponentMessage
	\brief Implementation of message for components communication
	*/
	class ComponentMessage
	{
	public:
		//! Custom constructor \param type message type \param data data to be sent
		ComponentMessage(ComponentMessageType type, std::any data) : m_msgType(type), m_msgData(data) {};
		ComponentMessageType m_msgType; //!< Type of a message
		std::any m_msgData; //!< Data of a message
	};

	/*! \class Component
	\brief Base class for all components
	*/
	class Component
	{
	protected:
		//! Owner
		GameObject* m_owner = nullptr;
		std::vector<ComponentMessageType> m_possibleMessages; //!< Collection of possible messages a component can receive

		std::map<ComponentMessageType, std::function<bool(std::any data)>> m_msgsMap; //!< Maps type to an appropriate function

		//! Sends message \param msg message to be sent
		virtual void sendMessage(const ComponentMessage& msg); 
	public:
		//! Called when component is attached to a gameobject \param owner component's owner
		virtual void onAttach(GameObject* owner) { m_owner = owner; }
		//! Gets owner of a component \return owner
		virtual GameObject* getOwner() { return m_owner; }
		//! Called when object is being detached from an owner
		virtual void onDetach();
		//! Called every tick. Updates components. \param timestep delta time
		virtual void onUpdate(float timestep) {};
		//! Called on an appropriate event \param e event occurred
		virtual void onEvent(Event& e) {};

		//! Receives messages and takes appropriate action \param msg received message
		bool receiveMessage(const ComponentMessage& msg)
		{
			if (m_msgsMap.find(msg.m_msgType) != m_msgsMap.end())
				return m_msgsMap[msg.m_msgType](msg.m_msgData);

			return false;
		}

		//! Get type info of the component \return unique type of the ptr to the component
		virtual const std::type_info& getType()
		{
			return typeid(decltype(*this));
		}

		//! Default destructor
		virtual ~Component() {};
	};
}