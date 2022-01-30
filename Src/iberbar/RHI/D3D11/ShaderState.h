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
			class CShaderProgram;
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
					CShaderProgram* pShaderProgram );
				virtual ~CShaderState();

			public:
				virtual IShaderProgram* GetShaderProgram() override;
				virtual IVertexDeclaration* GetVertexDeclaration() override { return m_pVertexDeclaration; }
				//virtual IUniformBuffer** GetUniformBuffers( EShaderType eShaderType ) override;

				CResult Initial();
				inline CDevice* GetDevice() { return m_pDevice; }
				inline CVertexDeclaration* GetVertexDeclarationInternal() { return m_pVertexDeclaration; }
				//inline CShader* GetShaderInternal( EShaderType eShaderType ) { return (CShader*)m_pShaderProgram->GetShader( eShaderType ); }

				inline ID3D11VertexShader* GetD3DVertexShader() { return m_pD3DVertexShader.Get(); }
				inline ID3D11PixelShader* GetD3DPixelShader() { return m_pD3DPixelShader.Get(); }
				inline ID3D11HullShader* GetD3DHullShader() { return m_pD3DHullShader.Get(); }
				inline ID3D11GeometryShader* GetD3DGeometryShader() { return m_pD3DGeometryShader.Get(); }
				inline ID3D11DomainShader* GetD3DDomainShader() { return m_pD3DDomainShader.Get(); }
				inline ID3D11ComputeShader* GetD3DComputeShader() { return m_pD3DComputeShader.Get(); }

				inline ID3D11InputLayout* GetD3DInputLayout() { return m_pD3DInputLayout.Get(); }

			protected:
				CDevice* m_pDevice;
				CVertexDeclaration* m_pVertexDeclaration;
				CShaderProgram* m_pShaderProgram;
				ComPtr<ID3D11InputLayout> m_pD3DInputLayout;
				
				ComPtr<ID3D11VertexShader> m_pD3DVertexShader;
				ComPtr<ID3D11PixelShader> m_pD3DPixelShader;
				ComPtr<ID3D11GeometryShader> m_pD3DGeometryShader;
				ComPtr<ID3D11DomainShader> m_pD3DDomainShader;
				ComPtr<ID3D11HullShader> m_pD3DHullShader;
				ComPtr<ID3D11ComputeShader> m_pD3DComputeShader;
			};



		}
	}
}


