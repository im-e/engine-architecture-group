/*! 
\file graphicsContext.h
\brief Sorts graphics context (e.g. OpenGL) out
*/

#pragma once

namespace Engine
{
	/*!
	\class GraphicsContext
	\brief Interface class for graphics context implementation (e.g. OpenGL or Direct3D)
	*/
	class GraphicsContext
	{
	public:
		virtual void init() = 0; //!< Initializes graphic's context
		virtual void swapBuffers() = 0; //!< Swaps buffers in a window
	};
}
