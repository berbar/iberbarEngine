
#pragma once


#include <iberbar/Gui/Widget.h>



namespace iberbar
{
	namespace Gui
	{
		class __iberbarGuiApi__ CContainer
			: public CWidget
		{
		public:
			CContainer();
		protected:
			CContainer( const CContainer& container );
			virtual ~CContainer();

		public:
			virtual CContainer* Clone() const override;
			virtual const char* GetWidgetType() override { return "Container"; }
			virtual bool IsContainer() const override { return true; }
			virtual void SetDialog( CDialog* pDialog ) override;

		public:
			void AddWidget( CWidget* pWidget );
			CWidget* FindWidget( const char* strId );
			void RemoveWidget( CWidget* pWidget );
			void RemoveWidgetsAll();
			int GetWidgetCount() const { return (int)m_Widgets.size(); }
			CWidget* GetWidgetAt( int nIndex ) { return m_Widgets[nIndex]; }
			void ForeachWidgets( std::function<void( CWidget* )> Func, int nDepth = -1 );


		protected:
			std::vector<CWidget*> m_Widgets;
		};
	}
}

