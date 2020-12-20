#pragma once

#include <iberbar/RHI/D3D9/Headers.h>
#include <iberbar/RHI/Shader.h>


namespace iberbar
{
	namespace RHI
	{
		namespace D3D9
		{
			class CDevice;

			class __iberbarD3DApi__ CShader
				: public IShader
			{
			public:
				CShader( CDevice* pDevice );
				virtual ~CShader();

			public:
				virtual CResult LoadFromSource( const char* strVertexSource, const char* strPixelSource ) override;
				virtual CResult LoadFromFile( const char* strVertexFile, const char* strPixelFile ) override;

				inline IDirect3DVertexShader9* GetD3DVertexShader() { return m_pD3DVertexShader; }
				inline ID3DXConstantTable* GetD3DVertexShaderConstTable() { return m_pD3DVertexShaderConstTable; }
				inline IDirect3DPixelShader9* GetD3DPixelShader() { return m_pD3DPixelShader; }
				inline ID3DXConstantTable* GetD3DPixelShaderConstTable() { return m_pD3DPixelShaderConstTable; }

			private:
				CResult LoadD3DVS( const char* strVertexFile );
				CResult LoadD3DPS( const char* strPixelFile );

			protected:
				CDevice* m_pDevice;

				IDirect3DVertexShader9* m_pD3DVertexShader;
				ID3DXConstantTable* m_pD3DVertexShaderConstTable;

				IDirect3DPixelShader9* m_pD3DPixelShader;
				ID3DXConstantTable* m_pD3DPixelShaderConstTable;
			};
		}
	}
}

