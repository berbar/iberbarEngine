
#include <iberbar/RHI/D3D9/ShaderVariables.h>
#include <iberbar/RHI/D3D9/Device.h>




namespace iberbar
{
	namespace RHI
	{
		namespace D3D9
		{
			template < typename T, UShaderVariableType tVarType >
			FORCEINLINE void TShaderVariableTable_SetValue(
				const CShaderReflection* pDecl,
				const char* strName,
				const T& value,
				uint8* pBuffer )
			{
				const CShaderReflectionVariable* pVarDeclNode = pDecl->GetBuffer()->GetVariableByNameInternal( strName );
				if ( pVarDeclNode == nullptr )
					return;

				if ( pVarDeclNode->GetTypeInternal()->GetVarType() != tVarType )
					return;

				T* ptr = (T*)(pBuffer + pVarDeclNode->GetOffset() );
				*ptr = value;
			}


			template < typename T, UShaderVariableType tVarType, UShaderVariableClass tVarClass >
			FORCEINLINE void TShaderVariableTable_SetValue(
				const CShaderReflection* pDecl,
				const char* strName,
				const T& value,
				uint8* pBuffer )
			{
				const CShaderReflectionVariable* pVarDeclNode = pDecl->GetBuffer()->GetVariableByNameInternal( strName );
				if ( pVarDeclNode == nullptr )
					return;

				if ( pVarDeclNode->GetTypeInternal()->GetVarType() != tVarType )
					return;

				T* ptr = (T*)( pBuffer + pVarDeclNode->GetOffset() );
				*ptr = value;
			}


#ifdef _DEBUG
			template < typename T >
			FORCEINLINE void TShaderVariableTable_SetValue_Debug(
				const CShaderReflection* pDecl,
				const char* strName,
				uint8* pBuffer,
				std::unordered_map<std::string, UShaderVariant>& VarsDebug )
			{
				auto iter = VarsDebug.find( strName );
				if ( iter == VarsDebug.end() )
				{
					auto pDeclNode = pDecl->GetBuffer()->GetVariableByNameInternal( strName );
					if ( pDeclNode == nullptr )
						return;
					UShaderVariant VarDebug;
					VarDebug.VarReflection = pDeclNode;
					VarDebug.VarData._v_void = (T*)(pBuffer + pDeclNode->GetOffset());
					VarsDebug.insert( std::make_pair( strName, VarDebug ) );
				}
			}
#endif
		}
	}
}





iberbar::RHI::D3D9::CShaderVariableTable::CShaderVariableTable( CDevice* pDevice )
	: m_pDevice( pDevice )
	, m_pShader( nullptr )
	, m_Buffer()
	, m_Textures()
	, m_SamplerStates()
#ifdef _DEBUG
	, m_VarsDebug()
#endif
{
	assert( m_pDevice );
	m_pDevice->AddRef();
	memset( m_Buffer.GetPointer(), 0, m_Buffer.GetDataSize() );
}


iberbar::RHI::D3D9::CShaderVariableTable::~CShaderVariableTable()
{
	m_Buffer.Clear();
	UNKNOWN_SAFE_RELEASE_NULL( m_pShader );
	UNKNOWN_SAFE_RELEASE_NULL( m_pDevice );
}


void iberbar::RHI::D3D9::CShaderVariableTable::SetShader( IShader* pShader )
{
	if ( m_pShader != pShader )
	{
		UNKNOWN_SAFE_RELEASE_NULL( m_pShader );
		m_pShader = (CShader*)pShader;
		UNKNOWN_SAFE_ADDREF( m_pShader );

		if ( m_pShader != nullptr )
		{
			CShaderReflection* pReflection = m_pShader->GetReflectionInner();

			m_Buffer.Resize( pReflection->GetBuffer()->GetBufferSize(), false );

			//m_Samplers.clear();
			//m_Samplers.resize( 8 );
			m_Textures.clear();
			m_Textures.resize( 8 );
			m_SamplerStates.clear();
			m_SamplerStates.resize( 8 );
		}
	}
}


void iberbar::RHI::D3D9::CShaderVariableTable::SetBool( const char* strName, bool value )
{
	TShaderVariableTable_SetValue<uint32, UShaderVariableType::VT_Boolean>(
		m_pShader->GetReflectionInner(),
		strName,
		( value == false ) ? 0 : 1,
		m_Buffer.GetPointer() );

#ifdef _DEBUG
	TShaderVariableTable_SetValue_Debug<int32>(
		m_pShader->GetReflectionInner(),
		strName,
		m_Buffer.GetPointer(),
		m_VarsDebug );
#endif
}


