#pragma once

#include <iberbar/Lua/LuaBase.h>

#ifdef __ANDROID__
#ifndef __FUNCDNAME__
#define __FUNCDNAME__ __FUNCTION__ 
#endif
#endif

#define iberbar_LuaError_ArgumentsCountIsNotMatch( L, n ) \
	luaL_error( L, "cfunction<%s>, count of arguments is not match. It requires %d.", __FUNCDNAME__, n )

#define iberbar_LuaError_ArgumentsCountIsNotMatch2( L, n1, n2 ) \
	luaL_error( L, "cfunction<%s>, count of arguments is not match. It requires %d or %d.", __FUNCDNAME__, n1, n2 )

#define iberbar_LuaError_ArgumentsCountIsNotMatch3( L, n1, n2, n3 ) \
	luaL_error( L, "cfunction<%s>, count of arguments is not match. It requires %d or %d or %d.", __FUNCDNAME__, n1, n2 )

#define iberbar_LuaError_ArgumentsCountIsNotMatch4( L, n1, n2, n3, n4 ) \
	luaL_error( L, "cfunction<%s>, count of arguments is not match. It requires %d or %d or %d or %d.", __FUNCDNAME__, n1, n2, n3, n4 )

#define iberbar_LuaError_ArgumentsCountIsNotMatch5( L, n1, n2, n3, n4, n5 ) \
	luaL_error( L, "cfunction<%s>, count of arguments is not match. It requires %d or %d or %d or %d or %d.", __FUNCDNAME__, n1, n2, n3, n4, n5 )

#define iberbar_LuaError_SelfIsNullPointer( L ) \
	luaL_error( L, "cfunction<%s>, self is a null pointer.", __FUNCDNAME__ )


#define iberbar_LuaCheckArguments( L, t, n ) \
	if ( t != n ) return iberbar_LuaError_ArgumentsCountIsNotMatch( L, n );

#define iberbar_LuaCheckArguments2( L, t, n1, n2 ) \
	if ( t != n1 && t != n2 ) return iberbar_LuaError_ArgumentsCountIsNotMatch2( L, n1, n2 );

#define iberbar_LuaCheckArguments3( L, t, n1, n2, n3 ) \
	if ( t != n1 && t != n2 && t != n3 ) return iberbar_LuaError_ArgumentsCountIsNotMatch3( L, n1, n2, n3 );

#define iberbar_LuaCheckArguments4( L, t, n1, n2, n3, n4 ) \
	if ( t != n1 && t != n2 && t != n3 && t != n4 ) return iberbar_LuaError_ArgumentsCountIsNotMatch4( L, n1, n2, n3, n4 );

#define iberbar_LuaCheckArguments5( L, t, n1, n2, n3, n4, n5 ) \
	if ( t != n1 && t != n2 && t != n3 && t != n4 && t != n5 ) return iberbar_LuaError_ArgumentsCountIsNotMatch5( L, n1, n2, n3, n4, n5 );
