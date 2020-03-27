#pragma once

/*! \file Editor.h
\brief Starting point for level editor
*/

#include <Engine.h>

namespace Editor
{
	/*! \class LevelEditor
	\brief Begins level editor
	*/
	class LevelEditor : public Engine::Application
	{
	public:
		//! Default constructor
		LevelEditor();
		//! Deconstructor
		~LevelEditor() override;
	};
}
