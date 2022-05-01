#pragma once

#include <iberbar/Renderer/Headers.h>

namespace iberbar
{
	namespace RHI
	{
		class ITexture;
	}

	namespace Renderer
	{
		class __iberbarRendererApi__ CSprite
			: public CRef
		{
		protected:
			RHI::ITexture* m_pTexture;
			CRect2f m_UV;
		};
	}
}