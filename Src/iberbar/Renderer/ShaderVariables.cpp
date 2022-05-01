
#include <iberbar/Renderer/ShaderVariables.h>
#include <iberbar/RHI/Shader.h>
#include <iberbar/RHI/ShaderReflection.h>
#include <iberbar/RHI/Texture.h>
#include <iberbar/RHI/RenderState.h>
#include <iberbar/RHI/Device.h>





namespace iberbar
{
	namespace Renderer
	{

	/*		template < typename T, RHI::UShaderVariableType tVarType >
			FORCEINLINE bool TShaderVariableTable_SetValue(
				const RHI::IShaderReflection* pDecl,
				const char* strName,
				const T& value,
				uint8* pBuffer )
			{
				const RHI::IShaderReflectionVariable* pVarDeclNode = pDecl->GetVariableByName( strName );
				if ( pVarDeclNode == nullptr )
					return false;

				if ( pVarDeclNode->GetType()->GetVariableType() != tVarType )
					return false;

				T* ptr = (T*)(pBuffer + pVarDeclNode->GetOffset());
				*ptr = value;

				return true;
			}*/


#ifdef _DEBUG
		template < typename T >
		FORCEINLINE void TShaderVariableTable_SetDebugVariables(
			const RHI::IShaderReflectionVariable* pReflectionVariable,
			const RHI::IShaderReflectionType* pReflectionType,
			const char* strName,
			const T* pValues,
			uint32 nCount,
			std::unordered_map<std::string, IShaderVariableVariant*>& DebugVars )
		{
			TShaderVariableVariant<T>* pDebugContainer = nullptr;
			auto DebugIter = DebugVars.find( strName );
			if ( DebugIter == DebugVars.end() )
			{
				DebugVars.insert( std::make_pair( strName, pDebugContainer = new TShaderVariableVariant<T>( pReflectionVariable, pReflectionType->GetElementCount() ) ) );
			}
			else
			{
				//pDebugContainer = dynamic_cast<TShaderVariableVariant<T>*>(DebugIter->second);
				pDebugContainer = (TShaderVariableVariant<T>*)DebugIter->second;
			}
			if ( pDebugContainer != nullptr )
			{
				pDebugContainer->Set( pValues, nCount );
			}
		}
#endif


			template < typename T, RHI::UShaderVariableType tVarType >
			FORCEINLINE bool TShaderVariableTable_SetValueArray(
				const RHI::IShaderReflection* pReflection,
				const char* strName,
				const T* pValues,
				uint32 nCount,
#ifdef _DEBUG
				uint8* pBuffer,
				std::unordered_map<std::string, IShaderVariableVariant*>& DebugVars )
#else
				uint8* pBuffer )
#endif
			{
				const RHI::IShaderReflectionVariable* pReflectionVariable = pReflection->GetVariableByName( strName );
				if ( pReflectionVariable == nullptr )
					return false;

				const RHI::IShaderReflectionType* pReflectionType = pReflectionVariable->GetType();
				if ( pReflectionType->GetVariableType() != tVarType || pReflectionType->GetVariableClass() != RHI::UShaderVariableClass::SVC_Scalar )
					return false;

				uint32 nCountSafe = tMin( nCount, pReflectionType->GetElementCount() );
				uint32 nSize = nCountSafe * sizeof( T );
				memcpy_s( pBuffer + pReflectionVariable->GetOffset(), nSize, pValues, nSize );

#ifdef _DEBUG
				TShaderVariableTable_SetDebugVariables<T>( pReflectionVariable, pReflectionType, strName, pValues, nCountSafe, DebugVars );
#endif
				return true;
			}


			template < typename T, RHI::UShaderVariableType tVarType, uint32 tRow, uint32 tCols >
			FORCEINLINE bool TShaderVariableTable_SetMatrixArray(
				const RHI::IShaderReflection* pReflection,
				const char* strName,
				const T* pValues,
				uint32 nCount,
#ifdef _DEBUG
				uint8* pBuffer,
				std::unordered_map<std::string, IShaderVariableVariant*>& DebugVars )
#else
				uint8* pBuffer )
