#pragma once


#include <iberbar/Paper2d/Headers.h>
#include <DirectXMath.h>


namespace iberbar
{
	namespace Paper2d
	{
		class CNodeTransform
		{
		public:
			CNodeTransform();

		public:
			void SetPosition( float x, float y );
			void SetPositionAbsolutely( float x, float y );
			void SetAnchor( float x, float y );
			void SetPivot( float x, float y );
			void SetContentSize( float w, float h );

			void UpdateTransform();
			void UpdateTransform( DirectX::CXMMATRIX mat );

			void ConvertFromAbsolutePoint( DirectX::XMFLOAT2& Point );
			void ConvertToAbsolutePoint( DirectX::XMFLOAT2& Point );

			const DirectX::XMFLOAT2& GetAnchor() const;
			const DirectX::XMFLOAT2& GetPivot() const;
			const DirectX::XMFLOAT2& GetPosition() const;
			const DirectX::XMFLOAT2& GetContentSize() const;
			const DirectX::XMMATRIX& GetMatrix() const;
			const DirectX::XMMATRIX& GetMatrixForChildren() const;

		private:
			DirectX::XMFLOAT2 m_floatx_Anchor; // 作用于子节点
			DirectX::XMFLOAT2 m_floatx_Pivot; // 作用于自身的偏移、缩放旋转
			DirectX::XMFLOAT2 m_floatx_Position;
			DirectX::XMFLOAT2 m_floatx_ContentSize;
			DirectX::XMFLOAT2 m_floatx_Scaling;

			DirectX::XMVECTOR m_vec_Anchor;
			DirectX::XMVECTOR m_vec_Pivot;
			DirectX::XMVECTOR m_vec_Translation;
			DirectX::XMVECTOR m_vec_Scaling;
			DirectX::XMMATRIX m_mat_Transform;
			DirectX::XMMATRIX m_mat_Transform_ForChildren;
		};

		static const size_t s = sizeof( CNodeTransform );
	}
}



FORCEINLINE void iberbar::Paper2d::CNodeTransform::SetPosition( float x, float y )
{
	m_floatx_Position = DirectX::XMFLOAT2( x, y );
	// 未加入Pivot
	DirectX::XMFLOAT2 PivotTemp( m_floatx_Pivot.x * m_floatx_ContentSize.x, m_floatx_Pivot.y * m_floatx_ContentSize.y );
	DirectX::XMFLOAT2 Offset( m_floatx_Position.x - PivotTemp.x, m_floatx_Position.y - PivotTemp.y );
	m_vec_Translation = DirectX::XMLoadFloat2( &Offset );
}


FORCEINLINE void iberbar::Paper2d::CNodeTransform::SetPositionAbsolutely( float x, float y )
{
	DirectX::XMFLOAT3 f3_PositionAbs( x, y, 0.0f );
	
	DirectX::XMVECTOR vec_PositionAbs = DirectX::XMLoadFloat3( &f3_PositionAbs );
	DirectX::XMVECTOR vec_Position = DirectX::XMVector3TransformCoord( vec_PositionAbs, DirectX::XMMatrixInverse( nullptr, m_mat_Transform_ForChildren ) );
	DirectX::XMFLOAT2 f2_Position;
	DirectX::XMStoreFloat2( &f2_Position, vec_Position );
	SetPosition( f2_Position.x, f2_Position.y );
}


FORCEINLINE void iberbar::Paper2d::CNodeTransform::SetAnchor( float x, float y )
{
	m_floatx_Anchor = DirectX::XMFLOAT2( x, y );

	m_vec_Anchor = DirectX::XMLoadFloat2( &m_floatx_Anchor );
}


FORCEINLINE void iberbar::Paper2d::CNodeTransform::SetPivot( float x, float y )
{
	m_floatx_Pivot = DirectX::XMFLOAT2( x, y );

	DirectX::XMFLOAT2 PivotTemp( m_floatx_Pivot.x * m_floatx_ContentSize.x, m_floatx_Pivot.y * m_floatx_ContentSize.y );
	m_vec_Pivot = DirectX::XMLoadFloat2( &PivotTemp );
	DirectX::XMFLOAT2 Offset( m_floatx_Position.x - PivotTemp.x, m_floatx_Position.y - PivotTemp.y );
	m_vec_Translation = DirectX::XMLoadFloat2( &Offset );
}


