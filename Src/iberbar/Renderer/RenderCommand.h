#pragma once

#include <iberbar/Renderer/Headers.h>
#include <iberbar/Renderer/ShaderVariables.h>

namespace iberbar
{
	namespace Renderer
	{

		enum class URenderCommandType
		{
			Unknown,
			Triangles,
			DrawMesh,
			Callback,
			Group
		};

		class __iberbarRendererApi__ CRenderCommand
		{
		public:
			CRenderCommand( URenderCommandType nType );
			virtual ~CRenderCommand();

		public:
			URenderCommandType GetCommandType() const;
			void SetZOrder( int nZOrder );
			int GetZOrder() const;

		protected:
			URenderCommandType m_nCommandType;
			int m_nZOrder;
		};
	}
}


FORCEINLINE iberbar::Renderer::URenderCommandType iberbar::Renderer::CRenderCommand::GetCommandType() const
{
	return m_nCommandType;
}


FORCEINLINE void iberbar::Renderer::CRenderCommand::SetZOrder( int nZOrder )
{
	m_nZOrder = nZOrder;
}


FORCEINLINE int iberbar::Renderer::CRenderCommand::GetZOrder() const
{
	return m_nZOrder;
}

