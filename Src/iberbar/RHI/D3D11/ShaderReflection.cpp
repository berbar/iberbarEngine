
#include <iberbar/RHI/D3D11/ShaderReflection.h>




iberbar::RHI::D3D11::CShaderReflectionType::CShaderReflectionType()
	: m_nVarType( UShaderVariableType::VT_Unknown )
	, m_nVarClass( UShaderVariableClass::SVC_Scalar )
	, m_nElementCount( 0 )
	, m_nRowCount( 0 )
	, m_nColumnCount( 0 )
	, m_Members()
{
}


int iberbar::RHI::D3D11::CShaderReflectionType::GetMemberCount() const
{
	return (int)m_Members.size();
}


const iberbar::RHI::IShaderReflectionMember* iberbar::RHI::D3D11::CShaderReflectionType::GetMemberByIndex( int nIndex ) const
{
	assert( nIndex >= 0 && nIndex < (int)m_Members.size() );
	return m_Members[ nIndex ];
}


const iberbar::RHI::IShaderReflectionMember* iberbar::RHI::D3D11::CShaderReflectionType::GetMemberByName( const char* pstrName ) const
{
	assert( pstrName );
	for ( int i = 0, s = (int)m_Members.size(); i < s; i++ )
	{
		if ( strcmp( m_Members[ i ]->GetName(), pstrName ) == 0 )
			return m_Members[ i ];
	}
	return nullptr;
}


void iberbar::RHI::D3D11::CShaderReflectionType::GetDesc( UShaderReflectionTypeDesc* pOutDesc ) const
{
	assert( pOutDesc );
	pOutDesc->nVarType = m_nVarType;
	pOutDesc->nVarClass = m_nVarClass;
	pOutDesc->nElements = m_nElementCount;
	pOutDesc->nRows = m_nRowCount;
	pOutDesc->nColumns = m_nColumnCount;
	pOutDesc->pstrName = m_strName.c_str();
}


iberbar::CResult iberbar::RHI::D3D11::CShaderReflectionType::Initial( ID3D11ShaderReflectionType* pD3DShaderReflectionType )
{
	D3D11_SHADER_TYPE_DESC D3DTypeDesc;
	pD3DShaderReflectionType->GetDesc( &D3DTypeDesc );

	m_strName = D3DTypeDesc.Name;

	switch ( D3DTypeDesc.Type )
	{
		case D3D_SHADER_VARIABLE_TYPE::D3D_SVT_VOID:
		{
			m_nVarType = UShaderVariableType::VT_Void;
			break;
		}

		case D3D_SHADER_VARIABLE_TYPE::D3D_SVT_FLOAT:
		{
			m_nVarType = UShaderVariableType::VT_Float;
			break;
		}

		case D3D_SHADER_VARIABLE_TYPE::D3D_SVT_INT:
		{
			m_nVarType = UShaderVariableType::VT_Int;
			break;
		}

		case D3D_SHADER_VARIABLE_TYPE::D3D_SVT_BOOL:
		{
			m_nVarType = UShaderVariableType::VT_Boolean;
			break;
		}

		default:break;
	}

	switch ( D3DTypeDesc.Class )
	{
		case D3D_SVC_SCALAR:
			m_nVarClass = UShaderVariableClass::SVC_Scalar;
			break;

		case D3D_SVC_VECTOR:
			m_nVarClass = UShaderVariableClass::SVC_Vector;
			break;

		case D3D_SVC_MATRIX_COLUMNS:
			m_nVarClass = UShaderVariableClass::SVC_Matrix;
			break;

		case D3D_SVC_STRUCT:
			m_nVarClass = UShaderVariableClass::SVC_Struct;
			break;

		default:
			break;
	}

	m_nElementCount = D3DTypeDesc.Elements;
	m_nRowCount = D3DTypeDesc.Rows;
	m_nColumnCount = D3DTypeDesc.Columns;

	if ( D3DTypeDesc.Members > 0 )
	{
		m_Members.resize( D3DTypeDesc.Members );
		CShaderReflectionMember* pMember;
		for ( UINT i = 0, s = D3DTypeDesc.Members; i < s; i++ )
		{
			m_Members[ i ] = pMember = new CShaderReflectionMember();
			pMember->Initial( pD3DShaderReflectionType->GetMemberTypeByIndex( i ), pD3DShaderReflectionType->GetMemberTypeName( i ) );
		}
	}

	return CResult();
}






iberbar::RHI::D3D11::CShaderReflectionMember::CShaderReflectionMember()
	: m_nOffset(0)
	, m_strName()
	, m_ReflectionType()
{
}


void iberbar::RHI::D3D11::CShaderReflectionMember::GetDesc( UShaderReflectionMemberDesc* pOutDesc ) const
{
	pOutDesc->nOffset = m_nOffset;
	pOutDesc->nVarType = m_ReflectionType.GetVariableType();
	pOutDesc->pstrName = m_strName.c_str();
}


