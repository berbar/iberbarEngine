
#pragma once

#include <iberbar/Gui/Element/BlendColor.h>
#include <iberbar/Gui/RenderElement.h>


namespace iberbar
{
	namespace Renderer
	{
		class CRenderer2d;
	}

	namespace RHI
	{
		class IShaderState;
	}

	namespace Gui
	{
		class __iberbarGuiApi__ CElementColorRect
			: public CRenderElement
		{
		public:
			CElementColorRect();
			virtual ~CElementColorRect();

		protected:
			CElementColorRect( const CElementColorRect& );

		public:
			virtual void Init() override;
			virtual CElementColorRect* Clone() const override { return new CElementColorRect( *this ); }
			virtual void Refresh() override;
			virtual void Update( float nElapsedTime ) override;
			virtual void Render() override;

		public:
			void SetColor( int state, CColor4B color ) { m_BlendColor.SetColor( state, color ); }

		protected:
			float        m_BlendColorRate;
			BlendColor   m_BlendColor;
		};


		IBERBAR_UNKNOWN_PTR_DECLARE( CElementColorRect );
	}
}
