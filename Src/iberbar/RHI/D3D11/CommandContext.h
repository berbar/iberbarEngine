#pragma once

#include <iberbar/RHI/D3D11/Headers.h>
#include <iberbar/RHI/D3D11/StateCache.h>
#include <iberbar/RHI/CommandContext.h>


namespace iberbar
{
	namespace RHI
	{
		namespace D3D11
		{
			class CDevice;
			class CShaderState;
			class CShaderVariableTable;
			class CVertexBuffer;
			class CIndexBuffer;
			class CBlendState;
			class CTexture;

			class CCommandContext
				: public ICommandContext
			{
			public:
				CCommandContext( CDevice* pDevice );
				virtual ~CCommandContext();

			public:
				virtual void SetVertexBuffer( uint32 nStreamIndex, IVertexBuffer* pVertexBuffer, uint32 nOffset ) override;
				virtual void SetIndexBuffer( IIndexBuffer* pIndexBuffer, uint32 nOffset ) override;
				virtual void SetShaderState( IShaderState* pShaderState ) override;
				//virtual void SetShaderVariableTable( EShaderType eShaderType, IShaderVariableTable* pShaderVariableTable ) override;
				virtual void SetTexture( EShaderType nShaderType, uint32 nTextureIndex, ITexture* pTexture ) override;
				virtual void SetSamplerState( EShaderType nShaderType, uint32 nSamplerIndex, ISamplerState* pSamplerState ) override;
				virtual void SetUniformBuffer( EShaderType nShaderType, uint32 nBufferIndex, IUniformBuffer* pUniformBuffer ) override;
				virtual void SetBlendFactor( const CColor4F& Factor ) override;
				virtual void SetBlendState( IBlendState* pBlendState, const CColor4F& Factor ) override;
				virtual void SetDepthStencilState( IDepthStencilState* pDepthStencilState, uint32 nStencilRef ) override;
				virtual void SetPrimitiveTopology( UPrimitiveType nPrimitiveType ) override;
				virtual void DrawPrimitive( uint32 nBaseVertexIndex, uint32 nNumPrimitives ) override;
				virtual void DrawIndexed( uint32 nIndexStart, uint32 nIndexCount, uint32 nBaseVertexStart ) override;


			protected:
				ID3D11DeviceContext* m_pD3DDeviceContext;
				UPrimitiveType m_nPrimitiveType;
				CStateCache m_StateCache;

			//protected:
			//	void PrepareDraw();
			//	void PrepareShaderVariableTable( EShaderType eShaderType, CShaderVariableTable* pShaderVariableTable );
			//	void PrepareShaderVariables();
			//	void PrepareBlendState();
			//	void CleanResources();

			//protected:
			//	CDevice* m_pDevice;
			//	CShaderState* m_pShaderState;
			//	CShaderVariableTable* m_pShaderVariableTables[ (int)EShaderType::__Count ];
			//	CVertexBuffer* m_pVertexBuffer;
			//	CIndexBuffer* m_pIndexBuffer;
			//	CBlendState* m_pBlendState;
			//	CBlendState* m_pBlendStateDefault;
			//	CTexture* m_pTextures[ 8 ];
			};
		}
	}
}