
//
//#include <iberbar/Gui/BaseTypes.h>
//#include <iberbar/Gui/Engine.h>
//#include <iberbar/Renderer/Renderer.h>
//#include <iberbar/Renderer/RendererSprite.h>
//#include <iberbar/RHI/ShaderState.h>
//#include <iberbar/RHI/ShaderVariables.h>
//
//#include <iberbar/Gui/Widgets/Container.h>
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//


// void lily::GuiCopyElementForWidget( const CGuiWidget* pSrc, CGuiWidget* pDest )
// {
// 	assert( pSrc );
// 	assert( pDest );
// 	if ( pSrc->getElementCount() > 0 )
// 	{
// 		uint32_t lc_copyCount = pSrc->getElementCount();
// 		pDest->createElements( lc_copyCount );
// 
// 		const CGuiElement* lc_temp = nullptr;
// 		CGuiElement_Ptr lc_clone;
// 		for ( uint32_t lc_i = 0; lc_i < lc_copyCount; lc_i ++ )
// 		{
// 			lc_clone = nullptr;
// 
// 			lc_temp = pSrc->getElement( lc_i );
// // 			if ( lc_temp )
// // 				lc_clone.attach( (CGuiElement*)lc_temp->clone() );
// 			if ( lc_temp )
// 				lc_clone = CGuiElement_Ptr::CloneInstance( lc_temp );
// 			pDest->setElement( lc_i, lc_clone );
// 			
// 		}
// 	}
// }

//
////--------------------------------------------------------------------------
//void iberbar::GuiWidget_RefreshChildren( CGuiWidget* pWidget )
//{
//	GuiWidgetVector lc_vector( pWidget );
//	GuiWidgetVector::iterator lc_iter = lc_vector.begin();
//	GuiWidgetVector::iterator lc_end = lc_vector.end();
//	for ( ; lc_iter != lc_end; ++ lc_iter )
//	{
//		lc_iter->refresh();
//	}
//}
//
//
////--------------------------------------------------------------------------
//void iberbar::GuiWidget_UpdateChildren( CGuiWidget* pWidget, float nElapsedTime )
//{
//	GuiWidgetVector lc_vector( pWidget );
//	GuiWidgetVector::iterator lc_iter = lc_vector.begin();
//	GuiWidgetVector::iterator lc_end = lc_vector.end();
//	for ( ; lc_iter != lc_end; ++ lc_iter )
//	{
//		//lc_iter->update( nElapsedTime );
//	}
//}
//
//
////--------------------------------------------------------------------------
//void iberbar::GuiWidget_RenderChildren( CGuiWidget* pWidget )
//{
//	GuiWidgetVector lc_vector( pWidget );
//	GuiWidgetVector::iterator lc_iter = lc_vector.begin();
//	GuiWidgetVector::iterator lc_end = lc_vector.end();
//	for ( ; lc_iter != lc_end; ++ lc_iter )
//	{
//		//if ( (*lc_iter) != CGuiWidget::sGetFocus() )
//			lc_iter->render();
//	}
//
//// 	if ( CGuiWidget::sGetFocus() &&
//// 		CGuiWidget::sGetFocus()->getParent() == pWidget )
//// 		CGuiWidget::sGetFocus()->render();
//}
//
