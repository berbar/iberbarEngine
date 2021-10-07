
#include <iberbar/RHI/OpenGL/Device.h>
#include <iberbar/RHI/OpenGL/Buffer.h>
#include <iberbar/RHI/OpenGL/CommandContext.h>
#include <iberbar/RHI/OpenGL/ShaderProgram.h>
#include <iberbar/RHI/OpenGL/ShaderState.h>
#include <iberbar/RHI/OpenGL/ShaderVariables.h>
#include <iberbar/RHI/OpenGL/VertexDeclaration.h>
#include <iberbar/RHI/OpenGL/Texture.h>
#include <iberbar/RHI/OpenGL/Types.h>



iberbar::RHI::OpenGL::CDevice::CDevice()
	: IDevice( UApiType::OpenGL )
	, m_bHasLostDevice( false )
#ifdef _WINDOWS
	, m_hWnd( nullptr )
	, m_hDC( nullptr )
	, m_hRC( nullptr )
#endif
	, m_ArrayBufferBound( 0 )
	, m_ElementArrayBufferBound( 0 )
	, m_SamplerStateList()
{
	memset( m_SamplerStateList, 0, sizeof( m_SamplerStateList ) );
}


iberbar::RHI::OpenGL::CDevice::~CDevice()
{
	glDeleteSamplers( 8, m_SamplerStateList );
	memset( m_SamplerStateList, 0, sizeof( m_SamplerStateList ) );

#ifdef _WIN32
	if ( m_hRC )
	{
		::wglMakeCurrent( NULL, NULL );
		::wglDeleteContext( m_hRC );
	}

	if ( m_hDC )
		::ReleaseDC( m_hWnd, m_hDC );
#endif
}


#ifdef _WIN32
iberbar::CResult iberbar::RHI::OpenGL::CDevice::CreateDevice( HWND hWnd, bool bWindowed /* = true */, int nSuitedWidth /* = 0 */, int nSuitedHeight /* = 0 */ )
{
	assert( m_hWnd == NULL );

	if ( hWnd == NULL )
		return CResult( ResultCode::Bad, "" );
	m_hWnd = hWnd;

	GLuint PixelFormat;
	PIXELFORMATDESCRIPTOR lc_PixelFormat =
	{
		sizeof( PIXELFORMATDESCRIPTOR ),		    // Size Of This Pixel Format Descriptor
		1,											// Version Number
		PFD_DRAW_TO_WINDOW |						// Format Must Support Window
		PFD_SUPPORT_OPENGL |						// Format Must Support OpenGL
		PFD_DOUBLEBUFFER,							// Must Support Double Buffering
		PFD_TYPE_RGBA,								// Request An RGBA Format
		24,										    // Select Our Color Depth
		0, 0, 0, 0, 0, 0,							// Color Bits Ignored
		0,											// No Alpha Buffer
		0,											// Shift Bit Ignored
		0,											// No Accumulation Buffer
		0, 0, 0, 0,									// Accumulation Bits Ignored
		32,											// 16Bit Z-Buffer (Depth Buffer)  
		0,											// No Stencil Buffer
		0,											// No Auxiliary Buffer
		PFD_MAIN_PLANE,								// Main Drawing Layer
		0,											// Reserved
		0, 0, 0										// Layer Masks Ignored
	};

	m_hDC = ::GetDC( hWnd );
	if ( m_hDC == NULL )
	{
		return CResult( ResultCode::Bad, "" );
	}

	PixelFormat = ::ChoosePixelFormat( m_hDC, &lc_PixelFormat );
	if ( PixelFormat == 0 )	// Did Windows Find A Matching Pixel Format?
	{
		return CResult( ResultCode::Bad, "" );								// Return FALSE
	}

	if ( !SetPixelFormat( m_hDC, PixelFormat, &lc_PixelFormat ) )		// Are We Able To Set The Pixel Format?
	{
		return CResult( ResultCode::Bad, "" );
	}

	if ( !( m_hRC=wglCreateContext( m_hDC ) ) )				// Are We Able To Get A Rendering Context?
	{
		return CResult( ResultCode::Bad, "" );
	}

	if ( !wglMakeCurrent( m_hDC, m_hRC ) )					// Try To Activate The Rendering Context
	{
		return CResult( ResultCode::Bad, "" );
	}


	glDisable( GL_POINT_SMOOTH );
	glDisable( GL_LINE_SMOOTH );
	glDisable( GL_POLYGON_SMOOTH );
	//glEnable( GL_POINT_SMOOTH );
	//glEnable( GL_LINE_SMOOTH );
	//glEnable( GL_POLYGON_SMOOTH );
 //	glHint( GL_POINT_SMOOTH_HINT, GL_NICEST ); // Make round points, not square points
 //	glHint( GL_LINE_SMOOTH_HINT, GL_NICEST );  // Antialias the lines
 //	glHint( GL_POLYGON_SMOOTH_HINT, GL_NICEST );
	//glHint( GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST );

	glEnable( GL_DEPTH );
	glEnable( GL_BLEND );
	glBlendFunc( GL_ONE, GL_ONE_MINUS_SRC_ALPHA );
	glEnable( GL_ALPHA_TEST );
	
	/*glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_MULTISAMPLE);*/
	//glEnable(GL_MULTISAMPLE);
	//glEnable( GL_SAMPLE_ALPHA_TO_ONE );

	glewInit();

	m_ContextSize = CSize2i( nSuitedWidth, nSuitedHeight );
	glViewport( 0, 0, m_ContextSize.w, m_ContextSize.h );						// Reset The Current Viewport

// 	glMatrixMode( GL_PROJECTION );						// Select The Projection Matrix
// 	glLoadIdentity();									// Reset The Projection Matrix
// 	gluPerspective( 90.0f, (GLfloat)m_GLSize.cx/(GLfloat)m_GLSize.cy, 1.0f, 1000.0f );
// 
// 	glMatrixMode( GL_MODELVIEW );
// 	glLoadIdentity();

	if ( m_CallbackCreated )
	{
		CResult ret = m_CallbackCreated( this );
		if ( ret.IsOK() == false )
			return ret;
	}

	return CResult();
}
#else
iberbar::CResult iberbar::RHI::OpenGL::CDevice::CreateDevice( int nSuitedWidth, int nSuitedHeight )
{
	m_ContextSize = CSize2i( nSuitedWidth, nSuitedHeight );
	glViewport( 0, 0, m_ContextSize.w, m_ContextSize.h );

	if ( m_CallbackCreated )
	{
		CResult ret = m_CallbackCreated( this );
		if ( ret.IsOK() == false )
			return ret;
	}

	return CResult();
}
#endif


