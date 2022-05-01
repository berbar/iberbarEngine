#pragma once

#include <iberbar/Paper2d/Headers.h>
#include <iberbar/Renderer/Renderer.h>
#include <iberbar/Renderer/RendererSprite.h>
#include <DirectXMath.h>

namespace iberbar
{
	namespace Paper2d
	{
		class CDrawContext
		{
		public:
			CDrawContext( Renderer::CRendererSprite* pRendererSprite, const DirectX::XMFLOAT4X4& matViewProjection );

		public:
			Renderer::CRenderer* GetRenderer();
			Renderer::CRendererSprite* GetRendererSprite();
			const DirectX::XMFLOAT4X4& GetMatViewProjection() const;

		private:
			Renderer::CRenderer* m_pRenderer;
			Renderer::CRendererSprite* m_pRendererSprite;
			DirectX::XMFLOAT4X4 m_matViewProjection;
		};
	}
}


FORCEINLINE iberbar::Renderer::CRenderer* iberbar::Paper2d::CDrawContext::GetRenderer()
{
	return m_pRenderer;
}


FORCEINLINE iberbar::Renderer::CRendererSprite* iberbar::Paper2d::CDrawContext::GetRendererSprite()
{
	return m_pRendererSprite;
}


FORCEINLINE const DirectX::XMFLOAT4X4& iberbar::Paper2d::CDrawContext::GetMatViewProjection() const
{
	return m_matViewProjection;
}
