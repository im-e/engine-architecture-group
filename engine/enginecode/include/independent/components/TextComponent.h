#pragma once

/*! \file TextComponent.h
\brief Defines text component for GameObjects
*/

#include "Component.h"
#include "rendering/OpenGLTextLabel.h"

namespace Engine
{
	/*! \class TextComponent
	\brief Implements text component
	*/
	class TextComponent : public Component
	{
	private:
		std::shared_ptr<TextLabel> m_textLabel; //!< Text label associated with the component
		
		std::string m_name; //!< Name of the label
		std::string m_font; //!< Font path
		unsigned int m_charSize; //!< Character size
		std::string m_text; //!< Text to be displayed

	public:
		//! Custom constructor \param key name of the label \param fontName path to the font \param charSize character size \param text actual text
		TextComponent(const std::string& key, const std::string& fontName, unsigned int charSize, const std::string& text) : m_name(key), m_font(fontName), m_charSize(charSize), m_text(text) 
		{
			m_textLabel = std::shared_ptr<TextLabel>(TextLabel::create(m_name, m_font, m_charSize, m_text, glm::vec2(), 0.0f, 0.0f, glm::vec3(0, 0, 0)));
		};

		void onAttach(GameObject* owner) override
		{
			m_owner = owner;
		}

		void onUpdate(float timestep) override
		{
			
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

		void onEvent(Event& e) override
		{

		}

		//! Get label associated with a GO \return text label
		std::shared_ptr<TextLabel> getLabel() { return m_textLabel; }

		inline const std::type_info& getType() override
		{
			return typeid(decltype(*this));
		}
	};

}
