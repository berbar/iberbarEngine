#pragma once

#include <iberbar/Renderer/Headers.h>
#include <iberbar/Renderer/ShaderVariables.h>

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
			//void SetShaderState( RHI::IShaderState* pShaderState );
			//void SetShaderVariableTable( RHI::EShaderType eShaderType, RHI::IShaderVariableTable* pShaderVariableTable );
			//RHI::IShaderState* GetShaderState() const;
			//CShaderVariableTable* GetShaderVariableTable( RHI::EShaderType eShaderType ) const;

		protected:
			URenderCommandType m_nCommandType;
			int m_nZOrder;
			//RHI::IShaderState* m_pShaderState;
			//CShaderVariableTable* m_pShaderVariableTableArray[ (int)RHI::EShaderType::__Count ];
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


//FORCEINLINE void iberbar::Renderer::CRenderCommand::SetShaderState( RHI::IShaderState* pEffectState )
//{
//	m_pShaderState = pEffectState;
//}


//FORCEINLINE void iberbar::Renderer::CRenderCommand::SetShaderVariableTable( RHI::EShaderType eShaderType, RHI::IShaderVariableTable* pShaderVariableTable )
//{
//	m_pShaderVariableTableArray[ (int)eShaderType ] = pShaderVariableTable;
//}


//FORCEINLINE iberbar::RHI::IShaderState* iberbar::Renderer::CRenderCommand::GetShaderState() const
//{
//	return m_pShaderState;
//}
//
//
//FORCEINLINE iberbar::Renderer::CShaderVariableTable* iberbar::Renderer::CRenderCommand::GetShaderVariableTable( RHI::EShaderType eShaderType ) const
//{
//	return m_pShaderVariableTableArray[ (int)eShaderType ];
//}
