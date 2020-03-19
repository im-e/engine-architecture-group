#include "engine_pch.h"
#include "data/json/JsonLayer.h"

#include "data/json/JsonLoader.h"

namespace Engine
{

	void JsonLayer::onAttach()
	{
		m_audio = std::make_shared<AudioManager>();

		Engine::JsonLoader::loadJson(m_filepath, *this);
		Engine::JsonLoader::loadGameObjects(m_gameobjectsFilepath, *this);

		for (auto& renderCommand : m_initCommands)
		{
			m_renderer->actionCommand(renderCommand.get());
		}


		//https://freesound.org/people/Mrthenoronha/sounds/370293/
		m_audio->loadSound("assets/audio/music/song.wav", true, true, false);
		m_audio->playSound("assets/audio/music/song.wav");


		// To test: Load some sounds here, add them manually to the map,
		// call Play somewhere further (e.g. update)
	}

	void JsonLayer::onDetach()
	{
		for (auto& renderCommand : m_exitCommands)
		{
			m_renderer->actionCommand(renderCommand.get());
		}

		for (auto& ptr : m_data)
		{
			delete ptr;
		}
	}

	void JsonLayer::onUpdate(float timestep)
	{
#ifdef NG_DEBUG
			NG_PROFILER_FUNCTION();
#endif
		
		{
			m_audio->updateLocation(timestep, getCamera()->getCamera()->getPosition());
			m_audio->update();
		}

		{
#ifdef NG_DEBUG
			NG_PROFILER_SCOPE("Camera update");
#endif
			m_cameraController->onUpdate(timestep);
		}
		
		{
#ifdef NG_DEBUG
			NG_PROFILER_SCOPE("GameObjects update");
#endif
			for (auto& GO : m_gameObjects)
			{
				GO.second->onUpdate(timestep);
			}
		}
		
		{
#ifdef NG_DEBUG
			NG_PROFILER_SCOPE("Predraw commands");
#endif
			for (auto& renderCommand : m_predrawCommands)
			{
				m_renderer->actionCommand(renderCommand.get());
			}
		}

		{
#ifdef NG_DEBUG
			NG_PROFILER_SCOPE("Renderer");
#endif
			m_renderer->beginScene(m_sceneData);

			for (auto& GO : m_gameObjects)
			{
				auto material = GO.second->getComponent<MaterialComponent>();			
				if (material != nullptr)
				{				
					m_renderer->submit(material->getMaterial());
				}			
			}
		}
		
		{
#ifdef NG_DEBUG
			NG_PROFILER_SCOPE("Postdraw commands");
#endif
			for (auto& renderCommand : m_postdrawCommands)
			{
				m_renderer->actionCommand(renderCommand.get());
			}

		}
	}

	void JsonLayer::onEvent(Event & e)
	{
		m_cameraController->onEvent(e);

		for (auto& GO : m_gameObjects)
		{
			GO.second->onEvent(e);
		}
	}
}