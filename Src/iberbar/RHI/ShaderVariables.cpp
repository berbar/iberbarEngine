

#include <iberbar/RHI/ShaderVariables.h>
#include <iberbar/RHI/Texture.h>




//
//
//
//
//iberbar::RHI::CShaderVariables::CShaderVariables()
//	: m_Bools()
//	, m_Samplers()
//{
//}
//
//
//iberbar::RHI::CShaderVariables::~CShaderVariables()
//{
//	Clean();
//}
//
//
//iberbar::RHI::CShaderVariables::CShaderVariables( const CShaderVariables& vars )
//	: m_Bools( vars.m_Bools )
//	, m_Samplers( vars.m_Samplers )
//{
//}
//
//
//void iberbar::RHI::CShaderVariables::Clean()
//{
//	m_Bools.clear();
//	m_Samplers.clear();
//}
//
//
//void iberbar::RHI::CShaderVariables::SetBool( const char* strName, bool Value )
//{
//	if ( FindKeyAndValue( m_Bools, strName ) == nullptr )
//	{
//		UShaderVariableNode<bool> node{ strName, Value };
//		m_Bools.push_back( node );
//	}
//}
//
//
//void iberbar::RHI::CShaderVariables::SetSampler( const char* strName, RHI::ITexture* pTexture, UTextureSamplerState SamplerState )
//{
//	if ( FindKeyAndValue( m_Samplers, strName ) == nullptr )
//	{
//		UShaderVariableNode<UShaderSampler> node { strName };
//		node.Value.pTexture = pTexture;
//		node.Value.State = SamplerState;
//		m_Samplers.push_back( node );
//	}
//}




