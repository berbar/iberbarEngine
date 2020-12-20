#pragma once

#include <iberbar/Poster/Elements/ElementBase.h>

namespace iberbar
{
	namespace Poster
	{
		class CElementLabel;
		class CFont;

		IBERBAR_UNKNOWN_PTR_DECLARE( CElementLabel );

		class __iberbarExports__ CElementLabel
			: public CElementBase
		{
		public:
			struct TextOptions
			{
			public:
				UAlignHorizental alignHorizental;
				UAlignVertical alignVertical;
				std::u32string emphasis;
				int maxLine;
				CColor4B foregroundColor;
				CColor4B backgroundColor;

				TextOptions()
					: alignHorizental( UAlignHorizental::Left )
					, alignVertical( UAlignVertical::Top )
					, emphasis( U"..." )
					, maxLine( 1 )
					, foregroundColor( 0xff, 0xff, 0xff, 0xff )
					, backgroundColor( 0x00, 0x00, 0x00, 0x00 )
				{
				}
			};

		public:
			CElementLabel();
			virtual ~CElementLabel();

		public:
			virtual void RenderSelf( CSurface* target ) override;

		public:
			void SetFont( CFont* font );
			void SetText( const char32_t* text ) { m_text = text; }
			TextOptions* GetTextOptions() { return &m_textOptions; }
			const TextOptions* GetTextOptions() const { return &m_textOptions; }

		protected:
			CFont* m_font;
			std::u32string m_text;
			TextOptions m_textOptions;
			CRect2i m_textFixRenderArea;
		};
	}
}
