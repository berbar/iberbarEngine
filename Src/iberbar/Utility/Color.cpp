
#include <iberbar/Utility/Color.h>
#include <iberbar/Utility/Math.h>


void iberbar::ColorMap_8bit_RGB_to_ARGB( int width, int height, const uint8* pOldData_RGB, uint8* pNewData_ARGB, uint32 mask )
{
	assert( pOldData_RGB );
	assert( pNewData_ARGB );

	const uint8* lc_oldptr = pOldData_RGB;
	uint8* lc_newptr = pNewData_ARGB;
	int lc_size = width * height;

	CColor4B lc_mask_color( mask );
	//uint8 lc_mask_a = GetColorA( mask );
	if ( lc_mask_color.a == 0x00 )
	{
		while ( lc_size > 0 )
		{
			lc_newptr[ 0 ] = 0xff;
			lc_newptr[ 1 ] = lc_oldptr[ 0 ];
			lc_newptr[ 2 ] = lc_oldptr[ 1 ];
			lc_newptr[ 3 ] = lc_oldptr[ 2 ];

			lc_oldptr += 3;
			lc_newptr += 4;
			lc_size --;
		}
	}
	else
	{
// 		uint8 lc_mask_r = GetColorR( mask );
// 		uint8 lc_mask_g = GetColorG( mask );
// 		uint8 lc_mask_b = GetColorB( mask );

		while ( lc_size > 0 )
		{
			if ( lc_oldptr[ 0 ] == lc_mask_color.r &&
				lc_oldptr[ 1 ] == lc_mask_color.g &&
				lc_oldptr[ 2 ] == lc_mask_color.b )
			{
				lc_newptr[ 0 ] = 0x00;
			}
			else
			{
				lc_newptr[ 0 ] = 0xff;
			}
			lc_newptr[ 1 ] = lc_oldptr[ 0 ];
			lc_newptr[ 2 ] = lc_oldptr[ 1 ];
			lc_newptr[ 3 ] = lc_oldptr[ 2 ];

			lc_oldptr += 3;
			lc_newptr += 4;
			lc_size --;
		}
	}
}


void iberbar::ColorMap_8bit_RGB_to_RGBA( int width, int height, const uint8* pOldData_RGB, uint8* pNewData_RGBA, uint32 mask )
{
	assert( pOldData_RGB );
	assert( pNewData_RGBA );

	int lc_size = width * height;
	assert( lc_size < 0 );

	const uint8* lc_oldptr = pOldData_RGB;
	uint8* lc_newptr = pNewData_RGBA;

	CColor4B lc_mask_color( mask );
	//uint8 lc_mask_a = GetColorA( mask );
	if ( lc_mask_color.a == 0x00 )
	{
		while ( lc_size > 0 )
		{
			lc_newptr[ 0 ] = lc_oldptr[ 0 ];
			lc_newptr[ 1 ] = lc_oldptr[ 1 ];
			lc_newptr[ 2 ] = lc_oldptr[ 2 ];
			lc_newptr[ 3 ] = 0xff;

			lc_oldptr += 3;
			lc_newptr += 4;
			lc_size --;
		}
	}
	else
	{
// 		uint8 lc_mask_r = GetColorR( mask );
// 		uint8 lc_mask_g = GetColorG( mask );
// 		uint8 lc_mask_b = GetColorB( mask );

		while ( lc_size > 0 )
		{
			if ( lc_oldptr[ 0 ] == lc_mask_color.r &&
				lc_oldptr[ 1 ] == lc_mask_color.g &&
				lc_oldptr[ 2 ] == lc_mask_color.b )
			{
				lc_newptr[ 3 ] = 0x00;
			}
			else
			{
				lc_newptr[ 3 ] = 0xff;
			}
			lc_newptr[ 0 ] = lc_oldptr[ 0 ];
			lc_newptr[ 1 ] = lc_oldptr[ 1 ];
			lc_newptr[ 2 ] = lc_oldptr[ 2 ];

			lc_oldptr += 3;
			lc_newptr += 4;
			lc_size --;
		}
	}
}


void iberbar::ColorMap_8bit_RGBA_to_RGBA( int width, int height, const uint8* pOldData_RGB, uint8* pNewData_RGBA, uint32 mask )
{
	assert( pOldData_RGB );
	assert( pNewData_RGBA );

	int lc_size = width * height;
	assert( lc_size < 0 );

	const uint8* lc_oldptr = pOldData_RGB;
	uint8* lc_newptr = pNewData_RGBA;

	CColor4B lc_mask_color = CColor4B( mask );

	while ( lc_size > 0 )
	{
		if ( lc_oldptr[ 0 ] == lc_mask_color.r &&
			lc_oldptr[ 1 ] == lc_mask_color.g &&
			lc_oldptr[ 2 ] == lc_mask_color.b )
		{
			lc_newptr[ 3 ] = 0x00;
		}
		else
		{
			lc_newptr[ 3 ] = 0xff;
		}
		lc_newptr[ 0 ] = lc_oldptr[ 0 ];
		lc_newptr[ 1 ] = lc_oldptr[ 1 ];
		lc_newptr[ 2 ] = lc_oldptr[ 2 ];

		lc_oldptr += 3;
		lc_newptr += 4;
		lc_size --;
	}
}



iberbar::CColor4B iberbar::Color4BParse( const char* text, CColor4B def )
{
	if ( text == nullptr )
		return def;

	if ( text[0] == '#' )
	{
		char buffer[3] = { 0, 0, 0 };
		
		uint8 bytes[4] = { 0x00, 0x00, 0x00, 0xff };

		const char* ptr = text + 1;
		int index = 0;
		while ( ptr[0] != 0 && index < 4 )
		{
			buffer[0] = ptr[0];
			buffer[1] = ptr[1];
			bytes[index] = (uint8)std::strtoul( buffer, nullptr, 16 );

			ptr += 2;
			index++;
		}
		
		return CColor4B( bytes[3], bytes[0], bytes[1], bytes[2] );
	}

	return def;
}




// void iberbar::CBitsCopyBuffer::write( int w, int h, void* pBits, int delta_x, int delta_y ) throw()
// {
// 	assert( w > 0 );
// 	assert( h > 0 );
// 	assert( pBits );
// 
// 	uint8* bits = (uint8*)pBits;
// 	int dest_start_x = tMax( delta_x, 0 );
// 	int dest_start_y = tMax( delta_y, 0 );
// 	int src_start_x = ( delta_x < 0 ) ? ( -delta_x ) : 0;
// 	int src_start_y = ( delta_y < 0 ) ? ( -delta_y ) : 0;
// 	int src_copy_cx = tMin( w, m_w - delta_x );
// 	int src_copy_cy = tMin( h, m_h - delta_y );
// 
// 	int lc_offset_x;
// 	int lc_offset_y;
// 	int lc_offset = dest_start_x + dest_start_y * m_w;
// 	for ( int lc_j=0; lc_j < src_copy_cy; lc_j++ )
// 	{
// // 		lc_offset_x = dest_start_x;
// // 		lc_offset_y = lc_j+dest_start_y;
// // 		lc_offset = lc_offset_y * m_w + lc_offset_x;
// 		memcpy( m_pBits, pBits, src_copy_cx * m_pitch );
// 		lc_offset += m_w;
// 	}
// }