FORCEINLINE void iberbar::Paper2d::CNodeTransform::SetContentSize( float w, float h )
{
	m_floatx_ContentSize = DirectX::XMFLOAT2( w, h );

	DirectX::XMFLOAT2 PivotTemp = DirectX::XMFLOAT2(
		m_floatx_Pivot.x * m_floatx_ContentSize.x,
		m_floatx_Pivot.y * m_floatx_ContentSize.y );
	m_vec_Pivot = DirectX::XMLoadFloat2( &PivotTemp );
	DirectX::XMFLOAT2 Offset( m_floatx_Position.x - PivotTemp.x, m_floatx_Position.y - PivotTemp.y );
	m_vec_Translation = DirectX::XMLoadFloat2( &Offset );
}


FORCEINLINE void iberbar::Paper2d::CNodeTransform::UpdateTransform()
{
	m_mat_Transform = DirectX::XMMatrixTransformation2D( m_vec_Pivot, 0, m_vec_Scaling, m_vec_Pivot, 0, m_vec_Translation );
	m_mat_Transform_ForChildren = DirectX::XMMatrixTranslation(
		m_floatx_ContentSize.x * m_floatx_Anchor.x,
		m_floatx_ContentSize.y * m_floatx_Anchor.y,
		0.0f );
	m_mat_Transform_ForChildren = DirectX::XMMatrixMultiply( m_mat_Transform, m_mat_Transform_ForChildren );
}


FORCEINLINE void iberbar::Paper2d::CNodeTransform::UpdateTransform( DirectX::CXMMATRIX mat )
{
	UpdateTransform();

	m_mat_Transform = DirectX::XMMatrixMultiply( mat, m_mat_Transform );
	m_mat_Transform_ForChildren = DirectX::XMMatrixMultiply( mat, m_mat_Transform_ForChildren );
}


FORCEINLINE void iberbar::Paper2d::CNodeTransform::ConvertFromAbsolutePoint( DirectX::XMFLOAT2& Point )
{
	DirectX::XMVECTOR vec = DirectX::XMLoadFloat2( &Point );
	DirectX::XMMATRIX matInv = DirectX::XMMatrixInverse( nullptr, m_mat_Transform_ForChildren );
	DirectX::XMVector2TransformCoord( vec, matInv );
	DirectX::XMStoreFloat2( &Point, vec );
}


FORCEINLINE void iberbar::Paper2d::CNodeTransform::ConvertToAbsolutePoint( DirectX::XMFLOAT2& Point )
{
	DirectX::XMVECTOR vec = DirectX::XMLoadFloat2( &Point );
	DirectX::XMVector2TransformCoord( vec, m_mat_Transform_ForChildren );
	DirectX::XMStoreFloat2( &Point, vec );
}


FORCEINLINE const DirectX::XMFLOAT2& iberbar::Paper2d::CNodeTransform::GetAnchor() const
{
	return m_floatx_Anchor;
}


FORCEINLINE const DirectX::XMFLOAT2& iberbar::Paper2d::CNodeTransform::GetPivot() const
{
	return m_floatx_Pivot;
}


FORCEINLINE const DirectX::XMFLOAT2& iberbar::Paper2d::CNodeTransform::GetPosition() const
{
	return m_floatx_Position;
}


FORCEINLINE const DirectX::XMFLOAT2& iberbar::Paper2d::CNodeTransform::GetContentSize() const
{
	return m_floatx_ContentSize;
}


FORCEINLINE const DirectX::XMMATRIX& iberbar::Paper2d::CNodeTransform::GetMatrix() const
{
	return m_mat_Transform;
}


FORCEINLINE const DirectX::XMMATRIX& iberbar::Paper2d::CNodeTransform::GetMatrixForChildren() const
{
	return m_mat_Transform_ForChildren;
}

