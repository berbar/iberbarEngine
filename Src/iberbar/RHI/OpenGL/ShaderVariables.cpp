

#include <iberbar/RHI/OpenGL/ShaderVariables.h>
#include <iberbar/RHI/OpenGL/ShaderProgram.h>
#include <iberbar/RHI/OpenGL/ShaderState.h>




namespace iberbar
{
	namespace RHI
	{
		namespace OpenGL
		{
			template < typename T, UShaderVariableType tVarType >
			FORCEINLINE void TShaderVariableTable_SetValue(
				const CShaderVariableDeclaration* pDecl,
				const char* strName,
				const T& value,
				uint8* pBuffer,
				std::vector<const UShaderVariableDeclarationNode*>& pVarList )
			{
				const UShaderVariableDeclarationNode* pVarDeclNode = pDecl->GetNode( strName );
				if ( pVarDeclNode == nullptr )
					return;

				if ( pVarDeclNode->nVarType != tVarType )
					return;

				T* ptr = (T*)(pBuffer + pVarDeclNode->nOffset);
				*ptr = value;

				bool bInsert = true;
				for ( size_t i = 0, s = pVarList.size(); i < s; i++ )
				{
					if ( strcmp( pVarList[ i ]->strName.c_str(), strName ) == 0 )
					{
						bInsert = false;
						break;
					}
				}
				if ( bInsert == true )
					pVarList.push_back( pVarDeclNode );
			}
		}
	}
}



iberbar::RHI::OpenGL::CShaderVariableTable::CShaderVariableTable()
	: m_Buffer()
	, m_pShaderState( nullptr )
	, m_Vars()
	, m_Samplers()
{
}


iberbar::RHI::OpenGL::CShaderVariableTable::~CShaderVariableTable()
{
	m_Buffer.Clear();
	UNKNOWN_SAFE_RELEASE_NULL( m_pShaderState );
}


void iberbar::RHI::OpenGL::CShaderVariableTable::SetShaderState( IShaderState* pShaderState )
{
	if ( m_pShaderState != pShaderState )
	{
		UNKNOWN_SAFE_RELEASE_NULL( m_pShaderState );
		m_pShaderState = (CShaderState*)pShaderState;
		UNKNOWN_SAFE_ADDREF( m_pShaderState );

		if ( m_pShaderState != nullptr )
		{
			m_Buffer.Resize( m_pShaderState->GetShaderVariableDeclaration()->GetTotalByteSize(), false );

			m_Samplers.clear();
			m_Samplers.resize( m_pShaderState->GetShaderVariableDeclaration()->GetSamplerStateMax() );
		}
	}
}


void iberbar::RHI::OpenGL::CShaderVariableTable::SetBool( UShaderType nShaderType, const char* strName, bool value )
{
	TShaderVariableTable_SetValue<uint32, UShaderVariableType::VT_Boolean>(
		m_pShaderState->GetShaderVariableDeclaration(),
		strName,
		(value == false) ? 0 : 1,
		m_Buffer.GetPointer(),
		m_Vars );
}


void iberbar::RHI::OpenGL::CShaderVariableTable::SetInt( UShaderType nShaderType, const char* strName, int value )
{
	TShaderVariableTable_SetValue<int32, UShaderVariableType::VT_Int>(
		m_pShaderState->GetShaderVariableDeclaration(),
		strName,
		value,
		m_Buffer.GetPointer(),
		m_Vars );
}


void iberbar::RHI::OpenGL::CShaderVariableTable::SetFloat( UShaderType nShaderType, const char* strName, float value )
{
	TShaderVariableTable_SetValue<float, UShaderVariableType::VT_Float>(
		m_pShaderState->GetShaderVariableDeclaration(),
		strName,
		value,
		m_Buffer.GetPointer(),
		m_Vars );
}


void iberbar::RHI::OpenGL::CShaderVariableTable::SetVector( UShaderType nShaderType, const char* strName, const DirectX::XMFLOAT4& value )
{
	TShaderVariableTable_SetValue<DirectX::XMFLOAT4, UShaderVariableType::VT_Vector4>(
		m_pShaderState->GetShaderVariableDeclaration(),
		strName,
		value,
		m_Buffer.GetPointer(),
		m_Vars );
}


void iberbar::RHI::OpenGL::CShaderVariableTable::SetMatrix( UShaderType nShaderType, const char* strName, const DirectX::XMFLOAT4X4& value )
{
	TShaderVariableTable_SetValue<DirectX::XMFLOAT4X4, UShaderVariableType::VT_Matrix44>(
		m_pShaderState->GetShaderVariableDeclaration(),
		strName,
		value,
		m_Buffer.GetPointer(),
		m_Vars );
}


void iberbar::RHI::OpenGL::CShaderVariableTable::SetSampler( const char* strName, RHI::ITexture* pTexture, UTextureSamplerState SamplerState )
{
	const UShaderVariableDeclarationNode* pVarDeclNode = m_pShaderState->GetShaderVariableDeclaration()->GetNode( strName );
	if ( pVarDeclNode == nullptr )
		return;
	if ( pVarDeclNode->nVarType != UShaderVariableType::VT_Sampler2D )
		return;

	auto& SamplerTemp = m_Samplers[ pVarDeclNode->nTextureStage ];
	SamplerTemp.nLocation = pVarDeclNode->nLocation;
	SamplerTemp.pTexture = pTexture;
	SamplerTemp.State = SamplerState;
}


bool iberbar::RHI::OpenGL::CShaderVariableTable::Compare( IShaderVariableTable* pVariableTable )
{
	CShaderVariableTable* pOther = (CShaderVariableTable*)pVariableTable;
	if ( m_Samplers.size() != pOther->m_Samplers.size() )
		return false;
	if ( m_Vars.size() != pOther->m_Vars.size() )
		return false;
	for ( size_t i = 0, s = m_Samplers.size(); i < s; i++ )
	{
		if ( m_Samplers[ i ] != pOther->m_Samplers[ i ] )
			return false;
	}
	if ( memcmp( m_Buffer.GetPointer(), pOther->m_Buffer.GetPointer(), m_Buffer.GetDataSize() ) != 0 )
		return false;
	return true;
}

