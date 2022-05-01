
#include <iberbar/Gui/Xml/XmlProc.h>
#include <iberbar/Gui/Xml/XmlState.h>
#include <iberbar/Gui/Xml/XmlProcHelper.h>


#include <iberbar/Gui/Widgets/Button.h>
#include <iberbar/Gui/Widgets/CheckBox.h>
#include <iberbar/Gui/Widgets/RadioBox.h>
#include <iberbar/Gui/Widgets/ListBox.h>
#include <iberbar/Gui/Widgets/EditBox.h>
#include <iberbar/Gui/Widgets/ProgressBar.h>

#include <iberbar/Gui/Element/ElemColorRect.h>
#include <iberbar/Gui/Element/ElemStateTexture.h>
#include <iberbar/Gui/Element/ElemStateLabel.h>

#include <iberbar/RHI/Texture.h>
#include <iberbar/Renderer/Font.h>

#include <iberbar/Utility/String.h>
#include <iberbar/Utility/StringConvert.h>
#include <iberbar/Utility/Log/OutputDevice.h>


void iberbar::Gui::XmlReadProc_Object( Xml::CNodeA* pXmlNode, CObject* pObject )
{
	const char* strId = pXmlNode->GetAttribute( "Id" );
	if ( strId != nullptr )
		pObject->SetId( strId );

	bool bVisible = Xml::GetAttributeBoolean( pXmlNode, "Visible", true, "True", "False" );
	pObject->SetVisible( bVisible );

	bool bEnable = Xml::GetAttributeBoolean( pXmlNode, "Enable", true, "True", "False" );
	pObject->SetEnable( bEnable );

	CPoint2i Position = pObject->GetPosition();
	const char* strX = pXmlNode->GetAttribute( "X" );
	const char* strY = pXmlNode->GetAttribute( "Y" );
	if ( StringIsNullOrEmpty( strX ) == false )
	{
		if ( tstring_find_ch( strX, '%' ) > 0 )
		{
			pObject->SetPercentX( true );
		}
		Position.x = strtol( strX, nullptr, 10 );
	}
	if ( StringIsNullOrEmpty( strY ) == false )
	{
		if ( tstring_find_ch( strY, '%' ) > 0 )
		{
			pObject->SetPercentY( true );
		}
		Position.y = strtol( strY, nullptr, 10 );
	}
	pObject->SetPosition( Position );

	CSize2i Size = pObject->GetSize();
	const char* strW = pXmlNode->GetAttribute( "Width" );
	const char* strH = pXmlNode->GetAttribute( "Height" );
	if ( StringIsNullOrEmpty( strW ) == false )
	{
		if ( tstring_find_ch( strW, '%' ) > 0 )
		{
			pObject->SetPercentW( true );
		}
		Size.w = strtol( strW, nullptr, 10 );
	}
	if ( StringIsNullOrEmpty( strH ) == false )
	{
		if ( tstring_find_ch( strH, '%' ) > 0 )
		{
			pObject->SetPercentH( true );
		}
		Size.h = strtol( strH, nullptr, 10 );
	}
	pObject->SetSize( Size );

	const char* strAlignHorizental = pXmlNode->GetAttribute( "AlignHorizental" );
	if ( StringIsNullOrEmpty( strAlignHorizental ) == false )
	{
		if ( strcmp( strAlignHorizental, "Left" ) == 0 )
			pObject->SetAlignHorizental( UAlignHorizental::Left );
		else if ( strcmp( strAlignHorizental, "Center" ) == 0 )
			pObject->SetAlignHorizental( UAlignHorizental::Center );
		else if ( strcmp( strAlignHorizental, "Right" ) == 0 )
			pObject->SetAlignHorizental( UAlignHorizental::Right );
	}

	const char* strAlignVertical = pXmlNode->GetAttribute( "AlignVertical" );
	if ( StringIsNullOrEmpty( strAlignVertical ) == false )
	{
		if ( strcmp( strAlignVertical, "Top" ) == 0 )
			pObject->SetAlignVertical( UAlignVertical::Top );
		else if ( strcmp( strAlignVertical, "Center" ) == 0 )
			pObject->SetAlignVertical( UAlignVertical::Center );
		else if ( strcmp( strAlignVertical, "Bottom" ) == 0 )
			pObject->SetAlignVertical( UAlignVertical::Bottom );
	}
}


