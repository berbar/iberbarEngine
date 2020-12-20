#pragma once

#include <iberbar/Utility/String.h>
#include <iberbar/Utility/Rect.h>
#include <iberbar/Font/FontBase.h>

namespace iberbar
{
	struct UTextLineInfo
	{
		const wchar_t* ptr;
		int width;
		int charCount;
	};

	struct UTextAdjustResult
	{
		std::vector<UTextLineInfo> lines;
		bool overflow;

		UTextAdjustResult()
			: lines()
			, overflow( false )
		{
		}
	};
	
	class TTextAdjustSolution
	{
	public:
		enum class UCharType
		{
			EngLetter,
			WhiteSpace,
			Else,
			None
		};

	public:
		TTextAdjustSolution();

	public:
		template < typename TFont >
		void Adjust( TFont* font, const wchar_t* ptr, CRect2i* pRect );

		template < typename TFont >
		void Adjust( TFont* font, const wchar_t* ptr, CRect2i* pRect, int maxLine, const wchar_t* ellipsisText );

		const UTextAdjustResult& GetLines() { return m_result; }

	private:
		void Reset();

		void AddChar_EngLetter( wchar_t character, int width );
		void AddChar_Else( wchar_t character, int width );
		void AddChar_EndLine();
		void AddChar_WhiteSpace( int width );
		void Wrap();

		// 判断是否当前行的第一个单词或字符
		bool IsFirstWordAtLine();

		//int GetCurrentWordCharCount();
		// 弹出当前单词并返回单词的字符数
		int PopCurrentEngWord();

		template < typename TFont >
		int MeasureCharWidth( TFont* font, int fontSizeDef, wchar_t charater );

	private:
		int m_lineWordCount;   // 当前行的首个词
		int m_lineWidth;   // 当前行的宽度
		int m_lineCharCount;   // 当前行的字符数

		UCharType m_wordCharType;
		int m_wordCharWidth;
		int m_wordCharCount;

		int m_lineCount;
		//std::vector< UTextLineInfo > m_lines;
		UTextAdjustResult m_result;
		//CRect2i m_rcRender;
	};
}


inline iberbar::TTextAdjustSolution::TTextAdjustSolution()
	: m_lineWordCount( 0 )
	, m_lineWidth( 0 )
	, m_lineCharCount( 0 )
	, m_wordCharType( UCharType::None )
	, m_wordCharWidth( 0 )
	, m_wordCharCount( 0 )
	, m_lineCount( 0 )
	, m_result()
{
}

FORCEINLINE void iberbar::TTextAdjustSolution::Reset()
{
	m_lineWordCount = 0;
	m_lineWidth = 0;
	m_lineCharCount = 0;
	m_wordCharType = UCharType::None;
	m_wordCharWidth = 0;
	m_wordCharCount = 0;
	m_lineCount = 0;
	m_result = UTextAdjustResult();
}

FORCEINLINE void iberbar::TTextAdjustSolution::AddChar_EngLetter( wchar_t character, int width )
{
	if ( m_wordCharType != UCharType::EngLetter )
	{
		m_wordCharCount = 0;
		m_wordCharWidth = 0;

		m_lineWordCount++;
	}
	m_wordCharType = UCharType::EngLetter;
	m_wordCharCount++;
	m_wordCharWidth += width;
	m_lineCharCount++;
}

FORCEINLINE void iberbar::TTextAdjustSolution::AddChar_Else( wchar_t character, int width )
{
	// 中文和标点等符号不组成词语
	m_wordCharType = UCharType::Else;

	// 中文一个字符为一个词
	m_lineWordCount++;

	m_lineWidth += width;
	m_lineCharCount++;
}

FORCEINLINE void iberbar::TTextAdjustSolution::AddChar_WhiteSpace( int width )
{
	m_wordCharType = UCharType::WhiteSpace;

	m_lineWidth += width;
	m_lineCharCount++; //空格也需要增进字符数量
}

FORCEINLINE void iberbar::TTextAdjustSolution::AddChar_EndLine()
{
	m_lineCharCount++;
	Wrap();
}