void iberbar::RHI::OpenGL::CDevice::LostDevice()
{
	m_bHasLostDevice = true;
	if ( m_CallbackLost )
	{
		m_CallbackLost( this );
	}
}


iberbar::CResult iberbar::RHI::OpenGL::CDevice::ResetDevice( int nBackBufferWidth, int nBackBufferHeight, bool bIsWindow )
{
	//assert( m_bHasLostDevice == true );

	if ( nBackBufferHeight == 0 )
	{
		nBackBufferHeight = 1;
	}

	glViewport( 0, 0, nBackBufferWidth, nBackBufferHeight );
	m_ContextSize = CSize2i( nBackBufferWidth, nBackBufferHeight );

	if ( m_CallbackReset )
	{
		CResult ret = m_CallbackReset( this );
		if ( ret.IsOK() == false )
			return ret;
	}
	
	m_bHasLostDevice = false;

	return CResult();
}


void iberbar::RHI::OpenGL::CDevice::CreateTexture( ITexture** ppOutTexture )
{
	assert( ppOutTexture );
	TSmartRefPtr<CTexture> pTexture = TSmartRefPtr<CTexture>::_sNew();
	UNKNOWN_SAFE_RELEASE_NULL( *ppOutTexture );
	(*ppOutTexture) = pTexture;
	(*ppOutTexture)->AddRef();
}


iberbar::CResult iberbar::RHI::OpenGL::CDevice::CreateVertexBuffer( uint32 nInSize, uint32 nUsage, IVertexBuffer** ppOutBuffer )
{
	assert( ppOutBuffer );
	TSmartRefPtr<CVertexBuffer> pVertexBuffer = TSmartRefPtr<CVertexBuffer>::_sNew( this, nInSize, nUsage );
	pVertexBuffer->Initial();
	UNKNOWN_SAFE_RELEASE_NULL( *ppOutBuffer );
	(*ppOutBuffer) = pVertexBuffer;
	(*ppOutBuffer)->AddRef();
	return CResult();
}


