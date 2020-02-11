#pragma once

/*! \file TextureComponent.h
\brief Implementation of a component responsible for applying textures to GOs.
*/

#include "Component.h"

namespace Engine
{
	/*! \class TextureComponent
	\brief Applies textures to GOs. 
	
		Inherits from Component.
	*/
	class TextureComponent : public Component
	{
	private:
		unsigned int m_texSlot; //!< Texture slot

	public:
		//! Custom constructor \param texSlot initial texture slot
		TextureComponent(unsigned int texSlot) : m_texSlot(texSlot)
		{}

		void onAttach(GameObject* owner) override
		{ 
			m_owner = owner; 
			setTexture(m_texSlot);

			m_possibleMessages = { ComponentMessageType::TextureSet };

			for (auto& msg : m_possibleMessages)
			{
				m_owner->getMap().insert(std::pair<ComponentMessageType, Component*>(msg, this));
				switch (msg) 
				{
				case ComponentMessageType::TextureSet:
					m_msgsMap[msg] = [this](std::any data)
					{
						auto t = std::any_cast<unsigned int>(data);
						setTexture(t);
						return true;
					};
					break;
				}
			}
		}

		void onUpdate(float timestep) override {};
		void onEvent(Event& e) override {};

		//! Sets new texture on a GO. \param tex new texture slot
		void setTexture(unsigned int tex)
		{		
			std::pair<std::string, void*> data("u_texData", (void*)tex);
			ComponentMessage msg(ComponentMessageType::UniformSet, data);
			sendMessage(msg);
		}

		inline const std::type_info& getType() override
		{
			return typeid(decltype(*this));
		}
	};
}