#endif
			{
				const RHI::IShaderReflectionVariable* pReflectionVariable = pReflection->GetVariableByName( strName );
				if ( pReflectionVariable == nullptr )
					return false;

				const RHI::IShaderReflectionType* pReflectionType = pReflectionVariable->GetType();
				if ( pReflectionType->GetVariableType() != tVarType )
					return false;
				if ( pReflectionType->GetVariableClass() != RHI::UShaderVariableClass::SVC_Vector &&
					pReflectionType->GetVariableClass() != RHI::UShaderVariableClass::SVC_Matrix )
					return false;
				if ( pReflectionType->GetRowCount() != tRow || pReflectionType->GetColumnCount() != tCols )
					return false;

				uint32 nCountSafe = tMin( nCount, pReflectionType->GetElementCount() );
				uint32 nSize = nCountSafe * sizeof( T );
				memcpy_s( pBuffer + pReflectionVariable->GetOffset(), nSize, pValues, nSize );

#ifdef _DEBUG
				TShaderVariableTable_SetDebugVariables<T>( pReflectionVariable, pReflectionType, strName, pValues, nCountSafe, DebugVars );
#endif
				return true;
			}

//
//			template < typename T, RHI::UShaderVariableType tVarType, RHI::UShaderVariableClass tVarClass >
//			FORCEINLINE bool TShaderVariableTable_SetValue(
//				const RHI::IShaderReflection* pDecl,
//				const char* strName,
//				const T& value,
//				uint8* pBuffer )
//			{
//				const RHI::IShaderReflectionVariable* pVarDeclNode = pDecl->GetVariableByName( strName );
//				if ( pVarDeclNode == nullptr )
//					return false;
//
//				RHI::UShaderReflectionTypeDesc TypeDesc;
//				pVarDeclNode->GetType()->GetDesc( &TypeDesc );
//				if ( TypeDesc.nVarType != tVarType )
//					return false;
//
//				T* ptr = (T*)(pBuffer + TypeDesc.nOffset);
//				*ptr = value;
//
//				return true;
//			}
//
//
//#ifdef _DEBUG
//			template < typename T >
//			FORCEINLINE void TShaderVariableTable_SetValue_Debug(
//				const RHI::IShaderReflection* pDecl,
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
//					VarDebug.VarData._v_void = (T*)(pBuffer + pDeclNode->GetOffset());
//					VarsDebug.insert( std::make_pair( strName, VarDebug ) );
//				}
//			}
//#endif


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





iberbar::Renderer::CShaderVariableTable::CShaderVariableTable()
	: m_pShader( nullptr )
	, m_pShaderReflection( nullptr )
	, m_pCommonMemory( nullptr )
	, m_Textures()
	, m_SamplerStates()
{
}


iberbar::Renderer::CShaderVariableTable::~CShaderVariableTable()
{
	Clear();
}


void iberbar::Renderer::CShaderVariableTable::SetShader( RHI::IShader* pShader )
{
	if ( pShader == nullptr )
	{
		Clear();
		return;
	}

	if ( m_pShader != pShader )
	{
		Clear();

		m_pShader = pShader;
		m_pShader->AddRef();

		m_pShaderReflection = pShader->GetReflection();

		m_Textures.resize( m_pShaderReflection->GetTextureCountTotal() );
		m_SamplerStates.resize( m_pShaderReflection->GetSamplerStateCountTotal() );

		uint32 nBufferSizeTotal = m_pShaderReflection->GetBufferSizeTotal();
		int nBufferCount = m_pShaderReflection->GetBufferCount();
		if ( nBufferSizeTotal > 0 && nBufferCount > 0 )
		{
			m_pCommonMemory = new uint8[ nBufferSizeTotal ];
			m_nCommonMemorySize = nBufferSizeTotal;
		}
	}
}


bool iberbar::Renderer::CShaderVariableTable::SetInt( const char* strName, int32 Value )
{
	if ( m_pCommonMemory == nullptr )
		return false;

	return TShaderVariableTable_SetValueArray< int32, RHI::UShaderVariableType::VT_Int>(
		m_pShaderReflection,
		strName,
		&Value,
		1,
#ifdef _DEBUG
		m_pCommonMemory,
		m_VarsDebug );
#else
		m_pCommonMemory );
#endif
}


