#pragma once


#include <iberbar/RHI/D3D9/Headers.h>
#include <iberbar/RHI/ShaderVariables.h>
#include <iberbar/RHI/Texture.h>
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
			class CShader;
			class CShaderState;
			struct UShaderVariableDeclarationNode;

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
				const UShaderVariableDeclarationNode* pDeclNode;
				UShaderVariantData VarData;
			};
#endif

			struct UShaderSampler
			{
				UShaderSampler()
					: pTexture( nullptr )
					, State()
				{
				}
				UShaderSampler( const UShaderSampler& sampler )
					: pTexture( sampler.pTexture )
					, State( sampler.State )
				{
				}
				bool operator == ( const UShaderSampler& other ) const;
				bool operator != ( const UShaderSampler& other ) const;
				TSmartRefPtr<RHI::ITexture> pTexture;
				UTextureSamplerState State;
			};


			class __iberbarD3DApi__ CShaderVariableTable
				: public IShaderVariableTable
			{
			public:
				CShaderVariableTable();
				virtual ~CShaderVariableTable();

			public:
				virtual void SetShaderState( IShaderState* pShaderState ) override;
				virtual void SetBool( UShaderType nShaderType, const char* strName, bool value ) override;
				virtual void SetInt( UShaderType nShaderType, const char* strName, int value ) override;
				virtual void SetFloat( UShaderType nShaderType, const char* strName, float value ) override;
				virtual void SetVector( UShaderType nShaderType, const char* strName, const DirectX::XMFLOAT4& value ) override;
				virtual void SetMatrix( UShaderType nShaderType, const char* strName, const DirectX::XMFLOAT4X4& value ) override;
				virtual void SetSampler( const char* strName, RHI::ITexture* pTexture, UTextureSamplerState SamplerState ) override;

				virtual bool Compare( IShaderVariableTable* pVariableTable ) override;

				const uint8* GetBuffer() const { return m_Buffer.GetPointer(); }
				const uint32 GetBufferSize() const { return m_Buffer.GetDataSize(); }
				const std::vector<const UShaderVariableDeclarationNode*> GetVars() const { return m_Vars; }
				const std::vector<UShaderSampler>& GetSamplers() const { return m_Samplers; }

			protected:
				TBuffer<uint8> m_Buffer;
				CShaderState* m_pShaderState;
				std::vector<const UShaderVariableDeclarationNode*> m_Vars;
				std::vector<UShaderSampler> m_Samplers;
#ifdef _DEBUG
				std::unordered_map<std::string, UShaderVariant> m_VarsDebug;
#endif
			};
		}
	}
}


inline bool iberbar::RHI::D3D9::UShaderSampler::operator==( const UShaderSampler& other ) const
{
	return (pTexture == other.pTexture && State == other.State );
}


inline bool iberbar::RHI::D3D9::UShaderSampler::operator!=( const UShaderSampler& other ) const
{
	return (pTexture != other.pTexture || State != other.State);
}

