
#include "./XmlProc.h"
#include <iberbar/Poster/ResourcesManager.h>
#include <iberbar/Poster/Elements/ElementLabel.h>
#include <iberbar/Poster/Elements/ElementImage.h>
#include <codecvt>



void iberbar::Poster::XmlReadProc_ElementBase( Xml::PTR_CNodeA node, CElementBase* pElement )
{
	const char* positionX = node->GetAttribute( "positionX" );
	const char* positionY = node->GetAttribute( "positionY" );
	const char* width = node->GetAttribute( "width" );
	const char* height = node->GetAttribute( "height" );

	CPoint2i position( 0, 0 );
	if ( positionX != nullptr )
		position.x = (int)strtol( positionX, nullptr, 10 );
	if ( positionY != nullptr )
		position.y = (int)strtol( positionY, nullptr, 10 );

	CSize2i size( 0, 0 );
	if ( width != nullptr )
		size.w = (int)strtol( width, nullptr, 10 );
	if ( height != nullptr )
		size.h = (int)strtol( height, nullptr, 10 );

	pElement->SetPosition( position );
	pElement->SetSize( size );
}

iberbar::CResult iberbar::Poster::XmlReadProc_ElementLabel( UXmlReadProcContext& context )
{
	const char* fontFace = context.node->GetAttribute( "fontFace" );
	const char* fontSizeStr = context.node->GetAttribute( "fontSize" );
	const char* fontWeightStr = context.node->GetAttribute( "fontWeight" );

	int fontSize = (fontSizeStr == nullptr) ? 10 : (int)strtol( fontSizeStr, nullptr, 10 );
	int fontWeight = 500;
	if ( fontWeightStr != nullptr )
	{
		if ( strcmp( fontWeightStr, "normal" ) == 0 )
			fontWeight = 500;
		else if ( strcmp( fontWeightStr, "light" ) == 0 )
			fontWeight = 100;
		else if ( strcmp( fontWeightStr, "bold" ) == 0 )
			fontWeight = 1000;
		else
			fontWeight = (int)strtol( fontWeightStr, nullptr, 10 );
	}

	CResult ret;

	PTR_CFont font = nullptr;
	ret = context.pResources->GetFont( fontFace, fontSize, fontWeight, &font );
	if ( ret.IsOK() == false )
		return MakeResult( ResultCode::Bad, "Failed to load font: %s", ret.data.c_str() );

	PTR_CElementLabel pElementLabel = PTR_CElementLabel::_sNew();
	pElementLabel->SetFont( font );

	const char* text = context.node->GetValueText();
	if ( text != nullptr )
	{
		std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> conv;
		pElementLabel->SetText( conv.from_bytes( text ).c_str() );
	}
	
	const char* foregroundColor = context.node->GetAttribute( "foregroundColor" );
	const char* backgroundColor = context.node->GetAttribute( "backgroundColor" );
	if ( foregroundColor != nullptr )
		pElementLabel->GetTextOptions()->foregroundColor = Color4BParse( foregroundColor );
	if ( backgroundColor != nullptr )
		pElementLabel->GetTextOptions()->backgroundColor = Color4BParse( backgroundColor );

	// 读取基本属性
	XmlReadProc_ElementBase( context.node, pElementLabel );
	
	(*context.ppOutElement) = (CElementLabel*)pElementLabel;
	(*context.ppOutElement)->AddRef();

	return CResult();
}


iberbar::CResult iberbar::Poster::XmlReadProc_ElementImage( UXmlReadProcContext& context )
{
	const char* src = context.node->GetAttribute( "src" );
	if ( src == nullptr )
		return MakeResult( ResultCode::Bad, "No src." );

	PTR_CSurface texture = nullptr;
	CResult ret = context.pResources->GetTextureFromFile( src, &texture, false );
	if ( ret.IsOK() == false )
		return MakeResult( ResultCode::Bad, "Failed to load image: %s", ret.data.c_str() );

	PTR_CElementImage pElementImage = PTR_CElementImage::_sNew();
	pElementImage->SetImage( texture );

	const char* cornerRadius = context.node->GetAttribute( "cornerRadius" );
	if ( cornerRadius != nullptr )
		pElementImage->SetCorner( (int)strtol( cornerRadius, nullptr, 10 ) );

	// 读取基本属性
	XmlReadProc_ElementBase( context.node, pElementImage );

	(*context.ppOutElement) = (CElementImage*)pElementImage;
	(*context.ppOutElement)->AddRef();

	return CResult();
}
