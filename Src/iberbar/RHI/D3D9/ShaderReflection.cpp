

#include <iberbar/RHI/D3D9/ShaderReflection.h>



iberbar::RHI::UShaderVariableType iberbar::RHI::D3D9::ConvertShaderParameterType( ID3DXConstantTable* pD3DConstantTable, D3DXHANDLE pD3DHandle, const D3DXCONSTANT_DESC& Desc, CShaderReflectionType** ppOutReflectionType )
{
	UShaderVariableType eParamType = UShaderVariableType::VT_Unknown;
	switch ( Desc.Type )
	{
		case D3DXPT_BOOL:
			eParamType = UShaderVariableType::VT_Boolean;
			break;

		case D3DXPT_INT:
			eParamType = UShaderVariableType::VT_Int;
			break;

		case D3DXPT_FLOAT:
			eParamType = UShaderVariableType::VT_Float;
			break;

		case D3DXPT_TEXTURE:
			eParamType = UShaderVariableType::VT_Texture;
			break;

		case D3DXPT_SAMPLER2D:
			eParamType = UShaderVariableType::VT_Sampler2D;
			break;

		case D3DXPT_VOID:
			if ( Desc.Class == D3DXPC_STRUCT )
			{
				eParamType = UShaderVariableType::VT_Void;
				( *ppOutReflectionType ) = new CShaderReflectionType();
				( *ppOutReflectionType )->Initial( pD3DConstantTable, pD3DHandle, Desc );
			}
			else if ( Desc.Class == D3DXPC_VECTOR )
			{
				eParamType = UShaderVariableType::VT_Void;
			}
			else if ( Desc.Class == D3DXPC_MATRIX_COLUMNS )
			{

			}
			break;

		default:
			break;
	}

	return eParamType;
}











iberbar::RHI::D3D9::CShaderReflectionVariable::CShaderReflectionVariable()
	: m_pD3DHandle( nullptr )
	, m_nOffset( 0 )
	, m_nRows( 0 )
	, m_nColumns( 0 )
	, m_nElementSize( 0 )
	, m_nElementCount( 0 )
	, m_nTotalSize( 0 )
	, m_nRegisterIndex( 0 )
	, m_ReflectionType()
	, m_strName()
{
}


iberbar::RHI::D3D9::CShaderReflectionVariable::~CShaderReflectionVariable()
{
}


const iberbar::RHI::IShaderReflectionType* iberbar::RHI::D3D9::CShaderReflectionVariable::GetType() const
{
	return &m_ReflectionType;
}


void iberbar::RHI::D3D9::CShaderReflectionVariable::GetDesc( UShaderReflectionVariableDesc* pOutDesc ) const
{
	assert( pOutDesc );

	pOutDesc->nVarType = m_ReflectionType.GetVarType();
	pOutDesc->nVarClass = m_ReflectionType.GetVarClass();
	pOutDesc->nOffset = m_nOffset;
}



iberbar::CResult iberbar::RHI::D3D9::CShaderReflectionVariable::Initial( ID3DXConstantTable* pD3DConstantTable, D3DXHANDLE pD3DHandle, const D3DXCONSTANT_DESC& Desc, uint32 nOffsetTotal )
{
	assert( pD3DConstantTable );
	assert( pD3DHandle );

	m_pD3DHandle = pD3DHandle;
	m_nOffset = nOffsetTotal;
	m_nRows = Desc.Rows;
	m_nColumns = Desc.Columns;
	m_nElementCount = Desc.Elements;
	m_nElementSize = Desc.Bytes / Desc.Elements;
	m_nTotalSize = Desc.Bytes;
	m_strName = Desc.Name;
	m_ReflectionType.Initial( pD3DConstantTable, pD3DHandle, Desc );
	if ( Desc.Type == D3DXPT_TEXTURE || Desc.Type == D3DXPT_SAMPLER2D )
	{
		m_nRegisterIndex = Desc.RegisterIndex;
	}

	return CResult();
}








iberbar::RHI::D3D9::CShaderReflectionType::CShaderReflectionType()
	: m_eVarType( UShaderVariableType::VT_Unknown )
	, m_eVarClass( UShaderVariableClass::SVC_Scalar )
	, m_nElementCount( 0 )
	, m_nRowCount( 0 )
	, m_nColumnCount( 0 )
	, m_strName()
	, m_ReflectionMembers()
{

}


