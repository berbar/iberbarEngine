
#include "Xml.h"
#include <iberbar/Lua/LuaError.h>
#include <iberbar/Lua/LuaCppInstantiateDef.h>
#include <iberbar/Lua/LuaCppBuilder.h>
#include <iberbar/Utility/Xml/RapidXml.h>

using namespace iberbar;

#define LuaCppNamespace "iberbar.Xml"

#define LuaCppName_DocumentA "CDocumentA"
#define LuaCppName_DocumentA_FullName LuaCppNamespace "." LuaCppName_DocumentA

#define LuaCppName_NodeA "CNodeA"
#define LuaCppName_NodeA_FullName LuaCppNamespace "." LuaCppName_NodeA

#define LuaCppName_NodeListA "CNodeListA"
#define LuaCppName_NodeListA_FullName LuaCppNamespace "." LuaCppName_NodeListA

namespace iberbar
{
	namespace Xml
	{
		//extern char LuaCppNamespace[] = "iberbar.Xml";

		//extern const char LuaCppName_DocumentA[] = "CDocumentA";

		//extern const char LuaCppName_NodeA[] = "CNodeA";

		//extern const char LuaCppName_NodeListA[] = "CNodeListA";

		int LuaCFunction_CreateRapidXmlDocumentA( lua_State* L );

		int LuaCppFunction_DocumentA_Load( lua_State* L );

		int LuaCppFunction_DocumentA_LoadFromFile( lua_State* L );

		int LuaCppFunction_DocumentA_GetRoot( lua_State* L );

		int LuaCppFunction_NodeA_SelectSingleNode( lua_State* L );

		int LuaCppFunction_NodeA_SelectNodes( lua_State* L );

		int LuaCppFunction_NodeA_GetAttribute( lua_State* L );

		int LuaCppFunction_NodeA_GetValueText( lua_State* L );

		int LuaCppFunction_NodeListA_GetNodeAt( lua_State* L );

		int LuaCppFunction_NodeListA_GetNodeCount( lua_State* L );

		void LuaCppClassBuilder_DocumentA( const char*, Lua::CClassBuilder* classBuilder );

		void LuaCppClassBuilder_NodeA( const char*, Lua::CClassBuilder* classBuilder );

		void LuaCppClassBuilder_NodeListA( const char*, Lua::CClassBuilder* classBuilder );
	}
}



void iberbar::Xml::RegisterLuaCpp( lua_State* L )
{
	Lua::CBuilder builder( L );
	builder.ResolveScope( []( Lua::CScopeBuilder* scope )
	{
		scope->AddFunctionOne( "CreateRapidXmlDocumentA", &LuaCFunction_CreateRapidXmlDocumentA );
		scope->AddClass( LuaCppName_DocumentA, &LuaCppClassBuilder_DocumentA );
		scope->AddClass( LuaCppName_NodeA, &LuaCppClassBuilder_NodeA );
		scope->AddClass( LuaCppName_NodeListA, &LuaCppClassBuilder_NodeListA );
	}, LuaCppNamespace );
}

void iberbar::Xml::LuaCppClassBuilder_DocumentA( const char*, Lua::CClassBuilder* classBuilder )
{
	classBuilder->AddMemberMethod( "Load", &LuaCppFunction_DocumentA_Load );
	classBuilder->AddMemberMethod( "LoadFromFile", &LuaCppFunction_DocumentA_LoadFromFile );
	classBuilder->AddMemberMethod( "GetRoot", &LuaCppFunction_DocumentA_GetRoot );
	classBuilder->AddDistructor( &Lua::Class_Unknown_Distructor_Release<CDocumentA> );
}

void iberbar::Xml::LuaCppClassBuilder_NodeA( const char*, Lua::CClassBuilder* classBuilder )
{
	classBuilder->AddMemberMethod( "SelectSingleNode", &LuaCppFunction_NodeA_SelectSingleNode );
	classBuilder->AddMemberMethod( "SelectNodes", &LuaCppFunction_NodeA_SelectNodes );
	classBuilder->AddMemberMethod( "GetAttribute", &LuaCppFunction_NodeA_GetAttribute );
	classBuilder->AddMemberMethod( "GetValueText", &LuaCppFunction_NodeA_GetValueText );
	classBuilder->AddDistructor( &Lua::Class_Unknown_Distructor_Release<CNodeA> );
}

void iberbar::Xml::LuaCppClassBuilder_NodeListA( const char*, Lua::CClassBuilder* classBuilder )
{
	classBuilder->AddMemberMethod( "GetNodeAt", &LuaCppFunction_NodeListA_GetNodeAt );
	classBuilder->AddMemberMethod( "GetNodeCount", &LuaCppFunction_NodeListA_GetNodeCount );
	classBuilder->AddDistructor( &Lua::Class_Unknown_Distructor_Release<CNodeListA> );
}


