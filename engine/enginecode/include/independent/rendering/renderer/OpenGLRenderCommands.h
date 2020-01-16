#pragma once

/*! \file OpenGLRenderCommands.h
\brief OpenGL implementation of rendering commands
*/

#include "Renderer.h"

#include <glad/glad.h>

namespace Engine
{
	//for OpenGL debugging
	inline void GLAPIENTRY
		MessageCallback(GLenum source,
			GLenum type,
			GLuint id,
			GLenum severity,
			GLsizei length,
			const GLchar* message,
			const void* userParam)
	{
		fprintf(stderr, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
			(type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : ""),
			type, severity, message);
	}

	/*! \class OpenGLClearDepthColorBufferCommand
	\brief Responsible for enabling OpenGL depth
	*/
	class OpenGLClearDepthColorBufferCommand : public RenderCommand
	{
	public:
		//! Default constructor \param alive shall the command be deleted straight after using it?
		OpenGLClearDepthColorBufferCommand(bool alive) { m_alive = alive; };

		void action() override
		{
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		}
	};

	/*! \class OpenGLSetClearColorCommand
	\brief responsible for setting OpenGL background color 
	*/
	class OpenGLSetClearColorCommand : public RenderCommand
	{
	private:
		float m_r; //!< Red value
		float m_g; //!< Blue value
		float m_b; //!< Green value
		float m_a; //!< Alpha value
	public:
		/*! Custom constructor 
		\param alive shall the command be deleted straight after using it?
		\param r red value
		\param g green value
		\param b blue value
		\param a alpha value
		*/
		OpenGLSetClearColorCommand(bool alive, float r, float g, float b, float a)
			: m_r(r), m_g(g), m_b(b), m_a(a)
		{
			m_alive = alive;
		};

		void action() override
		{
			glClearColor(m_r, m_g, m_b, m_a);
		}
	};

	/*! \class OpenGLSetDepthTestLessCommand
	\brief Responsible for setting OpenGL depth test
	*/
	class OpenGLSetDepthTestLessCommand : public RenderCommand
	{
	private:
		bool m_enabled; //!< Has the depth testing been enabled?
	public:
		//! Custom constructor \param enabled if the command's in effect \param alive shall the command be deleted straight after using it?
		OpenGLSetDepthTestLessCommand(bool alive, bool enabled) : m_enabled(enabled) { m_alive = alive; };

		void action() override
		{
			if (m_enabled == true)
			{
				glEnable(GL_DEPTH_TEST);
				glDepthFunc(GL_LESS);
			}
			else
			{
				glDisable(GL_DEPTH_TEST);
			}
		}
	};

	/*! \class OpenGLSetBackfaceCullingCommand
	\brief Responsible for setting OpenGL backface culling
	*/
	class OpenGLSetBackfaceCullingCommand : public RenderCommand
	{
	private:
		bool m_enabled; //!< Has the backface culling been enabled?
	public:
		//! Custom constructor \param enabled if the command's in effect \param alive shall the command be deleted straight after using it?
		OpenGLSetBackfaceCullingCommand(bool alive, bool enabled) : m_enabled(enabled) { m_alive = alive; };

		void action() override
		{
			if (m_enabled == true)
			{
				glEnable(GL_CULL_FACE);
				glCullFace(GL_BACK);
			}
			else
			{
				glDisable(GL_CULL_FACE);
			}
		}
	};

	/*! \class OpenGLSetDebugCommand
	\brief Responsible for setting OpenGL debug output
	*/
	class OpenGLSetDebugCommand : public RenderCommand
	{
	private:
		bool m_enabled; //!< Has the debug output been enabled?
	public:
		//! Custom constructor \param enabled if the command's in effect \param alive shall the command be deleted straight after using it?
		OpenGLSetDebugCommand(bool alive, bool enabled) : m_enabled(enabled) { m_alive = alive; };

		void action() override
		{		
#ifdef NG_DEBUG
			if (m_enabled == true)
			{
				// During init, enable debug output
				glEnable(GL_DEBUG_OUTPUT);
				glDebugMessageCallback(MessageCallback, 0);
			}
			else
			{
				glDisable(GL_DEBUG_OUTPUT);
			}
#endif
		}
	};

	/*! \class OpenGLSetMinusOneAlphaBlending
	\brief Responsible for setting blending alpha in OpenGL
	*/
	class OpenGLSetMinusOneAlphaBlending : public RenderCommand
	{
	private:
		bool m_enabled;  //!< Has the alpha blending been enabled?
	public:
		//! Custom constructor \param enabled has the command been enabled? \param alive shall the command be deleted straight after using it?
		OpenGLSetMinusOneAlphaBlending(bool alive, bool enabled) : m_enabled(enabled) { m_alive = alive; };
		void action() override
		{
			if (m_enabled == true)
			{
				glEnable(GL_BLEND);
				glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			}
			else
			{
				glDisable(GL_BLEND);
			}
		}
	};
}