
#include <iberbar/Renderer/Material.h>
#include <iberbar/RHI/Shader.h>
#include <iberbar/RHI/ShaderState.h>


namespace iberbar
{
	namespace Renderer
	{
		template < typename T >
		FORCEINLINE bool MaterialSetVariable( CShaderVariableTable* pVarTables, const char* pstrName, const T& Value )
		{
			bool Ret = false;
			for ( int i = 0, s = (int)RHI::EShaderType::__Count; i < s; i++ )
			{
				Ret = Ret && pVarTables[ i ].SetInt( pstrName, Value );
			}
			return Ret;
		}

		template < typename T >
		FORCEINLINE bool MaterialSetVariableArray( CShaderVariableTable* pVarTable, const char* pstrName, const T* pValues, uint32 nCount )
		{

		}
	}
}



iberbar::Renderer::CMaterial::CMaterial( RHI::IShaderProgram* pShaderProgram )
	: m_pMaterialParent( nullptr )
	, m_pShaderProgram( nullptr )
	, m_VariableTables()
{
	Initial( pShaderProgram );
}


iberbar::Renderer::CMaterial::CMaterial( CMaterial* pMaterialOrigin )
	: m_pMaterialParent( pMaterialOrigin )
	, m_pShaderProgram( nullptr )
	, m_VariableTables()
{
	assert( m_pMaterialParent );
	m_pMaterialParent->AddRef();
	Initial( m_pMaterialParent->m_pShaderProgram );
}


iberbar::Renderer::CMaterial::~CMaterial()
{
	UNKNOWN_SAFE_RELEASE_NULL( m_pMaterialParent );
	UNKNOWN_SAFE_RELEASE_NULL( m_pShaderProgram );
}


void iberbar::Renderer::CMaterial::Initial( RHI::IShaderProgram* pShaderProgram )
{
	assert( pShaderProgram );
	m_pShaderProgram = pShaderProgram;
	m_pShaderProgram->AddRef();
	for ( int i = 0, s = (int)RHI::EShaderType::__Count; i < s; i++ )
	{
		m_VariableTables[ i ].SetShader( m_pShaderProgram->GetShader( (RHI::EShaderType)i ) );
	}
}


bool iberbar::Renderer::CMaterial::SetInt( const char* pstrName, int32 Value )
{
	bool Ret = false;
	for ( int i = 0, s = (int)RHI::EShaderType::__Count; i < s; i++ )
	{
		Ret = Ret && m_VariableTables[ i ].SetInt( pstrName, Value );
	}
	return Ret;
}


bool iberbar::Renderer::CMaterial::SetIntArray( const char* pstrName, const int32* pValues, uint32 nCount )
{
	bool Ret = false;
	for ( int i = 0, s = (int)RHI::EShaderType::__Count; i < s; i++ )
	{
		Ret = Ret && m_VariableTables[ i ].SetIntArray( pstrName, pValues, nCount );
	}
	return Ret;
}


bool iberbar::Renderer::CMaterial::SetFloat( const char* pstrName, float Value )
{
	bool Ret = false;
	for ( int i = 0, s = (int)RHI::EShaderType::__Count; i < s; i++ )
	{
		Ret = Ret && m_VariableTables[ i ].SetFloat( pstrName, Value );
	}
	return Ret;
}


bool iberbar::Renderer::CMaterial::SetFloatArray( const char* pstrName, const float* pValues, uint32 nCount )
{
	bool Ret = false;
	for ( int i = 0, s = (int)RHI::EShaderType::__Count; i < s; i++ )
	{
		Ret = Ret && m_VariableTables[ i ].SetFloatArray( pstrName, pValues, nCount );
	}
	return Ret;
}


bool iberbar::Renderer::CMaterial::SetVector( const char* pstrName, const DirectX::XMFLOAT4& Value )
{
	bool Ret = false;
	for ( int i = 0, s = (int)RHI::EShaderType::__Count; i < s; i++ )
	{
		Ret = Ret && m_VariableTables[ i ].SetVector( pstrName, Value );
	}
	return Ret;
}


