


#include <iberbar/LuaCppApis/Xml.h>
#include <iberbar/LuaCppApis/Xml_Names.h>
#include <iberbar/Lua/LuaError.h>
#include <iberbar/Lua/LuaCppBuilder.h>
#include <iberbar/Lua/LuaCppInstantiateDef.h>
#include <iberbar/Utility/String.h>
#include <iberbar/Utility/Xml/RapidXml.h>


namespace iberbar
{
	namespace Xml
	{
		int LuaCFunction_CreateRapidDocument( lua_State* pLuaState );

		int LuaCppFunction_Document_ReadFile( lua_State* pLuaState );
		int LuaCppFunction_Document_ReadString( lua_State* pLuaState );
		int LuaCppFunction_Document_GetNodeRoot( lua_State* pLuaState );

		int LuaCppFunction_Node_SelectSingleNode( lua_State* pLuaState );
		int LuaCppFunction_Node_SelectNodes( lua_State* pLuaState );
		int LuaCppFunction_Node_GetAttribute( lua_State* pLuaState );
		int LuaCppFunction_Node_GetValueText( lua_State* pLuaState );
		int LuaCppFunction_Node_GetNameText( lua_State* pLuaState );

		static const luaL_Reg s_LuaCppClassReg_Document_Methods[] = {
			{ "ReadFile", &LuaCppFunction_Document_ReadFile },
			{ "ReadString", &LuaCppFunction_Document_ReadString },
			{ "GetNodeRoot", &LuaCppFunction_Document_GetNodeRoot },
			{ nullptr, nullptr }
		};
		static const Lua::UClassDefinition s_LuaCppClassReg_Document = {
			s_LuaCppNames_Document,
			s_LuaCppNames_Document_FullName,
			nullptr,
			nullptr,
			&Lua::Class_Unknown_Distructor_Release<CDocumentA>,
			s_LuaCppClassReg_Document_Methods
		};

		static const luaL_Reg s_LuaCppClassReg_Node_Methods[] = {
			{ "SelectSingleNode", &LuaCppFunction_Node_SelectSingleNode },
			{ "SelectNodes", &LuaCppFunction_Node_SelectNodes },
			{ "GetAttribute", &LuaCppFunction_Node_GetAttribute },
			{ "GetValueText", &LuaCppFunction_Node_GetValueText },
			{ "GetNameText", &LuaCppFunction_Node_GetNameText },
			{ nullptr, nullptr }
		};
		static const Lua::UClassDefinition s_LuaCppClassReg_Node = {
			s_LuaCppNames_Node,
			s_LuaCppNames_Node_FullName,
			nullptr,
			nullptr,
			&Lua::Class_Unknown_Distructor_Release<CNodeA>,
			s_LuaCppClassReg_Node_Methods
		};
	}
}



void iberbar::Xml::RegisterLuaCpp( lua_State* pLuaState )
{
	Lua::CBuilder Builder( pLuaState );
	Builder.ResolveScope( []( Lua::CScopeBuilder* pScope )
		{
			pScope->AddClass( s_LuaCppClassReg_Document );
			pScope->AddClass( s_LuaCppClassReg_Node );
			pScope->AddFunctionOne( "CreateRapidDocument", &LuaCFunction_CreateRapidDocument );
		}, "iberbar.Xml" );
}



int iberbar::Xml::LuaCFunction_CreateRapidDocument( lua_State* pLuaState )
{
	PTR_CDocumentA pDoc = CreateRapidXmlDocumentA();
	if ( pDoc == nullptr )
		lua_pushnil( pLuaState );
	else
		lua_pushcppref( pLuaState, s_LuaCppNames_Document_FullName, pDoc );
	return 1;
}


int iberbar::Xml::LuaCppFunction_Document_ReadFile( lua_State* pLuaState )
{
	int nTop = lua_gettop( pLuaState );
	iberbar_LuaCheckArguments( pLuaState, nTop, 2 );
	CDocumentA* pDoc = lua_tocppobject( pLuaState, 1, CDocumentA );
	if ( pDoc == nullptr )
	{
		lua_pushstring( pLuaState, "invalid self" );
		return 1;
	}
	const char* strFile = lua_tostring( pLuaState, 2 );
	if ( StringIsNullOrEmpty( strFile ) )
	{
		lua_pushstring( pLuaState, "invalid filepath" );
		return 1;
	}
	CResult Ret = pDoc->LoadFromFile( strFile );
	if ( Ret.IsOK() == false )
	{
		lua_pushstring( pLuaState, Ret.data.c_str() );
		return 1;
	}
	lua_pushboolean( pLuaState, 1 );
	return 1;
}


int iberbar::Xml::LuaCppFunction_Document_ReadString( lua_State* pLuaState )
{
	int nTop = lua_gettop( pLuaState );
	iberbar_LuaCheckArguments( pLuaState, nTop, 2 );
	CDocumentA* pDoc = lua_tocppobject( pLuaState, 1, CDocumentA );
	if ( pDoc == nullptr )
	{
		lua_pushstring( pLuaState, "invalid self" );
		return 1;
	}
	const char* strFile = lua_tostring( pLuaState, 2 );
	if ( StringIsNullOrEmpty( strFile ) )
	{
		lua_pushstring( pLuaState, "invalid source" );
		return 1;
	}
	CResult Ret = pDoc->Load( strFile );
	if ( Ret.IsOK() == false )
	{
		lua_pushstring( pLuaState, Ret.data.c_str() );
		return 1;
	}
	lua_pushboolean( pLuaState, 1 );
	return 1;
}