iberbar::RHI::D3D9::CShaderReflectionType::~CShaderReflectionType()
{
	if ( m_ReflectionMembers.empty() == false )
	{
		for ( auto iter = m_ReflectionMembers.begin(), end = m_ReflectionMembers.end(); iter != end; iter++ )
		{
			SAFE_DELETE( *iter );
		}
		m_ReflectionMembers.clear();
	}
}


const iberbar::RHI::IShaderReflectionMember* iberbar::RHI::D3D9::CShaderReflectionType::GetMemberByIndex( int nIndex ) const
{
	assert( nIndex >= 0 && nIndex < (int)m_ReflectionMembers.size() );
	return m_ReflectionMembers[nIndex];
}


const iberbar::RHI::IShaderReflectionMember* iberbar::RHI::D3D9::CShaderReflectionType::GetMemberByName( const char* pstrName ) const
{
	assert( pstrName );
	for ( CShaderReflectionMember* pMemberTemp : m_ReflectionMembers )
	{
		if ( strcmp( pMemberTemp->GetName(), pstrName ) == 0 )
		{
			return pMemberTemp;
		}
	}
	return nullptr;
}


void iberbar::RHI::D3D9::CShaderReflectionType::GetDesc( UShaderReflectionTypeDesc* pOutDesc ) const
{
	assert( pOutDesc );
	pOutDesc->nVarType = m_eVarType;
	pOutDesc->nVarClass = m_eVarClass;
	pOutDesc->nElements = m_nElementCount;
	pOutDesc->nRows = m_nRowCount;
	pOutDesc->nColumns = m_nColumnCount;
	pOutDesc->pstrName = m_strName.c_str();
}


iberbar::CResult iberbar::RHI::D3D9::CShaderReflectionType::Initial( ID3DXConstantTable* pD3DConstantTable, D3DXHANDLE pD3DHandle, const D3DXCONSTANT_DESC& Desc )
{
	m_strName = Desc.Name;

	HRESULT hResult;
	CResult cResult;

	if ( Desc.Type == D3DXPT_VOID )
	{
		if ( Desc.Class == D3DXPC_STRUCT )
		{
			m_eVarType = UShaderVariableType::VT_Void;
		}
	}
	else
	{
		switch ( Desc.Type )
		{
			case D3DXPT_BOOL:
				m_eVarType = UShaderVariableType::VT_Boolean;
				break;

			case D3DXPT_INT:
				m_eVarType = UShaderVariableType::VT_Int;
				break;

			case D3DXPT_FLOAT:
				m_eVarType = UShaderVariableType::VT_Float;
				break;

			case D3DXPT_TEXTURE:
				m_eVarType = UShaderVariableType::VT_Texture;
				break;

			case D3DXPT_SAMPLER2D:
			{
				m_eVarType = UShaderVariableType::VT_Sampler2D;
				//D3DXHANDLE pD3DHandleTexture = pD3DConstantTable->GetConstantDesc( pD3DHandle, 1 );
				//D3DXCONSTANT_DESC DescTexture;
				//pD3DConstantTable->GetConstantDesc( pD3DHandleTexture, &DescTexture, nullptr );
				break;
			}
				

			case D3DXPT_VOID:
				m_eVarType = UShaderVariableType::VT_Void;
				break;

			default:
				break;
		}

		switch ( Desc.Class )
		{
			case D3DXPC_SCALAR:
				m_eVarClass = UShaderVariableClass::SVC_Scalar;
				break;

			case D3DXPC_VECTOR:
				m_eVarClass = UShaderVariableClass::SVC_Vector;
				break;

			case D3DXPC_MATRIX_COLUMNS:
				m_eVarClass = UShaderVariableClass::SVC_Matrix;
				break;

			case D3DXPC_STRUCT:
				m_eVarClass = UShaderVariableClass::SVC_Struct;
				break;

			default:
				break;
		}
	}
	
	m_nElementCount = Desc.Elements;
	m_nRowCount = Desc.Rows;
	m_nColumnCount = Desc.Columns;

	if ( Desc.StructMembers > 0 )
	{
		m_ReflectionMembers.resize( Desc.StructMembers, nullptr );

		D3DXHANDLE pD3DHandleMember = nullptr;
		D3DXCONSTANT_DESC DescMember;
		CShaderReflectionMember* pReflectionMember = nullptr;
		for ( UINT i = 0, s = Desc.StructMembers; i < s; i++ )
		{
			pReflectionMember = new CShaderReflectionMember();
			m_ReflectionMembers[i] = pReflectionMember;

			pD3DHandleMember = pD3DConstantTable->GetConstant( pD3DHandle, i );
			if ( pD3DHandleMember == nullptr )
				return MakeResult( ResultCode::Bad, " " );

			hResult = pD3DConstantTable->GetConstantDesc( pD3DHandleMember, &DescMember, nullptr );
			if ( FAILED( hResult ) )
				return MakeResult( ResultCode::Bad, DXGetErrorDescriptionA( hResult ) );

			cResult = pReflectionMember->Initial( pD3DConstantTable, pD3DHandleMember, DescMember );
			if ( cResult.IsOK() == false )
				return cResult;
		}
	}

	return CResult();
}





