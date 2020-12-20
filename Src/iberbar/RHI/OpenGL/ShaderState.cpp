
#include <iberbar/RHI/OpenGL/ShaderState.h>
#include <iberbar/RHI/OpenGL/ShaderProgram.h>
#include <iberbar/RHI/OpenGL/Device.h>
#include <iberbar/RHI/OpenGL/VertexDeclaration.h>







iberbar::RHI::OpenGL::CShaderVariableDeclaration::CShaderVariableDeclaration()
	: m_nTotalByteSize( 0 )
	, m_nSamplerStateMax( 0 )
	, m_Nodes()
{
}


void iberbar::RHI::OpenGL::CShaderVariableDeclaration::AddNode( const char* strName, GLuint nLocation, GLenum nVarType, GLint nElementCount )
{
	UShaderVariableDeclarationNode node;
	node.strName = strName;
	node.nLocation = nLocation;
	node.nOffset = m_nTotalByteSize;
	node.nElementCount = nElementCount;
	switch ( nVarType )
	{
		case GL_BOOL:
			node.nVarType = UShaderVariableType::VT_Boolean;
			node.nElementSize = sizeof( uint32 );
			break;

		case GL_INT:
			node.nVarType = UShaderVariableType::VT_Int;
			node.nElementSize = sizeof( int32 );
			break;

		case GL_FLOAT:
			node.nVarType = UShaderVariableType::VT_Float;
			node.nElementSize = sizeof( float );
			break;

		case GL_FLOAT_VEC4:
			node.nVarType = UShaderVariableType::VT_Vector4;
			node.nElementSize = sizeof( float ) * 4;
			break;

		case GL_FLOAT_MAT4:
			node.nVarType = UShaderVariableType::VT_Matrix44;
			node.nElementSize = sizeof( float ) * 16;
			break;

		case GL_SAMPLER_2D:
			node.nVarType = UShaderVariableType::VT_Sampler2D;
			node.nElementSize = 0;
			node.nTextureStage = m_nSamplerStateMax;
			m_nSamplerStateMax++;
			break;

		default:
			break;
	}
	node.nTotalSize = node.nElementCount * node.nElementSize;
	m_Nodes.push_back( node );

	m_nTotalByteSize += node.nTotalSize;
}


const iberbar::RHI::OpenGL::UShaderVariableDeclarationNode* iberbar::RHI::OpenGL::CShaderVariableDeclaration::GetNode( const char* strName ) const
{
	auto iter = m_Nodes.begin();
	auto end = m_Nodes.end();
	for ( ; iter != end; iter++ )
	{
		if ( strcmp( iter->strName.c_str(), strName ) == 0 )
			return &(*iter);
	}
	return nullptr;
}






iberbar::RHI::OpenGL::CShaderState::CShaderState( CDevice* pDevice, CShaderProgram* pProgram, CVertexDeclaration* pVertexDeclaration )
	: IShaderState()
	, m_pDevice( pDevice )
	, m_pShaderProgram( pProgram )
	, m_pVertexDeclaration( pVertexDeclaration )
	, m_pShaderVariableDeclaration( nullptr )
{
	m_pDevice->AddRef();
	m_pShaderProgram->AddRef();
	m_pVertexDeclaration->AddRef();
}

iberbar::RHI::OpenGL::CShaderState::~CShaderState()
{
	SAFE_DELETE( m_pShaderVariableDeclaration );
	UNKNOWN_SAFE_RELEASE_NULL( m_pVertexDeclaration );
	UNKNOWN_SAFE_RELEASE_NULL( m_pShaderProgram );
	UNKNOWN_SAFE_RELEASE_NULL( m_pDevice );
}


iberbar::CResult iberbar::RHI::OpenGL::CShaderState::GenarateConstTable()
{
	if ( m_pShaderVariableDeclaration != nullptr )
		return MakeResult( ResultCode::Bad, "don't GenarateConstTable twice" );

	CResult ret;

	m_pShaderVariableDeclaration = new CShaderVariableDeclaration();

	GLuint nProgram = m_pShaderProgram->GetProgramUint();
	GLint nUniformCount = 0;
	glGetProgramiv( nProgram, GL_ACTIVE_UNIFORMS, &nUniformCount );
	if ( nUniformCount == 0 )
		return CResult();

#define MAX_UNIFORM_NAME_LENGTH 255
	GLint nStrLength = 0;
	GLuint nVarLocation = -1;
	GLenum nVarType = -1;
	GLint nVarElementCount = 0;
	//bool nVarIsArray = false;
	GLchar strName[ MAX_UNIFORM_NAME_LENGTH + 1 ];
	memset( strName, 0, sizeof( strName ) );
	for ( int i = 0; i < nUniformCount; i++ )
	{
		glGetActiveUniform( nProgram, i, MAX_UNIFORM_NAME_LENGTH, &nStrLength, &nVarElementCount, &nVarType, strName );
		strName[ nStrLength ] = '\0';

		//if ( length > 3 )
		//{
		//	char* c = strrchr( uniformName, '[' );
		//	if ( c )
		//	{
		//		*c = '\0';
		//		uniform.isArray = true;
		//	}
		//}
		nVarLocation = glGetUniformLocation( nProgram, strName );
		m_pShaderVariableDeclaration->AddNode( strName, nVarLocation, nVarType, nVarElementCount );
		//uniform.size = GetDataTypeSize( uniform.type );
		//uniform.bufferOffset = (uniform.size == 0) ? 0 : _totalBufferSize;
		//_activeUniformInfos[ uniformName ] = uniform;
		//_totalBufferSize += uniform.size * uniform.count;
		//_maxLocation = _maxLocation <= uniform.location ? (uniform.location + 1) : _maxLocation;
	}

	return CResult();
}
