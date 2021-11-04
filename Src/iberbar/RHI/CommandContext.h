#pragma once

#include <iberbar/RHI/Types.h>

namespace iberbar
{
	namespace RHI
	{
		class IShader;
		class IVertexBuffer;
		class IIndexBuffer;
		class IUniformBuffer;
		class ITexture;
		class IShaderState;
		class IShaderVariableTable;
		class IBlendState;
		class IDepthStencilState;
		class ISamplerState;

		class __iberbarRHIApi__ ICommandContext abstract
		{
		public:
			virtual ~ICommandContext() {}

		public:
			virtual void SetVertexBuffer( uint32 nStreamIndex, IVertexBuffer* pVertexBuffer, uint32 nOffset ) = 0;
			virtual void SetIndexBuffer( IIndexBuffer* pIndexBuffer, uint32 nOffset ) = 0;
			virtual void SetShaderState( IShaderState* pShaderState ) = 0;
			//virtual void SetShaderVariableTable( EShaderType eShaderType, IShaderVariableTable* pShaderVariableTable ) = 0;
			virtual void SetTexture( EShaderType nShaderType, uint32 nTextureIndex, ITexture* pTexture ) = 0;
			virtual void SetSamplerState( EShaderType nShaderType, uint32 nSamplerIndex, ISamplerState* pSamplerState ) = 0;
			virtual void SetUniformBuffer( EShaderType nShaderType, uint32 nBufferIndex, IUniformBuffer* pUniformBuffer ) = 0;
			virtual void SetBlendFactor( const CColor4F& Factor ) = 0;
			virtual void SetBlendState( IBlendState* pBlendState, const CColor4F& Factor ) = 0;
			virtual void SetDepthStencilState( IDepthStencilState* pDepthStencilState, uint32 nStencilRef ) = 0;
			virtual void SetPrimitiveTopology( UPrimitiveType nPrimitiveType ) = 0;
			//virtual void DrawElements( UPrimitiveType nPrimitiveType, UIndexFormat nIndexFormat, uint32 nCount, uint32 nOffset ) = 0;
			virtual void DrawPrimitive( uint32 nBaseVertexIndex, uint32 nNumPrimitives ) = 0;
			virtual void DrawIndexedPrimitive( uint32 nStartIndex, uint32 nBaseVertexIndex, uint32 nNumPrimitives ) = 0;
		};
	}
}