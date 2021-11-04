#pragma once


namespace iberbar
{
	namespace RHI
	{
		class IShaderState;
	}

	namespace Renderer
	{

		class CEffectBase
		{
		public:
			void SetVariables();
			virtual void Apply();

		protected:
			RHI::IShaderState* m_pShaderState;
		};


		class CEffectParameters
		{

		};
	}
}
