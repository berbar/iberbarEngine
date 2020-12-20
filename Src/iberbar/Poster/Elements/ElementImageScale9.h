#pragma once

#include <iberbar/Poster/Elements/ElementBase.h>
#include <iberbar/Poster/TextureScale9.h>

namespace iberbar
{
	namespace Poster
	{
		class CElementImageScale9;

		IBERBAR_UNKNOWN_PTR_DECLARE( CElementImageScale9 );

		class __iberbarExports__ CElementImageScale9
			: public CElementBase
		{
		public:
			CElementImageScale9();

		public:
			virtual void RenderSelf( CSurface* target ) override;

		public:
			CResult SetImage( const char* imageName );
			void SetCorner( int radius ) { m_cornerRadius = radius; }

		protected:
			std::string m_imageName;
			PTR_CTextureScale9 m_image;
			int m_cornerRadius;
		};
	}
}
