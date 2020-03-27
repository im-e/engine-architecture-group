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
		unsigned int m_texSlotSpecular; //!< Texture slot
		std::string m_diffName; //!< Diffuse texture name
		std::string m_normalName; //!< Normal texture name
		std::string m_parallaxName; //!< Parallax texture name
		std::string m_specName; //!< Specular texture name

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

		inline const std::type_info& getType() override
		{
			return typeid(decltype(*this));
		}

		//! Sets new texture on a GO. \param tex new texture slot
		void setTexture(unsigned int tex)
		{		
			std::pair<std::string, void*> data("u_texData", (void*)tex);
			ComponentMessage msg(ComponentMessageType::UniformSet, data);
			sendMessage(msg);
		}

		//! Assigns normal texture to the slot \param index new normal texture slot
		void assignNormalTexture(int index)
		{
			std::pair<std::string, void*> normalData("u_normalTexData", (void*)index);
			ComponentMessage msg(ComponentMessageType::UniformSet, normalData);
			sendMessage(msg);
		}

		//! Assigns parallax texture to the slot \param index new parallax texture slot
		void assignParallaxTexture(int index)
		{
			m_texSlotParallax = index;
			std::pair<std::string, void*> parallaxData("u_parallaxTexData", (void*)m_texSlotParallax);
			ComponentMessage msg(ComponentMessageType::UniformSet, parallaxData);
			sendMessage(msg);
		}

		//! Assigns specular texture to the slot \param index new specular texture slot
		void assignSpecularTexture(int index)
		{
			m_texSlotSpecular = index;
			std::pair<std::string, void*> specularData("u_specularTexData", (void*)m_texSlotSpecular);
			ComponentMessage msg(ComponentMessageType::UniformSet, specularData);
			sendMessage(msg);
		}

		//! Sets diffuse texture name \param name new diffuse texture name
		inline void setDiffuseTextureName(std::string name) { m_diffName = name; }
		//! Gets diffuse texture name \return current diffuse texture name
		inline std::string& getDiffName() { return m_diffName; }

		//! Sets normal texture name \param name new normal texture name
		inline void setNormalTextureName(std::string name) { m_normalName = name; }
		//! Gets normal texture name \return current normal texture name
		inline std::string& getNormalName() { return m_normalName; }

		//! Sets parallax texture name \param name new parallax texture name
		inline void setParallaxTextureName(std::string name) { m_parallaxName = name; }
		//! Gets parallax texture name \return current parallax texture name
		inline std::string& getParallaxName() { return m_parallaxName; }

		//! Sets specular texture name \param name new specular texture name
		inline void setSpecularTextureName(std::string name) { m_specName = name; }
		//! Gets specular texture name \return current specular texture name
		inline std::string& getSpecularName() { return m_specName; }
	};
}