void iberbar::Gui::XmlReadProc_Widget( const UXmlParserContext* pContext, Xml::CNodeA* pXmlNode, CWidget* pWidget, CRenderElement* pRenderElement )
{
	XmlReadProc_Object( pXmlNode, pWidget );

	const char* strAttrValue = nullptr;

	strAttrValue = pXmlNode->GetAttribute( "NeedClip" );
	if ( StringIsNullOrEmpty( strAttrValue ) == false )
	{
		if ( strcmp( strAttrValue, "True" ) == 0 )
			pWidget->SetNeedClip( true );
		else
			pWidget->SetNeedClip( false );
	}
}


void iberbar::Gui::XmlReadProc_Widget_Button( const UXmlParserContext* pContext, Xml::CNodeA* pXmlNode, CWidget* pWidget, CRenderElement* pRenderElement )
{
	XmlReadProc_Widget( pContext, pXmlNode, pWidget, pRenderElement );
}


void iberbar::Gui::XmlReadProc_Widget_CheckBox( const UXmlParserContext* pContext, Xml::CNodeA* pXmlNode, CWidget* pWidget, CRenderElement* pRenderElement )
{
	XmlReadProc_Widget_Button( pContext, pXmlNode, pWidget, pRenderElement );

	CCheckBox* pCheckBox = (CCheckBox*)pWidget;

	const char* strCheckedState = pXmlNode->GetAttribute( "Checked" );
	if ( StringIsNullOrEmpty( strCheckedState ) == false )
	{
		if ( strcmp( strCheckedState, "True" ) == 0 )
			pCheckBox->SetChecked( CCheckBox::UCheckState::CheckedTrue );
		else if ( strcmp( strCheckedState, "False" ) == 0 )
			pCheckBox->SetChecked( CCheckBox::UCheckState::CheckedFalse );
		else if ( strcmp( strCheckedState, "Indeterminate" ) == 0 )
			pCheckBox->SetChecked( CCheckBox::UCheckState::CheckedIndeterminate );
	}
	
	PTR_CRenderElement pElementTemp = nullptr;
	const char* strElementName = nullptr;
	
	strElementName = pXmlNode->GetAttribute( "Element-True" );
	pCheckBox->SetRenderElementTrue( strElementName );

	strElementName = pXmlNode->GetAttribute( "Element-False" );
	pCheckBox->SetRenderElementFalse( strElementName );

	strElementName = pXmlNode->GetAttribute( "Element-Indeterminate" );
	pCheckBox->SetRenderElementIndeterminate( strElementName );
}


void iberbar::Gui::XmlReadProc_Widget_RadioBox( const UXmlParserContext* pContext, Xml::CNodeA* pXmlNode, CWidget* pWidget, CRenderElement* pRenderElement )
{
	XmlReadProc_Widget_CheckBox( pContext, pXmlNode, pWidget, pRenderElement );
}


