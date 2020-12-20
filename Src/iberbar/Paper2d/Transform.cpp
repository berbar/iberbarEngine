
#include <iberbar/Paper2d/Transform.h>


iberbar::Paper2d::CNodeTransform::CNodeTransform()
	: m_floatx_Anchor( 0.0f, 0.0f )
	, m_floatx_Pivot( 0.0f, 0.0f )
	, m_floatx_Position( 0.0f, 0.0f )
	, m_floatx_ContentSize( 0.0f, 0.0f )
	, m_floatx_Scaling( 1.0f, 1.0f )
	, m_vec_Anchor()
	, m_vec_Pivot()
	, m_vec_Scaling()
	, m_vec_Translation()
	, m_mat_Transform( DirectX::XMMatrixIdentity() )
	, m_mat_Transform_ForChildren( DirectX::XMMatrixIdentity() )
{
	m_vec_Anchor = DirectX::XMLoadFloat2( &m_floatx_Anchor );
	m_vec_Pivot = DirectX::XMLoadFloat2( &m_floatx_Pivot );
	m_vec_Translation = DirectX::XMLoadFloat2( &m_floatx_Position );
	m_vec_Scaling = DirectX::XMLoadFloat2( &m_floatx_Scaling );
}