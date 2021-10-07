#pragma once

#include <iberbar/RHI/Resource.h>
#include <iberbar/Utility/Result.h>

namespace iberbar
{
	namespace RHI
	{
		class IDevice;
		class IShaderReflection;

		class __iberbarRHIApi__ IShader
			: public IResource
		{
		public:
			IShader( EShaderType eShaderType )
				: IResource( UResourceType::Shader )
				, m_eShaderType( eShaderType )
			{
			}

		public:
			virtual CResult Load( const void* pCodes, uint32 nCodeLen ) = 0;
			virtual CResult LoadFromFile( const char* pstrFile ) = 0;
			virtual CResult LoadFromSource( const char* pstrSource ) = 0;
			virtual IShaderReflection* GetReflection() = 0;
			EShaderType GetShaderType() const { return m_eShaderType; }

		protected:
			EShaderType m_eShaderType;
		};


		//class __iberbarRHIApi__ IVertexShader
		//	: public IShader
		//{
		//public:
		//	IVertexShader() : IShader( EShaderType::VertexShader ) {}
		//};


		//class __iberbarRHIApi__ IPixelShader
		//	: public IShader
		//{
		//public:
		//	IPixelShader() : IShader( EShaderType::PixelShader ) {}
		//};


		//class __iberbarRHIApi__ IHullShader
		//	: public IShader
		//{
		//public:
		//	IHullShader() : IShader( EShaderType::HullShader ) {}
		//};


		//class __iberbarRHIApi__ IGeometryShader
		//	: public IShader
		//{
		//public:
		//	IGeometryShader() : IShader( EShaderType::GeometryShader ) {}
		//};


		//class __iberbarRHIApi__ IDomainShader
		//	: public IShader
		//{
		//public:
		//	IDomainShader() : IShader( EShaderType::DomainShader ) {}
		//};


		//class __iberbarRHIApi__ IComputeShader
		//	: public IShader
		//{
		//public:
		//	IComputeShader() : IShader( EShaderType::ComputeShader ) {}
		//};
	}
}
