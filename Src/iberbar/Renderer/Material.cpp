
#include <iberbar/Renderer/Material.h>
#include <iberbar/Renderer/Renderer.h>
#include <iberbar/Renderer/Types.h>
#include <iberbar/RHI/Shader.h>
#include <iberbar/RHI/ShaderState.h>
#include <iberbar/RHI/ShaderReflection.h>
#include <iberbar/RHI/VertexDeclaration.h>
#include <iberbar/RHI/Device.h>



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
				Ret = Ret || pVarTables[ i ].SetInt( pstrName, Value );
			}
			return Ret;
		}

		template < typename T >
		FORCEINLINE bool MaterialSetVariableArray( CShaderVariableTable* pVarTable, const char* pstrName, const T* pValues, uint32 nCount )
		{

		}
	}
}



iberbar::Renderer::CMaterial::CMaterial()
	: m_pMaterialParent( nullptr )
	, m_pShaderState( nullptr )
	, m_Streams()
	, m_VariableTables()
{
	memset( m_VariableTables, 0, sizeof( m_VariableTables ) );
}


iberbar::Renderer::CMaterial::CMaterial( CMaterial* pMaterialOrigin )
	: m_pMaterialParent( pMaterialOrigin )
	, m_pShaderState( nullptr )
	, m_Streams()
	, m_VariableTables()
{
	assert( m_pMaterialParent );
	m_pMaterialParent->AddRef();

	memset( m_VariableTables, 0, sizeof( m_VariableTables ) );

	m_pShaderState = m_pMaterialParent->m_pShaderState;
	m_Streams = m_pMaterialParent->m_Streams;
	InitialVariableTables();
}


iberbar::Renderer::CMaterial::~CMaterial()
{
	UNKNOWN_SAFE_RELEASE_NULL( m_pMaterialParent );
	UNKNOWN_SAFE_RELEASE_NULL( m_pShaderState );
	for ( int i = 0, s = (int)RHI::EShaderType::__Count; i < s; i++ )
	{
		SAFE_DELETE( m_VariableTables[ i ] );
	}
}


void iberbar::Renderer::CMaterial::Initial( RHI::IShaderProgram* pShaderProgram )
{
	assert( pShaderProgram );

	assert( m_pShaderState == nullptr );

	RHI::IShader* pVertexShader = pShaderProgram->GetShader( RHI::EShaderType::VertexShader );
	assert( pVertexShader );
	RHI::IShaderReflection* pVertexShaderReflection = pVertexShader->GetReflection();
	RHI::UVertexElement VertexElements[ RHI::MaxVertexElementCount ];
	int nVertexElementsCount = pVertexShaderReflection->GetInputParameterCount();
	memset( VertexElements, 0, sizeof( VertexElements ) );
	uint32 m_nVertexElementUsage = 0;
	for ( int i = 0; i < nVertexElementsCount; i++ )
	{
		const RHI::UShaderInputParameterDesc& InputParameterDesc = pVertexShaderReflection->GetInputParameterDesc( i );
		
		VertexElements[ i ].nOffset = 0;
		VertexElements[ i ].nSemantic = InputParameterDesc.SemanticUsage;
		VertexElements[ i ].nSemanticIndex = InputParameterDesc.SemanticIndex;
		switch ( InputParameterDesc.SemanticUsage )
		{
			case RHI::UVertexDeclareUsage::Position:
			{
				VertexElements[ i ].nSlot = (int)EVertexElementSlot::Position;
				VertexElements[ i ].nFormat = RHI::UVertexFormat::FLOAT3;
				VertexElements[ i ].nStride = sizeof( float ) * 3;
				break;
			}
			case RHI::UVertexDeclareUsage::Color:
			{
				VertexElements[ i ].nSlot = (int)EVertexElementSlot::Color;
				VertexElements[ i ].nFormat = RHI::UVertexFormat::FLOAT4;
				VertexElements[ i ].nStride = sizeof( float ) * 4;
				break;
			}
			case RHI::UVertexDeclareUsage::Normal:
			{
				VertexElements[ i ].nSlot = (int)EVertexElementSlot::Normal;
				VertexElements[ i ].nFormat = RHI::UVertexFormat::FLOAT3;
				VertexElements[ i ].nStride = sizeof( float ) * 3;
				break;
			}
			case RHI::UVertexDeclareUsage::TexCoord:
			{
				VertexElements[ i ].nSlot = (int)EVertexElementSlot::Tex0 + InputParameterDesc.SemanticIndex;
				VertexElements[ i ].nFormat = RHI::UVertexFormat::FLOAT2;
				VertexElements[ i ].nStride = sizeof( float ) * 2;
				break;
			}
			default:break;
		}

		assert( (m_nVertexElementUsage & (0x1 << VertexElements[ i ].nSlot)) == 0 );
		m_nVertexElementUsage = m_nVertexElementUsage | (0x1 << VertexElements[ i ].nSlot);
		
		m_Streams.push_back( VertexElements[ i ].nSlot );
	}

	RHI::IDevice* pRhiDevice = CRenderer::sGetInstance()->GetRHIDevice();
	TSmartRefPtr<RHI::IVertexDeclaration> pVertexDeclaration;
	CResult cResult = pRhiDevice->CreateVertexDeclaration( &pVertexDeclaration, VertexElements, nVertexElementsCount );
	if ( cResult.IsOK() == false )
		return;

	cResult = pRhiDevice->CreateShaderState( &m_pShaderState, pVertexDeclaration, pShaderProgram );
	if ( cResult.IsOK() == false )
		return;

	InitialVariableTables();

	//RHI::IShader* pShader = nullptr;
	//RHI::IShaderReflection* pShaderReflection = nullptr;
	//const RHI::IShaderReflectionBuffer* pShaderReflectionBuffer = nullptr;
	//RHI::IUniformBuffer* pUniformBuffer = nullptr;
	//uint32 nCBufferCount = 0;
	//for ( int i = 0, s = (int)RHI::EShaderType::__Count; i < s; i++ )
	//{
	//	pShader = pShaderProgram->GetShader( (RHI::EShaderType)i );
	//	pShaderReflection = pShader->GetReflection();
	//	m_VariableTables[ i ].SetShader( pShader );
	//	nCBufferCount = pShaderReflection->GetBufferCount();
	//	if ( nCBufferCount > 0 )
	//	{
	//		m_ShaderUniformBuffers[ i ].resize( nCBufferCount );
	//		for ( uint32 nCBufferIndex = 0; nCBufferIndex < nCBufferCount; nCBufferIndex++ )
	//		{
	//			pShaderReflectionBuffer = pShaderReflection->GetBufferByIndex( nCBufferIndex );
	//			pRhiDevice->CreateUniformBuffer( &pUniformBuffer, pShaderReflectionBuffer->GetSize() );
	//			m_ShaderUniformBuffers[ i ][ nCBufferIndex ] = pUniformBuffer;
	//			pUniformBuffer = nullptr;
	//		}
	//	}
	//}
}


