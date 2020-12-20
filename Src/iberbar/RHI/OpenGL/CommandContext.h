#pragma once

#include <iberbar/RHI/CommandContext.h>
#include <iberbar/RHI/OpenGL/Headers.h>

namespace iberbar
{
	namespace RHI
	{
		class CShaderState;

		namespace OpenGL
		{
			class CDevice;
			class CVertexBuffer;
			class CIndexBuffer;
			class CShaderState;
			class CShaderVariableTable;
			class CTexture;

			class __iberbarRHIOpenGLApi__ CCommandContext
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

			private:
				void PrepareDraw();
				void PrepareShaderVariables();
				void SetUniform( UShaderVariableType nVarType, GLuint nLocation, const void* pData, uint32 nElementCount );
				void CleanResources();

			private:
				CDevice* m_pDevice;
				CVertexBuffer* m_pVertexBuffer;
				CIndexBuffer* m_pIndexBuffer;
				CShaderState* m_pShaderState;
				CShaderVariableTable* m_pShaderVariableTable;
				CTexture* m_pTextureSlot[ 8 ];
			};
		}
	}
}
