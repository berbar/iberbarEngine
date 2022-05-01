
#include <iberbar/RHI/D3D11/Effect.h>
#include <iberbar/RHI/D3D11/ShaderState.h>
#include <iberbar/RHI/D3D11/ShaderReflection.h>
#include <iberbar/RHI/D3D11/ShaderVariables.h>
#include <iberbar/RHI/D3D11/Shader.h>
#include <iberbar/RHI/D3D11/Buffer.h>
#include <iberbar/RHI/D3D11/Device.h>





iberbar::RHI::D3D11::CEffect::CEffect( IShaderState* pShaderState )
	: IEffect()
	, m_pShaderState( nullptr )
	, m_UniformMemorys()
	, m_UniformBuffers()
	//, m_UniformBuffersDirty()
{
	memset( m_UniformMemorys, 0, sizeof( m_UniformMemorys ) );
	memset( m_UniformBuffers, 0, sizeof( m_UniformBuffers ) );
	//memset( m_UniformBuffersDirty, 0, sizeof( m_UniformBuffersDirty ) );

	CShader* pShader = nullptr;
	CShaderReflection* pReflection = nullptr;
	const CShaderReflectionBuffer* pReflectionBuffer = nullptr;
	int nBufferSizeTotalForShader;
	for ( int i = 0, s = (int)EShaderType::__Count; i < s; i++ )
	{
		pShader = m_pShaderState->GetShaderInternal( (EShaderType)i );
		if ( pShader == nullptr )
			continue;

		pReflection = pShader->GetReflectionInternal();
		if ( pShader == nullptr )
			continue;

		nBufferSizeTotalForShader = pReflection->GetBufferSizeTotal();
		if ( nBufferSizeTotalForShader > 0 )
		{
			m_UniformMemorys[ i ] = new uint8[ nBufferSizeTotalForShader ];
			memset( m_UniformMemorys[ i ], 0, nBufferSizeTotalForShader );
		}
		
		for ( int nBufferIndex = 0, nBufferCount = pReflection->GetBufferCountInternal(); nBufferIndex < nBufferCount; nBufferIndex++ )
		{
			pReflectionBuffer = pReflection->GetBufferByIndexInternal( nBufferIndex );
			if ( pReflectionBuffer == nullptr )
				continue;

			m_UniformBuffers[ i ][ pReflectionBuffer->GetBindPoint() ] = new CUniformBuffer( m_pShaderState->GetDevice(), pReflectionBuffer->GetSize() );
		}
	}
}


iberbar::RHI::D3D11::CEffect::~CEffect()
{
	UNKNOWN_SAFE_RELEASE_NULL( m_pShaderState );
	for ( int i = 0, s = (int)EShaderType::__Count; i < s; i++ )
	{
		SAFE_DELETE_ARRAY( m_UniformMemorys[ i ] );
		for ( int j = 0; j < D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT; j++ )
		{
			UNKNOWN_SAFE_RELEASE_NULL( m_UniformBuffers[ i ][ j ] );
		}
	}
	
}


void iberbar::RHI::D3D11::CEffect::SetShaderVariables( EShaderType nShaderType, IShaderVariableTable* pShaderVariables )
{
	if ( m_UniformMemorys[ (int)nShaderType ] == nullptr )
		return;

	CShaderVariableTable* pShaderVariablesInternal = (CShaderVariableTable*)pShaderVariables;
	assert( pShaderVariablesInternal->GetShaderInternal() == m_pShaderState->GetShaderInternal( nShaderType ) );
	assert( pShaderVariablesInternal->GetMemory() );
	

	CShader* pShader = m_pShaderState->GetShaderInternal( nShaderType );
	assert( pShader != nullptr );

	CShaderReflection* pShaderReflection = pShader->GetReflectionInternal();
	assert( pShaderReflection != nullptr );

	int nBufferCount = pShaderReflection->GetBufferCountInternal();
	assert( nBufferCount > 0 );

	int nBufferSizeTotal = pShaderReflection->GetBufferSizeTotal();
	assert( nBufferSizeTotal == pShaderVariablesInternal->GetMemorySize() );

	const uint8* pShaderVariablesMemory = pShaderVariablesInternal->GetMemory();
	if ( memcmp( m_UniformMemorys[ (int)nShaderType ], pShaderVariablesMemory, 0 ) == 0 )
		return;

	CUniformBuffer** pSlots = m_UniformBuffers[ (int)nShaderType ];
	CUniformBuffer* pSlotTemp;
	int nSlotIndex;
	const CShaderReflectionBuffer* pShaderReflectionBuffer = nullptr;
	int nBufferMemoryOffset = 0;
	int nBufferMemorySize = 0;
	for ( int i = 0, s = nBufferCount; i < s; i++ )
	{
		pShaderReflectionBuffer = pShaderReflection->GetBufferByIndexInternal( i );
		nBufferMemoryOffset = pShaderReflectionBuffer->GetOffset();
		nBufferMemorySize = pShaderReflectionBuffer->GetSize();
		if ( memcmp( m_UniformMemorys[ (int)nShaderType ] + nBufferMemoryOffset, pShaderVariablesMemory + nBufferMemoryOffset, nBufferMemorySize ) == 0 )
			continue;

		nSlotIndex = pShaderReflectionBuffer->GetBindPoint();
		pSlotTemp = pSlots[ nSlotIndex ];
		if ( pSlotTemp == nullptr )
			continue;

		pSlotTemp->UpdateContents( pShaderVariablesMemory + nBufferMemoryOffset, nBufferMemorySize );
		//m_UniformBuffersDirty[ (int)nShaderType ][ nSlotIndex ] = 1;
	}
}

