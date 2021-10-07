#pragma once

#include <iberbar/RHI/Headers.h>
#include <iberbar/RHI/Types.h>
#include <DirectXMath.h>


namespace iberbar
{
	namespace RHI
	{
		class ITexture;
		class IShader;


#ifdef _DEBUG
#endif


		class __iberbarRHIApi__ IShaderVariableTable abstract
			: public CRef
		{
		public:
			virtual void SetShader( IShader* pShader ) = 0;
			virtual void SetBool( const char* strName, bool value ) = 0;
			virtual void SetInt( const char* strName, int value ) = 0;
			virtual void SetFloat( const char* strName, float value ) = 0;
			virtual void SetVector( const char* strName, const DirectX::XMFLOAT4& value ) = 0;
			virtual void SetMatrix( const char* strName, const DirectX::XMFLOAT4X4& value ) = 0;
			virtual void SetTexture( const char* strName, RHI::ITexture* pTexture ) = 0;
			virtual void SetSamplerState( const char* strName, const UTextureSamplerState& SamplerDesc ) = 0;

			virtual bool Compare( IShaderVariableTable* pVariableTable ) = 0;
		};

		union UShaderVariableTableUnion1
		{
			struct
			{
				IShaderVariableTable* pTableVertexShader;
				IShaderVariableTable* pTablePixelShader;
			};
			IShaderVariableTable* pTables[ 2 ];
		};


		class CShaderVariableTableUnion
		{
		public:
			CShaderVariableTableUnion()
				: pTableVertexShader( nullptr )
				, pTablePixelShader( nullptr )
				, pTableHullShader( nullptr )
				, pTableGeometryShader( nullptr )
				, pTableDomainShader( nullptr )
				, pTableComputeShader( nullptr )
			{
			}

			~CShaderVariableTableUnion()
			{
				for ( int i = 0, s = (int)EShaderType::__Count; i < s; i++ )
				{
					UNKNOWN_SAFE_RELEASE_NULL( pTables[ i ] );
				}
			}

		public:
			FORCEINLINE void SetVariableTable( EShaderType eShaderType, IShaderVariableTable* pTable )
			{
				UNKNOWN_SAFE_RELEASE_NULL( pTables[ (int)eShaderType ] );
				pTables[ (int)eShaderType ] = pTable;
				UNKNOWN_SAFE_ADDREF( pTables[ (int)eShaderType ] );
			}
			IShaderVariableTable* GetVariableTable( EShaderType eShaderType ) { return pTables[ (int)eShaderType ]; }
			IShaderVariableTable* GetVariableTableForVertexShader() { return pTableVertexShader; }
			IShaderVariableTable* GetVariableTableForPixelShader() { return pTablePixelShader; }
			IShaderVariableTable* GetVariableTableForHullShader() { return pTableHullShader; }
			IShaderVariableTable* GetVariableTableForGeometryShader() { return pTableGeometryShader; }
			IShaderVariableTable* GetVariableTableForDomainShader() { return pTableDomainShader; }
			IShaderVariableTable* GetVariableTableForComputeShader() { return pTableComputeShader; }

		private:
			union
			{
				struct
				{
					IShaderVariableTable* pTableVertexShader;
					IShaderVariableTable* pTablePixelShader;
					IShaderVariableTable* pTableHullShader;
					IShaderVariableTable* pTableGeometryShader;
					IShaderVariableTable* pTableDomainShader;
					IShaderVariableTable* pTableComputeShader;
				};
				IShaderVariableTable* pTables[ (int)EShaderType::__Count ];
			};
		};

	}
}


