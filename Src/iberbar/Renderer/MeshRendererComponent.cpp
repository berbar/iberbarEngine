
#include <iberbar/Renderer/MeshRendererComponent.h>
#include <iberbar/Renderer/Material.h>
#include <iberbar/Renderer/Mesh.h>
#include <iberbar/Renderer/Renderer.h>
#include <iberbar/RHI/ShaderState.h>
#include <iberbar/RHI/Device.h>


iberbar::Renderer::CMeshRendererComponent::CMeshRendererComponent()
	: m_pMesh( nullptr )
	, m_Materials(1)
	, m_RenderCommand()
{
}


iberbar::Renderer::CMeshRendererComponent::~CMeshRendererComponent()
{
	UNKNOWN_SAFE_RELEASE_NULL( m_pMesh );
	for ( auto iter = m_Materials.begin(), end = m_Materials.end(); iter != end; iter ++ )
	{
		UNKNOWN_SAFE_RELEASE_NULL( *iter );
	}
}


void iberbar::Renderer::CMeshRendererComponent::SetMesh( CMesh* pMesh )
{
	if ( m_pMesh == pMesh )
		return;

	if ( m_pMesh != nullptr )
		m_pMesh->Release();
	m_pMesh = pMesh;
	if ( m_pMesh != nullptr )
		m_pMesh->AddRef();

	m_RenderCommand.SetMesh( m_pMesh );
}


void iberbar::Renderer::CMeshRendererComponent::SetMaterial( CMaterial* pMaterial )
{
	//if ( m_Materials.empty() )
	//{
	//	m_Materials.push_back( pMaterial );
	//}
	//else
	//{
	//	if ( m_Materials[ 0 ] != nullptr )
	//		m_Materials[ 0 ]->Release();
	//	m_Materials[ 0 ] = pMaterial;
	//}
	//if ( pMaterial != nullptr )
	//	pMaterial->AddRef();
	SetMaterial( 0, pMaterial );
}


void iberbar::Renderer::CMeshRendererComponent::SetMaterialCount( int nCount )
{
	//if ( nCount <= 0 )
	//	return;

	//int nCountOld = (int)m_Materials.size();
	//if ( nCountOld == nCount )
	//	return;

	//if ( nCount < nCountOld )
	//{
	//	for ( int i = nCount; i < nCountOld; i++ )
	//	{
	//		UNKNOWN_SAFE_RELEASE_NULL( m_Materials[ i ] );
	//		UNKNOWN_SAFE_RELEASE_NULL( m_BoundShaderStates[ i ] );
	//	}
	//}
	//else
	//{
	//	m_Materials.resize( nCount );
	//	m_BoundShaderStates.resize( nCount );
	//	for ( int i = nCountOld; i < nCount; i++ )
	//	{
	//		m_Materials[ i ] = nullptr;
	//		m_BoundShaderStates[ i ] = nullptr;
	//	}
	//}
}


void iberbar::Renderer::CMeshRendererComponent::SetMaterial( int nIndex, CMaterial* pMaterial )
{
	if ( m_Materials.size() <= nIndex )
	{
		m_Materials.resize( (size_t)nIndex + 1 );
		m_Materials[ nIndex ] = pMaterial;
	}
	else
	{
		if ( m_Materials[ nIndex ] == pMaterial )
			return;

		if ( m_Materials[ nIndex ] != nullptr )
			m_Materials[ nIndex ]->Release();
		m_Materials[ nIndex ] = pMaterial;
	}

	if ( pMaterial != nullptr )
		pMaterial->AddRef();


	if ( nIndex == 0 )
	{
		if ( m_Materials[ 0 ] != nullptr )
		{
			m_RenderCommand.SetMaterial( m_Materials[ 0 ] );
		}
		else
		{
			m_RenderCommand.SetMaterial( nullptr );
		}
	}
}


void iberbar::Renderer::CMeshRendererComponent::Render()
{
	if ( m_pMesh == nullptr )
		return;

	CRenderer::sGetInstance()->AddCommand( &m_RenderCommand );
}