void iberbar::Gui::XmlReadProc_Widget_ListBox( const UXmlParserContext* pContext, Xml::CNodeA* pXmlNode, CWidget* pWidget, CRenderElement* pRenderElement )
{
	XmlReadProc_Widget( pContext, pXmlNode, pWidget, pRenderElement );

	CListBoxDictionary* pListBox = (CListBoxDictionary*)pWidget;

	const char* strAttrValue = nullptr;

	int nItemWidth = 0;
	int nItemHeight = 0;
	strAttrValue = pXmlNode->GetAttribute( "ItemWidth" );
	if ( StringIsNullOrEmpty( strAttrValue ) == false )
	{
		nItemWidth = strtol( strAttrValue, nullptr, 0 );
	}
	strAttrValue = pXmlNode->GetAttribute( "ItemHeight" );
	if ( StringIsNullOrEmpty( strAttrValue ) == false )
	{
		nItemHeight = strtol( strAttrValue, nullptr, 0 );
	}
	pListBox->SetItemSize( nItemWidth, nItemHeight );

	strAttrValue = pXmlNode->GetAttribute( "ItemDirection" );
	if ( StringIsNullOrEmpty( strAttrValue ) == false )
	{
		if ( strcmp( strAttrValue, "Vertical" ) == 0 )
			pListBox->SetItemDirection( UListBoxDirection::Vertical );
		else if ( strcmp( strAttrValue, "Horizental" ) == 0 )
			pListBox->SetItemDirection( UListBoxDirection::Horizental );
	}
	
	strAttrValue = pXmlNode->GetAttribute( "DragStyle" );
	if ( StringIsNullOrEmpty( strAttrValue ) == false )
	{
		if ( strcmp( strAttrValue, "DragView" ) == 0 )
			pListBox->SetDragStyle( EListBoxDragStyle::DragView );
		else if ( strcmp( strAttrValue, "DragItem" ) == 0 )
			pListBox->SetDragStyle( EListBoxDragStyle::DragItem );
		else
			pListBox->SetDragStyle( EListBoxDragStyle::None );
	}

	strAttrValue = pXmlNode->GetAttribute( "SelectType" );
	if ( StringIsNullOrEmpty( strAttrValue ) == false )
	{
		if ( strcmp( strAttrValue, "Single" ) == 0 )
			pListBox->SetSelectType( UListBoxSelectType::Single );
		else if ( strcmp( strAttrValue, "Multiple" ) == 00 )
			pListBox->SetSelectType( UListBoxSelectType::Multiple );
	}
}


void iberbar::Gui::XmlReadProc_Widget_EditBox( const UXmlParserContext* pContext, Xml::CNodeA* pXmlNode, CWidget* pWidget, CRenderElement* pRenderElement )
{
	XmlReadProc_Widget( pContext, pXmlNode, pWidget, pRenderElement );

	CEditBox* pEditBox = (CEditBox*)pWidget;

	PTR_CRenderElement pElementTemp = nullptr;
	const char* strElementName = nullptr;

	strElementName = pXmlNode->GetAttribute( "Element-TextField" );
	if ( StringIsNullOrEmpty( strElementName ) == false && pEditBox->FindElement( strElementName, &pElementTemp ) == true )
	{
		pEditBox->SetTextElementRef( (CEditBoxTextElementBase*)(CRenderElement*)pElementTemp );
	}
}


void iberbar::Gui::XmlReadProc_Widget_ProgressBar( const UXmlParserContext* pContext, Xml::CNodeA* pXmlNode, CWidget* pWidget, CRenderElement* pRenderElement )
{
	XmlReadProc_Widget( pContext, pXmlNode, pWidget, pRenderElement );

	CProgressBar* pProgressBar = (CProgressBar*)pWidget;

	
	const char* strAttrValue = nullptr;

	PTR_CRenderElement pElementTemp = nullptr;
	strAttrValue = pXmlNode->GetAttribute( "Element-Progress" );
	if ( StringIsNullOrEmpty( strAttrValue ) == false && pProgressBar->FindElement( strAttrValue, &pElementTemp ) == true )
	{
		pProgressBar->SetProgressRenderElementRef( (CRenderElement*)pElementTemp );
	}

	strAttrValue = pXmlNode->GetAttribute( "ProgressValueMax" );
	if ( StringIsNullOrEmpty( strAttrValue ) == false )
	{
		pProgressBar->SetProgressValueMax( strtol( strAttrValue, nullptr, 10 ) );
	}

	strAttrValue = pXmlNode->GetAttribute( "ProgressValue" );
	if ( StringIsNullOrEmpty( strAttrValue ) == false )
	{
		pProgressBar->SetProgressValue( strtol( strAttrValue, nullptr, 10 ) );
	}
}


iberbar::Gui::CWidget* iberbar::Gui::XmlCreateProc_Widget( const char* strType )
{
	return new CWidget();
}


