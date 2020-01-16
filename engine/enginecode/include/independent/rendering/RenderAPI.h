#pragma once

/*! \file RenderAPI.h
\brief Defines what rendering API is used
*/
namespace Engine
{
	/*! \class RenderAPI
	\brief Defines current render API
	*/
	class RenderAPI
	{
	public:
		/*! \enum API
		\brief Set of possible render APIs
		*/
		enum class API
		{
			None = 0, OpenGL = 1, Direct3D = 2
		};

		//! Get what API is currently in use. \return current render API
		inline static API getAPI() { return s_api; }

	private:
		static API s_api; //!< Current render API
	};
}