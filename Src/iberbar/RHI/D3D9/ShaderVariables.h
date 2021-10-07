#pragma once


#include <iberbar/RHI/D3D9/Headers.h>
#include <iberbar/RHI/D3D9/Shader.h>
#include <iberbar/RHI/D3D9/Texture.h>
#include <iberbar/RHI/D3D9/RenderState.h>
#include <iberbar/RHI/ShaderVariables.h>
#include <iberbar/Utility/Buffer.h>

#ifdef _DEBUG
#include <unordered_map>
#endif

namespace iberbar
{
	namespace RHI
	{
		namespace D3D9
		{

#ifdef _DEBUG
			union UShaderVariantData
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
				const CShaderReflectionVariable* VarReflection;
				UShaderVariantData VarData;
			};
#endif

			struct UShaderSampler
			{
				UShaderSampler()
					: pTexture( nullptr )
					, pSamplerState( nullptr )
				{
				}
				UShaderSampler( const UShaderSampler& Sampler )
					: pTexture( Sampler.pTexture )
					, pSamplerState( Sampler.pSamplerState )
				{
				}
				bool operator == ( const UShaderSampler& other ) const;
				bool operator != ( const UShaderSampler& other ) const;
				TSmartRefPtr<CTexture> pTexture;
				TSmartRefPtr<CSamplerState> pSamplerState;
			};


			class __iberbarD3DApi__ CShaderVariableTable
				: public IShaderVariableTable
			{
			public:
				CShaderVariableTable( CDevice* pDevice );
				virtual ~CShaderVariableTable();

			public:
				virtual void SetShader( IShader* pShader ) override;
				virtual void SetBool( const char* strName, bool value ) override;
				virtual void SetInt( const char* strName, int value ) override;
				virtual void SetFloat( const char* strName, float value ) override;
				virtual void SetVector( const char* strName, const DirectX::XMFLOAT4& value ) override;
				virtual void SetMatrix( const char* strName, const DirectX::XMFLOAT4X4& value ) override;
				virtual void SetTexture( const char* strName, RHI::ITexture* pTexture ) override;
				virtual void SetSamplerState( const char* strName, const UTextureSamplerState& SamplerDesc ) override;

				virtual bool Compare( IShaderVariableTable* pVariableTable ) override;

				CShader* GetShaderInternal() { return m_pShader; }
				const CShaderReflection* GetShaderReflection() const { return m_pShader->GetReflectionInner(); }
				const uint8* GetBuffer() const { return m_Buffer.GetPointer(); }
				const uint32 GetBufferSize() const { return m_Buffer.GetDataSize(); }
				//const std::vector<UShaderSampler>& GetSamplers() const { return m_Samplers; }
				const std::vector<TSmartRefPtr<CTexture>>& GetTextures() const { return m_Textures; }
				const std::vector<TSmartRefPtr<CSamplerState>>& GetSamplerStates() const { return m_SamplerStates; }

			protected:
				CDevice* m_pDevice;
				CShader* m_pShader;
				TBuffer<uint8> m_Buffer;
				
				std::vector<TSmartRefPtr<CTexture>> m_Textures;
				std::vector<TSmartRefPtr<CSamplerState>> m_SamplerStates;
#ifdef _DEBUG
				std::unordered_map<std::string, UShaderVariant> m_VarsDebug;
#endif
			};
		}
	}
}


inline bool iberbar::RHI::D3D9::UShaderSampler::operator==( const UShaderSampler& other ) const
{
	return (pTexture == other.pTexture && pSamplerState == other.pSamplerState );
}


inline bool iberbar::RHI::D3D9::UShaderSampler::operator!=( const UShaderSampler& other ) const
{
	return (pTexture != other.pTexture || pSamplerState != other.pSamplerState );
}

