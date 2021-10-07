

#include <iberbar/RHI/D3D9/ShaderState.h>
#include <iberbar/RHI/D3D9/Device.h>
#include <iberbar/RHI/D3D9/Shader.h>
#include <iberbar/RHI/D3D9/VertexDeclaration.h>



//
//
//iberbar::RHI::D3D9::CShaderVariableDeclaration::CShaderVariableDeclaration()
//	: m_nTotalByteSize( 0 )
//	, m_nSamplerStageMax( 0 )
//	, m_Nodes()
//{
//}
//
//
//void iberbar::RHI::D3D9::CShaderVariableDeclaration::AddNode( UShaderType nShaderType, D3DXHANDLE Handle, D3DXCONSTANT_DESC Desc )
//{
//	UShaderVariableDeclarationNode node;
//	node.nShaderType = nShaderType;
//	node.D3DHandle = Handle;
//	node.D3DDesc = Desc;
//	node.nOffset = m_nTotalByteSize;
//	node.nElementCount = Desc.Elements;
//	switch ( Desc.Type )
//	{
//		case D3DXPT_BOOL:
//			node.nVarType = UShaderVariableType::VT_Boolean;
//			node.nElementSize = sizeof( uint32 );
//			break;
//
//		case D3DXPT_INT:
//			node.nVarType = UShaderVariableType::VT_Int;
//			node.nElementSize = sizeof( int32 );
//			break;
//
//		case D3DXPT_FLOAT:
//			if ( Desc.Class == D3DXPC_MATRIX_COLUMNS )
//			{
//				node.nVarType = UShaderVariableType::VT_Matrix44;
//				node.nElementSize = sizeof( DirectX::XMFLOAT4X4 );
//			}
//			else if ( Desc.Class == D3DXPC_VECTOR )
//			{
//				node.nVarType = UShaderVariableType::VT_Vector4;
//				node.nElementSize = sizeof( DirectX::XMFLOAT4 );
//			}
//			else
//			{
//				node.nVarType = UShaderVariableType::VT_Float;
//				node.nElementSize = sizeof( float );
//			}
//			break;
//
//		case D3DXPT_SAMPLER2D:
//			node.nVarType = UShaderVariableType::VT_Sampler2D;
//			node.nElementSize = 0;
//			m_nSamplerStageMax = tMax( m_nSamplerStageMax, (int)Desc.RegisterIndex );
//			break;
//
//		default:
//			break;
//	}
//	node.nTotalSize = node.nElementCount * node.nElementSize;
//	m_Nodes[ (int)nShaderType ].push_back( node );
//
//	m_nTotalByteSize += node.nTotalSize;
//}
//
//
//const iberbar::RHI::D3D9::UShaderVariableDeclarationNode* iberbar::RHI::D3D9::CShaderVariableDeclaration::GetNode( UShaderType nShaderType, const char* strName ) const
//{
//	auto iter = m_Nodes[ (int)nShaderType ].begin();
//	auto end = m_Nodes[ (int)nShaderType ].end();
//	for ( ; iter != end; iter ++ )
//	{
//		if ( strcmp( iter->D3DDesc.Name, strName ) == 0 )
//			return &(*iter);
//	}
//	return nullptr;
//}





iberbar::RHI::D3D9::CShaderState::CShaderState(
	CDevice* pDevice,
	CVertexDeclaration* pVertexDeclaration,
	CVertexShader* pVertexShader,
	CPixelShader* pPixelShader )
	: IShaderState()
	, m_pDevice( pDevice )
	, m_pVertexDeclaration( pVertexDeclaration )
	, m_pVertexShader( pVertexShader )
	, m_pPixelShader( pPixelShader )
{
	m_pDevice->AddRef();
	m_pVertexDeclaration->AddRef();
	m_pVertexShader->AddRef();
	m_pPixelShader->AddRef();
	memset( m_pShaders + 2, 0, sizeof( CShader* ) * ( (int)EShaderType::__Count - 2 ) );
}


iberbar::RHI::D3D9::CShaderState::~CShaderState()
{
	UNKNOWN_SAFE_RELEASE_NULL( m_pVertexShader );
	UNKNOWN_SAFE_RELEASE_NULL( m_pPixelShader );
	UNKNOWN_SAFE_RELEASE_NULL( m_pDevice );
}


iberbar::RHI::IShader* iberbar::RHI::D3D9::CShaderState::GetShader( EShaderType eShaderType )
{
	return m_pShaders[ (int)eShaderType ];
}

