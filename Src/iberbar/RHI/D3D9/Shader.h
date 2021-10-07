#pragma once

#include <iberbar/RHI/D3D9/Headers.h>
#include <iberbar/RHI/D3D9/ShaderReflection.h>
#include <iberbar/RHI/Shader.h>



namespace iberbar
{
	namespace RHI
	{
		namespace D3D9
		{
			class CDevice;

			class CShader abstract
				: public IShader
			{
			public:
				CShader( CDevice* pDevice, EShaderType eShaderType );
				virtual ~CShader();

			public:
				virtual CResult LoadFromFile( const char* pstrFile ) override;
				virtual CResult LoadFromSource( const char* pstrSource ) override;
				virtual IShaderReflection* GetReflection() override { return GetReflectionInner(); }

				FORCEINLINE ID3DXConstantTable* GetD3DConstTable() { return m_pD3DConstTable; }
				FORCEINLINE CShaderReflection* GetReflectionInner() { return &m_Reflection; }

			protected:
				CDevice* m_pDevice;
				ID3DXConstantTable* m_pD3DConstTable;
				CShaderReflection m_Reflection;
			};


			class CVertexShader
				: public CShader
			{
			public:
				CVertexShader( CDevice* pDevice );
				virtual CResult Load( const void* pCodes, uint32 nCodeLen ) override;
				FORCEINLINE IDirect3DVertexShader9* GetD3DShader() { return m_pD3DShader.Get(); }
			protected:
				ComPtr<IDirect3DVertexShader9> m_pD3DShader;
			};


			class CPixelShader
				: public CShader
			{
			public:
				CPixelShader( CDevice* pDevice );
				virtual CResult Load( const void* pCodes, uint32 nCodeLen ) override;
				FORCEINLINE IDirect3DPixelShader9* GetD3DShader() { return m_pD3DShader.Get(); }
			protected:
				ComPtr<IDirect3DPixelShader9> m_pD3DShader;
			};


		} // end namespace D3D9
	} // end namespace RHI
} // end namespace iberbar