iberbar::CResult iberbar::RHI::D3D11::CShaderReflectionMember::Initial( ID3D11ShaderReflectionType* pD3DShaderReflectionType, const char* pstrName )
{
	D3D11_SHADER_TYPE_DESC D3DTypeDesc;
	pD3DShaderReflectionType->GetDesc( &D3DTypeDesc );
	m_nOffset = D3DTypeDesc.Offset;
	m_strName = pstrName;
	return m_ReflectionType.Initial( pD3DShaderReflectionType );
}






iberbar::RHI::D3D11::CShaderReflectionVariable::CShaderReflectionVariable()
	: m_nOffsetLocal( 0 )
	, m_nOffset( 0 )
	, m_nRows( 0 )
	, m_nColumns( 0 )
	, m_nElementSize( 0 )
	, m_nElementCount( 0 )
	, m_nTotalSize( 0 )
	, m_ReflectionType()
{
}


void iberbar::RHI::D3D11::CShaderReflectionVariable::GetDesc( UShaderReflectionVariableDesc* pOutDesc ) const
{
	pOutDesc->nOffset = m_nOffset;
	pOutDesc->nVarType = m_ReflectionType.GetVariableType();
}


iberbar::CResult iberbar::RHI::D3D11::CShaderReflectionVariable::Initial( ID3D11ShaderReflectionVariable* pD3DShaderReflectionVariable, uint32 nCBufferBytesOffset )
{
	D3D11_SHADER_VARIABLE_DESC D3DVariableDesc;
	pD3DShaderReflectionVariable->GetDesc( &D3DVariableDesc );
	m_nOffsetLocal = D3DVariableDesc.StartOffset;
	m_nOffset = nCBufferBytesOffset + D3DVariableDesc.StartOffset;
	m_nTotalSize = D3DVariableDesc.Size;
	m_strName = D3DVariableDesc.Name;

	return m_ReflectionType.Initial( pD3DShaderReflectionVariable->GetType() );
}




iberbar::RHI::D3D11::CShaderReflectionBuffer::CShaderReflectionBuffer()
	: m_nOffset( 0 )
	, m_nSize( 0 )
	, m_Variables()
	, m_strName()
{
}


const iberbar::RHI::IShaderReflectionVariable* iberbar::RHI::D3D11::CShaderReflectionBuffer::GetVariableByName( const char* pstrName ) const
{
	assert( pstrName && pstrName[0] != 0 );

	for ( int i = 0, s = (int)m_Variables.size(); i < s; i++ )
	{
		if ( strcmp( m_Variables[ i ].GetName(), pstrName ) == 0 )
			return &m_Variables[ i ];
	}
	return nullptr;
}


iberbar::CResult iberbar::RHI::D3D11::CShaderReflectionBuffer::Initial( const D3D11_SHADER_INPUT_BIND_DESC& D3DBindDesc, ID3D11ShaderReflectionConstantBuffer* pD3DShaderReflectionConstantBuffer, uint32 nOffset )
{
	D3D11_SHADER_BUFFER_DESC D3DBufferDesc;
	pD3DShaderReflectionConstantBuffer->GetDesc( &D3DBufferDesc );

	m_nBindPoint = D3DBindDesc.BindPoint;
	m_nOffset = nOffset;
	m_nSize = D3DBufferDesc.Size;
	m_strName = D3DBufferDesc.Name;

	if ( D3DBufferDesc.Variables > 0 )
	{
		m_Variables.resize( D3DBufferDesc.Variables );

		ID3D11ShaderReflectionVariable* pD3DShaderReflectionVariable = nullptr;
		CShaderReflectionVariable* pReflectionVariable = nullptr;
		for ( UINT nVarIndex = 0, nVarCount = D3DBufferDesc.Variables; nVarIndex < nVarCount; nVarIndex++ )
		{
			pD3DShaderReflectionVariable = pD3DShaderReflectionConstantBuffer->GetVariableByIndex( nVarIndex );

			pReflectionVariable = &m_Variables[ nVarIndex ];
			pReflectionVariable->Initial( pD3DShaderReflectionVariable, m_nOffset );
		}
	}

	return CResult();
}





iberbar::RHI::D3D11::CShaderReflection::CShaderReflection()
	: m_nBufferSizeTotal( 0 )
	, m_ConstBuffers()
	, m_Vars()
	, m_Samplers()
	, m_Textures()
{
}


const iberbar::RHI::IShaderReflectionBuffer* iberbar::RHI::D3D11::CShaderReflection::GetBufferByName( const char* pstrName ) const
{
	for ( int i = 0, s = m_ConstBuffers.size(); i < s; i++ )
	{
		if ( strcmp( m_ConstBuffers[ i ].GetName().c_str(), pstrName ) == 0 )
			return &m_ConstBuffers[ i ];
	}
	return nullptr;
}