iberbar::Gui::CWidget* iberbar::Gui::XmlCreateProc_Widget_Button( const char* strType )
{
	return new CButton();
}


iberbar::Gui::CWidget* iberbar::Gui::XmlCreateProc_Widget_CheckBox( const char* strType )
{
	return new CCheckBox();
}


iberbar::Gui::CWidget* iberbar::Gui::XmlCreateProc_Widget_RadioBox( const char* strType )
{
	return new CRadioBox();
}


iberbar::Gui::CWidget* iberbar::Gui::XmlCreateProc_Widget_ListBox( const char* strType )
{
	return new CListBoxDictionary();
}


iberbar::Gui::CWidget* iberbar::Gui::XmlCreateProc_Widget_EditBox( const char* strType )
{
	return new CEditBox();
}


iberbar::Gui::CWidget* iberbar::Gui::XmlCreateProc_Widget_ProgressBar( const char* strType )
{
	return new CProgressBar();
}


void iberbar::Gui::XmlReadProc_Element( const UXmlParserContext* pContext, Xml::CNodeA* pXmlNode, CRenderElement* pElement )
{
	XmlReadProc_Object( pXmlNode, pElement );

	const char* strZOrder = pXmlNode->GetAttribute( "ZOrder" );
	if ( StringIsNullOrEmpty( strZOrder ) == false )
	{
		int nZOrder = (int)strtol( strZOrder, nullptr, 10 );
		pElement->SetZOrder( nZOrder );
	}
}


void iberbar::Gui::XmlReadProc_Element_ColorRect( const UXmlParserContext* pContext, Xml::CNodeA* pXmlNode, CRenderElement* pElement )
{
	XmlReadProc_Element( pContext, pXmlNode, pElement );

	CElementColorRect* pElementColorRect = (CElementColorRect*)pElement;

	Xml::PTR_CNodeListA pNodeList_Color;
	pXmlNode->SelectNodes( "Color", &pNodeList_Color );
	if ( pNodeList_Color && pNodeList_Color->GetNodeCount() )
	{
		Xml::PTR_CNodeA pNode = nullptr;
		const char* strStateName = nullptr;
		const char* strColor = nullptr;
		int nState = -1;
		CColor4B color;
		for ( int i = 0, s = pNodeList_Color->GetNodeCount(); i < s; i++ )
		{
			pNodeList_Color->GetNodeAt( i, &pNode );
			strStateName = pNode->GetAttribute( "State" );
			strColor = pNode->GetAttribute( "Value" );
			if ( StringIsNullOrEmpty( strStateName ) == false && StringIsNullOrEmpty( strColor ) == false )
			{
				nState = XmlAttributeConvertToWidgetState( strStateName );
				color = XmlAttributeConvertToColor4B( strColor );
				pElementColorRect->SetColor( nState, color );
			}
		}
	}
}


void iberbar::Gui::XmlReadProc_Element_StateTexture( const UXmlParserContext* pContext, Xml::CNodeA* pXmlNode, CRenderElement* pElement )
{
	XmlReadProc_Element( pContext, pXmlNode, pElement );

	CElementStateTexture* pElementStateTexture = (CElementStateTexture*)pElement;

	Xml::PTR_CNodeListA pNodeList;
	pXmlNode->SelectNodes( "Texture", &pNodeList );
	if ( pNodeList && pNodeList->GetNodeCount() )
	{
		Xml::PTR_CNodeA pNode = nullptr;
		//const char* strStateName = nullptr;
		const char* strColor = nullptr;
		const char* strSrc = nullptr;
		const char* strUV = nullptr;
		//int nState = -1;
		CColor4B color;
		TSmartRefPtr<RHI::ITexture> pTexture = nullptr;
		CRect2f rcUV;
		for ( int i = 0, s = pNodeList->GetNodeCount(); i < s; i++ )
		{
			pNodeList->GetNodeAt( i, &pNode );
			//strStateName = pNode->GetAttribute( "State" );
			strColor = pNode->GetAttribute( "Color" );
			strSrc = pNode->GetAttribute( "Src" );
			strUV = pNode->GetAttribute( "UV" );
			color = XmlAttributeConvertToColor4B( strColor );
			pContext->GetTexture( strSrc, &pTexture );
			rcUV = XmlAttributeConvertToUV( strUV );
			pElementStateTexture->SetColor( nState, color );
			pElementStateTexture->SetTexture( nState, pTexture );
			pElementStateTexture->SetUV( nState, rcUV );
			//if ( StringIsNullOrEmpty( strStateName ) == false && StringIsNullOrEmpty( strColor ) == false )
			//{
			//	nState = XmlAttributeConvertToWidgetState( strStateName );
			//	color = XmlAttributeConvertToColor4B( strColor );
			//	pContext->GetTexture( strSrc, &pTexture );
			//	rcUV = XmlAttributeConvertToUV( strUV );
			//	pElementStateTexture->SetColor( nState, color );
			//	pElementStateTexture->SetTexture( nState, pTexture );
			//	pElementStateTexture->SetUV( nState, rcUV );
			//}
		}
	}
}


