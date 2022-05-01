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



			//class CUniformMemoryBuffer
			//{
			//public:
			//	CUniformMemoryBuffer();
			//	~CUniformMemoryBuffer();

			//	void SetShader( CShader* pShader );
			//	//void SetTexture( uint32 nSlot, CTexture* pTexture );
			//	//void SetSamplerState( uint32 nSlot, CSamplerState* pSamplerState );

			//	FORCEINLINE uint8* GetMemory() { return m_pMemory; }
			//	FORCEINLINE const uint8* GetMemory() const { return m_pMemory; }
			//	FORCEINLINE uint32 GetMemorySize() const { return m_nMemorySize; }
			//	FORCEINLINE const std::vector<uint8*>& GetHeads() const { return m_Heads; }

			//private:
			//	uint8* m_pMemory;
			//	uint32 m_nMemorySize;
			//	std::vector<uint8*> m_Heads;
			//};


			class CShaderVariableTable
				: public IShaderVariableTable
			{
			public:
				struct UBufferState
				{
					uint8 nSlot;
					uint8* pOffset;
				};

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
				const uint8* GetMemory() const { return m_pCommonMemory; }
				const uint32 GetMemorySize() const { return m_nCommonMemorySize; }
				//const std::vector<UBufferState>& GetBufferStates() { return m_Buffers; }
				//const uint8* GetBuffer( int nIndex ) const { return m_Buffers[ nIndex ]; }
				const std::vector<TSmartRefPtr<CTexture>>& GetTextures() const { return m_Textures; }
				const std::vector<TSmartRefPtr<CSamplerState>>& GetSamplerStates() const { return m_SamplerStates; }

			protected:
				CDevice* m_pDevice;
				CShader* m_pShader;
				//CUniformMemoryBuffer m_MemoryStates;
				//std::vector<uint8> m_CommonMemory;
				//std::vector<UBufferState> m_Buffers;
				//
				////std::vector<UShaderSampler> m_Samplers;
				uint8* m_pCommonMemory;
				uint32 m_nCommonMemorySize;
				std::vector<TSmartRefPtr<CTexture>> m_Textures;
				std::vector<TSmartRefPtr<CSamplerState>> m_SamplerStates;
#ifdef _DEBUG
				std::unordered_map<std::string, UShaderVariant> m_VarsDebug;
#endif
			};
		}
	}
}

