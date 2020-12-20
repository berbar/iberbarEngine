
#include <iberbar/RHI/OpenGL/Texture.h>
#include <iberbar/RHI/OpenGL/TextureReadFunc.h>

#include <iberbar/Utility/String.h>
#include <iberbar/Utility/Math.h>
#include <vector>


iberbar::RHI::OpenGL::CTexture::CTexture( void )
	: ITexture()
	, m_TextureUint( 0 )
{
}


iberbar::RHI::OpenGL::CTexture::~CTexture()
{
	glDeleteTextures( 1, &m_TextureUint );
}


iberbar::CResult iberbar::RHI::OpenGL::CTexture::CreateEmpty( int w, int h )
{
	assert( m_TextureUint == 0 );

	if ( w <= 0 ||
		h <= 0 )
		return MakeResult( ResultCode::Bad, "Invalid size" );

	uint32 size = w * h * 4;
	uint8* pixels = new uint8[ size ];
	if ( pixels == NULL )
		return MakeResult( ResultCode::Bad, "Out of memory" );
	memset( pixels, 0, size );

	GLuint lc_glTex = 0;
	glGenTextures( 1, &lc_glTex );
	glPixelStorei( GL_UNPACK_ALIGNMENT, 4 ); 
	glBindTexture( GL_TEXTURE_2D, lc_glTex );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels );

	m_TextureUint = lc_glTex;
	m_Size = CSize2i( w, h );

	return CResult();
}


iberbar::CResult iberbar::RHI::OpenGL::CTexture::CreateFromFileInMemory( const void* pData, uint32 nDataSize )
{
	return MakeResult( ResultCode::Bad, "not support now" );
}


iberbar::CResult iberbar::RHI::OpenGL::CTexture::CreateFromFileA( const char* strFile )
{
	assert( m_TextureUint == 0 );

	int nLen = strlen( strFile );
	int nExtPosition = tstring_rfind_ch( strFile, '.', nLen );
	if (nExtPosition < 0 || (nExtPosition +1 ) >= nLen)
		return MakeResult( ResultCode::Bad, "Invalid format of filepath" );

	std::string strExt = StringToLower( strFile + nExtPosition );
	PFunctionCreateTextureFromFileA pCreateFunc = nullptr;
	if ( strcmp( strExt.c_str(), ".png" ) == 0 )
	{
		pCreateFunc = &CreateTextureGLuintFromPngA;
	}

	if ( pCreateFunc == nullptr )
		return MakeResult( ResultCode::Bad, "No method to read file" );

	if ( pCreateFunc( strFile,
		&m_TextureUint,
		&m_Size,
		&m_Format ) == false )
		return MakeResult( ResultCode::Bad, "Failed to read file" );

	return CResult();
}

#ifdef _WIN32
iberbar::CResult iberbar::RHI::OpenGL::CTexture::CreateFromFileW( const wchar_t* strFile )
{
	assert( m_TextureUint == 0 );

	int nLen = wcslen( strFile );
	int nExtPosition = tstring_rfind_ch( strFile, L'.', nLen );
	if ( nExtPosition < 0 || (nExtPosition + 1) >= nLen )
		return MakeResult( ResultCode::Bad, "Invalid format of filepath" );

	std::wstring strExt = WStringToLower( strFile + nExtPosition );
	PFunctionCreateTextureFromFileW pCreateFunc = nullptr;
	if ( wcscmp( strExt.c_str(), L".png" ) == 0 )
	{
		pCreateFunc = &CreateTextureGLuintFromPngW;
	}

	if ( pCreateFunc == nullptr )
		return MakeResult( ResultCode::Bad, "No method to read file" );

	if ( pCreateFunc( strFile,
		&m_TextureUint,
		&m_Size,
		&m_Format ) == false )
		return MakeResult( ResultCode::Bad, "Failed to read file" );

	return CResult();
}
#endif


iberbar::CResult iberbar::RHI::OpenGL::CTexture::CreateFromPixels( int nTexWidth, int nTexHeight, void* pixels )
{
	assert( m_TextureUint == 0 );

	if ( nTexWidth <= 0 ||
		nTexHeight <= 0 ||
		pixels == NULL )
		return MakeResult( ResultCode::Bad, "Invalid arguments" );

	GLuint lc_glTex = 0;
	glGenTextures( 1, &lc_glTex );
	glPixelStorei( GL_UNPACK_ALIGNMENT, 4 ); 
	glBindTexture( GL_TEXTURE_2D, lc_glTex );
	glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, nTexWidth, nTexHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

	m_TextureUint = lc_glTex;
	m_Size = CSize2i( nTexWidth, nTexHeight );

	return CResult();
}


