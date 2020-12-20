//#pragma once
//
//#include <iberbar/Lua/LuaCppInstantiateDef.h>
//
//
//
//#define iberbar_Lua_CheckArgumentCount( top )
//
//namespace iberbar
//{
//	namespace Lua
//	{
//		class CCFunctionHelper
//		{
//
//		};
//
//		class CStaticFunctionParameters
//		{
//		public:
//			CFunctionParameters( lua_State* pLuaState );
//
//		public:
//			template< typename T >
//			T* GetCppObject( int narg );
//
//			template< typename T >
//			T GetTable( int narg, T( convertor )(lua_State* L, int narg) );
//
//		public:
//			lua_State* m_pLuaState;
//		};
//
//		template < typename TSelf >
//		class CMemberFunctionParameters
//		{
//		public:
//			CMemberFunctionParameters( lua_State* pLuaState );
//
//		public:
//			TSelf* GetSelf();
//
//			template< typename T >
//			T* GetCppObject( int narg );
//
//			template< typename T >
//			T GetTable( int narg, T (convertor)( lua_State* L, int narg ) );
//
//		public:
//			lua_State* m_pLuaState;
//		};
//
//
//		int lua_return_integer( lua_State* pLuaState, lua_Integer value );
//		int lua_return_string( lua_State* pLuaState, const char* value );
//		int lua_return_number( lua_State* pLuaState, lua_Number value );
//
//		template < typename T >
//		int lua_return_table( lua_State* pLuaState, const T& value )
//		{
//			ToLuaTable( value );
//			return 1;
//		}
//
//
//		class CCApiStack
//		{
//			CCApiStack( bool bIsMemberFunction );
//		private:
//
//		};
//	}
//}
//
//
//#define lua_arg( n ) n
//
//#define Mem_Arg
//
//
//
//
////
////
////template < typename TSelf >
////inline iberbar::LuaCpp::CMemberFunctionParameters<TSelf>::CMemberFunctionParameters( lua_State* pLuaState )
////	: m_pLuaState( pLuaState )
////{
////}
////
////template < typename TSelf >
////inline TSelf* iberbar::LuaCpp::CMemberFunctionParameters<TSelf>::GetSelf()
////{
////	return lua_self_tocppobject( m_pLuaState, TSelf );
////}
////
////
////template<typename TSelf>
////template<typename T>
////inline T* iberbar::LuaCpp::CMemberFunctionParameters<TSelf>::GetCppObject( int narg )
////{
////	return lua_tocppobject( m_pLuaState, narg + 1, T );
////}
////
////template<typename TSelf>
////template<typename T>
////inline T iberbar::LuaCpp::CMemberFunctionParameters<TSelf>::GetTable( int narg, T( convertor )(lua_State* L, int narg) )
////{
////	return convertor( m_pLuaState, narg + 1 );
////}
//
//
