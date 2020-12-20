#pragma once

#include <iberbar/RHI/Types.h>

namespace iberbar
{
	namespace RHI
	{
		class IShader;
		class IVertexBuffer;
		class IIndexBuffer;
		class ITexture;
		class IShaderState;
		class IShaderVariableTable;

		class __iberbarRHIApi__ ICommandContext abstract
			: public CRef
		{
		//public:
		//	ICommandContext()
		//		: m_pShaderVars( nullptr )
		//	{
		//	}

		public:
			virtual void SetVertexBuffer( IVertexBuffer* pVertexBuffer ) = 0;
			virtual void SetIndexBuffer( IIndexBuffer* pIndexBuffer ) = 0;
			virtual void SetShaderState( IShaderState* pShaderState ) = 0;
			virtual void SetShaderVariableTable( IShaderVariableTable* pShaderVariableTable ) = 0;
			virtual void DrawElements( UPrimitiveType nPrimitiveType, UIndexFormat nIndexFormat, uint32 nCount, uint32 nOffset ) = 0;

		//public:
		//	void SetShaderVariables( CShaderVariables* pShaderVars ) { m_pShaderVars = pShaderVars; }

		//protected:
		//	CShaderVariables* m_pShaderVars;
		};
	}
}