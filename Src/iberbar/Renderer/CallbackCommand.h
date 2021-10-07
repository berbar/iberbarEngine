#pragma once


#include <iberbar/Renderer/RenderCommand.h>
#include <functional>


namespace iberbar
{
	namespace Renderer
	{
		class __iberbarRendererApi__ CRenderCallbackCommand
			: public CRenderCommand
		{
		public:
			CRenderCallbackCommand();

			void SetProc( std::function<void()> Func ) { m_Func = Func; }
			void Execute() { if ( m_Func ) m_Func(); }

		protected:
			std::function<void()> m_Func;
		};
	}
}