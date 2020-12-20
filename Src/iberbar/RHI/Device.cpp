
#include <iberbar/RHI/Device.h>



namespace iberbar
{
	namespace RHI
	{
		void DefaultRender( IDevice* pDevice )
		{
		}
	}
}


iberbar::RHI::IDevice::IDevice( UApiType nApiType )
	: CRef()
	, m_nApiType( nApiType )
	, m_bIsWindow( false )
	, m_ContextSize( 0, 0 )
	, m_ClearColor( 1.0f, 1.0f, 1.0f )
	, m_CallbackCreated()
	, m_CallbackLost()
	, m_CallbackReset()
	, m_CallbackRender( std::bind( &DefaultRender, std::placeholders::_1 ) )
{
}