bool iberbar::Renderer::CShaderVariableTable::SetIntArray( const char* strName, const int32* pValue, uint32 nCount )
{
	if ( m_pCommonMemory == nullptr )
		return false;

	return TShaderVariableTable_SetValueArray< int32, RHI::UShaderVariableType::VT_Int>(
		m_pShaderReflection,
		strName,
		pValue,
		nCount,
#ifdef _DEBUG
		m_pCommonMemory,
		m_VarsDebug );
#else
		m_pCommonMemory );
#endif
}


bool iberbar::Renderer::CShaderVariableTable::SetFloat( const char* strName, float Value )
{
	if ( m_pCommonMemory == nullptr )
		return false;

	return TShaderVariableTable_SetValueArray< float, RHI::UShaderVariableType::VT_Float>(
		m_pShaderReflection,
		strName,
		&Value,
		1,
#ifdef _DEBUG
		m_pCommonMemory,
		m_VarsDebug );
#else
		m_pCommonMemory );
#endif
}


bool iberbar::Renderer::CShaderVariableTable::SetFloatArray( const char* strName, const float* pValues, uint32 nCount )
{
	if ( m_pCommonMemory == nullptr )
		return false;

	return TShaderVariableTable_SetValueArray< float, RHI::UShaderVariableType::VT_Float>(
		m_pShaderReflection,
		strName,
		pValues,
		nCount,
#ifdef _DEBUG
		m_pCommonMemory,
		m_VarsDebug );
#else
		m_pCommonMemory );
#endif
}


bool iberbar::Renderer::CShaderVariableTable::SetVector( const char* strName, const DirectX::XMFLOAT4& Value )
{
	if ( m_pCommonMemory == nullptr )
		return false;

	return TShaderVariableTable_SetMatrixArray< DirectX::XMFLOAT4, RHI::UShaderVariableType::VT_Float, 1, 4>(
		m_pShaderReflection,
		strName,
		&Value,
		1,
#ifdef _DEBUG
		m_pCommonMemory,
		m_VarsDebug );
#else
		m_pCommonMemory );
#endif
}


bool iberbar::Renderer::CShaderVariableTable::SetVectorArray( const char* strName, const DirectX::XMFLOAT4* pValues, uint32 nCount )
{
	if ( m_pCommonMemory == nullptr )
		return false;

	return TShaderVariableTable_SetMatrixArray< DirectX::XMFLOAT4, RHI::UShaderVariableType::VT_Float, 1, 4>(
		m_pShaderReflection,
		strName,
		pValues,
		nCount,
#ifdef _DEBUG
		m_pCommonMemory,
		m_VarsDebug );
#else
		m_pCommonMemory );
#endif
}


bool iberbar::Renderer::CShaderVariableTable::SetMatrix( const char* strName, const DirectX::XMFLOAT4X4& Value )
{
	if ( m_pCommonMemory == nullptr )
		return false;

	return TShaderVariableTable_SetMatrixArray< DirectX::XMFLOAT4X4, RHI::UShaderVariableType::VT_Float, 4, 4>(
		m_pShaderReflection,
		strName,
		&Value,
		1,
#ifdef _DEBUG
		m_pCommonMemory,
		m_VarsDebug );
#else
		m_pCommonMemory );
#endif
}


bool iberbar::Renderer::CShaderVariableTable::SetMatrixArray( const char* strName, const DirectX::XMFLOAT4X4* pValues, uint32 nCount )
{
	if ( m_pCommonMemory == nullptr )
		return false;

	return TShaderVariableTable_SetMatrixArray< DirectX::XMFLOAT4X4, RHI::UShaderVariableType::VT_Float, 4, 4 >(
		m_pShaderReflection,
		strName,
		pValues,
		nCount,
#ifdef _DEBUG
		m_pCommonMemory,
		m_VarsDebug );
#else
		m_pCommonMemory );
#endif
}


bool iberbar::Renderer::CShaderVariableTable::SetColor( const char* strName, const CColor4F& Value )
{
	if ( m_pCommonMemory == nullptr )
		return false;

	return TShaderVariableTable_SetMatrixArray< CColor4F, RHI::UShaderVariableType::VT_Float, 1, 4>(
		m_pShaderReflection,
		strName,
		&Value,
		1,
#ifdef _DEBUG
		m_pCommonMemory,
		m_VarsDebug );
#else
		m_pCommonMemory );
#endif
}


