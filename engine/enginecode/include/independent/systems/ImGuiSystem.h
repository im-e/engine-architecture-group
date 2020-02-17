#pragma once

/*!\file ImGuiSystem
\brief Responsible to start and stop ImGui
*/

#include "systems/system.h"

#include <imgui.h>
#include <examples/imgui_impl_opengl3.h>
#include <examples/imgui_impl_glfw.h>

namespace Engine
{
	/*! \class ImGuiSystem
	\brief responsible for handling ImGui system
	*/
	class ImGuiSystem : public System
	{
	private:

	public:
		// Inherited via System
		virtual void start(SystemSignal init, ...) override;

		virtual void stop(SystemSignal close, ...) override;

	};
}