void iberbar::Renderer::CMaterial::InitialVariableTables()
{
	if ( m_pShaderState )
	{
		m_pShaderState->AddRef();
		RHI::IShader* pShader;
		for ( int i = 0, s = (int)RHI::EShaderType::__Count; i < s; i++ )
		{
			pShader = m_pShaderState->GetShaderProgram()->GetShader( (RHI::EShaderType)i );
			if ( pShader )
			{
				m_VariableTables[ i ] = new CShaderVariableTable();
				m_VariableTables[ i ]->SetShader( pShader );
			}
		}
	}
}


bool iberbar::Renderer::CMaterial::SetInt( const char* pstrName, int32 Value )
{
	bool Ret = false;
	for ( int i = 0, s = (int)RHI::EShaderType::__Count; i < s; i++ )
	{
		Ret = Ret || (m_VariableTables[ i ] && m_VariableTables[ i ]->SetInt( pstrName, Value ));
	}
	return Ret;
}


bool iberbar::Renderer::CMaterial::SetIntArray( const char* pstrName, const int32* pValues, uint32 nCount )
{
	bool Ret = false;
	for ( int i = 0, s = (int)RHI::EShaderType::__Count; i < s; i++ )
	{
		Ret = Ret || (m_VariableTables[ i ] && m_VariableTables[ i ]->SetIntArray( pstrName, pValues, nCount ));
	}
	return Ret;
}


bool iberbar::Renderer::CMaterial::SetFloat( const char* pstrName, float Value )
{
	bool Ret = false;
	for ( int i = 0, s = (int)RHI::EShaderType::__Count; i < s; i++ )
	{
		Ret = Ret || (m_VariableTables[ i ] && m_VariableTables[ i ]->SetFloat( pstrName, Value ));
	}
	return Ret;
}


bool iberbar::Renderer::CMaterial::SetFloatArray( const char* pstrName, const float* pValues, uint32 nCount )
{
	bool Ret = false;
	for ( int i = 0, s = (int)RHI::EShaderType::__Count; i < s; i++ )
	{
		Ret = Ret || (m_VariableTables[ i ] && m_VariableTables[ i ]->SetFloatArray( pstrName, pValues, nCount ));
	}
	return Ret;
}


bool iberbar::Renderer::CMaterial::SetVector( const char* pstrName, const DirectX::XMFLOAT4& Value )
{
	bool Ret = false;
	for ( int i = 0, s = (int)RHI::EShaderType::__Count; i < s; i++ )
	{
		Ret = Ret || (m_VariableTables[ i ] && m_VariableTables[ i ]->SetVector( pstrName, Value ));
	}
	return Ret;
}


