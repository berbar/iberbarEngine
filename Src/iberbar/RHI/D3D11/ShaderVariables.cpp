
#include <iberbar/RHI/D3D11/ShaderVariables.h>
#include <iberbar/RHI/D3D11/ShaderReflection.h>
#include <iberbar/RHI/D3D11/Device.h>





namespace iberbar
{
	namespace RHI
	{
		namespace D3D11
		{
			template < typename T, UShaderVariableType tVarType >
			FORCEINLINE void TShaderVariableTable_SetValue(
				const CShaderReflection* pDecl,
				const char* strName,
				const T& value,
				uint8* pBuffer )
			{
				const CShaderReflectionVariable* pVarDeclNode = pDecl->GetVariableByName( strName );
				if ( pVarDeclNode == nullptr )
					return;

				if ( pVarDeclNode->GetTypeInternal()->GetVariableType() != tVarType )
					return;

				T* ptr = (T*)( pBuffer + pVarDeclNode->GetOffset() );
				*ptr = value;
			}


			template < typename T, UShaderVariableType tVarType, UShaderVariableClass tVarClass >
			FORCEINLINE void TShaderVariableTable_SetValue(
				const CShaderReflection* pDecl,
				const char* strName,
				const T& value,
				uint8* pBuffer )
			{
				const CShaderReflectionVariable* pVarDeclNode = pDecl->GetVariableByName( strName );
				if ( pVarDeclNode == nullptr )
					return;

				if ( pVarDeclNode->GetTypeInternal()->GetVariableType() != tVarType )
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
					auto pDeclNode = pDecl->GetVariableByName( strName );
					if ( pDeclNode == nullptr )
						return;
					UShaderVariant VarDebug;
					VarDebug.VarReflection = pDeclNode;
					VarDebug.VarData._v_void = (T*)( pBuffer + pDeclNode->GetOffset() );
					VarsDebug.insert( std::make_pair( strName, VarDebug ) );
				}
			}
#endif


//#ifdef _DEBUG
//			template < typename T >
//			FORCEINLINE void TShaderVariableTable_SetValue_Debug(
//				const CShaderReflection* pDecl,
//				const char* strName,
//				uint8* pBuffer,
//				std::unordered_map<std::string, UShaderVariant>& VarsDebug )
//			{
//				auto iter = VarsDebug.find( strName );
//				if ( iter == VarsDebug.end() )
//				{
//					auto pDeclNode = pDecl->GetVariableByName( strName );
//					if ( pDeclNode == nullptr )
//						return;
//					UShaderVariant VarDebug;
//					VarDebug.VarReflection = pDeclNode;
//					VarDebug.VarData._v_void = (T*)( pBuffer + pDeclNode->GetOffset() );
//					VarsDebug.insert( std::make_pair( strName, VarDebug ) );
//				}
//			}
//#endif
		}
	}
}



iberbar::RHI::D3D11::CShaderVariableTable::CShaderVariableTable( CDevice* pDevice )
	: m_pDevice( pDevice )
	, m_pShader( nullptr )
	, m_CommonMemory()
	, m_Buffers()
	, m_Textures()
	, m_SamplerStates()
//#ifdef _DEBUG
//	, m_VarsDebug()
//#endif
{
	assert( m_pDevice );
	m_pDevice->AddRef();
}


iberbar::RHI::D3D11::CShaderVariableTable::~CShaderVariableTable()
{
	m_Buffers.clear();
	UNKNOWN_SAFE_RELEASE_NULL( m_pShader );
}


void iberbar::RHI::D3D11::CShaderVariableTable::SetShader( IShader* pShader )
{
	if ( m_pShader != pShader )
	{
		UNKNOWN_SAFE_RELEASE_NULL( m_pShader );
		m_CommonMemory.clear();
		m_Buffers.clear();
		m_SamplerStates.clear();
		m_Textures.clear();

		m_pShader = (CShader*)pShader;
		UNKNOWN_SAFE_ADDREF( m_pShader );
		if ( m_pShader != nullptr )
		{
			CShaderReflection* pReflection = m_pShader->GetReflectionInternal();

			uint32 nBufferSizeTotal = pReflection->GetBufferSizeTotal();
			int nBufferCount = pReflection->GetBufferCountInternal();
			m_CommonMemory.resize( nBufferSizeTotal );
			m_Buffers.resize( nBufferCount );

			const CShaderReflectionBuffer* pShaderReflectionBuffer = nullptr;
			for ( int i = 0; i < nBufferCount; i++ )
			{
				pShaderReflectionBuffer = pReflection->GetBufferByIndexInternal( i );
				m_Buffers[ i ] = ( &m_CommonMemory.front() ) + pShaderReflectionBuffer->GetOffset();
			}

			m_SamplerStates.resize( 8 );
			m_Textures.resize( 8 );
		}
	}
}