bool iberbar::Renderer::CShaderVariableTable::SetColorArray( const char* strName, const CColor4F* pValues, uint32 nCount )
{
	if ( m_pCommonMemory == nullptr )
		return false;

	return TShaderVariableTable_SetMatrixArray< CColor4F, RHI::UShaderVariableType::VT_Float, 1, 4 >(
		m_pShaderReflection,
		strName,
		pValues,
		nCount,
#ifdef _DEBUG
		m_pCommonMemory,
		m_VarsDebug );
#else
		m_pCommonMemory );
#endif
}


bool iberbar::Renderer::CShaderVariableTable::SetStruct( const char* strName, const void* value, uint32 nSize )
{
	const RHI::IShaderReflectionVariable* pReflectionVariable = m_pShaderReflection->GetVariableByName( strName );
	if ( pReflectionVariable == nullptr )
		return false;

	const RHI::IShaderReflectionType* pReflectionType = pReflectionVariable->GetType();
	if ( pReflectionType->GetVariableClass() != RHI::UShaderVariableClass::SVC_Struct )
		return false;

	uint32 nSizeCopySafe = tMin( nSize, m_nCommonMemorySize - pReflectionVariable->GetOffset() );
	memcpy_s( m_pCommonMemory + pReflectionVariable->GetOffset(), nSizeCopySafe, value, nSizeCopySafe );

	return true;
}


bool iberbar::Renderer::CShaderVariableTable::SetTexture( const char* strName, RHI::ITexture* pTexture )
{
	const RHI::IShaderReflectionBindResource* pReflectionVar = m_pShaderReflection->GetTextureByName( strName );
	if ( pReflectionVar == nullptr )
		return false;

	int nSlot = pReflectionVar->GetBindSlot();
	if ( nSlot < 0 || nSlot >= pReflectionVar->GetBindCount() )
		return false;

	if ( m_Textures[ nSlot ] )
		m_Textures[ nSlot ]->Release();
	m_Textures[ nSlot ] = pTexture;
	if ( m_Textures[ nSlot ] )
		m_Textures[ nSlot ]->AddRef();

	return true;
}


bool iberbar::Renderer::CShaderVariableTable::SetSamplerState( const char* strName, RHI::ISamplerState* pSamplerState )
{
	const RHI::IShaderReflectionBindResource* pReflectionVar = m_pShaderReflection->GetTextureByName( strName );
	if ( pReflectionVar == nullptr )
		return false;

	int nSlot = pReflectionVar->GetBindSlot();
	if ( nSlot < 0 || nSlot >= pReflectionVar->GetBindCount() )
		return false;

	if ( m_SamplerStates[ nSlot ] )
		m_SamplerStates[ nSlot ]->Release();
	m_SamplerStates[ nSlot ] = pSamplerState;
	if ( m_SamplerStates[ nSlot ] )
		m_SamplerStates[ nSlot ]->AddRef();

	return true;
}


bool iberbar::Renderer::CShaderVariableTable::Compare( const CShaderVariableTable* pVariableTable ) const
{
	CShaderVariableTable* pOther = (CShaderVariableTable*)pVariableTable;

	if ( m_nCommonMemorySize != pOther->m_nCommonMemorySize )
		return false;
	if ( m_SamplerStates.size() != pOther->m_SamplerStates.size() )
		return false;
	if ( m_Textures.size() != pOther->m_Textures.size() )
		return false;
	if ( memcmp( m_pCommonMemory, pOther->m_pCommonMemory, m_nCommonMemorySize ) != 0 )
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

	return true;
}


void iberbar::Renderer::CShaderVariableTable::Clear()
{
	m_pShaderReflection = nullptr;
	UNKNOWN_SAFE_RELEASE_NULL( m_pShader );
	SAFE_DELETE_ARRAY( m_pCommonMemory );
	m_nCommonMemorySize = 0;
	for ( auto iter = m_Textures.begin(), end = m_Textures.end(); iter != end; iter++ )
	{
		UNKNOWN_SAFE_RELEASE_NULL( *iter );
	}
	m_Textures.clear();
	for ( auto iter = m_SamplerStates.begin(), end = m_SamplerStates.end(); iter != end; iter++ )
	{
		UNKNOWN_SAFE_RELEASE_NULL( *iter );
	}
	m_SamplerStates.clear();
}
