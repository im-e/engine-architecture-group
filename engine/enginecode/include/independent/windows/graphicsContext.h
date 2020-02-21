/*! 
\file graphicsContext.h
\brief Sorts graphics context (e.g. OpenGL) out
*/

#pragma once
#include <mutex>

namespace Engine
{
	/*!
	\class GraphicsContext
	\brief Interface class for graphics context implementation (e.g. OpenGL or Direct3D)
	*/
	class GraphicsContext
	{
	protected:
		std::mutex m_gpuAccessMutex; //!< Mutex to access GPU (graphics context)
	public:
		virtual void init() = 0; //!< Initializes graphic's context
		virtual void swapBuffers() = 0; //!< Swaps buffers in a window

		//! Unbinds current thread (and graphics context)
		virtual void unbindCurrentThread() = 0;
		//! Swaps to current thread (and graphics context)
		virtual void swapToCurrentThread() = 0;

		//! Destructor
		~GraphicsContext()
		{ 
			m_gpuAccessMutex.unlock();
		}
	};
}