void iberbar::Gui::XmlReadProc_Element_StateLabel( const UXmlParserContext* pContext, Xml::CNodeA* pXmlNode, CRenderElement* pElement )
{
	XmlReadProc_Element( pContext, pXmlNode, pElement );

	CElementStateLabel* pElementLabel = (CElementStateLabel*)pElement;

	Xml::PTR_CNodeA pNode_Font;
	pXmlNode->SelectSingleNode( "Font", &pNode_Font );
	if ( pNode_Font != nullptr )
	{
		UFontDesc FontDesc;
		FontDesc.Size = 20;
		FontDesc.Weight = 500;
		FontDesc.Italic = 0;

		const char* strFontFamily = pNode_Font->GetAttribute( "Family" );
		if ( StringIsNullOrEmpty( strFontFamily ) == false )
		{
			FontDesc.FamilyName = strFontFamily;
			FontDesc.Size = Xml::GetAttributeUInt32( pNode_Font, "Size", 20 );
			const char* strWeight = pNode_Font->GetAttribute( "Weight" );
			if ( strWeight != nullptr )
			{
				if ( strcmp( strWeight, "Bold" ) == 0 )
					FontDesc.Weight = 1000;
				else if ( strcmp( strWeight, "Normal" ) == 0 )
					FontDesc.Weight = 500;
			}
			TSmartRefPtr<Renderer::CFont> pFont;
			CResult Ret = pContext->GetFont( &pFont, FontDesc );
			if ( Ret.IsOK() == false )
			{
				pContext->pLogger->WriteFormat(
					Logging::ULevel::Warn,
					"GuiXmlParser",
					"Failed to get font(family=%s,size=%d,weight=%s): %s",
					strFontFamily,
					FontDesc.Size,
					strWeight,
					Ret.data.c_str() );
			}
			else if ( pFont == nullptr )
			{
				pContext->pLogger->WriteFormat(
					Logging::ULevel::Warn,
					"GuiXmlParser",
					"Failed to get font(family=%s,size=%d,weight=%s): no match",
					strFontFamily,
					FontDesc.Size,
					strWeight );
			}
			else
			{
				pElementLabel->SetFont( pFont );
			}
		}
	}

	Xml::PTR_CNodeListA pNodeList;
	pXmlNode->SelectNodes( "Color", &pNodeList );
	if ( pNodeList && pNodeList->GetNodeCount() )
	{
		Xml::PTR_CNodeA pNode = nullptr;
		const char* strStateName = nullptr;
		const char* strColor = nullptr;
		int nState = -1;
		CColor4B color;
		for ( int i = 0, s = pNodeList->GetNodeCount(); i < s; i++ )
		{
			pNodeList->GetNodeAt( i, &pNode );
			strStateName = pNode->GetAttribute( "State" );
			strColor = pNode->GetAttribute( "Value" );
			if ( StringIsNullOrEmpty( strStateName ) == false && StringIsNullOrEmpty( strColor ) == false )
			{
				nState = XmlAttributeConvertToWidgetState( strStateName );
				color = XmlAttributeConvertToColor4B( strColor );
				pElementLabel->SetColor( nState, color );
			}
		}
	}

	Xml::PTR_CNodeA pNode_Text;
	pXmlNode->SelectSingleNode( "Text", &pNode_Text );
	if ( pNode_Text )
	{
		const char* strAttrValue = nullptr;

		strAttrValue = pNode_Text->GetAttribute( "TextAlignHorizental" );
		if ( StringIsNullOrEmpty( strAttrValue ) == false )
		{
			UAlignHorizental nAlign = UAlignHorizental::Left;
			if ( XmlProcHelper::XmlAttributeConvertToAlignHorizental( strAttrValue, &nAlign ) )
				pElementLabel->SetTextAlignHorizental( nAlign );
		}

		strAttrValue = pNode_Text->GetAttribute( "TextAlignVertical" );
		if ( StringIsNullOrEmpty( strAttrValue ) == false )
		{
			UAlignVertical nAlign = UAlignVertical::Top;
			if ( XmlProcHelper::XmlAttributeConvertToAlignVertical( strAttrValue, &nAlign ) )
				pElementLabel->SetTextAlignVertical( nAlign );
		}

		const char* strText = pNode_Text->GetValueText();
		if ( StringIsNullOrEmpty( strText ) == false )
		{
			pElementLabel->SetTextA( strText );
		}
	}
}