iberbar::CResult iberbar::RHI::OpenGL::CDevice::CreateIndexBuffer( uint32 nStride, uint32 nInSize, uint32 nUsage, IIndexBuffer** ppOutBuffer )
{
	assert( ppOutBuffer );
	TSmartRefPtr<CIndexBuffer> pNewBuffer = TSmartRefPtr<CIndexBuffer>::_sNew( this, nStride, nInSize, nUsage );
	pNewBuffer->Initial();
	UNKNOWN_SAFE_RELEASE_NULL( *ppOutBuffer );
	(*ppOutBuffer) = pNewBuffer;
	(*ppOutBuffer)->AddRef();
	return CResult();
}


iberbar::CResult iberbar::RHI::OpenGL::CDevice::CreateShader( IShader** ppOutShader )
{
	assert( ppOutShader );
	TSmartRefPtr<CShaderProgram> pNewShader = TSmartRefPtr<CShaderProgram>::_sNew();
	UNKNOWN_SAFE_RELEASE_NULL( *ppOutShader );
	(*ppOutShader) = pNewShader;
	(*ppOutShader)->AddRef();
	return CResult();
}


iberbar::CResult iberbar::RHI::OpenGL::CDevice::CreateVertexDeclaration( IVertexDeclaration** ppOutDeclaration, const UVertexElement* pVertexElements, uint32 nVertexElementsCount, uint32 nStride )
{
	assert( ppOutDeclaration );
	TSmartRefPtr<CVertexDeclaration> pDeclaration = TSmartRefPtr<CVertexDeclaration>::_sNew( this, pVertexElements, nVertexElementsCount, nStride );
	UNKNOWN_SAFE_RELEASE_NULL( *ppOutDeclaration );
	(*ppOutDeclaration) = pDeclaration;
	(*ppOutDeclaration)->AddRef();
	return CResult();
}


iberbar::CResult iberbar::RHI::OpenGL::CDevice::CreateShaderState( IShaderState** ppOutShaderState, IShader* pShader, IVertexDeclaration* pVertexDeclaration )
{
	assert( ppOutShaderState );
	((CVertexDeclaration*)pVertexDeclaration)->Initial( (CShaderProgram*)pShader );
	TSmartRefPtr<CShaderState> pShaderState = TSmartRefPtr<CShaderState>::_sNew( this, (CShaderProgram*)pShader, (CVertexDeclaration*)pVertexDeclaration );
	CResult ret = pShaderState->GenarateConstTable();
	if ( ret.IsOK() == false )
		return ret;
	UNKNOWN_SAFE_RELEASE_NULL( *ppOutShaderState );
	(*ppOutShaderState) = pShaderState;
	(*ppOutShaderState)->AddRef();
	return CResult();
}


void iberbar::RHI::OpenGL::CDevice::CreateShaderVariableTable( IShaderVariableTable** ppOutShaderVarTable )
{
	assert( ppOutShaderVarTable );
	TSmartRefPtr<CShaderVariableTable> pShaderVarTable = TSmartRefPtr<CShaderVariableTable>::_sNew();
	UNKNOWN_SAFE_RELEASE_NULL( *ppOutShaderVarTable );
	(*ppOutShaderVarTable) = pShaderVarTable;
	(*ppOutShaderVarTable)->AddRef();
}


void iberbar::RHI::OpenGL::CDevice::CreateCommandContext( ICommandContext** ppOutContext )
{
	assert( ppOutContext );
	TSmartRefPtr<CCommandContext> pNewContext = TSmartRefPtr<CCommandContext>::_sNew( this );
	UNKNOWN_SAFE_RELEASE_NULL( *ppOutContext );
	(*ppOutContext) = pNewContext;
	(*ppOutContext)->AddRef();
}


