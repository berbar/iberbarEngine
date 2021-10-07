#pragma once


#include <iberbar/RHI/D3D11/Headers.h>
#include <iberbar/RHI/Types.h>

namespace iberbar
{
	namespace RHI
	{
		namespace D3D11
		{
			struct UShaderVariableDeclarationNode
			{
				UShaderVariableDeclarationNode()
					: nShaderType()
					, nVarType( UShaderVariableType::VT_Unknown )
					, nOffset( 0 )
					, nElementSize( 0 )
					, nElementCount( 0 )
					, nTotalSize( 0 )
					, D3DHandle( nullptr )
					, D3DDesc()
				{
				}
				UShaderType nShaderType;
				UShaderVariableType nVarType;
				uint32 nOffset;
				uint32 nElementSize;
				uint32 nElementCount;
				uint32 nTotalSize;
				D3DXHANDLE D3DHandle;
				D3DXCONSTANT_DESC D3DDesc;
			};


			class CShaderVariableDeclaration
			{
			public:
				CShaderVariableDeclaration();

			private:

			};
		}
	}
}