
#include <iberbar/Utility/StringConvert.h>
#include <iberbar/Utility/String.h>



iberbar::CPoint2i iberbar::StringConvertToPoint2i( const char* strText )
{
	CPoint2i point;
	CStringEasySplitHelper< char, 32, 2 > SplitHelper;
	int nSplitCount = SplitHelper.Split( strText, ',' );
	if ( nSplitCount == 2 )
	{
		point.x = strtol( SplitHelper[ 0 ], nullptr, 0 );
		point.y = strtol( SplitHelper[ 1 ], nullptr, 0 );
	}
	return point;
}


iberbar::CPoint2f iberbar::StringConvertToPoint2f( const char* strText )
{
	CPoint2f point;
	CStringEasySplitHelper< char, 32, 2 > SplitHelper;
	int nSplitCount = SplitHelper.Split( strText, ',' );
	if ( nSplitCount == 2 )
	{
		point.x = strtof( SplitHelper[ 0 ], nullptr );
		point.y = strtof( SplitHelper[ 1 ], nullptr );
	}
	return point;
}


iberbar::CColor4B iberbar::StringConvertToColor( const char* strText )
{
	if ( strText[ 0 ] == '#' )
	{
		return strtoul( strText + 1, nullptr, 16 );
	}
	else if ( strText[ 0 ] == '0' && strText[ 1 ] == 'x' )
	{
		return strtoul( strText + 2, nullptr, 16 );
	}
	else if ( strncmp( strText, "argb", 4 ) == 0 )
	{
		std::regex regex( "argb\\\(([0-9a-zA-Z].*),([0-9a-zA-Z].*),([0-9a-zA-Z].*),([0-9a-zA-Z].*)\\\)" );
		std::match_results<const char*> results;
		if ( std::regex_match( strText, results, regex ) == true )
		{
			if ( results.size() == 5 )
			{
				return CColor4B(
					strtoul( results[ 1 ].str().c_str(), nullptr, 0 ),
					strtoul( results[ 2 ].str().c_str(), nullptr, 0 ),
					strtoul( results[ 3 ].str().c_str(), nullptr, 0 ),
					strtoul( results[ 4 ].str().c_str(), nullptr, 0 ) );
			}
		}
	}

	return CColor4B();
}


iberbar::CRect2i iberbar::StringConvertToRect2i( const char* strText )
{
	CRect2i rect;
	CStringEasySplitHelper< char, 32, 4 > SplitHelper;
	int nSplitCount = SplitHelper.Split( strText, ',' );
	if ( nSplitCount == 4 )
	{
		rect.l = strtol( SplitHelper[ 0 ], nullptr, 10 );
		rect.t = strtol( SplitHelper[ 1 ], nullptr, 10 );
		rect.r = strtol( SplitHelper[ 2 ], nullptr, 10 );
		rect.b = strtol( SplitHelper[ 3 ], nullptr, 10 );
	}
	return rect;
}


iberbar::CRect2f iberbar::StringConvertToRect2f( const char* strText )
{
	CRect2f rect;
	CStringEasySplitHelper< char, 32, 4 > SplitHelper;
	int nSplitCount = SplitHelper.Split( strText, ',' );
	if ( nSplitCount == 4 )
	{
		rect.l = strtof( SplitHelper[ 0 ], nullptr );
		rect.t = strtof( SplitHelper[ 1 ], nullptr );
		rect.r = strtof( SplitHelper[ 2 ], nullptr );
		rect.b = strtof( SplitHelper[ 3 ], nullptr );
	}
	return rect;
}


iberbar::CRect2f iberbar::StringConvertToUV( const char* strText, const CSize2f& Size )
{
	CRect2f rect;
	CStringEasySplitHelper< char, 32, 4 > SplitHelper;
	int nSplitCount = SplitHelper.Split( strText, ',' );
	if ( nSplitCount == 4 )
	{
		rect.l = strtof( SplitHelper[ 0 ], nullptr );
		rect.t = strtof( SplitHelper[ 1 ], nullptr );
		rect.r = strtof( SplitHelper[ 2 ], nullptr );
		rect.b = strtof( SplitHelper[ 3 ], nullptr );

		rect.l /= Size.w;
		rect.t /= Size.h;
		rect.r /= Size.w;
		rect.b /= Size.h;

		//rect.l /= (Size.w - 1);
		//rect.t /= (Size.h - 1);
		//rect.r /= (Size.w - 1);
		//rect.b /= (Size.h - 1);
		//if ( tstring_find_ch( SplitHelper[ 0 ], '%' ) == true )
		//	rect.l /= 100.0f;
		//if ( tstring_find_ch( SplitHelper[ 1 ], '%' ) == true )
		//	rect.t /= 100.0f;
		//if ( tstring_find_ch( SplitHelper[ 2 ], '%' ) == true )
		//	rect.r /= 100.0f;
		//if ( tstring_find_ch( SplitHelper[ 3 ], '%' ) == true )
		//	rect.b /= 100.0f;
	}
	return rect;
}