iberbar::CResult iberbar::RHI::OpenGL::CDevice::Begin()
{
	if ( m_bHasLostDevice )
	{
		LostDevice();
		CResult RetReset = ResetDevice( m_ContextSize.w, m_ContextSize.h, m_bIsWindow );
		if ( RetReset.IsOK() == false )
			return RetReset;
		m_bHasLostDevice = false;
	}

	//---------------------------------
	// 清屏并初始化视觉
	//---------------------------------
	glClearColor( m_ClearColor.r, m_ClearColor.g, m_ClearColor.b, 1.0f );
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );	// Clear Screen And Depth Buffer
#ifdef WIN32
	glEnable( GL_DEPTH );
	glEnable( GL_MULTISAMPLE );
#endif

	return CResult();
}


void iberbar::RHI::OpenGL::CDevice::End()
{
#ifdef WIN32
	//将在后台缓冲区绘制的图形提交到前台缓冲区显示  
	SwapBuffers( m_hDC );
#endif
}

//
//void iberbar::RHI::CGLesDevice::useGLProgram( CGLShaderProgram* program )
//{
//
//	if ( program == NULL )
//	{
//		if ( m_pCurrentGLProgram )
//		{
//			m_pCurrentGLProgram->release();
//			m_pCurrentGLProgram = NULL;
//		}
//
//		glUseProgram( 0 );
//	}
//	else
//	{
//		assert( program->getProgram() != 0 );
//
//		if ( program == m_pCurrentGLProgram )
//			return;
//
//		if ( m_pCurrentGLProgram )
//			m_pCurrentGLProgram->release();
//		m_pCurrentGLProgram = program;
//		m_pCurrentGLProgram->addRef();
//
//		glUseProgram( m_pCurrentGLProgram->getProgram() );
//	}
//}
//
//
//void iberbar::RHI::CGLesDevice::getGLProgramCurrent( CGLShaderProgram** pp )
//{
//	if ( pp == nullptr )
//		return ;
//	UNKNOWN_SAFE_RELEASE_NULL( *pp );
//	( *pp ) = m_pCurrentGLProgram;
//	UNKNOWN_SAFE_ADDREF( *pp );
//}
//
//
//void iberbar::RHI::CGLesDevice::setBlendFunc( GLenum sfactor, GLenum dfactor )
//{
//	if ( sfactor != m_eBlendingSource || dfactor != m_eBlendingDest )
//	{
//		m_eBlendingSource = sfactor;
//		m_eBlendingDest = dfactor;
//
//		if ( sfactor == GL_ONE && dfactor == GL_ZERO )
//		{
//			glDisable( GL_BLEND );
//		}
//		else
//		{
//			glEnable( GL_BLEND );
//			glBlendFunc( sfactor, dfactor );
//		}
//	}
//}


void iberbar::RHI::OpenGL::CDevice::SetSamplerState( uint32 nStage, const UTextureSamplerState& SamplerState )
{
	if ( nStage >= 8 )
		return;

	GLuint nSamplerHandle = m_SamplerStateList[ nStage ];

#ifdef _WIN32

	//glTexParameteri;
	//glSamplerParameteri( nSamplerHandle, GL_TEXTURE_MAG_FILTER, ConvertTextureFilterType( SamplerState.nMagFilter ) );
	//glSamplerParameteri( nSamplerHandle, GL_TEXTURE_MIN_FILTER, ConvertTextureFilterType( SamplerState.nMinFilter ) );
	//glSamplerParameteri( nSamplerHandle, GL_TEXTURE_WRAP_S, ConvertTextureAddress( SamplerState.nAddressU ) );
	//glSamplerParameteri( nSamplerHandle, GL_TEXTURE_WRAP_T, ConvertTextureAddress( SamplerState.nAddressV ) );
#endif

#ifdef __ANDROID__
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, ConvertTextureFilterType( SamplerState.nMagFilter ) );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, ConvertTextureFilterType( SamplerState.nMinFilter ) );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, ConvertTextureAddress( SamplerState.nAddressU ) );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, ConvertTextureAddress( SamplerState.nAddressV ) );
#endif
}


void iberbar::RHI::OpenGL::CDevice::OnCreated()
{
#ifdef _WIN32
	glGenSamplers( 8, m_SamplerStateList );
	for ( int i = 0; i < 8; i++ )
	{
		glBindSampler( i, m_SamplerStateList[ i ] );
	}
#endif
}

