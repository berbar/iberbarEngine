#pragma once


#include <iberbar/RHI/Effect.h>
#include <iberbar/RHI/D3D11/Headers.h>


namespace iberbar
{
	namespace RHI
	{
		namespace D3D11
		{
			class CShader;
			class CDevice;


			class CEffect
				: public IEffect
			{
			public:
				CEffect();
				virtual ~CEffect();

				virtual void SetShader( EShaderType eShaderType, IShader* pShader ) override;
				virtual CResult Generate() override;

			private:
				CResult GenerateShaderDesc( EShaderType eShaderType );

			protected:
				CDevice* m_pDevice;
				union {
					CShader* m_pShaderSlots[ (int)EShaderType::__Count ];
					struct
					{
						CShader* m_pVertexShader;
						CShader* m_pPixelShader;
						CShader* m_pHullShader;
						CShader* m_pDomainShader;
						CShader* m_pGeometryShader;
						CShader* m_pComputeShader;
					};
				};
				

			};
		}
	}
}