bool iberbar::Renderer::CMaterial::SetVectorArray( const char* pstrName, const DirectX::XMFLOAT4* pValues, uint32 nCount )
{
	bool Ret = false;
	for ( int i = 0, s = (int)RHI::EShaderType::__Count; i < s; i++ )
	{
		Ret = Ret && m_VariableTables[ i ].SetVectorArray( pstrName, pValues, nCount );
	}
	return Ret;
}


bool iberbar::Renderer::CMaterial::SetMatrix( const char* pstrName, const DirectX::XMFLOAT4X4& Value )
{
	bool Ret = false;
	for ( int i = 0, s = (int)RHI::EShaderType::__Count; i < s; i++ )
	{
		Ret = Ret && m_VariableTables[ i ].SetMatrix( pstrName, Value );
	}
	return Ret;
}


bool iberbar::Renderer::CMaterial::SetMatrixArray( const char* pstrName, const DirectX::XMFLOAT4X4* pValues, uint32 nCount )
{
	bool Ret = false;
	for ( int i = 0, s = (int)RHI::EShaderType::__Count; i < s; i++ )
	{
		Ret = Ret && m_VariableTables[ i ].SetMatrixArray( pstrName, pValues, nCount );
	}
	return Ret;
}


bool iberbar::Renderer::CMaterial::SetColor( const char* pstrName, const CColor4F& Value )
{
	bool Ret = false;
	for ( int i = 0, s = (int)RHI::EShaderType::__Count; i < s; i++ )
	{
		Ret = Ret && m_VariableTables[ i ].SetColor( pstrName, Value );
	}
	return Ret;
}


bool iberbar::Renderer::CMaterial::SetColorArray( const char* pstrName, const CColor4F* pValues, uint32 nCount )
{
	bool Ret = false;
	for ( int i = 0, s = (int)RHI::EShaderType::__Count; i < s; i++ )
	{
		Ret = Ret && m_VariableTables[ i ].SetColorArray( pstrName, pValues, nCount );
	}
	return Ret;
}


bool iberbar::Renderer::CMaterial::SetStruct( const char* pstrName, const void* Value, uint32 nSize )
{
	bool Ret = false;
	for ( int i = 0, s = (int)RHI::EShaderType::__Count; i < s; i++ )
	{
		Ret = Ret && m_VariableTables[ i ].SetStruct( pstrName, Value, nSize );
	}
	return Ret;
}


bool iberbar::Renderer::CMaterial::SetTexture( const char* pstrName, RHI::ITexture** ppTextures, uint32 nElementCount, uint32 nElementStart )
{
	bool Ret = false;
	for ( int i = 0, s = (int)RHI::EShaderType::__Count; i < s; i++ )
	{
		Ret = Ret && m_VariableTables[ i ].SetTexture( pstrName, ppTextures[ 0 ] );
	}
	return Ret;
}


bool iberbar::Renderer::CMaterial::SetSamplerState( const char* pstrName, RHI::ISamplerState** ppSamplerStates, uint32 nElementCount, uint32 nElementStart )
{
	bool Ret = false;
	for ( int i = 0, s = (int)RHI::EShaderType::__Count; i < s; i++ )
	{
		Ret = Ret && m_VariableTables[ i ].SetSamplerState( pstrName, ppSamplerStates[ 0 ] );
	}
	return Ret;
}


void iberbar::Renderer::CMaterial::Reset()
{
	if ( m_pMaterialParent == nullptr )
	{

	}
	else
	{

	}
}


bool iberbar::Renderer::CMaterial::CampareWithMaterial( const CMaterial* pOther ) const
{
	if ( m_pShaderProgram != pOther->m_pShaderProgram )
		return false;
	for ( int i = 0, s = (int)RHI::EShaderType::__Count; i < s; i++ )
	{
		if ( m_VariableTables[ i ].Compare( &(pOther->m_VariableTables[ i ]) ) == false )
			return false;
	}
	return true;
}