int iberbar::Xml::LuaCppFunction_Document_GetNodeRoot( lua_State* pLuaState )
{
	int nTop = lua_gettop( pLuaState );
	iberbar_LuaCheckArguments( pLuaState, nTop, 1 );
	CDocumentA* pDoc = lua_tocppobject( pLuaState, 1, CDocumentA );
	if ( pDoc == nullptr )
	{
		lua_pushnil( pLuaState );
		return 1;
	}
	PTR_CNodeA pNode;
	pDoc->GetRoot( &pNode );
	if ( pNode == nullptr )
	{
		lua_pushnil( pLuaState );
		return 1;
	}
	lua_pushcppref( pLuaState, s_LuaCppNames_Node_FullName, pNode );
	return 1;
}



int iberbar::Xml::LuaCppFunction_Node_SelectSingleNode( lua_State* pLuaState )
{
	int nTop = lua_gettop( pLuaState );
	iberbar_LuaCheckArguments( pLuaState, nTop, 2 );
	CNodeA* pNode = lua_tocppobject( pLuaState, 1, CNodeA );
	if ( pNode == nullptr )
	{
		lua_pushnil( pLuaState );
		return 1;
	}
	const char* strName = lua_tostring( pLuaState, 2 );
	if ( StringIsNullOrEmpty( strName ) )
	{
		lua_pushnil( pLuaState );
		return 1;
	}
	PTR_CNodeA pNodeSelect;
	pNode->SelectSingleNode( strName, &pNodeSelect );
	if ( pNodeSelect == nullptr )
	{
		lua_pushnil( pLuaState );
		return 1;
	}
	lua_pushcppref( pLuaState, s_LuaCppNames_Node_FullName, pNodeSelect );
	return 1;
}


int iberbar::Xml::LuaCppFunction_Node_SelectNodes( lua_State* pLuaState )
{
	int nTop = lua_gettop( pLuaState );
	iberbar_LuaCheckArguments( pLuaState, nTop, 2 );
	CNodeA* pNode = lua_tocppobject( pLuaState, 1, CNodeA );
	if ( pNode == nullptr )
	{
		lua_pushnil( pLuaState );
		return 1;
	}
	const char* strName = lua_tostring( pLuaState, 2 );
	if ( StringIsNullOrEmpty( strName ) )
	{
		lua_pushnil( pLuaState );
		return 1;
	}
	PTR_CNodeListA pNodeList;
	int nNodeCount = 0;
	pNode->SelectNodes( strName, &pNodeList );
	if ( pNodeList == nullptr || ( nNodeCount = pNodeList->GetNodeCount() ) == 0 )
	{
		lua_pushnil( pLuaState );
		return 1;
	}

	PTR_CNodeA pNodeSelect;
	lua_createtable( pLuaState, nNodeCount, 0 );
	for ( int i = 0; i < nNodeCount; i++ )
	{
		pNodeList->GetNodeAt( i, &pNodeSelect );
		lua_pushcppref( pLuaState, s_LuaCppNames_Node_FullName, pNodeSelect );
		lua_rawseti( pLuaState, -2, (lua_Integer)i+1 );
	}
	
	return 1;
}


int iberbar::Xml::LuaCppFunction_Node_GetAttribute( lua_State* pLuaState )
{
	int nTop = lua_gettop( pLuaState );
	iberbar_LuaCheckArguments( pLuaState, nTop, 2 );
	CNodeA* pNode = lua_tocppobject( pLuaState, 1, CNodeA );
	if ( pNode == nullptr )
	{
		lua_pushnil( pLuaState );
		return 1;
	}
	const char* strName = lua_tostring( pLuaState, 2 );
	if ( StringIsNullOrEmpty( strName ) )
	{
		lua_pushnil( pLuaState );
		return 1;
	}
	const char* strAttrValue = pNode->GetAttribute( strName );
	if ( strAttrValue == nullptr )
		lua_pushnil( pLuaState );
	else
		lua_pushstring( pLuaState, strAttrValue );
	return 1;
}


int iberbar::Xml::LuaCppFunction_Node_GetValueText( lua_State* pLuaState )
{
	int nTop = lua_gettop( pLuaState );
	iberbar_LuaCheckArguments( pLuaState, nTop, 1 );
	CNodeA* pNode = lua_tocppobject( pLuaState, 1, CNodeA );
	if ( pNode == nullptr )
	{
		lua_pushnil( pLuaState );
		return 1;
	}
	const char* strValueText = pNode->GetValueText();
	if ( strValueText == nullptr )
		lua_pushnil( pLuaState );
	else
		lua_pushstring( pLuaState, strValueText );
	return 1;
}


int iberbar::Xml::LuaCppFunction_Node_GetNameText( lua_State* pLuaState )
{
	int nTop = lua_gettop( pLuaState );
	iberbar_LuaCheckArguments( pLuaState, nTop, 1 );
	CNodeA* pNode = lua_tocppobject( pLuaState, 1, CNodeA );
	if ( pNode == nullptr )
	{
		lua_pushnil( pLuaState );
		return 1;
	}
	const char* strNameText = pNode->NameText();
	if ( strNameText == nullptr )
		lua_pushnil( pLuaState );
	else
		lua_pushstring( pLuaState, strNameText );
	return 1;
}


