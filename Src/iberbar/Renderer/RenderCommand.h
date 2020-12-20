#pragma once

#include <iberbar/Renderer/Headers.h>

namespace iberbar
{
	namespace RHI
	{
		class IShaderState;
		class IShaderVariableTable;
	}

	namespace Renderer
	{

		enum class URenderCommandType
		{
			Unknown,
			Triangles
		};

		class __iberbarRendererApi__ CRenderCommand
		{
		public:
			CRenderCommand();
			virtual ~CRenderCommand();

		public:
			URenderCommandType GetCommandType() const;
			void SetZOrder( int nZOrder );
			int GetZOrder() const;
			void SetShaderState( RHI::IShaderState* pEffectState );
			void SetShaderVariableTable( RHI::IShaderVariableTable* pShaderVariableTable );
			RHI::IShaderState* GetShaderState() const;
			RHI::IShaderVariableTable* GetShaderVariableTable() const;

		protected:
			URenderCommandType m_nCommandType;
			int m_nZOrder;
			RHI::IShaderState* m_pShaderState;
			RHI::IShaderVariableTable* m_pShaderVariableTable;
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


FORCEINLINE void iberbar::Renderer::CRenderCommand::SetShaderState( RHI::IShaderState* pEffectState )
{
	m_pShaderState = pEffectState;
}


FORCEINLINE void iberbar::Renderer::CRenderCommand::SetShaderVariableTable( RHI::IShaderVariableTable* pShaderVariableTable )
{
	m_pShaderVariableTable = pShaderVariableTable;
}


FORCEINLINE iberbar::RHI::IShaderState* iberbar::Renderer::CRenderCommand::GetShaderState() const
{
	return m_pShaderState;
}


FORCEINLINE iberbar::RHI::IShaderVariableTable* iberbar::Renderer::CRenderCommand::GetShaderVariableTable() const
{
	return m_pShaderVariableTable;
}
