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
<<<<<<< HEAD
		unsigned int m_texSlotNormal; //!< Texture slot
		unsigned int m_texSlotParallax; //!< Texture slot
		unsigned int m_texSlotSpecular; //!< Texture slot
		std::string m_diffName; //!< Diffuse texture name
		std::string m_normalName;
		std::string m_parallaxName;
		std::string m_specName;
=======
		std::string m_diffName; //!< Diffuse texture name
>>>>>>> parent of 0280196... Merge Reece to master

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

<<<<<<< HEAD
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

		void assignSpecularTexture(int index)
=======
		inline const std::type_info& getType() override
>>>>>>> parent of 0280196... Merge Reece to master
		{
			return typeid(decltype(*this));
		}

		//! Sets diffuse texture name \param name new diffuse texture name
		inline void setDiffuseTextureName(std::string name) { m_diffName = name; }
		//! Gets diffuse texture name \return current diffuse texture name
		inline std::string& getDiffName() { return m_diffName; }
<<<<<<< HEAD

		inline void setNormalTextureName(std::string name) { m_normalName = name; }
		inline std::string& getNormalName() { return m_normalName; }

		inline void setParallaxTextureName(std::string name) { m_parallaxName = name; }
		inline std::string& getParallaxName() { return m_parallaxName; }

		inline void setSpecularTextureName(std::string name) { m_specName = name; }
		inline std::string& getSpecularName() { return m_specName; }
=======
>>>>>>> parent of 0280196... Merge Reece to master
	};
}
