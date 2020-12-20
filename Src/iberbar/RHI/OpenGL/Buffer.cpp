
#include <iberbar/RHI/OpenGL/Buffer.h>
#include <iberbar/RHI/OpenGL/Device.h>



iberbar::RHI::OpenGL::CVertexBuffer::CVertexBuffer( CDevice* pDevice, uint32 nSize, uint32 nUsage )
	: IVertexBuffer( nSize, nUsage )
	, m_pDevice( pDevice )
	, m_nBuffer( 0 )
	, m_bStreamDraw( false )
	, m_bLocked( false )
	, m_nLockSize( 0 )
	, m_LockBuffer()
{
	m_pDevice->AddRef();
}

iberbar::RHI::OpenGL::CVertexBuffer::~CVertexBuffer()
{
	Destory();
	UNKNOWN_SAFE_RELEASE_NULL( m_pDevice );
}

void iberbar::RHI::OpenGL::CVertexBuffer::Initial()
{
	glGenBuffers( 1, &m_nBuffer );
	//glBufferData( GL_ARRAY_BUFFER, GetInSize(), nullptr, GL_DYNAMIC_DRAW );
}

void iberbar::RHI::OpenGL::CVertexBuffer::Destory()
{
	if ( m_nBuffer > 0 )
	{
		glDeleteBuffers( 1, &m_nBuffer );
		m_nBuffer = 0;
	}
}

//void iberbar::RHI::OpenGL::CVertexBuffer::Write( void* pData, uint32 nOffset, uint32 nSize )
//{
//	assert( nOffset + nSize <= GetInSize() );
//	Bind();
//	//glBufferSubData( GL_ARRAY_BUFFER, nOffset, nSize, pData );
//	glBufferData( GL_ARRAY_BUFFER, nSize, pData, GetAccess() );
//}


iberbar::CResult iberbar::RHI::OpenGL::CVertexBuffer::Lock( uint32 nOffset, uint32 nSize, void** ppOut, uint32 nFlags )
{
	assert( m_bLocked == false );
	assert( nOffset + nSize <= GetInSize() );
	assert( nSize > 0 );

	if ( m_LockBuffer.GetDataSize() < nSize )
	{
		m_LockBuffer.Resize( nSize, false );
	}
	*ppOut = m_LockBuffer.GetPointer();

	m_bLocked = true;
	m_nLockSize = nSize;

	return CResult();
}


iberbar::CResult iberbar::RHI::OpenGL::CVertexBuffer::Unlock()
{
	assert( m_bLocked == true );

	m_pDevice->BindArrayBuffer( m_nBuffer );
	glBufferData( GL_ARRAY_BUFFER, m_nLockSize, m_LockBuffer.GetPointer(), GetAccess() );
	m_bLocked = false;

	return CResult();
}






iberbar::RHI::OpenGL::CIndexBuffer::CIndexBuffer( CDevice* pDevice, uint32 nStride, uint32 nInSize, uint32 nUsage )
	: IIndexBuffer( nStride, nInSize, nUsage )
	, m_pDevice( pDevice )
	, m_nBuffer( 0 )
	, m_bStreamDraw( false )
	, m_bLocked( false )
	, m_nLockSize( 0 )
	, m_LockBuffer()
{
	m_pDevice->AddRef();
}

iberbar::RHI::OpenGL::CIndexBuffer::~CIndexBuffer()
{
	Destory();
	UNKNOWN_SAFE_RELEASE_NULL( m_pDevice );
}

void iberbar::RHI::OpenGL::CIndexBuffer::Initial()
{
	glGenBuffers( 1, &m_nBuffer );
	//glBufferData( GL_ELEMENT_ARRAY_BUFFER, GetInSize(), nullptr, GL_DYNAMIC_DRAW );
}

void iberbar::RHI::OpenGL::CIndexBuffer::Destory()
{
	if ( m_nBuffer > 0 )
	{
		glDeleteBuffers( 1, &m_nBuffer );
		m_nBuffer = 0;
	}
}

//void iberbar::RHI::OpenGL::CIndexBuffer::Write( void* pData, uint32 nOffset, uint32 nSize )
//{
//	assert( nOffset + nSize <= GetInSize() );
//	Bind();
//	//glBufferSubData( GL_ELEMENT_ARRAY_BUFFER, nOffset, nSize, pData );
//	glBufferData( GL_ELEMENT_ARRAY_BUFFER, nSize, pData, GetAccess() );
//}


iberbar::CResult iberbar::RHI::OpenGL::CIndexBuffer::Lock( uint32 nOffset, uint32 nSize, void** ppOut, uint32 nFlags )
{
	assert( m_bLocked == false );
	assert( nOffset + nSize <= GetInSize() );
	assert( nSize > 0 );

	if ( m_LockBuffer.GetDataSize() < nSize )
	{
		m_LockBuffer.Resize( nSize, false );
	}
	*ppOut = m_LockBuffer.GetPointer();

	m_bLocked = true;
	m_nLockSize = nSize;

	return CResult();
}


iberbar::CResult iberbar::RHI::OpenGL::CIndexBuffer::Unlock()
{
	assert( m_bLocked == true );

	m_pDevice->BindElementArrayBuffer( m_nBuffer );
	glBufferData( GL_ELEMENT_ARRAY_BUFFER, m_nLockSize, m_LockBuffer.GetPointer(), GetAccess() );
	m_bLocked = false;

	return CResult();
}


