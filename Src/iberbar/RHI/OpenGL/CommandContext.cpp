
#include <iberbar/RHI/OpenGL/CommandContext.h>
#include <iberbar/RHI/OpenGL/Buffer.h>
#include <iberbar/RHI/OpenGL/ShaderProgram.h>
#include <iberbar/RHI/OpenGL/ShaderState.h>
#include <iberbar/RHI/OpenGL/ShaderVariables.h>
#include <iberbar/RHI/OpenGL/Texture.h>
#include <iberbar/RHI/OpenGL/Types.h>
#include <iberbar/RHI/OpenGL/VertexDeclaration.h>
#include <iberbar/RHI/ShaderVariables.h>


iberbar::RHI::OpenGL::CCommandContext::CCommandContext( CDevice* pDevice )
	: ICommandContext()
	, m_pDevice( pDevice )
	, m_pVertexBuffer( nullptr )
	, m_pIndexBuffer( nullptr )
	, m_pShaderState( nullptr )
	, m_pShaderVariableTable( nullptr )
{
	m_pDevice->AddRef();
	memset( m_pTextureSlot, 0, sizeof( m_pTextureSlot ) );
}


iberbar::RHI::OpenGL::CCommandContext::~CCommandContext()
{
	UNKNOWN_SAFE_RELEASE_NULL( m_pShaderState );
	UNKNOWN_SAFE_RELEASE_NULL( m_pShaderVariableTable );
	UNKNOWN_SAFE_RELEASE_NULL( m_pVertexBuffer );
	UNKNOWN_SAFE_RELEASE_NULL( m_pIndexBuffer );
	for ( int i = 0; i < 8; i++ )
	{
		UNKNOWN_SAFE_RELEASE_NULL( m_pTextureSlot[ i ] );
	}
	UNKNOWN_SAFE_RELEASE_NULL( m_pDevice );
}


void iberbar::RHI::OpenGL::CCommandContext::SetVertexBuffer( IVertexBuffer* pVertexBuffer )
{
	if ( m_pVertexBuffer == pVertexBuffer )
		return;
	UNKNOWN_SAFE_RELEASE_NULL( m_pVertexBuffer );
	m_pVertexBuffer = (CVertexBuffer*)pVertexBuffer;
	UNKNOWN_SAFE_ADDREF( m_pVertexBuffer );
}


void iberbar::RHI::OpenGL::CCommandContext::SetIndexBuffer( IIndexBuffer* pIndexBuffer )
{
	if ( m_pIndexBuffer == pIndexBuffer )
		return;
	UNKNOWN_SAFE_RELEASE_NULL( m_pIndexBuffer );
	m_pIndexBuffer = (CIndexBuffer*)pIndexBuffer;
	UNKNOWN_SAFE_ADDREF( m_pIndexBuffer );
}


void iberbar::RHI::OpenGL::CCommandContext::SetShaderState( IShaderState* pShaderState )
{
	if ( m_pShaderState == pShaderState )
		return;
	UNKNOWN_SAFE_RELEASE_NULL( m_pShaderState );
	m_pShaderState = (CShaderState*)pShaderState;
	UNKNOWN_SAFE_ADDREF( m_pShaderState );
}


void iberbar::RHI::OpenGL::CCommandContext::SetShaderVariableTable( IShaderVariableTable* pShaderVariableTable )
{
	if ( m_pShaderVariableTable != pShaderVariableTable )
	{
		UNKNOWN_SAFE_RELEASE_NULL( m_pShaderVariableTable );
		m_pShaderVariableTable = (CShaderVariableTable*)pShaderVariableTable;
		UNKNOWN_SAFE_ADDREF( m_pShaderVariableTable );
	}
}


void iberbar::RHI::OpenGL::CCommandContext::DrawElements( UPrimitiveType nPrimitiveType, UIndexFormat nIndexFormat, uint32 nCount, uint32 nOffset )
{
	PrepareDraw();
	m_pIndexBuffer->Bind();
	uint32 nIndexCount = nCount * 3;
	glDrawElements( ConvertPrimitiveType( nPrimitiveType ), nIndexCount, ConvertIndexFormat( nIndexFormat ), (GLvoid*)nOffset );
	CleanResources();
}


