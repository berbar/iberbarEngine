
#include <iberbar/RHI/Texture.h>


iberbar::RHI::ITexture::ITexture( void )
	: IResource( UResourceType::Texture )
	, m_Format( 0 )
	, m_Size( 0, 0 )
{
}