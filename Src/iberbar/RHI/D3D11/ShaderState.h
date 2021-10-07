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

				CResult Initial();
				CVertexDeclaration* GetVertexDeclaration() { return m_pVertexDeclaration; }
				CShader* GetShaderInternal( EShaderType eShaderType ) { return m_pShaders[ (int)eShaderType ]; }
				CVertexShader* GetVertexShader() { return m_pVertexShader; }
				CPixelShader* GetPixelShader() { return m_pPixelShader; }
				CHullShader* GetHullShader() { return m_pHullShader; }
				CGeometryShader* GetGeometryShader() { return m_pGeometryShader; }
				CDomainShader* GetDomainShader() { return m_pDomainShader; }
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
			};
		}
	}
}


