#pragma once


#ifdef _DEBUG

#include <iberbar/RHI/D3D11/Headers.h>
#include <iberbar/RHI/TestDraw.h>


namespace iberbar
{
	namespace RHI
	{
		class IShader;

		namespace D3D11
		{
			class CDevice;
			class CVertexShader;
			class CPixelShader;
			class CShaderProgram;
			class CShaderState;
			class CVertexBuffer;
			class CIndexBuffer;
			class CVertexDeclaration;

			class CTestDraw final
				: public ITestDraw
			{
			public:
				CTestDraw();
				virtual ~CTestDraw();

				void Initial( CDevice* pDevice );

				virtual void Draw() override;

			private:
				CDevice* m_pDevice;
				ComPtr<ID3D11DeviceContext> m_pD3DDeviceContext;
				CVertexBuffer* m_pVertexBuffer;
				CIndexBuffer* m_pIndexBuffer;
				//ComPtr<ID3D11InputLayout> m_pVertexLayout;
				//ComPtr<ID3D11VertexShader> m_pVertexShader;
				//ComPtr<ID3D11PixelShader> m_pPixelShader;
				CVertexShader* m_pVertexShader;
				CPixelShader* m_pPixelShader;
				CShaderProgram* m_pShaderProgram;
				CVertexDeclaration* m_pVertexDecl;
				CShaderState* m_pShaderState;
			};
		}
	}
}

#endif
