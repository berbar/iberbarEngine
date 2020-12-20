
#include <iberbar/RHI/D3D9/ShaderVariables.h>
#include <iberbar/RHI/D3D9/Shader.h>
#include <iberbar/RHI/D3D9/ShaderState.h>




namespace iberbar
{
	namespace RHI
	{
		namespace D3D9
		{
			template < typename T, UShaderVariableType tVarType >
			FORCEINLINE void TShaderVariableTable_SetValue(
				const CShaderVariableDeclaration* pDecl,
				UShaderType nShaderType,
				const char* strName,
				const T& value,
				uint8* pBuffer,
				std::vector<const UShaderVariableDeclarationNode*>& pVarList )
			{
				const UShaderVariableDeclarationNode* pVarDeclNode = pDecl->GetNode( nShaderType, strName );
				if ( pVarDeclNode == nullptr )
					return;

				if ( pVarDeclNode->nVarType != tVarType )
					return;

				T* ptr = (T*)(pBuffer + pVarDeclNode->nOffset);
				*ptr = value;

				bool bInsert = true;
				for ( size_t i = 0, s = pVarList.size(); i < s; i++ )
				{
					if ( strcmp( pVarList[ i ]->D3DDesc.Name, strName ) == 0 )
					{
						bInsert = false;
						break;
					}
				}
				if ( bInsert == true )
					pVarList.push_back( pVarDeclNode );
			}


#ifdef _DEBUG
			template < typename T >
			FORCEINLINE void TShaderVariableTable_SetValue_Debug(
				const CShaderVariableDeclaration* pDecl,
				UShaderType nShaderType,
				const char* strName,
				uint8* pBuffer,
				std::unordered_map<std::string, UShaderVariant>& VarsDebug )
			{
				auto iter = VarsDebug.find( strName );
				if ( iter == VarsDebug.end() )
				{
					auto pDeclNode = pDecl->GetNode( nShaderType, strName );
					if ( pDeclNode == nullptr )
						return;
					UShaderVariant VarDebug;
					VarDebug.pDeclNode = pDeclNode;
					VarDebug.VarData._v_void = (T*)(pBuffer + pDeclNode->nOffset);
					VarsDebug.insert( std::make_pair( strName, VarDebug ) );
				}
			}
#endif
		}
	}
}



iberbar::RHI::D3D9::CShaderVariableTable::CShaderVariableTable()
	: m_Buffer()
	, m_pShaderState( nullptr )
	, m_Vars()
	, m_Samplers()
#ifdef _DEBUG
	, m_VarsDebug()
#endif
{
	memset( m_Buffer.GetPointer(), 0, m_Buffer.GetDataSize() );
}


iberbar::RHI::D3D9::CShaderVariableTable::~CShaderVariableTable()
{
	m_Buffer.Clear();
	UNKNOWN_SAFE_RELEASE_NULL( m_pShaderState );
}


void iberbar::RHI::D3D9::CShaderVariableTable::SetShaderState( IShaderState* pShaderState )
{
	if ( m_pShaderState != pShaderState )
	{
		UNKNOWN_SAFE_RELEASE_NULL( m_pShaderState );
		m_pShaderState = (CShaderState*)pShaderState;
		UNKNOWN_SAFE_ADDREF( m_pShaderState );

		if ( m_pShaderState != nullptr )
		{
			const CShaderVariableDeclaration* pDecl = m_pShaderState->GetShaderVariableDeclaration();

			m_Buffer.Resize( pDecl->GetTotalByteSize(), false );

			m_Samplers.clear();
			m_Samplers.resize( pDecl->GetSamplerStageMax() + 1 );
		}
	}
}


void iberbar::RHI::D3D9::CShaderVariableTable::SetBool( UShaderType nShaderType, const char* strName, bool value )
{
	TShaderVariableTable_SetValue<uint32, UShaderVariableType::VT_Boolean>(
		m_pShaderState->GetShaderVariableDeclaration(),
		nShaderType,
		strName,
		( value == false ) ? 0 : 1,
		m_Buffer.GetPointer(),
		m_Vars );

#ifdef _DEBUG
	TShaderVariableTable_SetValue_Debug<int32>(
		m_pShaderState->GetShaderVariableDeclaration(),
		nShaderType,
		strName,
		m_Buffer.GetPointer(),
		m_VarsDebug );
#endif
}


