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
				virtual void SetShaderVariableTable( IShaderVariableTable* pShaderVariableTable ) override;
				virtual void DrawElements( UPrimitiveType nPrimitiveType, UIndexFormat nIndexFormat, uint32 nCount, uint32 nOffset ) override;

			protected:
				void PrepareDraw();
				void PrepareShaderVariables();
				void CleanResources();

			protected:
				CDevice* m_pDevice;
				CShaderState* m_pShaderState;
				CShaderVariableTable* m_pShaderVariableTable;
				CVertexBuffer* m_pVertexBuffer;
				CIndexBuffer* m_pIndexBuffer;
				CTexture* m_pTexture[ 8 ];
			};
		}
	}
}

