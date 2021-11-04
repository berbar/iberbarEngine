#pragma once

#include <iberbar/Renderer/Headers.h>
#include <DirectXMath.h>

#ifdef _DEBUG
#include <unordered_map>
#endif

namespace iberbar
{
	namespace RHI
	{
		class IShader;
		class IShaderReflection;
		class IShaderReflectionVariable;
		class ITexture;
		class ISamplerState;
	}

	namespace Renderer
	{
#ifdef _DEBUG
		/*union UShaderVariantData
		{
			void* _v_void;
			int32* _v_boolean;
			int32* _v_int;
			float* _v_float;
			DirectX::XMFLOAT4* _v_float4;
			DirectX::XMFLOAT4X4* _v_float4x4;
		};
		struct UShaderVariant
		{
			const RHI::IShaderReflectionVariable* VarReflection;
			UShaderVariantData VarData;
		};*/

		class IShaderVariableVariant abstract
		{
		};

		template < typename T >
		class TShaderVariableVariant
			: public IShaderVariableVariant
		{
		public:
			TShaderVariableVariant( const RHI::IShaderReflectionVariable* pReflectionVariable, uint32 nCountMax )
				: m_pReflectionVariable( pReflectionVariable )
				, m_Values()
			{
				m_Values.resize( nCountMax );
			}

			void Set( const T* pValues, uint32 nCount )
			{
				nCount = tMin( nCount, (uint32)m_Values.size() );
				for ( uint32 i = 0; i < nCount; i++ )
				{
					m_Values[ i ] = pValues[ i ];
				}
			}

		protected:
			const RHI::IShaderReflectionVariable* m_pReflectionVariable;
			std::vector<T> m_Values;
		};
#endif


		class __iberbarRendererApi__ CShaderVariableTable
		{
		public:
			CShaderVariableTable();
			~CShaderVariableTable();

			void SetShader( RHI::IShader* pShader );
			bool SetInt( const char* strName, int32 Value );
			bool SetIntArray( const char* strName, const int32* pValues, uint32 nCount );
			bool SetFloat( const char* strName, float value );
			bool SetFloatArray( const char* strName, const float* pValues, uint32 nCount );
			bool SetVector( const char* strName, const DirectX::XMFLOAT4& Value );
			bool SetVectorArray( const char* strName, const DirectX::XMFLOAT4* pValues, uint32 nCount );
			bool SetMatrix( const char* strName, const DirectX::XMFLOAT4X4& Value );
			bool SetMatrixArray( const char* strName, const DirectX::XMFLOAT4X4* pValues, uint32 nCount );
			bool SetColor( const char* strName, const CColor4F& Value );
			bool SetColorArray( const char* strName, const CColor4F* pValues, uint32 nCount );
			bool SetStruct( const char* strName, const void* Value, uint32 nSize );
			bool SetTexture( const char* strName, RHI::ITexture* pTexture );
			bool SetSamplerState( const char* strName, RHI::ISamplerState* pSamplerState );
			bool Compare( const CShaderVariableTable* pVariableTable ) const;

			const uint8* GetMemory() const { return m_pCommonMemory; }
			const uint32 GetMemorySize() const { return m_nCommonMemorySize; }
			const std::vector<RHI::ITexture*>& GetTextures() const { return m_Textures; }
			const std::vector<RHI::ISamplerState*>& GetSamplerStates() const { return m_SamplerStates; }

		protected:
			void Clear();

		protected:
			RHI::IShaderReflection* m_pShaderReflection;
			uint8* m_pCommonMemory;
			uint32 m_nCommonMemorySize;
			std::vector<RHI::ITexture*> m_Textures;
			std::vector<RHI::ISamplerState*> m_SamplerStates;
#ifdef _DEBUG
			std::unordered_map<std::string, IShaderVariableVariant*> m_VarsDebug;
#endif
		};
	}
}
