#pragma once

#include <iberbar/RHI/Headers.h>
#include <iberbar/RHI/Types.h>
#include <DirectXMath.h>


namespace iberbar
{
	namespace RHI
	{
		class ITexture;
		class IShaderState;


		class __iberbarRHIApi__ IShaderVariableTable abstract
			: public CRef
		{
		public:
			virtual void SetShaderState( IShaderState* pShaderState ) = 0;
			virtual void SetBool( UShaderType nShaderType, const char* strName, bool value ) = 0;
			virtual void SetInt( UShaderType nShaderType, const char* strName, int value ) = 0;
			virtual void SetFloat( UShaderType nShaderType, const char* strName, float value ) = 0;
			virtual void SetVector( UShaderType nShaderType, const char* strName, const DirectX::XMFLOAT4& value ) = 0;
			virtual void SetMatrix( UShaderType nShaderType, const char* strName, const DirectX::XMFLOAT4X4& value ) = 0;
			virtual void SetSampler( const char* strName, RHI::ITexture* pTexture, UTextureSamplerState SamplerState ) = 0;

			virtual bool Compare( IShaderVariableTable* pVariableTable ) = 0;
		};
	}
}


