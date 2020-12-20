

#include <iberbar/LuaCppApis/MsgPack.h>
#include <iberbar/Lua/LuaCppBuilder.h>
#include <msgpack.hpp>
#include <xmemory>
#include <memory_resource>
#include <assert.h>




namespace iberbar
{
	namespace MsgPack
	{
		class CPackerBuffer
		{
		public:
			CPackerBuffer();
			~CPackerBuffer();

			void write( const char*, size_t );
			const char* data() const { return m_pBuf; }
			size_t size() const { return m_nSize; }

		private:
			size_t m_nSize;
			size_t m_nSizeReverse;
			char* m_pBuf;
		};

		void PackLuaValue( lua_State* pLuaState, int nArg, msgpack::packer<CPackerBuffer>& pker );
		void PackLuaValue_Table( lua_State* pLuaState, int nTableIdx, msgpack::packer<CPackerBuffer>& pker );

		void UnpackToLua( lua_State* pLuaState, const msgpack::v2::object& obj );

		int LuaCFunction_Encode( lua_State* pLuaState );
		int LuaCFunction_Decode( lua_State* pLuaState );
	}
}


std::pmr::unsynchronized_pool_resource g_pool;


iberbar::MsgPack::CPackerBuffer::CPackerBuffer()
	: m_nSize( 0 )
	, m_nSizeReverse( 0 )
	, m_pBuf( nullptr )
{	
}


iberbar::MsgPack::CPackerBuffer::~CPackerBuffer()
{
	std::pmr::polymorphic_allocator<char> a( &g_pool );
	a.deallocate( m_pBuf, m_nSizeReverse );
}


void iberbar::MsgPack::CPackerBuffer::write( const char* pData, size_t nSize )
{
	std::pmr::polymorphic_allocator<char> a( &g_pool );
	size_t nSizeNew = m_nSize + nSize;
	if ( nSizeNew > m_nSizeReverse )
	{
		char* pBufferNew = a.allocate( nSizeNew );
		if ( m_pBuf != nullptr )
		{
			memcpy_s( pBufferNew, nSizeNew, m_pBuf, m_nSize );
			a.deallocate( m_pBuf, m_nSizeReverse );
		}
		m_pBuf = pBufferNew;
		m_nSizeReverse = nSizeNew;
	}
	memcpy_s( m_pBuf + m_nSize, nSize, pData, nSize );
	m_nSize = nSizeNew;
}








FORCEINLINE void iberbar::MsgPack::PackLuaValue( lua_State* pLuaState, int nArg, msgpack::packer<CPackerBuffer>& pker )
{
	int type = lua_type( pLuaState, nArg );
	if ( type == LUA_TNUMBER )
	{
		if ( lua_isinteger( pLuaState, nArg ) == 1 )
		{
			pker.pack( (int64_t)lua_tointeger( pLuaState, nArg ) );
		}
		else
		{
			pker.pack( (float)lua_tonumber( pLuaState, nArg ) );
		}
	}
	else if ( type == LUA_TBOOLEAN )
	{
		pker.pack( (bool)lua_toboolean( pLuaState, nArg ) );
	}
	else if ( type == LUA_TSTRING )
	{
		size_t len = 0;
		const char* vstr = lua_tolstring( pLuaState, nArg, &len );
		pker.pack( vstr );
	}
	else if ( type == LUA_TTABLE )
	{
		PackLuaValue_Table( pLuaState, nArg, pker );
	}
	else if ( type == LUA_TNIL )
	{
		pker.pack_nil();
	}
	else
	{
		pker.pack_nil();
	}
}


void iberbar::MsgPack::PackLuaValue_Table( lua_State* pLuaState, int nTableIdx, msgpack::packer<CPackerBuffer>& pker )
{
	if ( nTableIdx < 0 )
	{
		nTableIdx = lua_gettop( pLuaState ) + nTableIdx + 1;
	}

	lua_len( pLuaState, nTableIdx );
	int len = (int)lua_tointeger( pLuaState, -1 );
	lua_pop( pLuaState, 1 );
	if ( len > 0 )
	{
		pker.pack_array( len );
		for ( int i = 1; i <= len; i++ )
		{
			lua_rawgeti( pLuaState, nTableIdx, i );
			PackLuaValue( pLuaState, -1, pker );
			lua_pop( pLuaState, 1 );
		}
	}
	else
	{
		int nMapSize = 0;
		lua_pushnil( pLuaState );
		while ( lua_next( pLuaState, nTableIdx ) )
		{
			nMapSize++;
			lua_pop( pLuaState, 1 );
		}

		pker.pack_map( nMapSize );

		lua_pushnil( pLuaState );
		while ( lua_next( pLuaState, nTableIdx ) )
		{

			// 现在的栈：-1 => value; -2 => key; index => table
			// 拷贝一份 key 到栈顶，然后对它做 lua_tostring 就不会改变原始的 key 值了
			lua_pushvalue( pLuaState, -2 );

			// 现在的栈：-1 => key; -2 => value; -3 => key; index => table
			const char* key = lua_tostring( pLuaState, -1 );

			// 打包
			pker.pack( key );
			PackLuaValue( pLuaState, -2, pker );

			// 弹出 value 和拷贝的 key，留下原始的 key 作为下一次 lua_next 的参数
			lua_pop( pLuaState, 2 );

			// 现在的栈：-1 => key; index => table
		}
	}
}


