#pragma once


#include <iberbar/Paper2d/Headers.h>
#include <DirectXMath.h>


namespace iberbar
{
	namespace Paper2d
	{
		class __iberbarPaper2dApis__ CCamera
			: public CRef
		{
		public:
			CCamera();

		public:
            const DirectX::XMMATRIX& GetMatrix_Projection() const;
            const DirectX::XMMATRIX& GetMatrix_View() const;
            const DirectX::XMMATRIX& GetMatrix_ViewProjection() const;
			const DirectX::XMMATRIX& GetMatrix_ViewProjectionTranspose() const;
			const DirectX::XMFLOAT4X4& GetFloat4x4_ViewProjection() const;

		public:
			void CreateMatrixView( float nX, float nY );
			void CreateMatrixProjection( int nWidth, int nHeight );
			void BuildMatrixViewProjection();

		private:
			DirectX::XMMATRIX m_matProjection;
			DirectX::XMMATRIX m_matView;
			DirectX::XMMATRIX m_matViewProjection;
			DirectX::XMMATRIX m_matViewProjectionTranspose;
			DirectX::XMFLOAT4X4 m_float4x4_ViewProjection;
		};
	}
}


FORCEINLINE const DirectX::XMMATRIX& iberbar::Paper2d::CCamera::GetMatrix_Projection() const
{
	return m_matProjection;
}


FORCEINLINE const DirectX::XMMATRIX& iberbar::Paper2d::CCamera::GetMatrix_View() const
{
	return m_matView;
}


FORCEINLINE const DirectX::XMMATRIX& iberbar::Paper2d::CCamera::GetMatrix_ViewProjection() const
{
	return m_matViewProjection;
}


FORCEINLINE const DirectX::XMMATRIX& iberbar::Paper2d::CCamera::GetMatrix_ViewProjectionTranspose() const
{
	return m_matViewProjectionTranspose;
}


FORCEINLINE const DirectX::XMFLOAT4X4& iberbar::Paper2d::CCamera::GetFloat4x4_ViewProjection() const
{
	return m_float4x4_ViewProjection;
}


FORCEINLINE void iberbar::Paper2d::CCamera::CreateMatrixView( float nX, float nY )
{
	DirectX::XMFLOAT4 vecEye( nX, nY, 0.0f, 1.0 );
	DirectX::XMFLOAT4 vecEyeDirection( 0.0f, 0.0f, 1.0f, 0.0f );
	DirectX::XMFLOAT4 vecUpDirection( 0.0f, 1.0f, 0.0f, 0.0f );
	m_matView = DirectX::XMMatrixLookToLH(
		DirectX::XMLoadFloat4( &vecEye ),
		DirectX::XMLoadFloat4( &vecEyeDirection ),
		DirectX::XMLoadFloat4( &vecUpDirection ) );
}


FORCEINLINE void iberbar::Paper2d::CCamera::CreateMatrixProjection( int nWidth, int nHeight )
{
	m_matProjection = DirectX::XMMatrixOrthographicLH(
		(float)nWidth,
		(float)nHeight,
		-100.0f,
		100.0f );
}


FORCEINLINE void iberbar::Paper2d::CCamera::BuildMatrixViewProjection()
{
	m_matViewProjection = DirectX::XMMatrixMultiply( m_matView, m_matProjection );
	m_matViewProjectionTranspose = DirectX::XMMatrixTranspose( m_matViewProjection );
	DirectX::XMStoreFloat4x4( &m_float4x4_ViewProjection, m_matViewProjection );
}
