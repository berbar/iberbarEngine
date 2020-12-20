#pragma once

#include <iberbar/Font/Headers.h>

#undef DrawText
#undef GetCharWidth

namespace iberbar
{
	enum class UFontBitsFormat
	{
		ARGB,
		BGRA,
		RGBA
	};

	struct UFontDesc
	{
		//int Height;
		//int Width;
		int Size;
		int Weight;
		int Italic;
		//uint8 CharSet;
		//uint8 OutputPrecision;
		//uint8 Quality;
		//uint8 PitchAndFamily;
		std::string FamilyName;// 可以是文件
	};


	namespace TextDraw
	{
		// 文本换行方式
		enum class UFontDrawTextWorkBreak
		{
			// 从不换行
			NoBreak,

			// 只有换行符才换行
			Hyphenate,

			// 换行符或遇到边界时换行
			BreakAll
		};
	}



	// 文本溢出时的处理方式
	enum class UFontDrawTextOverflow
	{
		// 无视溢出
		Unset,

		// 裁剪，完全溢出的字符不再渲染，半溢出的字符只渲染在区域内的部分
		Clip,

		// 溢出的字符不渲染，结尾用特定文本代替
		Ellipsis,

		// 隐藏，完全溢出和半溢出的字符都不渲染
		Hidden
	};

	struct UFontDrawTextSpecialStyle
	{
		int nStart;
		int nCount;
		CColor4B nColor;
	};




	namespace TextDraw
	{
		struct UFontDrawTextOptions
		{
		public:
			typedef const UFontDrawTextSpecialStyle _SpecialStyle;
		public:
			UFontDrawTextOptions()
				: alignHorizental( UAlignHorizental::Left )
				, alignVertical( UAlignVertical::Top )
				, nWrapType( UFontDrawTextWorkBreak::NoBreak )
				, overflow( UFontDrawTextOverflow::Clip )
				, calculateRect( false )
				, bRenderText( true )
				, lineHeight( 0 )
				, ellipsis( nullptr )
				, maxLine( 0 )
				, pStyleSelected( nullptr )
			{
			}

			UAlignHorizental alignHorizental; // 水平对齐
			UAlignVertical alignVertical; // 垂直对齐
			UFontDrawTextWorkBreak nWrapType; // 换行方式
			UFontDrawTextOverflow overflow; // 文本移除处理方式
			bool calculateRect; // 是否计算绘制后的矩形区域
			bool bRenderText;
			int lineHeight; // 行高
			const wchar_t* ellipsis;
			int maxLine; // 最对行数绘制，若超出，末尾将以ellipsis展示
			const UFontDrawTextSpecialStyle* pStyleSelected;
		};


		template < typename TFont >
		struct TContextBeginDrawCharBitmaps
		{
			const TFont* pFont;
			const CRect2i* pRcViewport;
			const wchar_t* strText;
			int nCount;
			CRect2i Rect;
			CColor4B FillColor;
			const UFontDrawTextOptions* pOptions;

			TContextBeginDrawCharBitmaps()
				: pFont( nullptr )
				, pRcViewport( nullptr )
				, strText( nullptr )
				, nCount( 0 )
				, Rect()
				, FillColor()
				, pOptions( nullptr )
			{
			}
		};


		template < typename TFont, typename TFontCharBitmap >
		struct TContextDrawCharBitmap
		{
			wchar_t nChar;
			int nIndexAtText;
			int nLine;
			TFont* pFont;
			const TFontCharBitmap* pBitmap;
			const CPoint2i* pPointDrawAt;
			CColor4B FillColor;
			const CRect2i* pRcViewport;
		};
	}

	typedef TextDraw::UFontDrawTextOptions UFontDrawTextOptions;
	typedef TextDraw::UFontDrawTextWorkBreak UFontDrawTextWorkBreak;
}