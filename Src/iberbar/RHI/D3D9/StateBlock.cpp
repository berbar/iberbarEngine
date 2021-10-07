
#include <iberbar/RHI/D3D9/StateBlock.h>
#include <iberbar/RHI/D3D9/Device.h>


iberbar::RHI::D3D9::CStateBlock::CStateBlock( CDevice* pDevice, UStateBlockType nBlockState )
	: m_pDevice( pDevice )
	, m_nBlockState( nBlockState )
	, m_pD3DStateBlock( nullptr )
{
}


iberbar::RHI::D3D9::CStateBlock::~CStateBlock()
{
	D3D_SAFE_RELEASE( m_pD3DStateBlock );
}


iberbar::CResult iberbar::RHI::D3D9::CStateBlock::Initial()
{
	auto pD3DDevice = m_pDevice->GetD3DDevice();
	D3DSTATEBLOCKTYPE nD3DStateBlockType = D3DSBT_ALL;
	switch ( m_nBlockState )
	{
		case UStateBlockType::PixelState:
			nD3DStateBlockType = D3DSBT_PIXELSTATE;
			break;
		case UStateBlockType::VertexState:
			nD3DStateBlockType = D3DSBT_VERTEXSTATE;
			break;
		default:break;
	}
	HRESULT hResult = pD3DDevice->CreateStateBlock( nD3DStateBlockType, &m_pD3DStateBlock );
	if ( FAILED( hResult ) )
	{
		return MakeResult( ResultCode::Bad, DXGetErrorDescriptionA( hResult ) );
	}
	return CResult();
}


iberbar::CResult iberbar::RHI::D3D9::CStateBlock::Capture()
{
	HRESULT hResult = m_pD3DStateBlock->Capture();
	if ( FAILED( hResult ) )
	{
		return MakeResult( ResultCode::Bad, DXGetErrorDescriptionA( hResult ) );
	}
	return CResult();
}


iberbar::CResult iberbar::RHI::D3D9::CStateBlock::Apply()
{
	HRESULT hResult = m_pD3DStateBlock->Apply();
	if ( FAILED( hResult ) )
	{
		return MakeResult( ResultCode::Bad, DXGetErrorDescriptionA( hResult ) );
	}
	return CResult();
}