void iberbar::RHI::OpenGL::CCommandContext::PrepareDraw()
{
	glEnable( GL_BLEND );
	glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

	CShaderProgram* pShader = m_pShaderState->GetProgram();
	glUseProgram( pShader->GetProgramUint() );
	CVertexDeclaration* pVertexDeclaration = m_pShaderState->GetVertexDeclaration();

	m_pVertexBuffer->Bind();
	auto nAttrListCount = pVertexDeclaration->GetGLVertexAttributesCount();
	auto pAttrList = pVertexDeclaration->GetGLVertexAttributes();
	for ( uint32 i = 0; i < nAttrListCount; i++ )
	{
		const auto* attribute = &pAttrList[ i ];
		glEnableVertexAttribArray( attribute->nLocation );
		glVertexAttribPointer(
			attribute->nLocation,
			attribute->nSize,
			attribute->nType,
			attribute->bNormallized,
			pVertexDeclaration->GetStride(),
			(GLvoid*)attribute->nOffset );
	}


	PrepareShaderVariables();
	////
	//// 设置 shader 的 Sampler
	//auto& ShaderVars_Samplers = m_pShaderVars->GetSamplers();
	//if ( ShaderVars_Samplers.empty() == false )
	//{
	//	CTexture* pTextureTemp = nullptr;
	//	GLuint nTextureStage = 0;
	//	UShaderSampler* pShaderSampler;
	//	for ( size_t i = 0, s = ShaderVars_Samplers.size(); i < s; i++ )
	//	{
	//		GLuint nLocation = pShader->GetUniformLocation( ShaderVars_Samplers[ i ].strName.c_str() );
	//		if ( nLocation == GLShader_UniformLocation_Null )
	//			continue;

	//		pShaderSampler = &ShaderVars_Samplers[ i ].Value;
	//		pTextureTemp = (CTexture*)(ITexture*)(pShaderSampler->pTexture);

	//		// 暂时是不正规用法，迟点封装一下
	//		glActiveTexture( GL_TEXTURE0 + nTextureStage );
	//		glBindTexture( GL_TEXTURE_2D, pTextureTemp == nullptr ? 0 : pTextureTemp->GetTextureUint() );
	//		glUniform1i( nLocation, nTextureStage );

	//		m_pDevice->SetSamplerState( nTextureStage, pShaderSampler->State );

	//		nTextureStage++;
	//	}
	//}
	////
	//// 设置 Shader 的 bool布尔变量
	//auto& ShaderVars_Bools = m_pShaderVars->GetBools();
	//if ( ShaderVars_Bools.empty() == false )
	//{
	//	for ( size_t i = 0, s = ShaderVars_Bools.size(); i < s; i++ )
	//	{
	//		GLuint nLocation = pShader->GetUniformLocation( ShaderVars_Bools[ i ].strName.c_str() );
	//		if ( nLocation == GLShader_UniformLocation_Null )
	//			continue;

	//		glUniform1i( nLocation, ShaderVars_Bools[ i ].Value == false ? 0 : 1 );
	//	}
	//}
}


void iberbar::RHI::OpenGL::CCommandContext::PrepareShaderVariables()
{
	const auto& Samplers = m_pShaderVariableTable->GetSamplers();
	CTexture* pTextureTemp = nullptr;
	for ( size_t i = 0, s = Samplers.size(); i < s; i++ )
	{
		pTextureTemp = (CTexture*)(RHI::ITexture*)Samplers[ i ].pTexture;

		glActiveTexture( GL_TEXTURE0 + i );
		glBindTexture( GL_TEXTURE_2D, pTextureTemp == nullptr ? 0 : pTextureTemp->GetTextureUint() );
		glUniform1i( Samplers[ i ].nLocation, i );

		if ( pTextureTemp != nullptr )
		{
			m_pDevice->SetSamplerState( (uint32)i, Samplers[ i ].State );
		}
	}

	// 填充普通变量
	const uint8* pVarsBuffer = m_pShaderVariableTable->GetBuffer();
	const auto& VarsList = m_pShaderVariableTable->GetVars();
	const UShaderVariableDeclarationNode* pVarDeclNode = nullptr;
	//ID3DXConstantTable* pD3DConstTableArray[ 2 ];
	//pD3DConstTableArray[ (int)UShaderType::Vertex ] = m_pShaderState->GetShader()->GetD3DVertexShaderConstTable();
	//pD3DConstTableArray[ (int)UShaderType::Pixel ] = m_pShaderState->GetShader()->GetD3DPixelShaderConstTable();
	for ( size_t i = 0, s = VarsList.size(); i < s; i++ )
	{
		pVarDeclNode = VarsList[ i ];
		SetUniform(
			pVarDeclNode->nVarType,
			pVarDeclNode->nLocation,
			pVarsBuffer + pVarDeclNode->nOffset,
			pVarDeclNode->nElementCount );
		//glUniform1i( pVarDeclNode->nLocation, ShaderVars_Bools[ i ].Value == false ? 0 : 1 );
		//pD3DConstTableArray[ (int)pVarDeclNode->nShaderType ]->SetValue(
		//	pD3DDevice,
		//	pVarDeclNode->D3DHandle,
		//	pVarsBuffer + pVarDeclNode->nOffset,
		//	pVarDeclNode->nTotalSize );
	}
}


void iberbar::RHI::OpenGL::CCommandContext::SetUniform( UShaderVariableType nVarType, GLuint nLocation, const void* pData, uint32 nElementCount )
{
	switch ( nVarType )
	{
		case UShaderVariableType::VT_Boolean:
			glUniform1iv( nLocation, nElementCount, (const int32*)pData );
			break;

		case UShaderVariableType::VT_Int:
			glUniform1iv( nLocation, nElementCount, (const int32*)pData );
			break;

		case UShaderVariableType::VT_Float:
			glUniform1fv( nLocation, nElementCount, (const float*)pData );
			break;

		case UShaderVariableType::VT_Vector4:
			glUniform4fv( nLocation, nElementCount, (const float*)pData );
			break;

		case UShaderVariableType::VT_Matrix44:
			glUniformMatrix4fv( nLocation, nElementCount, GL_FALSE, (const float*)pData );
			break;

		default:
			break;
	}
}


void iberbar::RHI::OpenGL::CCommandContext::CleanResources()
{
	UNKNOWN_SAFE_RELEASE_NULL( m_pVertexBuffer );
	UNKNOWN_SAFE_RELEASE_NULL( m_pIndexBuffer );
	UNKNOWN_SAFE_RELEASE_NULL( m_pShaderState );
	UNKNOWN_SAFE_RELEASE_NULL( m_pShaderVariableTable );
}
