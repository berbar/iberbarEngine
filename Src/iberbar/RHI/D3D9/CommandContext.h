#pragma once

#include <iberbar/RHI/D3D9/Headers.h>
#include <iberbar/RHI/CommandContext.h>


namespace iberbar
{
	namespace RHI
	{
		namespace D3D9
		{
			class CDevice;
			class CShaderState;
			class CShaderVariableTable;
			class CVertexBuffer;
			class CIndexBuffer;
			class CBlendState;
			class CTexture;

			class __iberbarD3DApi__ CCommandContext
				: public ICommandContext
			{
			public:
				CCommandContext( CDevice* pDevice );
				virtual ~CCommandContext();

			public:
				virtual void SetVertexBuffer( IVertexBuffer* pVertexBuffer ) override;
				virtual void SetIndexBuffer( IIndexBuffer* pIndexBuffer ) override;
				virtual void SetShaderState( IShaderState* pShaderState ) override;
				virtual void SetShaderVariableTable( EShaderType eShaderType, IShaderVariableTable* pShaderVariableTable ) override;
				virtual void SetBlendState( IBlendState* pBlendState ) override;
				virtual void SetBlendStateDefault( IBlendState* pBlendState ) override;
				virtual void DrawElements( UPrimitiveType nPrimitiveType, UIndexFormat nIndexFormat, uint32 nCount, uint32 nOffset ) override;

			protected:
				void PrepareDraw();
				void PrepareShaderVariableTable( EShaderType eShaderType, CShaderVariableTable* pShaderVariableTable );
				void PrepareShaderVariables();
				void PrepareBlendState();
				void CleanResources();

			protected:
				CDevice* m_pDevice;
				CShaderState* m_pShaderState;
				CShaderVariableTable* m_pShaderVariableTables[ (int)EShaderType::__Count ];
				CVertexBuffer* m_pVertexBuffer;
				CIndexBuffer* m_pIndexBuffer;
				CBlendState* m_pBlendState;
				CBlendState* m_pBlendStateDefault;
				CTexture* m_pTexture[ 8 ];
			};
		}
	}
}