FORCEINLINE void iberbar::TTextAdjustSolution::Wrap()
{
	m_lineCount++;

	UTextLineInfo lineInfo;
	lineInfo.width = m_lineWidth;
	lineInfo.charCount = m_lineCharCount;
	m_result.lines.push_back( lineInfo );

	m_lineWidth = 0;
	m_lineCharCount = 0;
	m_lineWordCount = 0;
	m_wordCharCount = 0;
	m_wordCharWidth = 0;
	m_wordCharType = UCharType::None;
}

FORCEINLINE int iberbar::TTextAdjustSolution::PopCurrentEngWord()
{
	m_lineCharCount -= m_wordCharCount;
	m_lineWidth -= m_wordCharWidth;
	return m_wordCharCount;
}

FORCEINLINE bool iberbar::TTextAdjustSolution::IsFirstWordAtLine()
{
	return m_lineWordCount == 1;
}

template < typename TFont >
FORCEINLINE int iberbar::TTextAdjustSolution::MeasureCharWidth( TFont* font, int fontSizeDef, wchar_t charater )
{
	auto bitmap = font->GetCharBitmap( charater );
	if ( bitmap == nullptr )
		return fontSizeDef;
	return bitmap->GetCharWidth();
}


template < typename TFont >
void iberbar::TTextAdjustSolution::Adjust( TFont* font, const wchar_t* ptr, iberbar::CRect2i* pRect )
{
	// 重置
	Reset();

	const UFontDesc& fontDesc = font->GetFontDesc();
	int fontSize = fontDesc.Size;

	int charWidth = 0;
	int rectWidth = pRect->Width();

	int charWidth_WhiteSpace = this->MeasureCharWidth( font, fontSize, L' ' );

	while ( ptr[0] )
	{
		if ( IsWrap_Char32( *ptr ) )
		{
			AddChar_EndLine();
		}
		else
		{
			charWidth = this->MeasureCharWidth<TFont>( font, fontSize, *ptr );

			// 当改行的字符超出边界时
			if ( (m_lineWidth + charWidth) > rectWidth )
			{
				// 如果当前字符为英文
				if ( IsEngLetter_Char32( *ptr ) )
				{
					// 如果一个英文单词宽度大于rect宽度而且是第一个单词，先写完再换行
					if ( IsFirstWordAtLine() == true )
					{
						while ( IsEngLetter_Char32( *ptr ) )
						{
							charWidth = this->MeasureCharWidth<TFont>( font, fontSize, *ptr );
							AddChar_EngLetter( *ptr, charWidth );
							ptr++;
						}

						while ( Char32_IsWhiteSpace( *ptr ) )
						{
							AddChar_WhiteSpace( charWidth_WhiteSpace );
							ptr++;
						}

						if ( *ptr == L'\n' )
						{
							AddChar_EndLine();
						}
					}
					else
					{
						// 弹出单词，游标回到单词前面
						ptr -= PopCurrentEngWord() + 1;
					}

					//lc_Tag.changeLine();
					Wrap();
				}
				// 如果当前字符为空格
				else if ( ptr[0] == L' ' )
				{
					// 空格符超出范围不换行，直到空格结束
					while ( Char32_IsWhiteSpace( *ptr ) )
					{
						AddChar_WhiteSpace( charWidth_WhiteSpace );
						ptr++;
					}

					if ( *ptr == L'\n' )
					{
						AddChar_EndLine();
					}

					Wrap();
				}
				// 如果当前字符为其他字符
				else
				{
					// 中文和标点等符号不组成词语
					Wrap();
					ptr--;
				}
			}

			// 没有超出边界
			else
			{
				if ( IsEngLetter_Char32( *ptr ) )
				{
					// 连接成英文单词
					AddChar_EngLetter( *ptr, charWidth );
				}
				// 如果当前字符为空格
				else if ( Char32_IsWhiteSpace( *ptr ) )
				{
					AddChar_WhiteSpace( charWidth_WhiteSpace );
				}
				// 如果当前字符为其他字符
				else
				{
					AddChar_Else( *ptr, charWidth );
				}
			}
		}

		ptr++;
	} // end while

	//-------------------------------------------
	//  最后一行
	//-------------------------------------------
	Wrap();
}