void iberbar::RHI::D3D9::CShaderVariableTable::SetInt( const char* strName, int value )
{
	TShaderVariableTable_SetValue<int32, UShaderVariableType::VT_Int>(
		m_pShader->GetReflectionInner(),
		strName,
		value,
		m_Buffer.GetPointer() );

#ifdef _DEBUG
	TShaderVariableTable_SetValue_Debug<int32>(
		m_pShader->GetReflectionInner(),
		strName,
		m_Buffer.GetPointer(),
		m_VarsDebug );
#endif
}


void iberbar::RHI::D3D9::CShaderVariableTable::SetFloat( const char* strName, float value )
{
	TShaderVariableTable_SetValue<float, UShaderVariableType::VT_Float>(
		m_pShader->GetReflectionInner(),
		strName,
		value,
		m_Buffer.GetPointer() );

#ifdef _DEBUG
	TShaderVariableTable_SetValue_Debug<float>(
		m_pShader->GetReflectionInner(),
		strName,
		m_Buffer.GetPointer(),
		m_VarsDebug );
#endif
}


void iberbar::RHI::D3D9::CShaderVariableTable::SetVector( const char* strName, const DirectX::XMFLOAT4& value )
{
	TShaderVariableTable_SetValue<DirectX::XMFLOAT4, UShaderVariableType::VT_Void, UShaderVariableClass::SVC_Vector>(
		m_pShader->GetReflectionInner(),
		strName,
		value,
		m_Buffer.GetPointer() );

#ifdef _DEBUG
	TShaderVariableTable_SetValue_Debug<DirectX::XMFLOAT4>(
		m_pShader->GetReflectionInner(),
		strName,
		m_Buffer.GetPointer(),
		m_VarsDebug );
#endif
}


void iberbar::RHI::D3D9::CShaderVariableTable::SetMatrix( const char* strName, const DirectX::XMFLOAT4X4& value )
{
	TShaderVariableTable_SetValue<DirectX::XMFLOAT4X4, UShaderVariableType::VT_Void, UShaderVariableClass::SVC_Matrix>(
		m_pShader->GetReflectionInner(),
		strName,
		value,
		m_Buffer.GetPointer() );

#ifdef _DEBUG
	TShaderVariableTable_SetValue_Debug<DirectX::XMFLOAT4X4>(
		m_pShader->GetReflectionInner(),
		strName,
		m_Buffer.GetPointer(),
		m_VarsDebug );
#endif
}


void iberbar::RHI::D3D9::CShaderVariableTable::SetTexture( const char* strName, ITexture* pTexture )
{
	//const CShaderReflectionVariable* pReflectionVar = m_pShader->GetReflectionInner()->GetBuffer()->GetVariableByNameInternal( strName );
	//if ( pReflectionVar == nullptr )
	//	return;
	//if ( pReflectionVar->GetTypeInternal()->GetVarType() != UShaderVariableType::VT_Texture )
	//	return;
	//if ( pReflectionVar->GetRegisterIndex() >= (uint32)m_Textures.size() )
	//	return;

	//m_Textures[ pReflectionVar->GetRegisterIndex() ] = (CTexture*)pTexture;
	m_Textures[ 0 ] = (CTexture*)pTexture;
}


void iberbar::RHI::D3D9::CShaderVariableTable::SetSamplerState( const char* strName, const UTextureSamplerState& SamplerDesc )
{
	const CShaderReflectionVariable* pReflectionVar = m_pShader->GetReflectionInner()->GetBuffer()->GetVariableByNameInternal( strName );
	if ( pReflectionVar == nullptr )
		return;
	if ( pReflectionVar->GetTypeInternal()->GetVarType() != UShaderVariableType::VT_Sampler2D )
		return;
	if ( pReflectionVar->GetRegisterIndex() >= (uint32)m_SamplerStates.size() )
		return;

	TSmartRefPtr<CSamplerState> pSamplerState;
	CResult cRet = m_pDevice->CreateSamplerState( (ISamplerState**)&pSamplerState, SamplerDesc );
	if ( cRet.IsOK() == false )
		return;
	m_SamplerStates[ pReflectionVar->GetRegisterIndex() ] = pSamplerState;
}


bool iberbar::RHI::D3D9::CShaderVariableTable::Compare( IShaderVariableTable* pVariableTable )
{
	CShaderVariableTable* pOther = (CShaderVariableTable*)pVariableTable;
	//if ( m_Samplers.size() != pOther->m_Samplers.size() )
	//	return false;
	if ( m_Buffer.GetDataSize() != pOther->m_Buffer.GetDataSize() )
		return false;
	for ( size_t i = 0, s = m_Textures.size(); i < s; i++ )
	{
		if ( m_Textures[ i ] != pOther->m_Textures[ i ] )
			return false;
	}
	for ( size_t i = 0, s = m_SamplerStates.size(); i < s; i++ )
	{
		if ( m_SamplerStates[ i ] != pOther->m_SamplerStates[ i ] )
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

