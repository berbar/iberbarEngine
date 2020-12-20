#pragma once

#include <iberbar/RHI/OpenGL/Headers.h>
#include <iberbar/RHI/ShaderVariables.h>
#include <iberbar/RHI/Texture.h>
#include <iberbar/Utility/Buffer.h>



namespace iberbar
{
	namespace RHI
	{
		namespace OpenGL
		{
			class CShader;
			class CShaderState;
			struct UShaderVariableDeclarationNode;


			struct UShaderSampler
			{
				UShaderSampler()
					: nLocation( -1 )
					, pTexture( nullptr )
					, State()
				{
				}
				UShaderSampler( const UShaderSampler& sampler )
					: nLocation( sampler.nLocation )
					, pTexture( sampler.pTexture )
					, State( sampler.State )
				{
				}
				bool operator == ( const UShaderSampler& other ) const;
				bool operator != ( const UShaderSampler& other ) const;
				GLuint nLocation;
				TSmartRefPtr<RHI::ITexture> pTexture;
				UTextureSamplerState State;
			};


			class __iberbarRHIOpenGLApi__ CShaderVariableTable
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
				const std::vector<UShaderSampler>& GetSamplers() { return m_Samplers; }

			protected:
				TBuffer<uint8> m_Buffer;
				CShaderState* m_pShaderState;
				std::vector<const UShaderVariableDeclarationNode*> m_Vars;
				std::vector<UShaderSampler> m_Samplers;
			};
		}
	}
}


inline bool iberbar::RHI::OpenGL::UShaderSampler::operator==( const UShaderSampler& other ) const
{
	return (pTexture == other.pTexture && State == other.State);
}


inline bool iberbar::RHI::OpenGL::UShaderSampler::operator!=( const UShaderSampler& other ) const
{
	return (pTexture != other.pTexture || State != other.State);
}
