#pragma once

#include "Component.h"
#include "rendering/OpenGLTextLabel.h"

namespace Engine
{
	class TextComponent : public Component
	{
	private:
		std::shared_ptr<TextLabel> m_textLabel;
		
		std::string m_name;
		std::string m_font;
		unsigned int m_charSize;
		std::string m_text;

	public:
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

		}

		void onEvent(Event& e) override
		{

		}

		std::shared_ptr<TextLabel> getLabel() { return m_textLabel; }

		inline const std::type_info& getType() override
		{
			return typeid(decltype(*this));
		}
	};

}
