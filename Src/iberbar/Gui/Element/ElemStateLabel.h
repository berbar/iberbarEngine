
#pragma once

#include <iberbar/Gui/Element/BlendColor.h>
#include <iberbar/Gui/RenderElement.h>

namespace iberbar
{
	namespace Renderer
	{
		class CFont;
	}

	namespace Gui
	{
		class __iberbarGuiApi__ CElementStateLabel
			: public CRenderElement
		{
		public:
			CElementStateLabel( void );
			~CElementStateLabel();

		protected:
			CElementStateLabel( const CElementStateLabel& element );

		public:
			virtual CElementStateLabel* Clone() const override { return new CElementStateLabel( *this ); }

			virtual void Refresh() override;
			virtual void Update( float nElapsedTime ) override;
			virtual void Render() override;


		public:
			void SetFont( Renderer::CFont* pFont );
			void SetColor( int state, const CColor4B& color ) { m_BlendColor.SetColor( state, color ); }
			void SetTextA( const char* strText );
			void SetTextW( const wchar_t* strText );
			void SetTextAlignHorizental( UAlignHorizental nAlign ) { m_nAlignHorizental = nAlign; }
			void SetTextAlignVertical( UAlignVertical nAlign ) { m_nAlignVertical = nAlign; }

		protected:
			Renderer::CFont* m_pFont;
			float m_BlendColorRate;
			UAlignHorizental m_nAlignHorizental;
			UAlignVertical m_nAlignVertical;
			std::wstring m_strText;
			BlendColor m_BlendColor;
		};


		IBERBAR_UNKNOWN_PTR_DECLARE( CElementStateLabel );
	}

}


