#pragma once

#include <iberbar/Renderer/Types.h>
#include <iberbar/Renderer/RenderCommandQueue.h>
#include <iberbar/Renderer/CallbackCommand.h>


namespace iberbar
{
	namespace Renderer
	{
		class IRenderer;

		// 为单独的渲染队列指定特殊的渲染处理器
		class __iberbarRendererApi__ CBaseRendererProcessor abstract
		{
		public:
			CBaseRendererProcessor() {}
			virtual ~CBaseRendererProcessor() {}

			virtual void VisitQueue( CRenderQueue* pQueue ) = 0;
			virtual void Flush() = 0;

		protected:
			void DefaultVisitCommand( CRenderCommand* pCommand );
		};
	}
}



FORCEINLINE void iberbar::Renderer::CBaseRendererProcessor::DefaultVisitCommand( CRenderCommand* pCommand )
{
	switch ( pCommand->GetCommandType() )
	{
	case URenderCommandType::Callback:
		Flush();
		((CRenderCallbackCommand*)pCommand)->Execute();
		break;

	case URenderCommandType::Group:
		//Flush();
		//(m_pRenderer->*m_pRendererVisitQueue)(pCommand);
		break;

	default:
		break;
	}
}
