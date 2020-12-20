#pragma once

#include <iberbar/Utility/Unknown.h>
#include <iberbar/Utility/Clonable.h>
#include <iberbar/Utility/Rect.h>
#include <iberbar/Utility/Result.h>
#include <iberbar/Poster/Surface.h>

namespace iberbar
{
	namespace Poster
	{
		class CElementBase;

		IBERBAR_UNKNOWN_PTR_DECLARE( CElementBase );

		class __iberbarExports__ CElementBase
			: public CRef
			//, public IClonable
		{
		public:
			CElementBase();
			virtual ~CElementBase() {}

		public:
			void SetPosition( const CPoint2i& position ) { m_position = position; }
			void SetSize( const CSize2i& size ) { m_nDataSize = size; }
			void SetAlign( UAlignHorizental h, UAlignVertical v, UAlignStyle s = UAlignStyle::Inner ) { m_alignStyle = s; m_alignHorizental = h; m_alignVertical = v; }

		public:
			void Render( CSurface* target );
			bool AddSubElement( CElementBase* element );
			const CRect2i& GetBounding() { return m_bounding; }

		protected:
			virtual void UpdateBounding();
			virtual void RenderSelf( CSurface* target ) = 0;
			void RenderSubElements( CSurface* target );

		protected:
			CPoint2i m_position;
			CSize2i m_nDataSize;
			UAlignStyle m_alignStyle;
			UAlignHorizental m_alignHorizental;
			UAlignVertical m_alignVertical;
			CRect2i m_bounding;

		private:
			CElementBase* m_parentElement;
			std::vector< PTR_CElementBase > m_subElements;
		};

		class CElementEmpty;
		IBERBAR_UNKNOWN_PTR_DECLARE( CElementEmpty );

		class __iberbarExports__ CElementEmpty
			: public CElementBase
		{
		public:
			virtual void RenderSelf( CSurface* target ) override {}
		};
	}
}
