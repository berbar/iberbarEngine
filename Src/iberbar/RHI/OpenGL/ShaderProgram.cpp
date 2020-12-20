

#include <iberbar/RHI/OpenGL/ShaderProgram.h>
#include <iberbar/Utility/String.h>
#include <iberbar/Utility/FileHelper.h>



iberbar::CResult iberbar::RHI::OpenGL::LoadShaderFromSource( GLenum shaderType, const char* pSource, GLuint* pShader )
{
	assert( pSource );
	assert( pShader );

	GLuint shader = glCreateShader( shaderType );
	if (shader <= 0)
		return MakeResult( ResultCode::Bad, u8"创建shader失败" );


	glShaderSource( shader, 1, &pSource, NULL );
	glCompileShader( shader );
	GLint compiled = 0;
	glGetShaderiv( shader, GL_COMPILE_STATUS, &compiled );
	if (!compiled)
	{
		CResult retCompile( ResultCode::Bad, "" );

		GLint infoLen = 0;
		glGetShaderiv( shader, GL_INFO_LOG_LENGTH, &infoLen );
		if (infoLen)
		{
			retCompile.data.resize( infoLen + 1 );
			glGetShaderInfoLog( shader, infoLen, NULL, &retCompile.data.front() );
			retCompile.data[infoLen] = 0;
		}

		const char* ptr = retCompile.data.c_str();

		glDeleteShader( shader );
		shader = 0;

		return retCompile;
	}

	*pShader = shader;

	return CResult();
}



iberbar::RHI::OpenGL::CShaderProgram::CShaderProgram( void )
	: m_VertexShader( 0 )
	, m_FragmentShader( 0 )
	, m_Program( 0 )
{
}


iberbar::RHI::OpenGL::CShaderProgram::~CShaderProgram()
{
	if ( m_Program > 0 )
		glDeleteProgram( m_Program );
	if ( m_VertexShader > 0 )
		glDeleteShader( m_VertexShader );
	if ( m_FragmentShader > 0 )
		glDeleteShader( m_FragmentShader );
}


iberbar::CResult iberbar::RHI::OpenGL::CShaderProgram::LoadFromSource( const char* pVertexSource, const char* pFragmentSource )
{
	if (StringIsNullOrEmpty( pVertexSource ))
		return MakeResult( ResultCode::Bad, u8"空白的顶点着色器代码" );

	if (StringIsNullOrEmpty( pFragmentSource ))
		return MakeResult( ResultCode::Bad, u8"空白的片元着色器代码" );

	CResult ret;

	ret = LoadShaderFromSource( GL_VERTEX_SHADER, pVertexSource, &m_VertexShader );
	if (ret.IsOK() == false )
		return ret;

	ret = LoadShaderFromSource( GL_FRAGMENT_SHADER, pFragmentSource, &m_FragmentShader );
	if ( ret.IsOK() == false )
		return ret;

	m_Program = glCreateProgram();
	if (m_Program <= 0)
		return MakeResult( ResultCode::Bad, "" );

	glAttachShader( m_Program, m_VertexShader );
	glAttachShader( m_Program, m_FragmentShader );
	glLinkProgram( m_Program );
	GLint linkStatus = GL_FALSE;
	glGetProgramiv( m_Program, GL_LINK_STATUS, &linkStatus );
	if (linkStatus != GL_TRUE)
	{
		ret.code = ResultCode::Bad;

		// 获取错误信息
		GLint bufLength = 0;
		glGetProgramiv( m_Program, GL_INFO_LOG_LENGTH, &bufLength );
		if (bufLength)
		{
			ret.data.resize( bufLength + 1 );
			char* buf = (char*)malloc( bufLength + 1 );
			glGetProgramInfoLog( m_Program, bufLength, NULL, ret.data.data() );
			buf[bufLength] = 0;
		}

		glDeleteProgram( m_Program );
		m_Program = 0;

		return ret;
	}

	return ret;
}


iberbar::CResult iberbar::RHI::OpenGL::CShaderProgram::LoadFromFile( const char* pVertexFile, const char* pFragmentFile )
{
	if ( StringIsNullOrEmpty( pVertexFile ) == true )
		return MakeResult( ResultCode::Bad, u8"无效的顶点着色器文件" );

	if ( StringIsNullOrEmpty( pFragmentFile ) == true )
		return MakeResult( ResultCode::Bad, u8"无效的片元着色器文件" );

	std::string strVertexSource;
	std::string strFragmentSource;

	CFileHelper fileHelper;

	if (fileHelper.OpenFileA( pVertexFile, "r" ) == false)
		return MakeResult( ResultCode::Bad, u8"打开顶点着色器文件失败" );
	strVertexSource = fileHelper.ReadAsText();
	fileHelper.CloseFile();

	if (fileHelper.OpenFileA( pFragmentFile, "r" ) == false)
		return MakeResult( ResultCode::Bad, u8"打开片元着色器文件失败" );
	strFragmentSource = fileHelper.ReadAsText();
	fileHelper.CloseFile();

	return LoadFromSource( strVertexSource.c_str(), strFragmentSource.c_str() );
}





iberbar::RHI::OpenGL::CShaderProgramManager::CShaderProgramManager( void )
	: m_pUsingShaderProgram( NULL )
{
}


iberbar::RHI::OpenGL::CShaderProgramManager::~CShaderProgramManager()
{
	UNKNOWN_SAFE_RELEASE_NULL( m_pUsingShaderProgram );

	auto iter = m_vShaderPrograms.begin();
	auto end  = m_vShaderPrograms.end();
	for ( ; iter != end; iter++ )
	{
		UNKNOWN_SAFE_RELEASE_NULL( iter->second );
	}
	m_vShaderPrograms.clear();
}


bool iberbar::RHI::OpenGL::CShaderProgramManager::AddShaderProgram( const TCHAR* strKey, CShaderProgram* pShaderProgram )
{
	if ( strKey == NULL || strKey[ 0 ] == 0 )
		return false;
	if ( pShaderProgram == NULL )
		return false;

	if ( FindShaderProgram( strKey ) == true )
		return false;

	m_vShaderPrograms.insert( std::make_pair( strKey, pShaderProgram ) );
	pShaderProgram->AddRef();

	return true;
}


bool iberbar::RHI::OpenGL::CShaderProgramManager::FindShaderProgram( const TCHAR* strKey, CShaderProgram** ppOutShaderProgram )
{
	if ( StringIsNullOrEmpty( strKey ) == true )
		return false;

	auto iter = m_vShaderPrograms.find( strKey );
	if (iter == m_vShaderPrograms.end())
		return false;

	CShaderProgram* pShader = iter->second;
	if ( pShader == nullptr )
		return false;

	if ( ppOutShaderProgram )
	{
		if ( *ppOutShaderProgram )
			( *ppOutShaderProgram )->Release();
		( *ppOutShaderProgram ) = pShader;
		( *ppOutShaderProgram )->AddRef();
	}

	return true;
}


void iberbar::RHI::OpenGL::CShaderProgramManager::UseShaderProgram( CShaderProgram* pUseShaderProgram )
{
	if ( pUseShaderProgram == m_pUsingShaderProgram )
		return ;

	if ( m_pUsingShaderProgram )
		m_pUsingShaderProgram->Release();

	m_pUsingShaderProgram = pUseShaderProgram;

	if ( m_pUsingShaderProgram )
	{
		m_pUsingShaderProgram->AddRef();
		glUseProgram( m_pUsingShaderProgram->GetProgramUint() );
	}
	else
	{
		glUseProgram( 0 );
	}
}
