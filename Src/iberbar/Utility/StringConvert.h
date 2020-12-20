#pragma once

#include <iberbar/Utility/Color.h>
#include <iberbar/Utility/Rect.h>

namespace iberbar
{
	CPoint2i __iberbarUtilityApi__ StringConvertToPoint2i( const char* strText );
	CPoint2f __iberbarUtilityApi__ StringConvertToPoint2f( const char* strText );
	CColor4B __iberbarUtilityApi__ StringConvertToColor( const char* strText );
	CRect2i __iberbarUtilityApi__ StringConvertToRect2i( const char* strText );
	CRect2f __iberbarUtilityApi__ StringConvertToRect2f( const char* strText );

	CRect2f __iberbarUtilityApi__ StringConvertToUV( const char* strText, const CSize2f& Size );
}

