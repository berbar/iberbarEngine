
#include <iberbar/Poster/Elements/ElementImage.h>
#include <iberbar/Poster/Sprite.h>



iberbar::Poster::CElementImage::CElementImage()
	: m_image( nullptr )
	, m_cornerRadius( 0 )
{
}

void iberbar::Poster::CElementImage::RenderSelf( CSurface* target )
{
	if ( m_image == nullptr )
		return;

	if ( m_cornerRadius == 0 )
	{
		DrawTexture( target, m_bounding, m_image );
	}
	else
	{
		DrawTextureInCornersBox( target, m_bounding, m_cornerRadius, m_image );
	}
}

