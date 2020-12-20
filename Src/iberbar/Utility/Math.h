
#pragma once

namespace iberbar
{
// 	const uint32 nDSfs[]= {
// 		0x00000001, 0x00000002, 0x00000004, 0x00000008,
// 		0x00000010, 0x00000020, 0x00000040, 0x00000080,
// 		0x00000100, 0x00000200, 0x00000400, 0x00000800,
// 		0x00001000, 0x00002000, 0x00004000, 0x00008000,
// 		0x00010000, 0x00020000, 0x00040000, 0x00080000,
// 		0x00100000, 0x00200000, 0x00400000, 0x00800000,
// 		0x01000000, 0x02000000, 0x04000000, 0x08000000,
// 		0x10000000, 0x20000000, 0x40000000, 0x80000000,
// 	};

	template< typename Type >
	inline Type tMin( Type a, Type b )
	{
		return ( a<b ) ? a : b;
	}

	template< typename Type >
	inline Type tMax( Type a, Type b )
	{
		return ( a>b ) ? a : b;
	}


	template< typename Type >
	inline bool tHasMask( Type val, Type mask )
	{
		return ( val&mask ) == mask;
	}

// 	inline bool HasMask( DWORD val, DWORD mask )
// 	{
// 		return ( val&mask ) == mask;
// 	}


	inline bool FloatEqual( float a, float b )
	{
		float c = a - b;
		if ( c < 0.000001 || c > -0.000001 )
			return true;
		return false;
	}
}