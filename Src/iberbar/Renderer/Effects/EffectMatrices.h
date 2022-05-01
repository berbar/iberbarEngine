#pragma once

#include <iberbar/Renderer/Effect.h>
#include <DirectXMath.h>

namespace iberbar
{
	namespace Renderer
	{
		class __iberbarRendererApi__ CEffectMatrices
			: public CEffectBase
		{
		protected:
			struct _Data
			{
				DirectX::XMFLOAT4X4 ViewMatrix;
				DirectX::XMFLOAT4X4 ProjectionMatrix;
				DirectX::XMFLOAT4X4 ViewProjectionMatrix;
			};

		public:
			CResult Initial();
			void SetViewMatrix( const DirectX::XMFLOAT4X4& Matrix );
			void SetProjectionMatrix( const DirectX::XMFLOAT4X4& Matrix );
			void Apply();

		protected:
			uint32 m_DirtyData;
			_Data m_Data;
		};
	}
}


inline void iberbar::Renderer::CEffectMatrices::SetViewMatrix( const DirectX::XMFLOAT4X4& Matrix )
{
	m_Data.ViewMatrix = Matrix;
	m_DirtyData = 1;
}


inline void iberbar::Renderer::CEffectMatrices::SetProjectionMatrix( const DirectX::XMFLOAT4X4& Matrix )
{
	m_Data.ProjectionMatrix = Matrix;
	m_DirtyData = 1;
}
