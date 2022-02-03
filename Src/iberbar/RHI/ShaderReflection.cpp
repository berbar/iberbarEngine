
#include <iberbar/RHI/ShaderReflection.h>




iberbar::RHI::IShaderReflectionBuffer::IShaderReflectionBuffer()
	: m_nBindPoint( 0 )
	, m_nOffset( 0 )
	, m_nSize( 0 )
	, m_nVariableCount( 0 )
	, m_strName()
{
}



iberbar::RHI::IShaderReflectionVariable::IShaderReflectionVariable()
	: m_nOffsetInBuffer( 0 )
	, m_nOffset( 0 )
	, m_nTotalSize( 0 )
	, m_strName()
{

}




iberbar::RHI::IShaderReflectionBindResource::IShaderReflectionBindResource()
	: m_nVarType( UShaderVariableType::VT_Unknown )
	, m_nBindSlot( -1 )
	, m_nBindCount( 0 )
	, m_strName()
{
}



iberbar::RHI::IShaderReflectionType::IShaderReflectionType()
	: m_nVarType( UShaderVariableType::VT_Unknown )
	, m_nVarClass( UShaderVariableClass::SVC_Scalar )
	, m_nElementCount( 0 )
	, m_nRowCount( 0 )
	, m_nColumnCount( 0 )
	, m_nMemberCount( 0 )
	, m_strName()
{

}




iberbar::RHI::IShaderReflectionMember::IShaderReflectionMember()
	: m_nOffset( 0 )
	, m_strName()
{
}




iberbar::RHI::IShaderReflection::IShaderReflection()
	: m_nBufferSizeTotal( 0 )
	, m_nBufferCount( 0 )
	, m_nTextureCountTotal( 0 )
	, m_nSamplerStateCountTotal( 0 )
	, m_nInputParametersCount( 0 )
	, m_InputParametersDesc()
{
	memset( m_InputParametersDesc, 0, sizeof( m_InputParametersDesc ) );
}