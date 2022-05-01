#pragma once


#include <iberbar/RHI/D3D11/Headers.h>
#include <iberbar/RHI/D3D11/ShaderReflection.h>
#include <iberbar/RHI/Shader.h>


namespace iberbar
{
	namespace RHI
	{
		namespace D3D11
		{
			class CDevice;
			//class CConstantBuffer;
			class CUniformBuffer;

			class CShader abstract
				: public IShader
			{
			public:
				CShader( CDevice* pDevice, EShaderType eShaderType );
				virtual ~CShader();

			public:
				virtual CResult LoadFromFile( const char* pstrFile ) override;
				virtual CResult LoadFromSource( const char* pstrSource ) override;
				virtual IShaderReflection* GetReflection() override;

				CResult CreateReflection( const void* pCodes, uint32 nCodeLen );
				FORCEINLINE CShaderReflection* GetReflectionInternal() { return m_pReflection; }
				//FORCEINLINE const std::vector<CConstantBuffer*>& GetConstantBuffers() { return m_ConstantBuffers; }
				//FORCEINLINE const uint8* GetConstantBuffersMemory() const
				//{
				//	if (m_ConstBuffersData.empty())
				//		return nullptr;
				//	return &(m_ConstBuffersData.front());
				//}

			protected:
				CDevice* m_pDevice;
				CShaderReflection* m_pReflection;
				//std::vector<CConstantBuffer*> m_ConstantBuffers;
				//std::vector<uint8> m_ConstBuffersData;
			};


			class CVertexShader
				: public CShader
			{
			public:
				CVertexShader( CDevice* pDevice );
				virtual CResult Load( const void* pCodes, uint32 nCodeLen ) override;
				FORCEINLINE ID3D11VertexShader* GetD3DShader() { return m_pD3DShader.Get(); }
				FORCEINLINE const uint8* GetCodePointer() { return &m_Code.front(); }
				FORCEINLINE uint32 GetCodeSize() { return m_Code.size(); }
			protected:
				ComPtr<ID3D11VertexShader> m_pD3DShader;
				std::vector<uint8> m_Code;
			};


			class CPixelShader
				: public CShader
			{
			public:
				CPixelShader( CDevice* pDevice );
				virtual CResult Load( const void* pCodes, uint32 nCodeLen ) override;
				FORCEINLINE ID3D11PixelShader* GetD3DShader() { return m_pD3DShader.Get(); }
			protected:
				ComPtr<ID3D11PixelShader> m_pD3DShader;
			};


			class CGeometryShader
				: public CShader
			{
			public:
				CGeometryShader( CDevice* pDevice );
				virtual CResult Load( const void* pCodes, uint32 nCodeLen ) override;
				FORCEINLINE ID3D11GeometryShader* GetD3DShader() { return m_pD3DShader.Get(); }
			protected:
				ComPtr<ID3D11GeometryShader> m_pD3DShader;
			};


			class CHullShader
				: public CShader
			{
			public:
				CHullShader( CDevice* pDevice );
				virtual CResult Load( const void* pCodes, uint32 nCodeLen ) override;
				FORCEINLINE ID3D11HullShader* GetD3DShader() { return m_pD3DShader.Get(); }
			protected:
				ComPtr<ID3D11HullShader> m_pD3DShader;
			};


			class CDomainShader
				: public CShader
			{
			public:
				CDomainShader( CDevice* pDevice );
				virtual CResult Load( const void* pCodes, uint32 nCodeLen ) override;
				FORCEINLINE ID3D11DomainShader* GetD3DShader() { return m_pD3DShader.Get(); }
			protected:
				ComPtr<ID3D11DomainShader> m_pD3DShader;
			};


			class CComputeShader
				: public CShader
			{
			public:
				CComputeShader( CDevice* pDevice );
				virtual CResult Load( const void* pCodes, uint32 nCodeLen ) override;
				FORCEINLINE ID3D11ComputeShader* GetD3DShader() { return m_pD3DShader.Get(); }
			protected:
				ComPtr<ID3D11ComputeShader> m_pD3DShader;
			};


			class CShaderProgram
				: public IShaderProgram
			{
			public:
				CShaderProgram(
					CDevice* pDevice,
					IShader* pVS,
					IShader* pPS,
					IShader* pHS,
					IShader* pGS,
					IShader* pDS );
				virtual ~CShaderProgram();
				inline CVertexShader* GetVertexShader() { return (CVertexShader*)m_pShaders[ (int)EShaderType::VertexShader ]; }
				inline CPixelShader* GetPixelShader() { return (CPixelShader*)m_pShaders[ (int)EShaderType::PixelShader ]; }
				inline CGeometryShader* GetGeometryShader() { return (CGeometryShader*)m_pShaders[ (int)EShaderType::GeometryShader ]; }
				inline CHullShader* GetHullShader() { return (CHullShader*)m_pShaders[ (int)EShaderType::HullShader ]; }
				inline CDomainShader* GetDomainShader() { return (CDomainShader*)m_pShaders[ (int)EShaderType::DomainShader ]; }

				void Initial();

			protected:
				CDevice* m_pDevice;
			};
		}
	}
}