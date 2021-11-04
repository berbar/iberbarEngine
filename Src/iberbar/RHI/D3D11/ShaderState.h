#pragma once

#include <iberbar/RHI/D3D11/Headers.h>
#include <iberbar/RHI/ShaderState.h>



namespace iberbar
{
	namespace RHI
	{
		namespace D3D11
		{
			class CDevice;
			class CShader;
			class CVertexDeclaration;
			class CVertexShader;
			class CPixelShader;
			class CGeometryShader;
			class CHullShader;
			class CDomainShader;
			class CComputeShader;
			class CUniformBuffer;

			class CShaderState
				: public IShaderState
			{
			public:
				CShaderState(
					CDevice* pDevice,
					CVertexDeclaration* pVertexDeclaration,
					CVertexShader* pVertexShader,
					CPixelShader* pPixelShader,
					CHullShader* pHullShader,
					CGeometryShader* pGeometryShader,
					CDomainShader* pDomainShader );
				virtual ~CShaderState();

			public:
				virtual IShader* GetShader( EShaderType eShaderType ) override;
				virtual IUniformBuffer** GetUniformBuffers( EShaderType eShaderType ) override;

				CResult Initial();
				CVertexDeclaration* GetVertexDeclaration() { return m_pVertexDeclaration; }

				CDevice* GetDevice() { return m_pDevice; }
				CShader* GetShaderInternal( EShaderType eShaderType ) { return m_pShaders[ (int)eShaderType ]; }
				CVertexShader* GetVertexShader() { return m_pVertexShader; }
				CPixelShader* GetPixelShader() { return m_pPixelShader; }
				CHullShader* GetHullShader() { return m_pHullShader; }
				CGeometryShader* GetGeometryShader() { return m_pGeometryShader; }
				CDomainShader* GetDomainShader() { return m_pDomainShader; }

				ID3D11VertexShader* GetD3DVertexShader() { return m_pD3DVertexShader.Get(); }
				ID3D11PixelShader* GetD3DPixelShader() { return m_pD3DPixelShader.Get(); }
				ID3D11HullShader* GetD3DHullShader() { return m_pD3DHullShader.Get(); }
				ID3D11GeometryShader* GetD3DGeometryShader() { return m_pD3DGeometryShader.Get(); }
				ID3D11DomainShader* GetD3DDomainShader() { return m_pD3DDomainShader.Get(); }
				ID3D11ComputeShader* GetD3DComputeShader() { return m_pD3DComputeShader.Get(); }

				ID3D11InputLayout* GetD3DInputLayout() { return m_pD3DInputLayout.Get(); }

			protected:
				CDevice* m_pDevice;
				CVertexDeclaration* m_pVertexDeclaration;
				ComPtr<ID3D11InputLayout> m_pD3DInputLayout;
				union
				{
					struct
					{
						CVertexShader* m_pVertexShader;
						CPixelShader* m_pPixelShader;
						CHullShader* m_pHullShader;
						CGeometryShader* m_pGeometryShader;
						CDomainShader* m_pDomainShader;
					};
					CShader* m_pShaders[ (int)EShaderType::__Count ];
				};
				ComPtr<ID3D11VertexShader> m_pD3DVertexShader;
				ComPtr<ID3D11PixelShader> m_pD3DPixelShader;
				ComPtr<ID3D11GeometryShader> m_pD3DGeometryShader;
				ComPtr<ID3D11DomainShader> m_pD3DDomainShader;
				ComPtr<ID3D11HullShader> m_pD3DHullShader;
				ComPtr<ID3D11ComputeShader> m_pD3DComputeShader;

				CUniformBuffer* m_UniformBuffers[ (int)EShaderType::__Count ][ D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT ];
			};



		}
	}
}


