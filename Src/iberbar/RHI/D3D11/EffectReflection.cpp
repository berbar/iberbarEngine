
#include <iberbar/RHI/D3D11/EffectReflection.h>





iberbar::RHI::D3D11::CEffectReflection::CEffectReflection()
	: m_ReflectionVarArraySlots()
	, m_ReflectionCBufferArraySlots()
{
}


iberbar::CResult iberbar::RHI::D3D11::CEffectReflection::Initial( EShaderType eShaderType, ID3D11ShaderReflection* pD3DShaderReflection )
{
	HRESULT hResult;
	D3D11_SHADER_DESC ShaderDesc;
	D3D11_SHADER_INPUT_BIND_DESC ShaderInputBindDesc;
	ID3D11ShaderReflectionConstantBuffer* pShaderConstBuffer;
	D3D11_SHADER_BUFFER_DESC ShaderCBufferDesc;
	ID3D11ShaderReflectionVariable* pShaderVar;
	


	std::vector< UEffectReflectionVariable >& VarArray = m_ReflectionVarArraySlots[(int)eShaderType];
	std::vector< UEffectReflectionConstBuffer >& ConstBufferArray = m_ReflectionCBufferArraySlots[(int)eShaderType];
	int nCBufferOffset = 0;

	hResult = pD3DShaderReflection->GetDesc( &ShaderDesc );
	if ( FAILED( hResult ) )
		return MakeResult( ResultCode::Bad, "" );


	for ( int nResourceIndex = 0; nResourceIndex < ShaderDesc.BoundResources; nResourceIndex++ )
	{
		hResult = pD3DShaderReflection->GetResourceBindingDesc( nResourceIndex, &ShaderInputBindDesc );
		if ( FAILED( hResult ) )
			break;

		if ( ShaderInputBindDesc.Type == D3D_SIT_CBUFFER )
		{
			pShaderConstBuffer = pD3DShaderReflection->GetConstantBufferByName( ShaderInputBindDesc.Name );
			if ( pShaderConstBuffer == nullptr )
				continue;

			hResult = pShaderConstBuffer->GetDesc( &ShaderCBufferDesc );
			if ( FAILED( hResult ) )
				continue;

			UEffectReflectionConstBuffer ConstBuffer;
			ConstBuffer.nBindPoint = ShaderInputBindDesc.BindPoint;
			ConstBuffer.strName = ShaderCBufferDesc.Name;
			ConstBuffer.nOffset = nCBufferOffset;
			ConstBuffer.nSize = ShaderCBufferDesc.Size;
			nCBufferOffset += ConstBuffer.nSize;
			ConstBufferArray.push_back( ConstBuffer );

			for ( int nVarIndex = 0; nVarIndex < ShaderCBufferDesc.Variables; nVarIndex++ )
			{
				VarArray.push_back( sBuildReflectionVariable( ConstBuffer.nOffset, pShaderConstBuffer->GetVariableByIndex( nVarIndex ) ) );
			}
		}
		else if ( ShaderInputBindDesc.Type == D3D_SIT_TEXTURE )
		{

		}
		else if ( ShaderInputBindDesc.Type == D3D_SIT_SAMPLER )
		{

		}
	}
}


iberbar::RHI::D3D11::UEffectReflectionVariable iberbar::RHI::D3D11::CEffectReflection::sBuildReflectionVariable( uint32 nConstBufferBytesOffset, ID3D11ShaderReflectionVariable* pD3DShaderReflectionVariable )
{
	ID3D11ShaderReflectionType* pD3DReflectionType;
	D3D11_SHADER_VARIABLE_DESC ShaderVarDesc;
	D3D11_SHADER_TYPE_DESC D3DReflectionTypeDesc;

	pD3DShaderReflectionVariable->GetDesc( &ShaderVarDesc );
	pD3DReflectionType = pD3DShaderReflectionVariable->GetType();
	pD3DReflectionType->GetDesc( &D3DReflectionTypeDesc );

	UEffectReflectionVariable Var;
	Var.strName = ShaderVarDesc.Name;
	Var.nOffset = ShaderVarDesc.StartOffset;
	Var.nOffsetAbs = nConstBufferBytesOffset + ShaderVarDesc.StartOffset;
	Var.nTotalSize = ShaderVarDesc.Size;
	Var.nElementSize = D3DReflectionTypeDesc.Elements;
	Var.nElementCount = D3DReflectionTypeDesc.Elements;
	return Var;
}