iberbar::RHI::D3D9::CShaderReflectionMember::CShaderReflectionMember()
	: m_strName()
	, m_eVarType( UShaderVariableType::VT_Unknown )
	, m_pReflectionType( nullptr )
	, m_nOffset( 0 )
{
}


iberbar::RHI::D3D9::CShaderReflectionMember::~CShaderReflectionMember()
{
	SAFE_DELETE( m_pReflectionType );
}


void iberbar::RHI::D3D9::CShaderReflectionMember::GetDesc( UShaderReflectionMemberDesc* pOutDesc ) const
{
	assert( pOutDesc );
	pOutDesc->nVarType = m_eVarType;
	pOutDesc->nOffset = m_nOffset;
	pOutDesc->pstrName = m_strName.c_str();
}


iberbar::CResult iberbar::RHI::D3D9::CShaderReflectionMember::Initial( ID3DXConstantTable* pD3DConstantTable, D3DXHANDLE pD3DHandle, const D3DXCONSTANT_DESC& Desc )
{
	m_strName = Desc.Name;
	m_eVarType = ConvertShaderParameterType( pD3DConstantTable, pD3DHandle, Desc, &m_pReflectionType );
	m_nOffset = 0;
	return CResult();
}







iberbar::RHI::D3D9::CShaderReflectionBuffer::CShaderReflectionBuffer()
	: m_nSize( 0 )
	, m_nSamplerCount( 0 )
	, m_Variables()
{
}


iberbar::CResult iberbar::RHI::D3D9::CShaderReflectionBuffer::Initial( ID3DXConstantTable* pD3DConstantTable )
{
	D3DXCONSTANTTABLE_DESC TableDesc;
	HRESULT hResult;
	CResult cResult;

	hResult = pD3DConstantTable->GetDesc( &TableDesc );
	if ( FAILED( hResult ) )
		return MakeResult( ResultCode::Bad, DXGetErrorDescriptionA( hResult ) );

	if ( TableDesc.Constants == 0 )
		return CResult();

	m_Variables.resize( TableDesc.Constants );

	D3DXHANDLE pHandle = nullptr;
	D3DXCONSTANT_DESC ConstDesc;
	UINT nCount;
	for ( UINT i = 0; i < TableDesc.Constants; i++ )
	{
		pHandle = pD3DConstantTable->GetConstant( nullptr, i );
		if ( pHandle == nullptr )
			return MakeResult( ResultCode::Bad, "" );

		hResult = pD3DConstantTable->GetConstantDesc( pHandle, &ConstDesc, &nCount );
		if ( FAILED( hResult ) )
			return MakeResult( ResultCode::Bad, DXGetErrorDescriptionA( hResult ) );

		cResult = m_Variables[ i ].Initial( pD3DConstantTable, pHandle, ConstDesc, 0 );
		if ( cResult.IsOK() == false )
			return cResult;

		m_nSize += ConstDesc.Bytes;
	}

	return CResult();
}


const iberbar::RHI::D3D9::CShaderReflectionVariable* iberbar::RHI::D3D9::CShaderReflectionBuffer::GetVariableByNameInternal( const char* pstrName ) const
{
	for ( int i = 0, s = m_Variables.size(); i < s; i++ )
	{
		if ( strcmp( m_Variables[ i ].GetName(), pstrName ) == 0 )
			return &m_Variables[ i ];
	}
	return nullptr;
}





iberbar::RHI::D3D9::CShaderReflection::CShaderReflection()
	: m_Buffer()
{
}


iberbar::CResult iberbar::RHI::D3D9::CShaderReflection::Initial( ID3DXConstantTable* pD3DConstantTable )
{
	return m_Buffer.Initial( pD3DConstantTable );
}