iberbar::CResult iberbar::RHI::D3D11::CShaderReflection::Initial( const void* pCodes, uint32 nCodeLen )
{
	HRESULT hResult;

	ComPtr<ID3D11ShaderReflection> pD3DShaderReflection;
	hResult = D3DReflect( pCodes, nCodeLen, __uuidof( ID3D11ShaderReflection ), (void**)&pD3DShaderReflection );
	if ( FAILED( hResult ) )
	{
		return MakeResult( ResultCode::Bad, "" );
	}

	D3D11_SHADER_DESC D3DDesc;
	D3D11_SHADER_INPUT_BIND_DESC D3DInputBindDesc;
	ID3D11ShaderReflectionConstantBuffer* pD3DShaderReflectionCBuffer = nullptr;
	CShaderReflectionBuffer* pConstBuffer = nullptr;

	pD3DShaderReflection->GetDesc( &D3DDesc );
	if ( D3DDesc.BoundResources > 0 )
	{
		if ( D3DDesc.ConstantBuffers > 0 )
		{
			m_ConstBuffers.resize( D3DDesc.ConstantBuffers );
		}

		int nConstBufferIndex = 0;
		uint32 nConstBufferOffset = 0;

		for ( int i = 0, s = D3DDesc.BoundResources; i < s; i++ )
		{
			pD3DShaderReflection->GetResourceBindingDesc( i, &D3DInputBindDesc );
			switch ( D3DInputBindDesc.Type )
			{
				case D3D_SHADER_INPUT_TYPE::D3D_SIT_SAMPLER:
					m_Samplers.push_back( CShaderReflectionBindResource( UShaderVariableType::VT_Sampler2D, D3DInputBindDesc ) );
					break;

				case D3D_SHADER_INPUT_TYPE::D3D_SIT_TEXTURE:
					m_Textures.push_back( CShaderReflectionBindResource( UShaderVariableType::VT_Texture, D3DInputBindDesc ) );
					break;

				case D3D_SHADER_INPUT_TYPE::D3D_SIT_CBUFFER:
					pD3DShaderReflectionCBuffer = pD3DShaderReflection->GetConstantBufferByIndex( nConstBufferIndex );
					pConstBuffer = &m_ConstBuffers[ nConstBufferIndex ];
					pConstBuffer->Initial( D3DInputBindDesc, pD3DShaderReflectionCBuffer, nConstBufferOffset );
					nConstBufferOffset += pConstBuffer->GetSize();
					nConstBufferIndex++;

					for ( int nVarIndex = 0, nVarCount = (int)pConstBuffer->GetVariableCountInternal(); nVarIndex < nVarCount; nVarIndex++ )
					{
						m_Vars.push_back( pConstBuffer->GetVariableByIndexInternal( nVarIndex ) );
					}

					m_nBufferSizeTotal = nConstBufferOffset;
					break;

				default:
					break;
			}
		}
	}
	/*if ( D3DDesc.ConstantBuffers > 0 )
	{
		m_ConstBuffers.resize( D3DDesc.ConstantBuffers );

		uint32 nConstBufferOffset = 0;
		for ( UINT i = 0, s = D3DDesc.ConstantBuffers; i < s; i++ )
		{
			pD3DShaderReflectionCBuffer = pD3DShaderReflection->GetConstantBufferByIndex( i );
			pConstBuffer = &m_ConstBuffers[ i ];
			pConstBuffer->Initial( pD3DShaderReflectionCBuffer, nConstBufferOffset );
			nConstBufferOffset += pConstBuffer->GetSize();

			for ( int nVarIndex = 0, nVarCount = (int)pConstBuffer->GetVariableCountInternal(); nVarIndex < nVarCount; nVarIndex++ )
			{
				m_Vars.push_back( pConstBuffer->GetVariableByIndexInternal( nVarIndex ) );
			}
		}
		m_nBufferSizeTotal = nConstBufferOffset;
	}*/

	return CResult();
}


const iberbar::RHI::D3D11::CShaderReflectionVariable* iberbar::RHI::D3D11::CShaderReflection::GetVariableByName( const char* pstrName ) const
{
	for ( int i = 0, s = (int)m_Vars.size(); i < s; i++ )
	{
		if ( strcmp( m_Vars[ i ]->GetName(), pstrName ) == 0 )
			return m_Vars[ i ];
	}
	return nullptr;
}


const iberbar::RHI::D3D11::CShaderReflectionBindResource* iberbar::RHI::D3D11::CShaderReflection::GetSamplerByName( const char* pstrName ) const
{
	for ( int i = 0, s = (int)m_Samplers.size(); i < s; i++ )
	{
		if ( strcmp( m_Samplers[ i ].GetName(), pstrName ) == 0 )
			return &m_Samplers[ i ];
	}
	return nullptr;
}


const iberbar::RHI::D3D11::CShaderReflectionBindResource* iberbar::RHI::D3D11::CShaderReflection::GetTextureByName( const char* pstrName ) const
{
	for ( int i = 0, s = (int)m_Textures.size(); i < s; i++ )
	{
		if ( strcmp( m_Textures[ i ].GetName(), pstrName ) == 0 )
			return &m_Textures[ i ];
	}
	return nullptr;
}