int iberbar::Xml::LuaCFunction_CreateRapidXmlDocumentA( lua_State* L )
{
	//const char* className;
	//int metatable;
	//int ret = LuaCpp::Class_Default_Constructor_New_GetMetatable( L, &className, &metatable );
	//if ( ret > 0 ) return ret;

	Xml::PTR_CDocumentA document = Xml::CreateRapidXmlDocumentA();
	CDocumentA* pointer = document;
	Lua::Class_Default_PushObjectWithName( L, LuaCppName_DocumentA_FullName, pointer );
	pointer->AddRef();

	return 1;
}

int iberbar::Xml::LuaCppFunction_DocumentA_Load( lua_State* L )
{
	return 0;
}

int iberbar::Xml::LuaCppFunction_DocumentA_LoadFromFile( lua_State* L )
{
	int top = lua_gettop( L );
	if ( top != 2 )
		return iberbar_LuaError_ArgumentsCountIsNotMatch( L, 2 );

	Xml::CDocumentA* document = lua_self_tocppobject( L, Xml::CDocumentA );
	const char* filepath = lua_tostring( L, 2 );

	CResult ret = document->LoadFromFile( filepath );

	lua_pushboolean( L, ret.IsOK() ? 1 : 0 );
	lua_pushstring( L, ret.data.c_str() );

	return 2;
}

int iberbar::Xml::LuaCppFunction_DocumentA_GetRoot( lua_State* L )
{
	Xml::CDocumentA* document = lua_self_tocppobject( L, Xml::CDocumentA );

	Xml::CNodeA* node = nullptr;
	document->GetRoot( &node );
	Lua::Class_Default_PushObjectWithName( L, LuaCppName_NodeA_FullName, node );

	return 1;
}

int iberbar::Xml::LuaCppFunction_NodeA_SelectSingleNode( lua_State* L )
{
	int top = lua_gettop( L );
	if ( top != 2 )
		return iberbar_LuaError_ArgumentsCountIsNotMatch( L, 2 );

	Xml::CNodeA* node = lua_self_tocppobject( L, Xml::CNodeA );
	const char* tagName = lua_tostring( L, 2 );

	Xml::CNodeA* nodeChild = nullptr;
	node->SelectSingleNode( tagName, &nodeChild );
	Lua::Class_Default_PushObjectWithName( L, LuaCppName_NodeA_FullName, nodeChild );

	return 1;
}

int iberbar::Xml::LuaCppFunction_NodeA_SelectNodes( lua_State* L )
{
	int top = lua_gettop( L );
	if ( top != 2 )
		return iberbar_LuaError_ArgumentsCountIsNotMatch( L, 2 );

	Xml::CNodeA* node = lua_self_tocppobject( L, Xml::CNodeA );
	const char* tagName = lua_tostring( L, 2 );

	Xml::CNodeListA* nodeList = nullptr;
	node->SelectNodes( tagName, &nodeList );
	Lua::Class_Default_PushObjectWithName( L, LuaCppName_NodeListA_FullName, nodeList );

	return 1;
}

int iberbar::Xml::LuaCppFunction_NodeA_GetAttribute( lua_State* L )
{
	int top = lua_gettop( L );
	if ( top != 2 )
		return iberbar_LuaError_ArgumentsCountIsNotMatch( L, 2 );

	Xml::CNodeA* node = lua_self_tocppobject( L, Xml::CNodeA );
	const char* attrName = lua_tostring( L, 2 );

	const char* attrValue = node->GetAttribute( attrName );
	if ( attrValue == nullptr )
		lua_pushnil( L );
	else
		lua_pushstring( L, attrValue );

	return 1;
}

int iberbar::Xml::LuaCppFunction_NodeA_GetValueText( lua_State* L )
{
	int top = lua_gettop( L );
	if ( top != 1 )
		return iberbar_LuaError_ArgumentsCountIsNotMatch( L, 1 );

	Xml::CNodeA* node = lua_self_tocppobject( L, Xml::CNodeA );

	const char* value = node->GetValueText();
	if ( value == nullptr )
		lua_pushnil( L );
	else
		lua_pushstring( L, value );

	return 1;
}

int iberbar::Xml::LuaCppFunction_NodeListA_GetNodeAt( lua_State* L )
{
	int top = lua_gettop( L );
	if ( top != 2 )
		return iberbar_LuaError_ArgumentsCountIsNotMatch( L, 2 );

	Xml::CNodeListA* nodeList = lua_self_tocppobject( L, Xml::CNodeListA );
	int index = lua_tointeger( L, 2 );

	Xml::CNodeA* node = nullptr;
	nodeList->GetNodeAt( index, &node );

	Lua::Class_Default_PushObjectWithName( L, LuaCppName_NodeA_FullName, node );

	return 1;
}

int iberbar::Xml::LuaCppFunction_NodeListA_GetNodeCount( lua_State* L )
{
	int top = lua_gettop( L );
	if ( top != 1 )
		return iberbar_LuaError_ArgumentsCountIsNotMatch( L, 1 );

	Xml::CNodeListA* nodeList = lua_self_tocppobject( L, Xml::CNodeListA );

	lua_pushinteger( L, nodeList->GetNodeCount() );

	return 1;
}


