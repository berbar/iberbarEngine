#pragma once



#include <iberbar/RHI/D3D11/Headers.h>
#include <iberbar/RHI/Types.h>


namespace iberbar
{
	namespace RHI
	{
		namespace D3D11
		{
			struct UEffectReflectionVariable
			{
				UEffectReflectionVariable()
					: nShaderType()
					, nVarType( UShaderVariableType::VT_Unknown )
					, nOffset( 0 )
					, nOffsetAbs( 0 )
					, nElementSize( 0 )
					, nElementCount( 0 )
					, nTotalSize( 0 )
					, strName()
					//, D3DHandle( nullptr )
					//, D3DDesc()
				{
				}
				EShaderType nShaderType;
				UShaderVariableType nVarType;
				uint32 nOffset;
				uint32 nOffsetAbs;
				uint32 nElementSize;
				uint32 nElementCount;
				uint32 nTotalSize;
				std::string strName;
			};

			struct UEffectReflectionConstBuffer
			{
				uint32 nBindPoint;
				uint32 nOffset;
				uint32 nSize;
				std::string strName;
			};

			class CEffectReflection
			{
			public:
				CEffectReflection();

				CResult Initial( EShaderType eShaderType, ID3D11ShaderReflection* pD3DShaderReflection );

				const std::vector< UEffectReflectionVariable >& GetVarList( EShaderType eShaderType ) const { return m_ReflectionVarArraySlots[(int)eShaderType]; }
				const std::vector< UEffectReflectionConstBuffer >& GetCBufferList( EShaderType eShaderType ) const { return m_ReflectionCBufferArraySlots[(int)eShaderType]; }

			private:
				static UEffectReflectionVariable sBuildReflectionVariable( uint32 nConstBufferBytesOffset, ID3D11ShaderReflectionVariable* pD3DShaderReflectionVariable );


			private:
				std::vector< UEffectReflectionVariable > m_ReflectionVarArraySlots[ (int)EShaderType::__Count ];
				std::vector< UEffectReflectionConstBuffer > m_ReflectionCBufferArraySlots[(int)EShaderType::__Count ];
			};
		}
	}
}

