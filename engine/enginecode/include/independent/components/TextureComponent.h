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
		unsigned int m_texSlotNormal; //!< Texture slot
		unsigned int m_texSlotParallax; //!< Texture slot
		unsigned int m_texSlotShadow; //!< Texture slot

	public:
		//! Custom constructor \param texSlot initial texture slot
		TextureComponent(unsigned int texSlot) : m_texSlot(texSlot)
		{}

		void onAttach(GameObject* owner) override
		{ 
			m_owner = owner; 
			setTexture(m_texSlot);
			//setNormalTextures(m_texSlot, m_texSlotNormal);
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
		void assignNormalTexture(int index)
		{
			m_texSlotNormal = index;
			std::pair<std::string, void*> normalData("u_normalTexData", (void*)m_texSlotNormal);
			ComponentMessage msg(ComponentMessageType::UniformSet, normalData);
			sendMessage(msg);
		}
		void assignParallaxTexture(int index)
		{
			m_texSlotParallax = index;
			std::pair<std::string, void*> parallaxData("u_parallaxTexData", (void*)m_texSlotParallax);
			ComponentMessage msg(ComponentMessageType::UniformSet, parallaxData);
			sendMessage(msg);
		}
		void assignShadowTexture(int index)
		{
			m_texSlotShadow = index;
			std::pair<std::string, void*> shadowData("u_shadowTexData", (void*)m_texSlotShadow);
			ComponentMessage msg(ComponentMessageType::UniformSet, shadowData);
			sendMessage(msg);
		}
	};
}
