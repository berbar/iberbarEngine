#pragma once

#include <iberbar/Poster/Elements/ElementBase.h>

namespace iberbar
{
	namespace Poster
	{
		class CElementImage;

		IBERBAR_UNKNOWN_PTR_DECLARE( CElementImage );

		class __iberbarExports__ CElementImage
			: public CElementBase
		{
		public:
			CElementImage();

		public:
			virtual void RenderSelf( CSurface* target ) override;

		public:
			void SetImage( CSurface* image ) { m_image = image; }
			void SetCorner( int radius ) { m_cornerRadius = radius; }

		protected:
			PTR_CSurface m_image;
			int m_cornerRadius;
		};
	}
}
