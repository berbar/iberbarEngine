#pragma once

#include <iberbar/Renderer/Processor/BaseRendererProcessor.h>

namespace iberbar
{
	namespace RHI
	{
		class ICommandContext;
	}

	namespace Renderer
	{
		class CRenderCommand;
		class CTrianglesCommand;
		class CRenderGroupCommand;
		class CRenderer2dState;

		class __iberbarRendererApi__ CDefaultRendererProcessor
			: public CBaseRendererProcessor
		{
		public:
			virtual void VisitQueue( CRenderQueue* pQueue ) override;
			virtual void Flush() override;

		protected:
			void VisitCommandList( const URenderCommandList& commandList );
			void VisitCommand( CRenderCommand* pCommand );
			void DrawBatchTriangles();
			void DrawOneTriangles( CTrianglesCommand* pCommand );
			void Flush();
			void ProcessGroupCommand( CRenderGroupCommand* pCommand );

		private:
			CRenderer2dState* m_pState;
			RHI::ICommandContext* m_pCommandContext;
			RHI::IIndexBuffer* m_pIndexBuffer;
			RHI::IVertexBuffer* m_VertexBuffers[ RHI::MaxVertexElementCount ];
		};
	}
}