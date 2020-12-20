//#pragma once
//
//#include <iberbar/Gui/Headers.h>
//#include <iberbar/Gui/Engine.h>
//#include <iberbar/Utility/Command.h>
//#include <functional>
//
//namespace iberbar
//{
//	namespace Gui
//	{
//		class __iberbarGuiApi__ CCommand_AddDialog final
//			: public CBaseCommand
//		{
//			iberbarGuiOverride_OperatorNewAndDelete_1( CCommand_AddDialog )
//		public:
//			CCommand_AddDialog( CDialog * pDialog );
//			virtual ~CCommand_AddDialog();
//			virtual void Execute() override;
//		private:
//			CDialog* m_pDlg;
//		};
//
//
//		class __iberbarGuiApi__ CCommand_RemoveDialog final
//			: public CBaseCommand
//		{
//			iberbarGuiOverride_OperatorNewAndDelete_1( CCommand_RemoveDialog )
//		public:
//			CCommand_RemoveDialog( CDialog * pDialog );
//			virtual ~CCommand_RemoveDialog();
//			virtual void Execute() override;
//		private:
//			CDialog* m_pDlg;
//		};
//
//
//
//	}
//}