//
//#include <iberbar/Gui/Commands.h>
//#include <iberbar/Gui/Dialog.h>
//
//
//
//
//
//iberbar::Gui::CCommand_AddDialog::CCommand_AddDialog( CDialog* pDlg )
//	: m_pDlg( pDlg )
//{
//	UNKNOWN_SAFE_ADDREF( m_pDlg );
//}
//
//
//iberbar::Gui::CCommand_AddDialog::~CCommand_AddDialog()
//{
//	UNKNOWN_SAFE_RELEASE_NULL( m_pDlg );
//}
//
//
//void iberbar::Gui::CCommand_AddDialog::Execute()
//{
//	CEngine::sGetInstance()->AddDialog( m_pDlg );
//}
//
//
//
//
//
//
//
//iberbar::Gui::CCommand_RemoveDialog::CCommand_RemoveDialog( CDialog* pDlg )
//	: m_pDlg( pDlg )
//{
//	UNKNOWN_SAFE_ADDREF( m_pDlg );
//}
//
//
//iberbar::Gui::CCommand_RemoveDialog::~CCommand_RemoveDialog()
//{
//	UNKNOWN_SAFE_RELEASE_NULL( m_pDlg );
//}
//
//
//void iberbar::Gui::CCommand_RemoveDialog::Execute()
//{
//	CEngine::sGetInstance()->RemoveDialog( m_pDlg );
//}
//
//
//
//
//