void iberbar::RHI::D3D11::CShaderVariableTable::SetBool( const char* strName, bool value )
{
	TShaderVariableTable_SetValue<uint32, UShaderVariableType::VT_Boolean>(
		m_pShader->GetReflectionInternal(),
		strName,
		( value == false ) ? 0 : 1,
		&m_CommonMemory.front() );

#ifdef _DEBUG
	TShaderVariableTable_SetValue_Debug<int32>(
		m_pShader->GetReflectionInternal(),
		strName,
		&m_CommonMemory.front(),
		m_VarsDebug );
#endif
}


void iberbar::RHI::D3D11::CShaderVariableTable::SetInt( const char* strName, int value )
{
	TShaderVariableTable_SetValue<int32, UShaderVariableType::VT_Int>(
		m_pShader->GetReflectionInternal(),
		strName,
		value,
		&m_CommonMemory.front() );

#ifdef _DEBUG
	TShaderVariableTable_SetValue_Debug<int32>(
		m_pShader->GetReflectionInternal(),
		strName,
		&m_CommonMemory.front(),
		m_VarsDebug );
#endif
}


void iberbar::RHI::D3D11::CShaderVariableTable::SetFloat( const char* strName, float value )
{
	TShaderVariableTable_SetValue<float, UShaderVariableType::VT_Float>(
		m_pShader->GetReflectionInternal(),
		strName,
		value,
		&m_CommonMemory.front() );

#ifdef _DEBUG
	TShaderVariableTable_SetValue_Debug<int32>(
		m_pShader->GetReflectionInternal(),
		strName,
		&m_CommonMemory.front(),
		m_VarsDebug );
#endif
}


void iberbar::RHI::D3D11::CShaderVariableTable::SetVector( const char* strName, const DirectX::XMFLOAT4& value )
{
	TShaderVariableTable_SetValue<DirectX::XMFLOAT4, UShaderVariableType::VT_Void, UShaderVariableClass::SVC_Vector>(
		m_pShader->GetReflectionInternal(),
		strName,
		value,
		&m_CommonMemory.front() );

#ifdef _DEBUG
	TShaderVariableTable_SetValue_Debug<int32>(
		m_pShader->GetReflectionInternal(),
		strName,
		&m_CommonMemory.front(),
		m_VarsDebug );
#endif
}


void iberbar::RHI::D3D11::CShaderVariableTable::SetMatrix( const char* strName, const DirectX::XMFLOAT4X4& value )
{
	TShaderVariableTable_SetValue<DirectX::XMFLOAT4X4, UShaderVariableType::VT_Float, UShaderVariableClass::SVC_Matrix>(
		m_pShader->GetReflectionInternal(),
		strName,
		value,
		&m_CommonMemory.front() );

#ifdef _DEBUG
	TShaderVariableTable_SetValue_Debug<int32>(
		m_pShader->GetReflectionInternal(),
		strName,
		&m_CommonMemory.front(),
		m_VarsDebug );
#endif
}


void iberbar::RHI::D3D11::CShaderVariableTable::SetTexture( const char* strName, ITexture* pTexture )
{
	const CShaderReflectionBindResource* pReflectionVar = m_pShader->GetReflectionInternal()->GetTextureByName( strName );
	if ( pReflectionVar == nullptr )
		return;

	m_Textures[ pReflectionVar->GetBindPoint() ] = (CTexture*)pTexture;
}


void iberbar::RHI::D3D11::CShaderVariableTable::SetSamplerState( const char* strName, const UTextureSamplerState& SamplerDesc )
{
	const CShaderReflectionBindResource* pReflectionVar = m_pShader->GetReflectionInternal()->GetSamplerByName( strName );
	if ( pReflectionVar == nullptr )
		return;

	
	TSmartRefPtr<CSamplerState> pSampleState;
	CResult cRet = m_pDevice->CreateSamplerState( (ISamplerState**)(&pSampleState), SamplerDesc  );
	if ( cRet.IsOK() == false )
	{
		return;
	}
	m_SamplerStates[ pReflectionVar->GetBindPoint() ] = pSampleState;
}


bool iberbar::RHI::D3D11::CShaderVariableTable::Compare( IShaderVariableTable* pVariableTable )
{
	CShaderVariableTable* pOther = (CShaderVariableTable*)pVariableTable;
	//if ( m_Samplers.size() != pOther->m_Samplers.size() )
	//	return false;
	if ( m_CommonMemory.size() != pOther->m_CommonMemory.size() )
		return false;
	for ( size_t i = 0, s = m_SamplerStates.size(); i < s; i++ )
	{
		if ( m_SamplerStates[ i ] != pOther->m_SamplerStates[ i ] )
			return false;
	}
	for ( size_t i = 0, s = m_Textures.size(); i < s; i++ )
	{
		if ( m_Textures[ i ] != pOther->m_Textures[ i ] )
			return false;
	}
	if ( memcmp( &m_CommonMemory.front(), &pOther->m_CommonMemory.front(), m_CommonMemory.size() ) != 0 )
		return false;
	return true;
}
