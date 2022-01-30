#pragma once

#include <iberbar/Renderer/Resource.h>



namespace iberbar
{
	namespace RHI
	{
		class IShader;
	}

	namespace Renderer
	{
		class __iberbarRendererApi__ CShaderProgram
			: public CRenderResource
		{
		public:
			CShaderProgram(
				RHI::IShader* pVS,
				RHI::IShader* pPS,
				RHI::IShader* pHS,
				RHI::IShader* pGS,
				RHI::IShader* pDS );
			virtual ~CShaderProgram();

			inline RHI::IShader* GetShader( RHI::EShaderType nShaderType ) { return m_pShaders[ (int)nShaderType ]; }

		protected:
			RHI::IShader* m_pShaders[ (int)RHI::EShaderType::__Count ];
		};
	}
}