template < typename TFont >
void iberbar::TTextAdjustSolution::Adjust( TFont* font, const wchar_t* ptr, iberbar::CRect2i* pRect, int maxLine, const wchar_t* ellipsisText )
{
	// 重置
	Reset();

	const UFontDesc& fontDesc = font->GetFontDesc();
	int fontSize = fontDesc.Size;

	int charWidth = 0;
	int rectWidth = pRect->Width();
	int lineMaxWidth = rectWidth;
	maxLine = ( maxLine > 0 ) ? maxLine : INT_MAX;
	int lineMaxIndex = maxLine - 1;

	// 获取结束符的长度
	int ellipsisWidth = 0;
	if ( maxLine > 0 )
	{
		const wchar_t* ellipsisPtr = ellipsisText;
		while ( *ellipsisPtr != 0 )
		{
			ellipsisWidth += this->MeasureCharWidth( font, fontSize, *ellipsisPtr );
			ellipsisPtr++;
		}
	}

	if ( lineMaxIndex == 0 )
	{
		lineMaxWidth = rectWidth - ellipsisWidth;
	}

	int charWidth_WhiteSpace = this->MeasureCharWidth( font, fontSize, L' ' );
	
	while ( ptr[0] )
	{
		if ( IsWrap_Char32( *ptr ) )
		{
			AddChar_EndLine();

			if ( m_lineCount >= maxLine )
			{
				m_result.overflow = true;
				break;
			}
		}
		else
		{
			charWidth = this->MeasureCharWidth<TFont>( font, fontSize, *ptr );

			// 当改行的字符超出边界时
			if ( (m_lineWidth + charWidth) > lineMaxWidth )
			{
				// 如果当前字符为英文
				if ( IsEngLetter_Char32( *ptr ) )
				{
					// 如果一个英文单词宽度大于rect宽度而且是第一个单词，先写完再换行
					if ( IsFirstWordAtLine() == true )
					{
						while ( IsEngLetter_Char32( *ptr ) )
						{
							charWidth = this->MeasureCharWidth<TFont>( font, fontSize, *ptr );
							AddChar_EngLetter( *ptr, charWidth );
							ptr++;
						}

						while ( Char32_IsWhiteSpace( *ptr ) )
						{
							AddChar_WhiteSpace( charWidth_WhiteSpace );
							ptr++;
						}

						if ( *ptr == L'\n' )
						{
							AddChar_EndLine();
						}
						else
						{
							Wrap();
						}
					}
					else
					{
						// 弹出单词，游标回到单词前面
						ptr -= PopCurrentEngWord() + 1;
						Wrap();
					}
				}
				// 如果当前字符为空格
				else if ( ptr[0] == L' ' )
				{
					// 空格符超出范围不换行，直到空格结束
					while ( Char32_IsWhiteSpace( *ptr ) )
					{
						AddChar_WhiteSpace( charWidth_WhiteSpace );
						ptr++;
					}

					if ( *ptr == L'\n' )
					{
						AddChar_EndLine();
					}
					else
					{
						Wrap();
					}
				}
				// 如果当前字符为其他字符
				else
				{
					// 中文和标点等符号不组成词语
					Wrap();
					ptr--;
				}

				if ( m_lineCount >= maxLine )
				{
					m_result.overflow = true;
					break;
				}
			}

			// 没有超出边界
			else
			{
				if ( IsEngLetter_Char32( *ptr ) )
				{
					// 连接成英文单词
					AddChar_EngLetter( *ptr, charWidth );
				}
				// 如果当前字符为空格
				else if ( Char32_IsWhiteSpace( *ptr ) )
				{
					AddChar_WhiteSpace( charWidth_WhiteSpace );
				}
				// 如果当前字符为其他字符
				else
				{
					AddChar_Else( *ptr, charWidth );
				}
			}
		}

		ptr++;
	} // end while

	if ( m_result.overflow == true )
	{

	}

	//-------------------------------------------
	//  最后一行
	//-------------------------------------------
	Wrap();
}