void iberbar::Gui::XmlReadProc_Element_EditBoxText( const UXmlParserContext* pContext, Xml::CNodeA* pXmlNode, CRenderElement* pElement )
{
	XmlReadProc_Element( pContext, pXmlNode, pElement );

	CEditBoxTextElement* pTextElement = (CEditBoxTextElement*)pElement;

	const char* strAttrValue = nullptr;

	Xml::PTR_CNodeA pXmlNode_TextField = nullptr;
	
	pXmlNode->SelectSingleNode( "TextField", &pXmlNode_TextField );

	if ( pXmlNode_TextField != nullptr )
	{
		strAttrValue = pXmlNode_TextField->GetAttribute( "TextColor" );
		if ( StringIsNullOrEmpty( strAttrValue ) == false )
		{
			pTextElement->SetTextColor( XmlAttributeConvertToColor4B( strAttrValue ) );
		}

		strAttrValue = pXmlNode_TextField->GetAttribute( "SelTextColor" );
		if ( StringIsNullOrEmpty( strAttrValue ) == false )
		{
			pTextElement->SetSelTextColor( XmlAttributeConvertToColor4B( strAttrValue ) );
		}

		strAttrValue = pXmlNode_TextField->GetAttribute( "SelBgColor" );
		if ( StringIsNullOrEmpty( strAttrValue ) == false )
		{
			pTextElement->SetSelBgColor( XmlAttributeConvertToColor4B( strAttrValue ) );
		}

		strAttrValue = pXmlNode_TextField->GetAttribute( "CaretColor" );
		if ( StringIsNullOrEmpty( strAttrValue ) == false )
		{
			pTextElement->SetCaretColor( XmlAttributeConvertToColor4B( strAttrValue ) );
		}

		strAttrValue = pXmlNode_TextField->GetAttribute( "BgZOrder" );
		if ( StringIsNullOrEmpty( strAttrValue ) == false )
		{
			pTextElement->SetBgZOrder( (int)strtol( strAttrValue, nullptr, 10 ) );
		}

		strAttrValue = pXmlNode_TextField->GetAttribute( "TextAlignHorizental" );
		if ( StringIsNullOrEmpty( strAttrValue ) == false )
		{
			UAlignHorizental nAlign = UAlignHorizental::Left;
			if ( XmlProcHelper::XmlAttributeConvertToAlignHorizental( strAttrValue, &nAlign ) )
				pTextElement->SetTextAlignHorizental( nAlign );
		}
	}

	Xml::PTR_CNodeA pNode_Font;
	pXmlNode->SelectSingleNode( "Font", &pNode_Font );
	if ( pNode_Font != nullptr )
	{
		UFontDesc FontDesc;
		FontDesc.Size = 20;
		FontDesc.Weight = 500;
		FontDesc.Italic = 0;

		if ( XmlProcHelper::XmlNodeReadFontDesc( pNode_Font, &FontDesc ) == true )
		{
			TSmartRefPtr<Renderer::CFont> pFont;
			if ( pContext->GetFont( &pFont, FontDesc ).IsOK() )
			{
				pTextElement->SetFont( pFont );
			}
		}
	}
}