iberbar::CResult iberbar::RHI::OpenGL::CTexture::SetPixels( const void* pixels, int nx, int ny, int nw, int nh )
{
	if ( pixels == NULL )
		return MakeResult( ResultCode::Bad, "Invalid arguments" );
	if ( nw <= 0 || nh <= 0 )
		return MakeResult( ResultCode::Bad, "Invalid arguments" );

	glPixelStorei( GL_UNPACK_ALIGNMENT, 4 ); 
	glBindTexture( GL_TEXTURE_2D, m_TextureUint );
	glTexSubImage2D( GL_TEXTURE_2D,
		0,
		nx, ny,
		nw, nh,
		GL_RGBA, GL_UNSIGNED_BYTE,
		pixels );

	return CResult();
}


iberbar::CResult iberbar::RHI::OpenGL::CTexture::SaveToFileA( const char* strFile )
{
	return MakeResult( ResultCode::Bad, "not support to save file for opengl texture" );
}


#ifdef _WIN32
iberbar::CResult iberbar::RHI::OpenGL::CTexture::SaveToFileW( const wchar_t* strFile )
{
	return MakeResult( ResultCode::Bad, "not support to save file for opengl texture" );
}
#endif








//
//
//bool iberbar::CreateGLTextureFromFileW( const wchar_t* filename, CGLTextureNode** ppTexNode )
//{
//	if ( filename == NULL )
//		return false;
//	if ( filename[ 0 ] == 0 )
//		return false;
//
//#ifdef UNICODE
//	const TCHAR* lc_filename = filename;
//#else
//	std_tstring lc_tstring;
//	UnicodeToUtf8( strFilename, &lc_tstring );
//	const TCHAR* lc_filename = lc_tstring.c_str();
//#endif
//
//	PTR_CGLTextureManager lc_pTexManager = CGLTextureManager::GetShared();
//
//	// 查找，如果存在已有的纹理，则直接返回
//	if ( lc_pTexManager->findTexture( lc_filename, ppTexNode ) == true )
//		return true;
//
//	// 没有已经加载的纹理，创建新的纹理并加入管理器
//	PTR_CGLTextureNode lc_pTexNode;
//
//	lc_pTexNode.attach( new PTR_CGLTextureNode::_InterfaceT() );
//	if ( lc_pTexNode == NULL )
//		return false;
//	if ( lc_pTexNode->createFromFile( lc_filename ) == false )
//		return false;
//	lc_pTexManager->managerTexture( lc_filename, lc_pTexNode );
//
//	// 赋值到外部变量
//	if ( ppTexNode )
//	{
//		if ( *ppTexNode )
//			( *ppTexNode )->Release();
//		( *ppTexNode ) = lc_pTexNode;
//		( *ppTexNode )->AddRef();
//	}
//
//	return true;
//}
//
//
//bool iberbar::CreateGLTextureFromFileA( const char* filename, CGLTextureNode** ppTexNode )
//{
//	if ( filename == NULL )
//		return false;
//	if ( filename[ 0 ] == 0 )
//		return false;
//
//#ifdef UNICODE
//	std_tstring lc_tstring;
//	Utf8ToUnicode( filename, &lc_tstring );
//	const TCHAR* lc_filename = lc_tstring.c_str();
//#else
//	const TCHAR* lc_filename = strFilename;
//#endif
//
//	PTR_CGLTextureManager lc_pTexManager = CGLTextureManager::GetShared();
//
//	// 查找，如果存在已有的纹理，则直接返回
//	if ( lc_pTexManager->findTexture( lc_filename, ppTexNode ) == true )
//		return true;
//
//	// 没有已经加载的纹理，创建新的纹理并加入管理器
//	PTR_CGLTextureNode lc_pTexNode;
//	
//	lc_pTexNode.attach( new PTR_CGLTextureNode::_InterfaceT() );
//	if ( lc_pTexNode == NULL )
//		return false;
//	if ( lc_pTexNode->createFromFile( lc_filename ) == false )
//		return false;
//	lc_pTexManager->managerTexture( lc_filename, lc_pTexNode );
//
//	// 赋值到外部变量
//	if ( ppTexNode )
//	{
//		if ( *ppTexNode )
//			( *ppTexNode )->Release();
//		( *ppTexNode ) = lc_pTexNode;
//		( *ppTexNode )->AddRef();
//	}
//
//	return true;
//}
//
//
//bool iberbar::CreateGLTextureEmpty( int nTexWidth, int nTexHeight, CGLTextureNode** ppOutTexture )
//{
//	if ( ppOutTexture == NULL )
//		return false;
//
//	PTR_CGLTextureNode lc_texture = NULL;
//
//	lc_texture.attach( new PTR_CGLTextureNode::_InterfaceT() );
//	if ( lc_texture == NULL )
//		return false;
//
//	if ( lc_texture->createEmpty( nTexWidth, nTexHeight ) == false )
//		return false;
//
//	if ( ( *ppOutTexture ) )
//		( *ppOutTexture )->Release();
//	( *ppOutTexture ) = lc_texture;
//	( *ppOutTexture )->AddRef();
//
//	return true;
//}
//
//
//bool iberbar::CreateGLTextureFromMemory( int nTexWidth, int nTexHeight, void* pPixels, CGLTextureNode** ppOutTexture )
//{
//	if ( ppOutTexture == NULL )
//		return false;
//
//	PTR_CGLTextureNode lc_texture = NULL;
//
//	lc_texture.attach( new PTR_CGLTextureNode::_InterfaceT() );
//	if ( lc_texture == NULL )
//		return false;
//
//	if ( lc_texture->createFromPixels( nTexWidth, nTexHeight, pPixels ) == false )
//		return false;
//
//	if ( ( *ppOutTexture ) )
//		( *ppOutTexture )->Release();
//	( *ppOutTexture ) = lc_texture;
//	( *ppOutTexture )->AddRef();
//
//	return true;
//}
//
//
//
//
//
//
//
//iberbar::PTR_CGLTextureManager iberbar::CGLTextureManager::sm_pShared( nullptr );
//
//
//
//iberbar::CGLTextureManager::CGLTextureManager( void )
//{
//}
//
//
//iberbar::CGLTextureManager::~CGLTextureManager()
//{
//	TextureList::node_iterator lc_iter = m_TextureCache.begin();
//	TextureList::node_iterator lc_end  = m_TextureCache.end();
//	for ( ; lc_iter != lc_end; lc_iter ++ )
//	{
//		if ( (*lc_iter)->data )
//		{
//			UNKNOWN_SAFE_RELEASE_NULL( (*lc_iter)->data );
//			(*lc_iter)->data = NULL;
//		}
//	}
//	m_TextureCache.clear();
//}
//
//
//bool iberbar::CGLTextureManager::managerTexture( const TCHAR* key, CGLTextureNode* texture )
//{
//	if ( texture == NULL ||
//		key == NULL ||
//		key[ 0 ] == 0 )
//		return false;
//
//	if ( m_TextureCache.find( key, NULL ) == true )
//		return false;
//
//	m_TextureCache.add( key, texture );
//	texture->AddRef();
//	return true;
//}
//
//
//bool iberbar::CGLTextureManager::findTexture( const TCHAR* key, CGLTextureNode** ppOutTexture )
//{
//	if ( key == NULL ||
//		key[ 0 ] == 0 )
//		return false;
//
//	CGLTextureNode* lc_texture;
//	if ( m_TextureCache.find( key, &lc_texture ) == false )
//		return false;
//
//	if ( ppOutTexture )
//	{
//		if ( *ppOutTexture )
//			( *ppOutTexture )->Release();
//		( *ppOutTexture ) = lc_texture;
//		( *ppOutTexture )->AddRef();
//	}
//
//	return true;
////	return false;
//}
//
//
//void iberbar::CGLTextureManager::flush()
//{
//	TextureList::node_iterator lc_iter = m_TextureCache.begin();
//	for ( ; lc_iter != m_TextureCache.end(); )
//	{
//		if ( ( *lc_iter )->data && ( *lc_iter )->data->Refcount() == 1 )
//		{
//			( *lc_iter )->data->Release();
//			( *lc_iter )->data = NULL;
//			lc_iter = m_TextureCache.erase( lc_iter );
//		}
//		else
//			lc_iter ++;
//	}
//}









