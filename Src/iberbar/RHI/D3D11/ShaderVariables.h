#pragma once



#include <iberbar/RHI/D3D11/Headers.h>
#include <iberbar/RHI/D3D11/Shader.h>
#include <iberbar/RHI/D3D11/Texture.h>
#include <iberbar/RHI/D3D11/RenderState.h>
#include <iberbar/RHI/ShaderVariables.h>
#include <iberbar/Utility/Buffer.h>

#ifdef _DEBUG
#include <unordered_map>
#endif

namespace iberbar
{
	namespace RHI
	{
		namespace D3D11
		{
			class CDevice;

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
				UShaderSampler( const UShaderSampler& sampler )
					: pTexture( sampler.pTexture )
					, pSamplerState( sampler.pSamplerState )
				{
				}
				inline bool operator == ( const UShaderSampler& other ) const
				{
					return pTexture == other.pTexture && pSamplerState == other.pSamplerState;
				}
				inline bool operator != ( const UShaderSampler& other ) const
				{
					return pTexture != other.pTexture || pSamplerState != other.pSamplerState;
				}
				TSmartRefPtr<CTexture> pTexture;
				TSmartRefPtr<CSamplerState> pSamplerState;
			};

			class CShaderVariableTable
				: public IShaderVariableTable
			{
			public:
				CShaderVariableTable( CDevice* pDevice );
				virtual ~CShaderVariableTable();

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
				const CShaderReflection* GetShaderReflection() const { return m_pShader->GetReflectionInternal(); }
				const uint8* GetBuffer( int nIndex ) const { return m_Buffers[ nIndex ]; }
				const std::vector<TSmartRefPtr<CTexture>>& GetTextures() const { return m_Textures; }
				const std::vector<TSmartRefPtr<CSamplerState>>& GetSamplerStates() const { return m_SamplerStates; }

			protected:
				CDevice* m_pDevice;
				CShader* m_pShader;
				std::vector<uint8> m_CommonMemory;
				std::vector<uint8*> m_Buffers;
				//std::vector<UShaderSampler> m_Samplers;
				std::vector<TSmartRefPtr<CTexture>> m_Textures;
				std::vector<TSmartRefPtr<CSamplerState>> m_SamplerStates;
#ifdef _DEBUG
				std::unordered_map<std::string, UShaderVariant> m_VarsDebug;
#endif
			};
		}
	}
}