void iberbar::RHI::D3D9::CShaderVariableTable::SetInt( UShaderType nShaderType, const char* strName, int value )
{
	TShaderVariableTable_SetValue<int32, UShaderVariableType::VT_Int>(
		m_pShaderState->GetShaderVariableDeclaration(),
		nShaderType,
		strName,
		value,
		m_Buffer.GetPointer(),
		m_Vars );

#ifdef _DEBUG
	TShaderVariableTable_SetValue_Debug<int32>(
		m_pShaderState->GetShaderVariableDeclaration(),
		nShaderType,
		strName,
		m_Buffer.GetPointer(),
		m_VarsDebug );
#endif
}


void iberbar::RHI::D3D9::CShaderVariableTable::SetFloat( UShaderType nShaderType, const char* strName, float value )
{
	TShaderVariableTable_SetValue<float, UShaderVariableType::VT_Float>(
		m_pShaderState->GetShaderVariableDeclaration(),
		nShaderType,
		strName,
		value,
		m_Buffer.GetPointer(),
		m_Vars );

#ifdef _DEBUG
	TShaderVariableTable_SetValue_Debug<float>(
		m_pShaderState->GetShaderVariableDeclaration(),
		nShaderType,
		strName,
		m_Buffer.GetPointer(),
		m_VarsDebug );
#endif
}


void iberbar::RHI::D3D9::CShaderVariableTable::SetVector( UShaderType nShaderType, const char* strName, const DirectX::XMFLOAT4& value )
{
	TShaderVariableTable_SetValue<DirectX::XMFLOAT4, UShaderVariableType::VT_Vector4>(
		m_pShaderState->GetShaderVariableDeclaration(),
		nShaderType,
		strName,
		value,
		m_Buffer.GetPointer(),
		m_Vars );

#ifdef _DEBUG
	TShaderVariableTable_SetValue_Debug<DirectX::XMFLOAT4>(
		m_pShaderState->GetShaderVariableDeclaration(),
		nShaderType,
		strName,
		m_Buffer.GetPointer(),
		m_VarsDebug );
#endif
}


void iberbar::RHI::D3D9::CShaderVariableTable::SetMatrix( UShaderType nShaderType, const char* strName, const DirectX::XMFLOAT4X4& value )
{
	TShaderVariableTable_SetValue<DirectX::XMFLOAT4X4, UShaderVariableType::VT_Matrix44>(
		m_pShaderState->GetShaderVariableDeclaration(),
		nShaderType,
		strName,
		value,
		m_Buffer.GetPointer(),
		m_Vars );

#ifdef _DEBUG
	TShaderVariableTable_SetValue_Debug<DirectX::XMFLOAT4X4>(
		m_pShaderState->GetShaderVariableDeclaration(),
		nShaderType,
		strName,
		m_Buffer.GetPointer(),
		m_VarsDebug );
#endif
}


void iberbar::RHI::D3D9::CShaderVariableTable::SetSampler( const char* strName, RHI::ITexture* pTexture, UTextureSamplerState SamplerState )
{
	const UShaderVariableDeclarationNode* pDeclNode = m_pShaderState->GetShaderVariableDeclaration()->GetNode( UShaderType::Pixel, strName );
	if ( pDeclNode == nullptr )
		return;
	if ( pDeclNode->nVarType != UShaderVariableType::VT_Sampler2D )
		return;

	auto& SamplerTemp = m_Samplers[ pDeclNode->D3DDesc.RegisterIndex ];
	SamplerTemp.pTexture = pTexture;
	SamplerTemp.State = SamplerState;
}


bool iberbar::RHI::D3D9::CShaderVariableTable::Compare( IShaderVariableTable* pVariableTable )
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


//void iberbar::RHI::D3D9::CShaderVariableTable::ExecuteVars()
//{
//	//const uint8* pVarsBuffer = m_pShaderVariableTable->GetBuffer();
//	//const auto& VarsList = m_pShaderVariableTable->GetVars();
//	const RHI::D3D9::UShaderVariableDeclarationNode* pVarDeclNode = nullptr;
//	ID3DXConstantTable* pD3DConstTable = nullptr;
//	for ( size_t i = 0, s = m_Vars.size(); i < s; i++ )
//	{
//		pVarDeclNode = m_Vars[ i ];
//		
//		pD3DConstTable->SetValue( )
//		pD3DConstTable->SetBool( m_pDevice->GetD3DDevice(), pHandle, ShaderVars_Bools[ i ].Value == false ? FALSE : TRUE );
//	}
//}