void iberbar::MsgPack::UnpackToLua( lua_State* pLuaState, const msgpack::v2::object& obj )
{
	switch ( obj.type )
	{
		case msgpack::v2::type::object_type::NEGATIVE_INTEGER:
		case msgpack::v2::type::object_type::POSITIVE_INTEGER:
		{
			lua_pushinteger( pLuaState, obj.as<int>() );
			break;
		}

		case msgpack::v2::type::object_type::FLOAT32:
		{
			lua_pushnumber( pLuaState, obj.as<float>() );
			break;
		}

		case msgpack::v2::type::object_type::FLOAT64:
		{
			lua_pushnumber( pLuaState, obj.as<double>() );
			break;
		}

		case msgpack::v2::type::object_type::BOOLEAN:
		{
			lua_pushboolean( pLuaState, obj.as<bool>() == true ? 1 : 0 );
			break;
		}

		case msgpack::v2::type::object_type::STR:
		{
			lua_pushlstring( pLuaState, obj.via.str.ptr, obj.via.str.size );
			break;
		}

		case msgpack::v2::type::object_type::ARRAY:
		{
			lua_newtable( pLuaState );
			msgpack::v2::object* n;
			for ( uint32_t i = 0, s = obj.via.array.size; i < s; i++ )
			{
				n = &obj.via.array.ptr[ i ];
				UnpackToLua( pLuaState, *n );
				lua_rawseti( pLuaState, -2, (lua_Integer)i+1 );
			}
			break;
		}

		case msgpack::v2::type::object_type::MAP:
		{
			lua_newtable( pLuaState );
			// 访问具体键值对
			msgpack::v2::object_kv* pkv;
			msgpack::v2::object_kv* pkv_end;
			msgpack::v2::object pk, pv;
			if ( obj.via.map.size > 0 )
			{
				pkv = obj.via.map.ptr;
				pkv_end = obj.via.map.ptr + obj.via.map.size;
				do
				{
					pk = pkv->key;
					pv = pkv->val;

					if ( pkv->key.type == msgpack::v2::type::object_type::STR )
					{
						lua_pushlstring( pLuaState, pkv->key.via.str.ptr, pkv->key.via.str.size );
						UnpackToLua( pLuaState, pkv->val );
						lua_settable( pLuaState, -3 );
					}

					++pkv;
				} while ( pkv < pkv_end );
			}
			break;
		}

		default:
		{
			lua_pushnil( pLuaState );
			break;
		}
	}
	
}





int iberbar::MsgPack::LuaCFunction_Encode( lua_State* pLuaState )
{
	int top = lua_gettop( pLuaState );
	if ( top == 0 )
	{
		lua_pushnil( pLuaState );
		return 1;
	}

	CPackerBuffer buffer;
	msgpack::v2::packer<iberbar::MsgPack::CPackerBuffer> pker( &buffer );
	for ( int i = 1; i <= top; i++ )
	{
		PackLuaValue( pLuaState, i, pker );
	}

	lua_pushlstring( pLuaState, buffer.data(), buffer.size() );
	return 1;
}


int iberbar::MsgPack::LuaCFunction_Decode( lua_State* pLuaState )
{
	int top = lua_gettop( pLuaState );
	if ( top == 0 )
	{
		return 0;
	}

	if ( lua_isstring( pLuaState, 1 ) == false )
	{
		return 0;
	}

	size_t len = 0;
	const char* buf = lua_tolstring( pLuaState, 1, &len );

	msgpack::v2::object_handle obj_handle;
	msgpack::v2::object obj;
	msgpack::v2::type::object_type type;
	size_t offset = 0;
	int nret = 0;
	while ( offset < len )
	{
		obj_handle = msgpack::v2::unpack( buf, len, offset );
		obj = obj_handle.get();

		UnpackToLua( pLuaState, obj );
		
		nret++;
	}

	return nret;
}





void iberbar::MsgPack::RegisterLuaCpp( lua_State* pLuaState )
{
	LuaCpp::CBuilder builder( pLuaState );
	builder.ResolveScope(
		[]( LuaCpp::CScopeBuilder* scope )
		{
			scope->AddFunctionOne( "Encode", LuaCFunction_Encode );
			scope->AddFunctionOne( "Decode", LuaCFunction_Decode );
		}, "iberbar.MsgPack"
	);
}

