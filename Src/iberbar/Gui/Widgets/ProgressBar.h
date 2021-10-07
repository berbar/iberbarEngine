#pragma once

#include <iberbar/Gui/Widget.h>

namespace iberbar
{
	namespace Gui
	{
		class __iberbarGuiApi__ CProgressBar
			: public CWidget
		{
		public:
			CProgressBar();
			virtual ~CProgressBar();

		protected:
			CProgressBar( const CProgressBar& Other );

		public:
			virtual CProgressBar* Clone() const override { return new CProgressBar( *this ); }

		public:
			void SetProgressRenderElementRef( CRenderElement* pElement );
			void SetProgressValueMax( int nValueMax ) { m_nProgressValueMax = nValueMax; UpdateProgressRenderElement(); }
			void SetProgressValue( int nValue ) { m_nProgressValue = nValue; UpdateProgressRenderElement(); }
			int GetProgressValueMax() const { return m_nProgressValueMax; }
			int GetProgressValue() const { return m_nProgressValue; }

		protected:
			void UpdateProgressRenderElement();;


		protected:
			CRenderElement* m_pRenderElementRef_Progress;
			int m_nProgressValueMax;
			int m_nProgressValue;
		};
	}
}