iberbar::Gui::CRenderElement* iberbar::Gui::XmlCreateProc_Element( const char* strType )
{
	return new CRenderElement();
}


iberbar::Gui::CRenderElement* iberbar::Gui::XmlCreateProc_Element_ColorRect( const char* strType )
{
	return new CElementColorRect();
}


iberbar::Gui::CRenderElement* iberbar::Gui::XmlCreateProc_Element_StateTexture( const char* strType )
{
	return new CElementStateTexture();
}


iberbar::Gui::CRenderElement* iberbar::Gui::XmlCreateProc_Element_StateLabel( const char* strType )
{
	return new CElementStateLabel();
}


iberbar::Gui::CRenderElement* iberbar::Gui::XmlCreateProc_Element_EditBoxText( const char* strType )
{
	return new CEditBoxTextElement();
}


int iberbar::Gui::XmlAttributeConvertToWidgetState( const char* strStateName )
{
	if ( strcmp( strStateName, "All" ) == 0 )
		return -1;
	if ( strcmp( strStateName, "Normal" ) == 0 )
		return (int)UWidgetState::Normal;
	if ( strcmp( strStateName, "MouseOver" ) == 0 )
		return (int)UWidgetState::MouseOver;
	if ( strcmp( strStateName, "Focus" ) == 0 )
		return (int)UWidgetState::Focus;
	if ( strcmp( strStateName, "Pressed" ) == 0 )
		return (int)UWidgetState::Pressed;
	if ( strcmp( strStateName, "Disabled" ) == 0 )
		return (int)UWidgetState::Disabled;
	if ( strcmp( strStateName, "Hidden" ) == 0 )
		return (int)UWidgetState::Hidden;
	return -2;
}


iberbar::CColor4B iberbar::Gui::XmlAttributeConvertToColor4B( const char* strColor )
{
	return StringConvertToColor( strColor );
}


iberbar::CRect2f iberbar::Gui::XmlAttributeConvertToUV( const char* strUV )
{
	CStringEasySplitHelper<char, 32, 4> splitHelper;
	int nSplitCount = splitHelper.Split( strUV, ',' );
	//int nSplitCount = TStringSplit( strUV, ',', &ppSplits, 32, 4 );
	//if ( nSplitCount == 2 )
	//{
	//	float nXY = strtof( ppSplits[ 0 ], nullptr );
	//	float nWH = strtof( ppSplits[ 1 ], nullptr );
	//	return CRect2f( CPoint2f( nXY, nXY ), CSize2f( nWH, nWH ) );
	//}
	//else if ( nSplitCount == 3 )
	//{
	//	float nX = strtof( ppSplits[ 0 ], nullptr );
	//	float nY = strtof( ppSplits[ 1 ], nullptr );
	//	float nWH = strtof( ppSplits[ 2 ], nullptr );
	//	return CRect2f( CPoint2f( nX, nY ), CSize2f( nWH, nWH ) );
	//}
	if ( nSplitCount == 4 )
	{
		float nL = strtof( splitHelper.Buffers[ 0 ], nullptr );
		float nT = strtof( splitHelper.Buffers[ 1 ], nullptr );
		float nR = strtof( splitHelper.Buffers[ 2 ], nullptr );
		float nB = strtof( splitHelper.Buffers[ 3 ], nullptr );
		return CRect2f( nL, nT, nR, nB );
	}
	return CRect2f( 0, 0, 0, 0 );
}



