#pragma once
/*! \file ColourComponent.h
\brief Implement component that controls the colour of a game object

*/

#include "Component.h"

namespace Engine
{
	/*! \class ColourComponent
	\brief Defines component to set the colour of a game object.

		Inherits from Component
	*/
	class ColourComponent : public Component
	{
	private:
		float m_r, m_g, m_b; //!< Floats containing the game objects red, blue and green values 

	public:
		/*! Custom constructor
		\param r is the GO red value
		\param g is the GO green value
		\param b is the GO blue value
		*/
		ColourComponent(float r, float g, float b) : m_r(r), m_g(g), m_b(b) {};
		
		void onAttach(GameObject* owner) override
		{
			m_owner = owner;
		}

		void onUpdate(float timestep) override
		{
			std::pair<std::string, void*> data("u_fontColour", (void*)new glm::vec3(m_r, m_g, m_b));
			ComponentMessage msg(ComponentMessageType::UniformSet, data);
			sendMessage(msg);
		}

		void onDetach() override
		{

		}

		void onEvent(Event& e) override
		{

		}
	};
}