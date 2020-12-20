
#include <iberbar/Renderer/OpenGL/Renderer.h>
#include <iberbar/Renderer/OpenGL/Device.h>
#include <iberbar/Renderer/RenderCommand.h>
#include <iberbar/Renderer/TrianglesCommand.h>


iberbar::Renderer::OpenGL::CRenderer::CRenderer()
	: CRendererBase()
	, m_pDevice( nullptr )
	, m_bIsRendering( false )
{
}

iberbar::Renderer::OpenGL::CRenderer::~CRenderer()
{
	UNKNOWN_SAFE_RELEASE_NULL( m_pDevice );
}

void iberbar::Renderer::OpenGL::CRenderer::DrawBatchTriangles()
{
}

void iberbar::Renderer::OpenGL::CRenderer::Init()
{
	CRendererBase::Init();
}

void iberbar::Renderer::OpenGL::CRenderer::SetDevice( CDevice* pDevice )
{
	assert( pDevice );
	assert( m_pDevice == nullptr );
	m_pDevice = pDevice;
	m_pDevice->AddRef();
}