bool iberbar::Renderer::CMaterial::SetVectorArray( const char* pstrName, const DirectX::XMFLOAT4* pValues, uint32 nCount )
{
	bool Ret = false;
	for ( int i = 0, s = (int)RHI::EShaderType::__Count; i < s; i++ )
	{
		Ret = Ret || (m_VariableTables[ i ] && m_VariableTables[ i ]->SetVectorArray( pstrName, pValues, nCount ));
	}
	return Ret;
}


bool iberbar::Renderer::CMaterial::SetMatrix( const char* pstrName, const DirectX::XMFLOAT4X4& Value )
{
	bool Ret = false;
	for ( int i = 0, s = (int)RHI::EShaderType::__Count; i < s; i++ )
	{
		Ret = Ret || (m_VariableTables[ i ] && m_VariableTables[ i ]->SetMatrix( pstrName, Value ));
	}
	return Ret;
}


bool iberbar::Renderer::CMaterial::SetMatrixArray( const char* pstrName, const DirectX::XMFLOAT4X4* pValues, uint32 nCount )
{
	bool Ret = false;
	for ( int i = 0, s = (int)RHI::EShaderType::__Count; i < s; i++ )
	{
		Ret = Ret || (m_VariableTables[ i ] && m_VariableTables[ i ]->SetMatrixArray( pstrName, pValues, nCount ));
	}
	return Ret;
}


bool iberbar::Renderer::CMaterial::SetColor( const char* pstrName, const CColor4F& Value )
{
	bool Ret = false;
	for ( int i = 0, s = (int)RHI::EShaderType::__Count; i < s; i++ )
	{
		Ret = Ret || (m_VariableTables[ i ] && m_VariableTables[ i ]->SetColor( pstrName, Value ));
	}
	return Ret;
}


bool iberbar::Renderer::CMaterial::SetColorArray( const char* pstrName, const CColor4F* pValues, uint32 nCount )
{
	bool Ret = false;
	for ( int i = 0, s = (int)RHI::EShaderType::__Count; i < s; i++ )
	{
		Ret = Ret || (m_VariableTables[ i ] && m_VariableTables[ i ]->SetColorArray( pstrName, pValues, nCount ));
	}
	return Ret;
}


bool iberbar::Renderer::CMaterial::SetStruct( const char* pstrName, const void* Value, uint32 nSize )
{
	bool Ret = false;
	for ( int i = 0, s = (int)RHI::EShaderType::__Count; i < s; i++ )
	{
		Ret = Ret || (m_VariableTables[ i ] && m_VariableTables[ i ]->SetStruct( pstrName, Value, nSize ));
	}
	return Ret;
}


bool iberbar::Renderer::CMaterial::SetTexture( const char* pstrName, RHI::ITexture* pTexture )
{
	bool Ret = false;
	for ( int i = 0, s = (int)RHI::EShaderType::__Count; i < s; i++ )
	{
		Ret = Ret || (m_VariableTables[ i ] && m_VariableTables[ i ]->SetTexture( pstrName, pTexture ));
	}
	return Ret;
}


bool iberbar::Renderer::CMaterial::SetTexture( const char* pstrName, RHI::ITexture** ppTextures, uint32 nElementCount, uint32 nElementStart )
{
	bool Ret = false;
	for ( int i = 0, s = (int)RHI::EShaderType::__Count; i < s; i++ )
	{
		Ret = Ret || (m_VariableTables[ i ] && m_VariableTables[ i ]->SetTexture( pstrName, ppTextures[ 0 ] ));
	}
	return Ret;
}


bool iberbar::Renderer::CMaterial::SetSamplerState( const char* pstrName, RHI::ISamplerState* pSamplerState )
{
	bool Ret = false;
	for ( int i = 0, s = (int)RHI::EShaderType::__Count; i < s; i++ )
	{
		Ret = Ret || (m_VariableTables[ i ] && m_VariableTables[ i ]->SetSamplerState( pstrName, pSamplerState ));
	}
	return Ret;
}


bool iberbar::Renderer::CMaterial::SetSamplerState( const char* pstrName, RHI::ISamplerState** ppSamplerStates, uint32 nElementCount, uint32 nElementStart )
{
	bool Ret = false;
	for ( int i = 0, s = (int)RHI::EShaderType::__Count; i < s; i++ )
	{
		Ret = Ret || (m_VariableTables[ i ] && m_VariableTables[ i ]->SetSamplerState( pstrName, ppSamplerStates[ 0 ] ));
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
	if ( m_pShaderState != pOther->m_pShaderState )
		return false;
	for ( int i = 0, s = (int)RHI::EShaderType::__Count; i < s; i++ )
	{
		if ( m_VariableTables[ i ] && m_VariableTables[ i ]->Compare( (pOther->m_VariableTables[ i ]) ) == false )
			return false;
	}
	return true;
}
