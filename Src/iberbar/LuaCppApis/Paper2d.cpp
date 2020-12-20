
#include <iberbar/LuaCppApis/Paper2d.h>
#include <iberbar/LuaCppApis/Paper2d_Names.h>
#include <iberbar/LuaCppApis/Rhi.h>
#include <iberbar/Lua/LuaError.h>
#include <iberbar/Lua/LuaCppInstantiateDef.h>
#include <iberbar/Lua/LuaCppBuilder.h>
#include <iberbar/Lua/LuaCallback.h>

#include <iberbar/Paper2d/Node.h>
#include <iberbar/Paper2d/Camera.h>
#include <iberbar/Paper2d/Scene.h>
#include <iberbar/Paper2d/Image.h>
#include <iberbar/Paper2d/Animation.h>
#include <iberbar/Paper2d/Director.h>
#include <iberbar/Paper2d/Component.h>
#include <iberbar/Paper2d/ComponentContainer.h>
#include <iberbar/Paper2d/Component_CustomAttributes.h>
#include <iberbar/Paper2d/Component_Updatable.h>
#include <iberbar/Paper2d/Component_HandleMouseInput.h>
#include <iberbar/Paper2d/Component_HandleKeyboardInput.h>
#include <iberbar/Paper2d/Component_Transform.h>
#include <iberbar/Paper2d/Component_Bounding.h>
#include <iberbar/Paper2d/Terrain.h>

#include <iberbar/RHI/Texture.h>

#include <iberbar/Utility/String.h>
#include <iberbar/Utility/Input.h>


namespace iberbar
{
	namespace Paper2d
	{
		template < typename T >
		class TComponent_ForLua
			: public T
		{
		public:
			TComponent_ForLua()
				: m_pLuaCallbackHandler( nullptr )
			{
			}
			void BindCallback( Lua::CLuaCallbackHandler* pLuaCallbackHandler )
			{
				m_pLuaCallbackHandler = pLuaCallbackHandler;
			}
			void BindCallbackOnAttach( Lua::CLuaCallbackHandler* pLuaCallbackHandler )
			{
				m_pLuaCallbackHandler_OnAttach = pLuaCallbackHandler;
			}
			void BindCallbackOnRemove( Lua::CLuaCallbackHandler* pLuaCallbackHandler )
			{
				m_pLuaCallbackHandler_OnRemove = pLuaCallbackHandler;
			}
		protected:
			FORCEINLINE void LuaCall_OnAttachOrRemove( Lua::CLuaCallbackHandler* pLuaCallbackHandler, const char* strCppName )
			{
				if ( pLuaCallbackHandler != nullptr && pLuaCallbackHandler->PushFunction() == true )
				{
					lua_State* pLuaState = pLuaCallbackHandler->GetLuaState();

					int nArg = 1;
					if ( pLuaCallbackHandler->HasExt() == true )
					{
						if ( pLuaCallbackHandler->PushExt() == true )
							nArg++;
					}

					lua_pushcppref( pLuaState, strCppName, this );

					int ret = lua_pcall( pLuaState, nArg, LUA_MULTRET, 0 );
					if ( ret != 0 )
					{
						int type = lua_type( pLuaState, -1 );
						const char* err = lua_tostring( pLuaState, -1 );
						lua_pop( pLuaState, 1 );
					}
				}
			}

		protected:
			std::string m_strLuaCppName;
			Lua::PTR_CLuaCallbackHandler m_pLuaCallbackHandler;
			Lua::PTR_CLuaCallbackHandler m_pLuaCallbackHandler_OnAttach;
			Lua::PTR_CLuaCallbackHandler m_pLuaCallbackHandler_OnRemove;
		};


		// CNode
		int LuaCppFunction_Node_Init( lua_State* pLuaState );
		int LuaCppFunction_Node_SetId( lua_State* pLuaState );
		int LuaCppFunction_Node_GetId( lua_State* pLuaState );
		int LuaCppFunction_Node_SetVisible( lua_State* pLuaState );
		int LuaCppFunction_Node_SetEnable( lua_State* pLuaState );
		int LuaCppFunction_Node_GetVisible( lua_State* pLuaState );
		int LuaCppFunction_Node_GetEnable( lua_State* pLuaState );
		int LuaCppFunction_Node_IsVisible( lua_State* pLuaState );
		int LuaCppFunction_Node_IsEnable( lua_State* pLuaState );
		int LuaCppFunction_Node_SetPosition( lua_State* pLuaState );
		int LuaCppFunction_Node_GetPosition( lua_State* pLuaState );
		int LuaCppFunction_Node_SetSize( lua_State* pLuaState );
		int LuaCppFunction_Node_GetSize( lua_State* pLuaState );
		int LuaCppFunction_Node_SetAnchorPoint( lua_State* pLuaState );
		int LuaCppFunction_Node_GetAnchorPoint( lua_State* pLuaState );
		int LuaCppFunction_Node_SetPivot( lua_State* pLuaState );
		int LuaCppFunction_Node_GetPivot( lua_State* pLuaState );
		int LuaCppFunction_Node_SetZOrder( lua_State* pLuaState );
		int LuaCppFunction_Node_GetZOrder( lua_State* pLuaState );
		int LuaCppFunction_Node_InsertChild( lua_State* pLuaState );
		int LuaCppFunction_Node_RemoveChild( lua_State* pLuaState );
		int LuaCppFunction_Node_RemoveAllChildren( lua_State* pLuaState );
		int LuaCppFunction_Node_FindChild( lua_State* pLuaState );
		int LuaCppFunction_Node_GetChildren( lua_State* pLuaState );
		int LuaCppFunction_Node_AddComponent( lua_State* pLuaState );
		int LuaCppFunction_Node_GetComponent( lua_State* pLuaState );
		int LuaCppFunction_Node_GetComponentDefaultCustomAttributes( lua_State* pLuaState );


		// CCamera
		int LuaCppFunction_Camera_SetPosition( lua_State* pLuaState );
		int LuaCppFunction_Camera_SetOrthographicProjection( lua_State* pLuaState );
		int LuaCppFunction_Camera_Build( lua_State* pLuaState );


		// CScene
		int LuaCppFunction_Scene_GetCamera( lua_State* pLuaState );


		// CImage
		int LuaCppFunction_Image_SetTexture( lua_State* pLuaState );
		int LuaCppFunction_Image_SetUV( lua_State* pLuaState );
		int LuaCppFunction_Image_SetColor( lua_State* pLuaState );


		// CGridTerrain
		int LuaCppFunction_GridTerrain_SetGridSize( lua_State* pLuaState );
		int LuaCppFunction_GridTerrain_GetGridRow( lua_State* pLuaState );
		int LuaCppFunction_GridTerrain_GetGridCol( lua_State* pLuaState );


		// CAnimationController
		int LuaCppFunction_AnimationController_BindCallback( lua_State* pLuaState );
		int LuaCppFunction_AnimationController_Play( lua_State* pLuaState );
		int LuaCppFunction_AnimationController_Stop( lua_State* pLuaState );
		int LuaCppFunction_AnimationController_Update( lua_State* pLuaState );
		int LuaCppFunction_AnimationController_GetTexture( lua_State* pLuaState );
		int LuaCppFunction_AnimationController_GetCurrentColor( lua_State* pLuaState );
		int LuaCppFunction_AnimationController_GetCurrentUV( lua_State* pLuaState );
		int LuaCppFunction_AnimationController_GetName( lua_State* pLuaState );


		// CComponent
		int LuaCppFunction_Component_GetOwner( lua_State* pLuaState );
		int LuaCppFunction_Component_SetName( lua_State* pLuaState );
		int LuaCppFunction_Component_GetName( lua_State* pLuaState );


		// CComponent_CustomAttributes
		int LuaCppFunction_ComponentCustomAttributes_AddAttribute( lua_State* pLuaState );
		int LuaCppFunction_ComponentCustomAttributes_GetAttribute( lua_State* pLuaState );
		int LuaCppFunction_ComponentCustomAttributes_GetAttributeString( lua_State* pLuaState );
		int LuaCppFunction_ComponentCustomAttributes_GetAttributeBoolean( lua_State* pLuaState );
		int LuaCppFunction_ComponentCustomAttributes_GetAttributeInt( lua_State* pLuaState );
		int LuaCppFunction_ComponentCustomAttributes_GetAttributeFloat( lua_State* pLuaState );

		template < typename T >
		int LuaCppFunction_Component_BindCallbackOnAttach( lua_State* pLuaState );
		template < typename T >
		int LuaCppFunction_Component_BindCallbackOnRemove( lua_State* pLuaState );
		template < typename T >
		int LuaCppFunction_Component_BindCallback( lua_State* pLuaState );


		// CComponent_BoundingBox
		int LuaCppFunction_ComponentBoundingBox_Contains( lua_State* pLuaState );
		int LuaCppFunction_ComponentBoundingBox_ContainsPoint( lua_State* pLuaState );
		int LuaCppFunction_ComponentBoundingBox_GetBox( lua_State* pLuaState );


		// CDirector::RunScene
		int LuaCFunction_RunScene( lua_State* pLuaState );
		int LuaCFunction_GetScene( lua_State* pLuaState );


		void LuaCppClassBuilder_Node( const char*, LuaCpp::CClassBuilder* cb );
		void LuaCppClassBuilder_Camera( const char*, LuaCpp::CClassBuilder* cb );
		void LuaCppClassBuilder_Scene( const char*, LuaCpp::CClassBuilder* cb );
		void LuaCppClassBuilder_Image( const char*, LuaCpp::CClassBuilder* cb );
		void LuaCppClassBuilder_GridTerrain( const char*, LuaCpp::CClassBuilder* cb );
		void LuaCppClassBuilder_AnimationController( const char*, LuaCpp::CClassBuilder* cb );
		void LuaCppClassBuilder_Component( const char*, LuaCpp::CClassBuilder* cb );
		void LuaCppClassBuilder_ComponentCustomAttributes( const char*, LuaCpp::CClassBuilder* cb );
		void LuaCppClassBuilder_ComponentUpdatable( const char*, LuaCpp::CClassBuilder* cb );
		void LuaCppClassBuilder_ComponentHandleMouseInput( const char*, LuaCpp::CClassBuilder* cb );
		void LuaCppClassBuilder_ComponentHandleKeyboardInput( const char*, LuaCpp::CClassBuilder* cb );
		void LuaCppClassBuilder_ComponentTransform( const char*, LuaCpp::CClassBuilder* cb );
		void LuaCppClassBuilder_ComponentBoundingBox( const char*, LuaCpp::CClassBuilder* cb );







		class CComponent_Updatable_ForLua
			: public TComponent_ForLua<CComponent_Updatable>
		{
		public:
			virtual void OnAttach() override;
			virtual void OnRemove() override;
			virtual void Update( float nDelta ) override;
		};



		class CComponent_HandleMouseInput_ForLua
			: public TComponent_ForLua<CComponent_HandleMouseInput>
		{
		public:
			virtual void OnAttach() override;
			virtual void OnRemove() override;
			virtual void OnMouseEvent( const UComponentMouseEventData* EventData ) override;
		};



		class CComponent_HandleKeyboardInput_ForLua
			: public TComponent_ForLua<CComponent_HandleKeyboardInput>
		{
		public:
			virtual void OnAttach() override;
			virtual void OnRemove() override;
			virtual void OnKeyboardEvent( const UComponentKeyboardEventData* EventData ) override;
		};



		class CComponent_Transform_ForLua
			: public TComponent_ForLua<CComponent_Transform>
		{
		public:
			virtual void OnAttach() override;
			virtual void OnRemove() override;
			virtual void OnUpdateTransform() override;
		};
	}
}






extern const char iberbar::Paper2d::LuaCppNames::ComponentBounding[] = "CComponent_Bounding";
extern const char iberbar::Paper2d::LuaCppNames::ComponentBounding_FullName[] = "iberbar.Paper2d.CComponent_Bounding";

extern const char iberbar::Paper2d::LuaCppNames::ComponentBoundingBox[] = "CComponent_BoundingBox";
extern const char iberbar::Paper2d::LuaCppNames::ComponentBoundingBox_FullName[] = "iberbar.Paper2d.CComponent_BoundingBox";






void iberbar::Paper2d::RegisterLuaCpp( lua_State* pLuaState )
{
	LuaCpp::CBuilder builder( pLuaState );
	builder.ResolveScope( []( LuaCpp::CScopeBuilder* scope )
		{
			scope->AddClass( LuaCppNames::Node, &LuaCppClassBuilder_Node );
			scope->AddClass( LuaCppNames::Camera, &LuaCppClassBuilder_Camera );
			scope->AddClass( LuaCppNames::Scene, &LuaCppClassBuilder_Scene, LuaCppNames::Node_FullName );
			scope->AddClass( LuaCppNames::Image, &LuaCppClassBuilder_Image, LuaCppNames::Node_FullName );
			scope->AddClass( LuaCppNames::GridTerrain, &LuaCppClassBuilder_GridTerrain, LuaCppNames::Node_FullName );
			scope->AddClass( LuaCppNames::AnimationController, &LuaCppClassBuilder_AnimationController );
			scope->AddClass( LuaCppNames::Component, &LuaCppClassBuilder_Component );
			scope->AddClass( LuaCppNames::ComponentCustomAttributes, &LuaCppClassBuilder_ComponentCustomAttributes, LuaCppNames::Component_FullName );
			scope->AddClass( LuaCppNames::ComponentUpdatable, &LuaCppClassBuilder_ComponentUpdatable, LuaCppNames::Component_FullName );
			scope->AddClass( LuaCppNames::ComponentHandleMouseInput, &LuaCppClassBuilder_ComponentHandleMouseInput, LuaCppNames::Component_FullName );
			scope->AddClass( LuaCppNames::ComponentHandleKeyboardInput, &LuaCppClassBuilder_ComponentHandleKeyboardInput, LuaCppNames::Component_FullName );
			scope->AddClass( LuaCppNames::ComponentTransform, &LuaCppClassBuilder_ComponentTransform, LuaCppNames::Component_FullName );
			scope->AddClass( LuaCppNames::ComponentBoundingBox, &LuaCppClassBuilder_ComponentBoundingBox, LuaCppNames::Component_FullName );

			scope->AddFunctionOne( "RunScene", &LuaCFunction_RunScene );
			scope->AddFunctionOne( "GetScene", &LuaCFunction_GetScene );
		}, LuaCppNames::Namespace );
}







int iberbar::Paper2d::LuaCppFunction_Node_Init( lua_State* pLuaState )
{
	int t = lua_gettop( pLuaState );
	if ( t != 1 )
		return iberbar_LuaError_ArgumentsCountIsNotMatch( pLuaState, 1 );

	CNode* pNode = lua_tocppobject( pLuaState, 1, CNode );
	if ( pNode != nullptr )
	{
		pNode->Init();
	}
	return 0;
}


int iberbar::Paper2d::LuaCppFunction_Node_SetId( lua_State* pLuaState )
{
	int t = lua_gettop( pLuaState );
	if ( t != 2 )
		return iberbar_LuaError_ArgumentsCountIsNotMatch( pLuaState, 2 );

	CNode* pNode = lua_tocppobject( pLuaState, 1, CNode );
	const char* strId = lua_tostring( pLuaState, 2 );
	if ( pNode != nullptr && strId != nullptr )
	{
		pNode->SetId( strId );
	}
	return 0;
}


int iberbar::Paper2d::LuaCppFunction_Node_GetId( lua_State* pLuaState )
{
	int t = lua_gettop( pLuaState );
	if ( t != 1 )
		return iberbar_LuaError_ArgumentsCountIsNotMatch( pLuaState, 1 );

	CNode* pNode = lua_tocppobject( pLuaState, 1, CNode );
	if ( pNode != nullptr )
	{
		lua_pushstring( pLuaState, pNode->GetId().c_str() );
	}
	else
	{
		lua_pushnil( pLuaState );
	}
	return 1;
}


int iberbar::Paper2d::LuaCppFunction_Node_SetVisible( lua_State* pLuaState )
{
	int t = lua_gettop( pLuaState );
	if ( t != 2 )
		return iberbar_LuaError_ArgumentsCountIsNotMatch( pLuaState, 2 );

	CNode* pNode = lua_tocppobject( pLuaState, 1, CNode );
	bool bVisible = lua_toboolean( pLuaState, 2 );
	if ( pNode != nullptr )
	{
		pNode->SetVisiable( bVisible );
	}
	return 0;
}


int iberbar::Paper2d::LuaCppFunction_Node_SetEnable( lua_State* pLuaState )
{
	int t = lua_gettop( pLuaState );
	if ( t != 2 )
		return iberbar_LuaError_ArgumentsCountIsNotMatch( pLuaState, 2 );

	CNode* pNode = lua_tocppobject( pLuaState, 1, CNode );
	bool bEnable = lua_toboolean( pLuaState, 2 );
	if ( pNode != nullptr )
	{
		pNode->SetEnable( bEnable );
	}
	return 0;
}


int iberbar::Paper2d::LuaCppFunction_Node_GetVisible( lua_State* pLuaState )
{
	int t = lua_gettop( pLuaState );
	if ( t != 1 )
		return iberbar_LuaError_ArgumentsCountIsNotMatch( pLuaState, 1 );

	CNode* pNode = lua_tocppobject( pLuaState, 1, CNode );
	if ( pNode != nullptr )
	{
		lua_pushboolean( pLuaState, pNode->GetVisible() );
	}
	else
	{
		lua_pushnil( pLuaState );
	}
	return 1;
}


int iberbar::Paper2d::LuaCppFunction_Node_GetEnable( lua_State* pLuaState )
{
	int t = lua_gettop( pLuaState );
	if ( t != 1 )
		return iberbar_LuaError_ArgumentsCountIsNotMatch( pLuaState, 1 );

	CNode* pNode = lua_tocppobject( pLuaState, 1, CNode );
	if ( pNode != nullptr )
	{
		lua_pushboolean( pLuaState, pNode->GetEnable() );
	}
	else
	{
		lua_pushnil( pLuaState );
	}
	return 1;
}


int iberbar::Paper2d::LuaCppFunction_Node_IsVisible( lua_State* pLuaState )
{
	int t = lua_gettop( pLuaState );
	if ( t != 1 )
		return iberbar_LuaError_ArgumentsCountIsNotMatch( pLuaState, 1 );

	CNode* pNode = lua_tocppobject( pLuaState, 1, CNode );
	if ( pNode != nullptr )
	{
		lua_pushboolean( pLuaState, pNode->IsVisible() );
	}
	else
	{
		lua_pushnil( pLuaState );
	}
	return 1;
}


int iberbar::Paper2d::LuaCppFunction_Node_IsEnable( lua_State* pLuaState )
{
	int t = lua_gettop( pLuaState );
	if ( t != 1 )
		return iberbar_LuaError_ArgumentsCountIsNotMatch( pLuaState, 1 );

	CNode* pNode = lua_tocppobject( pLuaState, 1, CNode );
	if ( pNode != nullptr )
	{
		lua_pushboolean( pLuaState, pNode->IsEnable() );
	}
	else
	{
		lua_pushnil( pLuaState );
	}
	return 1;
}


int iberbar::Paper2d::LuaCppFunction_Node_SetPosition( lua_State* pLuaState )
{
	int t = lua_gettop( pLuaState );
	if ( t != 3 )
		return iberbar_LuaError_ArgumentsCountIsNotMatch( pLuaState, 3 );

	CNode* pNode = lua_tocppobject( pLuaState, 1, CNode );
	float nX = (float)lua_tonumber( pLuaState, 2 );
	float nY = (float)lua_tonumber( pLuaState, 3 );
	if ( pNode != nullptr )
	{
		pNode->SetPosition( nX, nY );
	}
	return 0;
}


int iberbar::Paper2d::LuaCppFunction_Node_GetPosition( lua_State* pLuaState )
{
	int t = lua_gettop( pLuaState );
	if ( t != 1 )
		return iberbar_LuaError_ArgumentsCountIsNotMatch( pLuaState, 1 );

	CNode* pNode = lua_tocppobject( pLuaState, 1, CNode );
	if ( pNode != nullptr )
	{
		const DirectX::XMFLOAT2& ptPosition = pNode->GetPosition();
		lua_newtable( pLuaState );
		int nLuaTable = lua_gettop( pLuaState );
		lua_pushstring( pLuaState, "x" );
		lua_pushnumber( pLuaState, (lua_Number)ptPosition.x );
		lua_settable( pLuaState, nLuaTable );
		lua_pushstring( pLuaState, "y" );
		lua_pushnumber( pLuaState, (lua_Number)ptPosition.y );
		lua_settable( pLuaState, nLuaTable );
	}
	else
	{
		lua_pushnil( pLuaState );
	}
	return 1;
}


int iberbar::Paper2d::LuaCppFunction_Node_SetSize( lua_State* pLuaState )
{
	int t = lua_gettop( pLuaState );
	if ( t != 3 )
		return iberbar_LuaError_ArgumentsCountIsNotMatch( pLuaState, 3 );

	CNode* pNode = lua_tocppobject( pLuaState, 1, CNode );
	float nW = (float)lua_tonumber( pLuaState, 2 );
	float nH = (float)lua_tonumber( pLuaState, 3 );
	if ( pNode != nullptr )
	{
		pNode->SetContentSize( nW, nH );
	}
	return 0;
}


int iberbar::Paper2d::LuaCppFunction_Node_GetSize( lua_State* pLuaState )
{
	int t = lua_gettop( pLuaState );
	if ( t != 1 )
		return iberbar_LuaError_ArgumentsCountIsNotMatch( pLuaState, 1 );

	CNode* pNode = lua_tocppobject( pLuaState, 1, CNode );
	if ( pNode != nullptr )
	{
		const DirectX::XMFLOAT2& Size = pNode->GetContentSize();
		lua_newtable( pLuaState );
		int nLuaTable = lua_gettop( pLuaState );
		lua_pushstring( pLuaState, "x" );
		lua_pushnumber( pLuaState, (lua_Number)Size.x );
		lua_settable( pLuaState, nLuaTable );
		lua_pushstring( pLuaState, "y" );
		lua_pushnumber( pLuaState, (lua_Number)Size.y );
		lua_settable( pLuaState, nLuaTable );
	}
	else
	{
		lua_pushnil( pLuaState );
	}
	return 1;
}


int iberbar::Paper2d::LuaCppFunction_Node_SetAnchorPoint( lua_State* pLuaState )
{
	int t = lua_gettop( pLuaState );
	if ( t != 3 )
		return iberbar_LuaError_ArgumentsCountIsNotMatch( pLuaState, 3 );

	CNode* pNode = lua_tocppobject( pLuaState, 1, CNode );
	float nX = (float)lua_tonumber( pLuaState, 2 );
	float nY = (float)lua_tonumber( pLuaState, 3 );
	if ( pNode != nullptr )
	{
		pNode->SetAnchor( nX, nY );
	}
	return 0;
}


int iberbar::Paper2d::LuaCppFunction_Node_GetAnchorPoint( lua_State* pLuaState )
{
	int t = lua_gettop( pLuaState );
	if ( t != 1 )
		return iberbar_LuaError_ArgumentsCountIsNotMatch( pLuaState, 1 );

	CNode* pNode = lua_tocppobject( pLuaState, 1, CNode );
	if ( pNode != nullptr )
	{
		const DirectX::XMFLOAT2& AnchorPoint = pNode->GetAnchor();
		lua_newtable( pLuaState );
		int nLuaTable = lua_gettop( pLuaState );
		lua_pushstring( pLuaState, "x" );
		lua_pushnumber( pLuaState, (lua_Number)AnchorPoint.x );
		lua_settable( pLuaState, nLuaTable );
		lua_pushstring( pLuaState, "y" );
		lua_pushnumber( pLuaState, (lua_Number)AnchorPoint.y );
		lua_settable( pLuaState, nLuaTable );
	}
	else
	{
		lua_pushnil( pLuaState );
	}
	return 1;
}


int iberbar::Paper2d::LuaCppFunction_Node_SetPivot( lua_State* pLuaState )
{
	int t = lua_gettop( pLuaState );
	if ( t != 3 )
		return iberbar_LuaError_ArgumentsCountIsNotMatch( pLuaState, 3 );

	CNode* pNode = lua_tocppobject( pLuaState, 1, CNode );
	float nX = (float)lua_tonumber( pLuaState, 2 );
	float nY = (float)lua_tonumber( pLuaState, 3 );
	if ( pNode != nullptr )
	{
		pNode->SetPivot( nX, nY );
	}
	return 0;
}


int iberbar::Paper2d::LuaCppFunction_Node_GetPivot( lua_State* pLuaState )
{
	int t = lua_gettop( pLuaState );
	if ( t != 1 )
		return iberbar_LuaError_ArgumentsCountIsNotMatch( pLuaState, 1 );

	CNode* pNode = lua_tocppobject( pLuaState, 1, CNode );
	if ( pNode != nullptr )
	{
		const DirectX::XMFLOAT2& AnchorPoint = pNode->GetPivot();
		lua_newtable( pLuaState );
		int nLuaTable = lua_gettop( pLuaState );
		lua_pushstring( pLuaState, "x" );
		lua_pushnumber( pLuaState, (lua_Number)AnchorPoint.x );
		lua_settable( pLuaState, nLuaTable );
		lua_pushstring( pLuaState, "y" );
		lua_pushnumber( pLuaState, (lua_Number)AnchorPoint.y );
		lua_settable( pLuaState, nLuaTable );
	}
	else
	{
		lua_pushnil( pLuaState );
	}
	return 1;
}


int iberbar::Paper2d::LuaCppFunction_Node_SetZOrder( lua_State* pLuaState )
{
	int t = lua_gettop( pLuaState );
	if ( t != 2 )
		return iberbar_LuaError_ArgumentsCountIsNotMatch( pLuaState, 2 );

	CNode* pNode = lua_tocppobject( pLuaState, 1, CNode );
	int nX = (int)lua_tointeger( pLuaState, 2 );
	if ( pNode != nullptr )
	{
		pNode->SetZOrder( nX );
	}
	return 0;
}


int iberbar::Paper2d::LuaCppFunction_Node_GetZOrder( lua_State* pLuaState )
{
	int t = lua_gettop( pLuaState );
	if ( t != 1 )
		return iberbar_LuaError_ArgumentsCountIsNotMatch( pLuaState, 1 );

	CNode* pNode = lua_tocppobject( pLuaState, 1, CNode );
	if ( pNode != nullptr )
	{
		lua_pushinteger( pLuaState, pNode->GetZOrder() );
	}
	else
	{
		lua_pushnil( pLuaState );
	}
	return 1;
}


int iberbar::Paper2d::LuaCppFunction_Node_InsertChild( lua_State* pLuaState )
{
	int t = lua_gettop( pLuaState );
	if ( t != 2 )
		return iberbar_LuaError_ArgumentsCountIsNotMatch( pLuaState, 2 );

	CNode* pNode = lua_tocppobject( pLuaState, 1, CNode );
	CNode* pNodeChild = lua_tocppobject( pLuaState, 2, CNode );
	if ( pNode != nullptr && pNodeChild != nullptr )
	{
		pNode->InsertChild( pNodeChild );
	}
	return 0;
}


int iberbar::Paper2d::LuaCppFunction_Node_RemoveChild( lua_State* pLuaState )
{
	int t = lua_gettop( pLuaState );
	if ( t != 2 )
		return iberbar_LuaError_ArgumentsCountIsNotMatch( pLuaState, 2 );

	CNode* pNode = lua_tocppobject( pLuaState, 1, CNode );
	CNode* pNodeChild = lua_tocppobject( pLuaState, 2, CNode );
	if ( pNode != nullptr && pNodeChild != nullptr )
	{
		pNode->RemoveChild( pNodeChild );
	}
	return 0;
}


int iberbar::Paper2d::LuaCppFunction_Node_RemoveAllChildren( lua_State* pLuaState )
{
	int t = lua_gettop( pLuaState );
	if ( t != 1 )
		return iberbar_LuaError_ArgumentsCountIsNotMatch( pLuaState, 1 );

	CNode* pNode = lua_tocppobject( pLuaState, 1, CNode );
	if ( pNode != nullptr )
	{
		pNode->RemoveAllChildren();
	}
	return 0;
}


int iberbar::Paper2d::LuaCppFunction_Node_FindChild( lua_State* pLuaState )
{
	int t = lua_gettop( pLuaState );
	if ( t != 2 && t != 3 )
		return iberbar_LuaError_ArgumentsCountIsNotMatch2( pLuaState, 2, 3 );

	CNode* pNode = lua_tocppobject( pLuaState, 1, CNode );
	if ( pNode == nullptr )
	{
		lua_pushnil( pLuaState );
		return 1;
	}

	const char* strId = lua_tostring( pLuaState, 2 );
	if ( StringIsNullOrEmpty( strId ) )
	{
		lua_pushnil( pLuaState );
		return 1;
	}

	CNode* pNodeFind = pNode->FindChild( strId );
	if ( pNodeFind == nullptr )
	{
		lua_pushnil( pLuaState );
		return 1;
	}

	if ( lua_istable( pLuaState, 3 ) == true )
	{
		LuaCpp::Class_Ref_PushObject( pLuaState, 3, pNodeFind );
		//lua_pushcppref( pLuaState, LuaCppNames::Node_FullName, pNodeFind );
	}
	else
	{
		lua_pushcppref( pLuaState, LuaCppNames::Node_FullName, pNodeFind );
	}

	return 1;
}


int iberbar::Paper2d::LuaCppFunction_Node_GetChildren( lua_State* pLuaState )
{
	int t = lua_gettop( pLuaState );
	if ( t != 1 )
		return iberbar_LuaError_ArgumentsCountIsNotMatch( pLuaState, 1 );

	CNode* pNode = lua_tocppobject( pLuaState, 1, CNode );
	if ( pNode == nullptr )
	{
		lua_pushnil( pLuaState );
		return 1;
	}

	auto& Children = pNode->GetChildren();
	lua_newtable( pLuaState );
	int table = lua_gettop( pLuaState );
	int n = 0;
	auto iter = Children.begin();
	auto end = Children.end();
	for ( ; iter != end; iter++ )
	{
		n++;

		lua_pushcppref( pLuaState, LuaCppNames::Node_FullName, (*iter) );
		lua_rawseti( pLuaState, table, n );
	}

	return 1;
}


int iberbar::Paper2d::LuaCppFunction_Node_AddComponent( lua_State* pLuaState )
{
	int t = lua_gettop( pLuaState );
	if ( t != 2 )
		return iberbar_LuaError_ArgumentsCountIsNotMatch( pLuaState, 2 );

	CNode* pNode = lua_tocppobject( pLuaState, 1, CNode );
	CComponent* pComponent = lua_tocppobject( pLuaState, 2, CComponent );
	if ( pNode != nullptr && pComponent != nullptr )
	{
		bool ret = pNode->GetComponentContainer()->Add( pComponent );
		lua_pushboolean( pLuaState, ret == false ? 0 : 1 );
	}
	else
	{
		lua_pushboolean( pLuaState, 0 );
	}
	return 1;
}


int iberbar::Paper2d::LuaCppFunction_Node_GetComponent( lua_State* pLuaState )
{
	int t = lua_gettop( pLuaState );
	if ( t != 2 )
		return iberbar_LuaError_ArgumentsCountIsNotMatch( pLuaState, 2 );

	CNode* pNode = lua_tocppobject( pLuaState, 1, CNode );
	const char* strName = lua_tostring( pLuaState, 2 );
	if ( pNode != nullptr && StringIsNullOrEmpty( strName ) == false )
	{
		CComponent* pComponent = pNode->GetComponentContainer()->Get( strName );
		if ( pComponent == nullptr )
			lua_pushnil( pLuaState );
		else
		{
			lua_pushcppref( pLuaState, LuaCppNames::Component_FullName, pComponent );
		}
	}
	else
	{
		lua_pushnil( pLuaState );
	}
	return 1;
}


int iberbar::Paper2d::LuaCppFunction_Node_GetComponentDefaultCustomAttributes( lua_State* pLuaState )
{
	int t = lua_gettop( pLuaState );
	if ( t != 1 )
		return iberbar_LuaError_ArgumentsCountIsNotMatch( pLuaState, 1 );

	CNode* pNode = lua_tocppobject( pLuaState, 1, CNode );
	if ( pNode != nullptr )
	{
		CComponent* pComponent = pNode->GetComponentContainer()->Get( CComponent_CustomAttributes::sm_DefaultName );
		if ( pComponent == nullptr )
			lua_pushnil( pLuaState );
		else
		{
			lua_pushcppref( pLuaState, LuaCppNames::ComponentCustomAttributes_FullName, pComponent );
		}
	}
	else
	{
		lua_pushnil( pLuaState );
	}
	return 1;
}








int iberbar::Paper2d::LuaCppFunction_Camera_SetPosition( lua_State* pLuaState )
{
	int t = lua_gettop( pLuaState );
	if ( t != 3 )
		return iberbar_LuaError_ArgumentsCountIsNotMatch( pLuaState, 3 );

	CCamera* pCamera = lua_tocppobject( pLuaState, 1, CCamera );
	float nX = (float)lua_tonumber( pLuaState, 2 );
	float nY = (float)lua_tonumber( pLuaState, 3 );
	if ( pCamera != nullptr )
	{
		pCamera->CreateMatrixView( nX, nY );
	}
	return 0;
}


int iberbar::Paper2d::LuaCppFunction_Camera_SetOrthographicProjection( lua_State* pLuaState )
{
	int t = lua_gettop( pLuaState );
	if ( t != 3 )
		return iberbar_LuaError_ArgumentsCountIsNotMatch( pLuaState, 3 );

	CCamera* pCamera = lua_tocppobject( pLuaState, 1, CCamera );
	int nW = (int)lua_tointeger( pLuaState, 2 );
	int nH = (int)lua_tointeger( pLuaState, 3 );
	if ( pCamera != nullptr )
	{
		pCamera->CreateMatrixProjection( nW, nH );
	}
	return 0;
}


int iberbar::Paper2d::LuaCppFunction_Camera_Build( lua_State* pLuaState )
{
	int t = lua_gettop( pLuaState );
	if ( t != 1 )
		return iberbar_LuaError_ArgumentsCountIsNotMatch( pLuaState, 1 );

	CCamera* pCamera = lua_tocppobject( pLuaState, 1, CCamera );
	if ( pCamera != nullptr )
	{
		pCamera->BuildMatrixViewProjection();
	}
	return 0;
}








int iberbar::Paper2d::LuaCppFunction_Scene_GetCamera( lua_State* pLuaState )
{
	int t = lua_gettop( pLuaState );
	if ( t != 1 )
		return iberbar_LuaError_ArgumentsCountIsNotMatch( pLuaState, 1 );

	CScene* pScene = lua_tocppobject( pLuaState, 1, CScene );
	if ( pScene != nullptr )
	{
		CCamera* pCamera = pScene->GetCamera();
		if ( pCamera == nullptr )
		{
			lua_pushnil( pLuaState );
		}
		else
		{
			lua_pushcppref( pLuaState, LuaCppNames::Camera_FullName, pCamera );
		}
	}
	else
	{
		lua_pushnil( pLuaState );
	}
	return 1;
}







int iberbar::Paper2d::LuaCppFunction_Image_SetTexture( lua_State* pLuaState )
{
	int t = lua_gettop( pLuaState );
	if ( t != 2 )
		return iberbar_LuaError_ArgumentsCountIsNotMatch( pLuaState, 2 );

	CImage* pImage = lua_tocppobject( pLuaState, 1, CImage );
	RHI::ITexture* pTexture = lua_tocppobject( pLuaState, 2, RHI::ITexture );
	if ( pImage != nullptr )
	{
		pImage->SetTexture( pTexture );
	}
	return 0;
}


int iberbar::Paper2d::LuaCppFunction_Image_SetUV( lua_State* pLuaState )
{
	int t = lua_gettop( pLuaState );
	if ( t != 5 )
		return iberbar_LuaError_ArgumentsCountIsNotMatch( pLuaState, 5 );

	CImage* pImage = lua_tocppobject( pLuaState, 1, CImage );
	float nL = (float)lua_tonumber( pLuaState, 2 );
	float nT = (float)lua_tonumber( pLuaState, 3 );
	float nR = (float)lua_tonumber( pLuaState, 4 );
	float nB = (float)lua_tonumber( pLuaState, 5 );
	if ( pImage != nullptr )
	{
		pImage->SetUV( CRect2f( nL, nT, nR, nB ) );
	}
	return 0;
}


int iberbar::Paper2d::LuaCppFunction_Image_SetColor( lua_State* pLuaState )
{
	int t = lua_gettop( pLuaState );
	if ( t != 4 && t != 5 )
		return iberbar_LuaError_ArgumentsCountIsNotMatch2( pLuaState, 4, 5 );

	CImage* pImage = lua_tocppobject( pLuaState, 1, CImage );
	uint8 nR = (uint8)lua_tointeger( pLuaState, 2 );
	uint8 nG = (uint8)lua_tointeger( pLuaState, 3 );
	uint8 nB = (uint8)lua_tointeger( pLuaState, 4 );
	uint8 nA = 0xff;
	if ( t == 5 )
		nA = (uint8)lua_tointeger( pLuaState, 5 );
	if ( pImage != nullptr )
	{
		pImage->SetColor( CColor4B( nA, nR, nG, nB ) );
	}
	return 0;
}






int iberbar::Paper2d::LuaCppFunction_GridTerrain_SetGridSize( lua_State* pLuaState )
{
	int t = lua_gettop( pLuaState );
	if ( t != 3 )
		return iberbar_LuaError_ArgumentsCountIsNotMatch( pLuaState, 3);

	CGridTerrain* pTerrain = lua_tocppobject( pLuaState, 1, CGridTerrain );
	int nW = (int)lua_tointeger( pLuaState, 2 );
	int nH = (int)lua_tointeger( pLuaState, 3 );
	if ( pTerrain != nullptr )
	{
		pTerrain->SetGridSize( nW, nH );
	}
	return 0;
}


int iberbar::Paper2d::LuaCppFunction_GridTerrain_GetGridRow( lua_State* pLuaState )
{
	int t = lua_gettop( pLuaState );
	if ( t != 1 )
		return iberbar_LuaError_ArgumentsCountIsNotMatch( pLuaState, 1 );

	CGridTerrain* pTerrain = lua_tocppobject( pLuaState, 1, CGridTerrain );
	if ( pTerrain == nullptr )
	{
		lua_pushinteger( pLuaState, 0 );
	}
	else
	{
		lua_pushinteger( pLuaState, pTerrain->GetGridRow() );
	}
	return 1;
}


int iberbar::Paper2d::LuaCppFunction_GridTerrain_GetGridCol( lua_State* pLuaState )
{
	int t = lua_gettop( pLuaState );
	if ( t != 1 )
		return iberbar_LuaError_ArgumentsCountIsNotMatch( pLuaState, 1 );

	CGridTerrain* pTerrain = lua_tocppobject( pLuaState, 1, CGridTerrain );
	if ( pTerrain == nullptr )
	{
		lua_pushinteger( pLuaState, 0 );
	}
	else
	{
		lua_pushinteger( pLuaState, pTerrain->GetGridCol() );
	}
	return 1;
}







int iberbar::Paper2d::LuaCppFunction_AnimationController_BindCallback( lua_State* pLuaState )
{
	return 0;
}


int iberbar::Paper2d::LuaCppFunction_AnimationController_Play( lua_State* pLuaState )
{
	int t = lua_gettop( pLuaState );
	if ( t != 1 && t != 2 )
		return iberbar_LuaError_ArgumentsCountIsNotMatch( pLuaState, 2 );

	CAnimationController* pAnim = lua_tocppobject( pLuaState, 1, CAnimationController );
	int bLoop = true;
	if ( t == 2 )
		bLoop = lua_toboolean( pLuaState, 2 );
	if ( pAnim != nullptr )
	{
		pAnim->Play( bLoop );
	}
	return 0;
}


int iberbar::Paper2d::LuaCppFunction_AnimationController_Stop( lua_State* pLuaState )
{
	int t = lua_gettop( pLuaState );
	if ( t != 1 )
		return iberbar_LuaError_ArgumentsCountIsNotMatch( pLuaState, 1 );

	CAnimationController* pAnim = lua_tocppobject( pLuaState, 1, CAnimationController );
	if ( pAnim != nullptr )
	{
		pAnim->Stop();
	}
	return 0;
}


int iberbar::Paper2d::LuaCppFunction_AnimationController_Update( lua_State* pLuaState )
{
	int t = lua_gettop( pLuaState );
	if ( t != 2 )
		return iberbar_LuaError_ArgumentsCountIsNotMatch( pLuaState, 2 );

	CAnimationController* pAnim = lua_tocppobject( pLuaState, 1, CAnimationController );
	float nDelta = (float)lua_tonumber( pLuaState, 2 );
	if ( pAnim != nullptr )
	{
		pAnim->Update( nDelta );
	}
	return 0;
}


int iberbar::Paper2d::LuaCppFunction_AnimationController_GetTexture( lua_State* pLuaState )
{
	int t = lua_gettop( pLuaState );
	if ( t != 1 )
		return iberbar_LuaError_ArgumentsCountIsNotMatch( pLuaState, 1 );

	CAnimationController* pAnim = lua_tocppobject( pLuaState, 1, CAnimationController );
	if ( pAnim != nullptr )
	{
		RHI::ITexture* pTexture = pAnim->GetTexture();
		if ( pTexture == nullptr )
			lua_pushnil( pLuaState );
		else
		{
			lua_pushcppref( pLuaState, RHI::LuaCppName_Texture_FullName, pTexture );
		}
	}
	else
	{
		lua_pushnil( pLuaState );
	}
	return 1;
}


int iberbar::Paper2d::LuaCppFunction_AnimationController_GetCurrentColor( lua_State* pLuaState )
{
	int t = lua_gettop( pLuaState );
	if ( t != 1 )
		return iberbar_LuaError_ArgumentsCountIsNotMatch( pLuaState, 1 );

	CAnimationController* pAnim = lua_tocppobject( pLuaState, 1, CAnimationController );
	if ( pAnim != nullptr )
	{
		const UAnimationFrameNode* pFrameNode = pAnim->GetFrameNodeCurrent();
		if ( pFrameNode == nullptr )
			lua_pushnil( pLuaState );
		else
		{
			CColor4B Color = pFrameNode->Color;
			lua_newtable( pLuaState );
			int nLuaTable = lua_gettop( pLuaState );

			lua_pushstring( pLuaState, "a" );
			lua_pushinteger( pLuaState, Color.a );
			lua_settable( pLuaState, nLuaTable );

			lua_pushstring( pLuaState, "r" );
			lua_pushinteger( pLuaState, Color.r );
			lua_settable( pLuaState, nLuaTable );

			lua_pushstring( pLuaState, "g" );
			lua_pushinteger( pLuaState, Color.g );
			lua_settable( pLuaState, nLuaTable );

			lua_pushstring( pLuaState, "b" );
			lua_pushinteger( pLuaState, Color.b );
			lua_settable( pLuaState, nLuaTable );
		}
	}
	else
	{
		lua_pushnil( pLuaState );
	}
	return 1;
}


int iberbar::Paper2d::LuaCppFunction_AnimationController_GetCurrentUV( lua_State* pLuaState )
{
	int t = lua_gettop( pLuaState );
	if ( t != 1 )
		return iberbar_LuaError_ArgumentsCountIsNotMatch( pLuaState, 1 );

	CAnimationController* pAnim = lua_tocppobject( pLuaState, 1, CAnimationController );
	if ( pAnim != nullptr )
	{
		const UAnimationFrameNode* pFrameNode = pAnim->GetFrameNodeCurrent();
		if ( pFrameNode == nullptr )
			lua_pushnil( pLuaState );
		else
		{
			const CRect2f& rcTexCoord = pFrameNode->rcTexCoord;
			lua_newtable( pLuaState );
			int nLuaTable = lua_gettop( pLuaState );

			lua_pushstring( pLuaState, "l" );
			lua_pushnumber( pLuaState, rcTexCoord.l );
			lua_settable( pLuaState, nLuaTable );

			lua_pushstring( pLuaState, "t" );
			lua_pushnumber( pLuaState, rcTexCoord.t );
			lua_settable( pLuaState, nLuaTable );

			lua_pushstring( pLuaState, "r" );
			lua_pushnumber( pLuaState, rcTexCoord.r );
			lua_settable( pLuaState, nLuaTable );

			lua_pushstring( pLuaState, "b" );
			lua_pushnumber( pLuaState, rcTexCoord.b );
			lua_settable( pLuaState, nLuaTable );
		}
	}
	else
	{
		lua_pushnil( pLuaState );
	}
	return 1;
}


int iberbar::Paper2d::LuaCppFunction_AnimationController_GetName( lua_State* pLuaState )
{
	int t = lua_gettop( pLuaState );
	if ( t != 1 )
		return iberbar_LuaError_ArgumentsCountIsNotMatch( pLuaState, 1 );

	CAnimationController* pAnim = lua_tocppobject( pLuaState, 1, CAnimationController );
	if ( pAnim != nullptr )
	{
		lua_pushstring( pLuaState, pAnim->GetName().c_str() );
	}
	else
	{
		lua_pushnil( pLuaState );
	}
	return 1;
}







int iberbar::Paper2d::LuaCppFunction_Component_GetOwner( lua_State* pLuaState )
{
	int t = lua_gettop( pLuaState );
	if ( t != 1 )
		return iberbar_LuaError_ArgumentsCountIsNotMatch( pLuaState, 1 );

	CComponent* pComponent = lua_tocppobject( pLuaState, 1, CComponent );
	if ( pComponent )
	{
		CNode* pNode = pComponent->GetOwner();
		if ( pNode == nullptr )
		{
			lua_pushnil( pLuaState );
		}
		else
		{
			lua_pushcppref( pLuaState, LuaCppNames::Node_FullName, pNode );
		}
	}
	else
	{
		lua_pushnil( pLuaState );
	}
	return 1;
}


int iberbar::Paper2d::LuaCppFunction_Component_SetName( lua_State* pLuaState )
{
	int t = lua_gettop( pLuaState );
	if ( t != 2 )
		return iberbar_LuaError_ArgumentsCountIsNotMatch( pLuaState, 2 );

	CComponent* pComponent = lua_tocppobject( pLuaState, 1, CComponent );
	const char* strName = lua_tostring( pLuaState, 2 );
	if ( pComponent && strName != nullptr )
	{
		pComponent->SetName( strName );
	}
	return 0;
}


int iberbar::Paper2d::LuaCppFunction_Component_GetName( lua_State* pLuaState )
{
	int t = lua_gettop( pLuaState );
	if ( t != 1 )
		return iberbar_LuaError_ArgumentsCountIsNotMatch( pLuaState, 1 );

	CComponent* pComponent = lua_tocppobject( pLuaState, 1, CComponent );
	if ( pComponent )
	{
		lua_pushstring( pLuaState, pComponent->GetName().c_str() );
	}
	else
	{
		lua_pushnil( pLuaState );
	}
	return 1;
}








int iberbar::Paper2d::LuaCppFunction_ComponentCustomAttributes_AddAttribute( lua_State* pLuaState )
{
	int t = lua_gettop( pLuaState );
	if ( t != 3 )
		return iberbar_LuaError_ArgumentsCountIsNotMatch( pLuaState, 3 );

	CComponent_CustomAttributes* pComponent = lua_tocppobject( pLuaState, 1, CComponent_CustomAttributes );
	const char* strName = lua_tostring( pLuaState, 2 );
	if ( pComponent == nullptr && StringIsNullOrEmpty( strName ) == true )
	{
		lua_pushboolean( pLuaState, 0 );
		return 1;
	}

	bool bRet = false;
	if ( lua_isboolean( pLuaState, 3 ) )
		bRet = pComponent->AddAttributeBoolean( strName, lua_toboolean( pLuaState, 3 ) );
	else if ( lua_isinteger( pLuaState, 3 ) )
		bRet = pComponent->AddAttributeInt( strName, (int64)lua_tointeger( pLuaState, 3 ) );
	else if ( lua_isnumber( pLuaState, 3 ) )
		bRet = pComponent->AddAttributeFloat( strName, (float)lua_tonumber( pLuaState, 3 ) );
	else if ( lua_isstring( pLuaState, 3 ) )
		bRet = pComponent->AddAttributeString( strName, lua_tostring( pLuaState, 3 ) );
	else
		bRet = false;
	lua_pushboolean( pLuaState, bRet == false ? 0 : 1 );

	return 1;
}


int iberbar::Paper2d::LuaCppFunction_ComponentCustomAttributes_GetAttribute( lua_State* pLuaState )
{
	int t = lua_gettop( pLuaState );
	if ( t != 2 )
		return iberbar_LuaError_ArgumentsCountIsNotMatch( pLuaState, 2 );

	CComponent_CustomAttributes* pComponent = lua_tocppobject( pLuaState, 1, CComponent_CustomAttributes );
	const char* strName = lua_tostring( pLuaState, 2 );
	if ( pComponent == nullptr || StringIsNullOrEmpty( strName ) == true )
	{
		lua_pushnil( pLuaState );
		return 1;
	}

	const CVariant* pVar = pComponent->GetAttribute( strName );
	if ( pVar == nullptr )
	{
		lua_pushnil( pLuaState );
		return 1;
	}

	UVariantType nVarType = pVar->GetValueType();
	if ( nVarType == UVariantType::VT_Boolean )
		lua_pushboolean( pLuaState, pVar->ToBoolean().value() );
	else if ( nVarType == UVariantType::VT_String )
		lua_pushstring( pLuaState, pVar->ToString() );
	else if ( nVarType == UVariantType::VT_Int )
		lua_pushinteger( pLuaState, (lua_Integer)pVar->ToInt().value() );
	else if ( nVarType == UVariantType::VT_UInt )
		lua_pushinteger( pLuaState, (lua_Integer)pVar->ToUInt().value() );
	else if ( nVarType == UVariantType::VT_Float )
		lua_pushnumber( pLuaState, (lua_Number)pVar->ToFloat().value() );
	else
		lua_pushnil( pLuaState );

	return 1;
}


int iberbar::Paper2d::LuaCppFunction_ComponentCustomAttributes_GetAttributeString( lua_State* pLuaState )
{
	int t = lua_gettop( pLuaState );
	if ( t != 2 )
		return iberbar_LuaError_ArgumentsCountIsNotMatch( pLuaState, 2 );

	CComponent_CustomAttributes* pComponent = lua_tocppobject( pLuaState, 1, CComponent_CustomAttributes );
	const char* strName = lua_tostring( pLuaState, 2 );
	if ( pComponent == nullptr || StringIsNullOrEmpty( strName ) == true )
	{
		lua_pushnil( pLuaState );
		return 1;
	}

	const CVariant* pVar = pComponent->GetAttribute( strName );
	if ( pVar == nullptr )
	{
		lua_pushnil( pLuaState );
		return 1;
	}

	const char* Value = pVar->ToString();
	if ( Value == nullptr )
	{
		lua_pushnil( pLuaState );
		return 1;
	}

	lua_pushstring( pLuaState, Value );
	return 1;
}


int iberbar::Paper2d::LuaCppFunction_ComponentCustomAttributes_GetAttributeBoolean( lua_State* pLuaState )
{
	int t = lua_gettop( pLuaState );
	if ( t != 2 )
		return iberbar_LuaError_ArgumentsCountIsNotMatch( pLuaState, 2 );

	CComponent_CustomAttributes* pComponent = lua_tocppobject( pLuaState, 1, CComponent_CustomAttributes );
	const char* strName = lua_tostring( pLuaState, 2 );
	if ( pComponent == nullptr || StringIsNullOrEmpty( strName ) == true )
	{
		lua_pushnil( pLuaState );
		return 1;
	}

	const CVariant* pVar = pComponent->GetAttribute( strName );
	if ( pVar == nullptr )
	{
		lua_pushnil( pLuaState );
		return 1;
	}

	std::optional<bool> Value = pVar->ToString();
	if ( Value.has_value() == false )
	{
		lua_pushnil( pLuaState );
		return 1;
	}

	lua_pushboolean( pLuaState, Value.value() );
	return 1;
}


int iberbar::Paper2d::LuaCppFunction_ComponentCustomAttributes_GetAttributeInt( lua_State* pLuaState )
{
	int t = lua_gettop( pLuaState );
	if ( t != 2 )
		return iberbar_LuaError_ArgumentsCountIsNotMatch( pLuaState, 2 );

	CComponent_CustomAttributes* pComponent = lua_tocppobject( pLuaState, 1, CComponent_CustomAttributes );
	const char* strName = lua_tostring( pLuaState, 2 );
	if ( pComponent == nullptr || StringIsNullOrEmpty( strName ) == true )
	{
		lua_pushnil( pLuaState );
		return 1;
	}

	const CVariant* pVar = pComponent->GetAttribute( strName );
	if ( pVar == nullptr )
	{
		lua_pushnil( pLuaState );
		return 1;
	}

	std::optional<int64> Value = pVar->ToInt();
	if ( Value.has_value() == false )
	{
		lua_pushnil( pLuaState );
		return 1;
	}

	lua_pushinteger( pLuaState, Value.value() );
	return 1;
}


int iberbar::Paper2d::LuaCppFunction_ComponentCustomAttributes_GetAttributeFloat( lua_State* pLuaState )
{
	int t = lua_gettop( pLuaState );
	if ( t != 2 )
		return iberbar_LuaError_ArgumentsCountIsNotMatch( pLuaState, 2 );

	CComponent_CustomAttributes* pComponent = lua_tocppobject( pLuaState, 1, CComponent_CustomAttributes );
	const char* strName = lua_tostring( pLuaState, 2 );
	if ( pComponent == nullptr || StringIsNullOrEmpty( strName ) == true )
	{
		lua_pushnil( pLuaState );
		return 1;
	}

	const CVariant* pVar = pComponent->GetAttribute( strName );
	if ( pVar == nullptr )
	{
		lua_pushnil( pLuaState );
		return 1;
	}

	std::optional<float> Value = pVar->ToFloat();
	if ( Value.has_value() == false )
	{
		lua_pushnil( pLuaState );
		return 1;
	}

	lua_pushnumber( pLuaState, Value.value() );
	return 1;
}







template < typename T >
int iberbar::Paper2d::LuaCppFunction_Component_BindCallbackOnAttach( lua_State* pLuaState )
{
	int t = lua_gettop( pLuaState );
	if ( t != 2 && t != 3 )
		return iberbar_LuaError_ArgumentsCountIsNotMatch2( pLuaState, 2, 3 );

	TComponent_ForLua<T>* pComponent = lua_tocppobject( pLuaState, 1, TComponent_ForLua<T> );
	if ( pComponent )
	{
		lua_Integer nLuaCallback = lua_toluacallback( pLuaState, 2 );
		lua_Integer nLuaExt = 0;
		if ( t == 3 )
			nLuaExt = lua_toluacallback_extparam( pLuaState, 3 );
		Lua::PTR_CLuaCallbackHandler pLuaCallbackHandler = Lua::PTR_CLuaCallbackHandler::_sNew( pLuaState, nLuaCallback, nLuaExt );
		pComponent->BindCallbackOnAttach( pLuaCallbackHandler );
	}
	return 0;
}


template < typename T >
int iberbar::Paper2d::LuaCppFunction_Component_BindCallbackOnRemove( lua_State* pLuaState )
{
	int t = lua_gettop( pLuaState );
	if ( t != 2 && t != 3 )
		return iberbar_LuaError_ArgumentsCountIsNotMatch2( pLuaState, 2, 3 );

	TComponent_ForLua<T>* pComponent = lua_tocppobject( pLuaState, 1, TComponent_ForLua<T> );
	if ( pComponent )
	{
		lua_Integer nLuaCallback = lua_toluacallback( pLuaState, 2 );
		lua_Integer nLuaExt = 0;
		if ( t == 3 )
			nLuaExt = lua_toluacallback_extparam( pLuaState, 3 );
		Lua::PTR_CLuaCallbackHandler pLuaCallbackHandler = Lua::PTR_CLuaCallbackHandler::_sNew( pLuaState, nLuaCallback, nLuaExt );
		pComponent->BindCallbackOnRemove( pLuaCallbackHandler );
	}
	return 0;
}


template < typename T >
int iberbar::Paper2d::LuaCppFunction_Component_BindCallback( lua_State* pLuaState )
{
	int t = lua_gettop( pLuaState );
	if ( t != 2 && t != 3 )
		return iberbar_LuaError_ArgumentsCountIsNotMatch2( pLuaState, 2, 3 );

	TComponent_ForLua<T>* pComponent = lua_tocppobject( pLuaState, 1, TComponent_ForLua<T> );
	if ( pComponent )
	{
		lua_Integer nLuaCallback = lua_toluacallback( pLuaState, 2 );
		lua_Integer nLuaExt = 0;
		if ( t == 3 )
			nLuaExt = lua_toluacallback_extparam( pLuaState, 3 );
		Lua::PTR_CLuaCallbackHandler pLuaCallbackHandler = Lua::PTR_CLuaCallbackHandler::_sNew( pLuaState, nLuaCallback, nLuaExt );
		pComponent->BindCallback( pLuaCallbackHandler );
	}
	return 0;
}







int iberbar::Paper2d::LuaCppFunction_ComponentBoundingBox_Contains( lua_State* pLuaState )
{
	int t = lua_gettop( pLuaState );
	if ( t != 2 )
		return iberbar_LuaError_ArgumentsCountIsNotMatch( pLuaState, 2 );

	CComponent_BoundingBox* pComponent = lua_tocppobject( pLuaState, 1, CComponent_BoundingBox );
	if ( pComponent == nullptr )
	{
		lua_pushnil( pLuaState );
		return 1;
	}

	CComponent* pComponentOther = lua_tocppobject( pLuaState, 2, CComponent );
	if ( pComponentOther == nullptr || pComponentOther->IsComponentType( UComponentType::Bounding ) == false  )
	{
		lua_pushnil( pLuaState );
		return 1;
	}

	UComponentBoundingType nBoundingType = ((CComponent_Bounding*)pComponentOther)->GetBoundingType();
	if ( nBoundingType == UComponentBoundingType::BoundingBox )
	{
		lua_pushinteger( pLuaState, (lua_Integer)pComponent->Contains( (CComponent_BoundingBox*)pComponentOther ) );
	}
	else
	{
		lua_pushnil( pLuaState );
	}
	
	return 1;
}


int iberbar::Paper2d::LuaCppFunction_ComponentBoundingBox_ContainsPoint( lua_State* pLuaState )
{
	int t = lua_gettop( pLuaState );
	if ( t != 3 )
		return iberbar_LuaError_ArgumentsCountIsNotMatch( pLuaState, 3 );

	CComponent_BoundingBox* pComponent = lua_tocppobject( pLuaState, 1, CComponent_BoundingBox );
	if ( pComponent == nullptr )
	{
		lua_pushnil( pLuaState );
		return 1;
	}

	float y = (float)lua_tonumber( pLuaState, 2 );
	float x = (float)lua_tonumber( pLuaState, 3 );
	DirectX::XMFLOAT2 f2_Point( x, y );
	DirectX::ContainmentType ret = pComponent->Contains( f2_Point );
	lua_pushinteger( pLuaState, (lua_Integer)ret );
	return 1;
}


int iberbar::Paper2d::LuaCppFunction_ComponentBoundingBox_GetBox( lua_State* pLuaState )
{
	int t = lua_gettop( pLuaState );
	if ( t != 1 )
		return iberbar_LuaError_ArgumentsCountIsNotMatch( pLuaState, 1 );

	CComponent_BoundingBox* pComponent = lua_tocppobject( pLuaState, 1, CComponent_BoundingBox );
	if ( pComponent == nullptr )
	{
		lua_pushnil( pLuaState );
		return 1;
	}

	const DirectX::BoundingBox& Box = pComponent->GetBox();

	lua_newtable( pLuaState );

	lua_newtable( pLuaState );

	lua_pushstring( pLuaState, "x" );
	lua_pushnumber( pLuaState, Box.Center.x );
	lua_settable( pLuaState, -3 );

	lua_pushstring( pLuaState, "y" );
	lua_pushnumber( pLuaState, Box.Center.y );
	lua_settable( pLuaState, -3 );

	lua_pushstring( pLuaState, "center" );
	lua_rotate( pLuaState, -2, 1 );
	lua_settable( pLuaState, -3 );

	lua_newtable( pLuaState );

	lua_pushstring( pLuaState, "x" );
	lua_pushnumber( pLuaState, Box.Extents.x );
	lua_settable( pLuaState, -3 );

	lua_pushstring( pLuaState, "y" );
	lua_pushnumber( pLuaState, Box.Extents.y );
	lua_settable( pLuaState, -3 );

	lua_pushstring( pLuaState, "extents" );
	lua_rotate( pLuaState, -2, 1 );
	lua_settable( pLuaState, -3 );

	return 1;
}







int iberbar::Paper2d::LuaCFunction_RunScene( lua_State* pLuaState )
{
	int t = lua_gettop( pLuaState );
	if ( t != 1 )
		return iberbar_LuaError_ArgumentsCountIsNotMatch( pLuaState, 1 );

	CScene* pScene = lua_tocppobject( pLuaState, 1, CScene );
	CDirector::sGetShared()->RunScene( pScene );
	return 0;
}


int iberbar::Paper2d::LuaCFunction_GetScene( lua_State* pLuaState )
{
	int t = lua_gettop( pLuaState );
	if ( t != 1 )
		return iberbar_LuaError_ArgumentsCountIsNotMatch( pLuaState, 1 );

	CScene* pScene = CDirector::sGetShared()->GetRunningScene();
	if ( pScene == nullptr )
	{
		lua_pushnil( pLuaState );
		return 1;
	}
	lua_pushcppref( pLuaState, LuaCppNames::Scene_FullName, pScene );
	return 1;
}








void iberbar::Paper2d::LuaCppClassBuilder_Node( const char*, LuaCpp::CClassBuilder* cb )
{
	cb->AddMemberMethod( "Init", &LuaCppFunction_Node_Init );
	cb->AddMemberMethod( "SetId", &LuaCppFunction_Node_SetId );
	cb->AddMemberMethod( "GetId", &LuaCppFunction_Node_GetId );
	cb->AddMemberMethod( "SetVisible", &LuaCppFunction_Node_SetVisible );
	cb->AddMemberMethod( "SetEnable", &LuaCppFunction_Node_SetEnable );
	cb->AddMemberMethod( "GetVisible", &LuaCppFunction_Node_GetVisible );
	cb->AddMemberMethod( "GetEnable", &LuaCppFunction_Node_GetEnable );
	cb->AddMemberMethod( "IsVisible", &LuaCppFunction_Node_IsVisible );
	cb->AddMemberMethod( "IsEnable", &LuaCppFunction_Node_IsEnable );
	cb->AddMemberMethod( "SetPosition", &LuaCppFunction_Node_SetPosition );
	cb->AddMemberMethod( "GetPosition", &LuaCppFunction_Node_GetPosition );
	cb->AddMemberMethod( "SetContentSize", &LuaCppFunction_Node_SetSize );
	cb->AddMemberMethod( "GetContentSize", &LuaCppFunction_Node_GetSize );
	cb->AddMemberMethod( "SetAnchor", &LuaCppFunction_Node_SetAnchorPoint );
	cb->AddMemberMethod( "GetAnchor", &LuaCppFunction_Node_GetAnchorPoint );
	cb->AddMemberMethod( "SetPivot", &LuaCppFunction_Node_SetPivot );
	cb->AddMemberMethod( "GetPivot", &LuaCppFunction_Node_GetPivot );
	cb->AddMemberMethod( "SetZOrder", &LuaCppFunction_Node_SetZOrder );
	cb->AddMemberMethod( "GetZOrder", &LuaCppFunction_Node_GetZOrder );
	cb->AddMemberMethod( "InsertChild", &LuaCppFunction_Node_InsertChild );
	cb->AddMemberMethod( "RemoveChild", &LuaCppFunction_Node_RemoveChild );
	cb->AddMemberMethod( "RemoveAllChildren", &LuaCppFunction_Node_RemoveAllChildren );
	cb->AddMemberMethod( "FindChild", &LuaCppFunction_Node_FindChild );
	cb->AddMemberMethod( "GetChildren", &LuaCppFunction_Node_GetChildren );
	cb->AddMemberMethod( "AddComponent", &LuaCppFunction_Node_AddComponent );
	cb->AddMemberMethod( "GetComponent", &LuaCppFunction_Node_GetComponent );
	cb->AddMemberMethod( "GetComponentDefaultCustomAttributes", &LuaCppFunction_Node_GetComponentDefaultCustomAttributes );

	cb->AddConstructor( &LuaCpp::Class_Unknown_Constructor_New<CNode> );
	cb->AddDistructor( &LuaCpp::Class_Unknown_Distructor_Release<CNode> );
}


void iberbar::Paper2d::LuaCppClassBuilder_Camera( const char*, LuaCpp::CClassBuilder* cb )
{
	cb->AddMemberMethod( "SetPosition", &LuaCppFunction_Camera_SetPosition );
	cb->AddMemberMethod( "SetOrthographicProjection", &LuaCppFunction_Camera_SetOrthographicProjection );
	cb->AddMemberMethod( "Build", &LuaCppFunction_Camera_Build );

	cb->AddDistructor( &LuaCpp::Class_Unknown_Distructor_Release<CCamera> );
}


void iberbar::Paper2d::LuaCppClassBuilder_Scene( const char*, LuaCpp::CClassBuilder* cb )
{
	cb->AddMemberMethod( "GetCamera", &LuaCppFunction_Scene_GetCamera );

	cb->AddConstructor( &LuaCpp::Class_Unknown_Constructor_New<CScene> );
	cb->AddDistructor( &LuaCpp::Class_Unknown_Distructor_Release<CScene> );
}


void iberbar::Paper2d::LuaCppClassBuilder_Image( const char*, LuaCpp::CClassBuilder* cb )
{
	cb->AddMemberMethod( "SetTexture", &LuaCppFunction_Image_SetTexture );
	cb->AddMemberMethod( "SetColor", &LuaCppFunction_Image_SetColor );
	cb->AddMemberMethod( "SetUV", &LuaCppFunction_Image_SetUV );

	cb->AddConstructor( &LuaCpp::Class_Unknown_Constructor_New<CImage> );
	cb->AddDistructor( &LuaCpp::Class_Unknown_Distructor_Release<CImage> );
	cb->AddStaticMethod( "dynamic_cast", &LuaCpp::Class_Unknown_Dynamic_Cast<CImage> );
}


void iberbar::Paper2d::LuaCppClassBuilder_GridTerrain( const char*, LuaCpp::CClassBuilder* cb )
{
	cb->AddMemberMethod( "SetGridSize", &LuaCppFunction_GridTerrain_SetGridSize );
	cb->AddMemberMethod( "GetGridRow", &LuaCppFunction_GridTerrain_GetGridRow );
	cb->AddMemberMethod( "GetGridCol", &LuaCppFunction_GridTerrain_GetGridCol );

	cb->AddConstructor( &LuaCpp::Class_Unknown_Constructor_New<CGridTerrain> );
	cb->AddDistructor( &LuaCpp::Class_Unknown_Distructor_Release<CGridTerrain> );
	cb->AddStaticMethod( "dynamic_cast", &LuaCpp::Class_Unknown_Dynamic_Cast<CGridTerrain> );
}


void iberbar::Paper2d::LuaCppClassBuilder_AnimationController( const char*, LuaCpp::CClassBuilder* cb )
{
	cb->AddMemberMethod( "BindCallback", &LuaCppFunction_AnimationController_BindCallback );
	cb->AddMemberMethod( "Play", &LuaCppFunction_AnimationController_Play );
	cb->AddMemberMethod( "Stop", &LuaCppFunction_AnimationController_Stop );
	cb->AddMemberMethod( "Update", &LuaCppFunction_AnimationController_Update );
	cb->AddMemberMethod( "GetTexture", &LuaCppFunction_AnimationController_GetTexture );
	cb->AddMemberMethod( "GetCurrentColor", &LuaCppFunction_AnimationController_GetCurrentColor );
	cb->AddMemberMethod( "GetCurrentUV", &LuaCppFunction_AnimationController_GetCurrentUV );
	cb->AddMemberMethod( "GetName", &LuaCppFunction_AnimationController_GetName );

	cb->AddConstructor( &LuaCpp::Class_Unknown_Constructor_New<CAnimationController> );
	cb->AddDistructor( &LuaCpp::Class_Unknown_Distructor_Release<CAnimationController> );
}


void iberbar::Paper2d::LuaCppClassBuilder_Component( const char*, LuaCpp::CClassBuilder* cb )
{
	cb->AddMemberMethod( "GetOwner", &LuaCppFunction_Component_GetOwner );
	cb->AddMemberMethod( "SetName", &LuaCppFunction_Component_SetName );
	cb->AddMemberMethod( "GetName", &LuaCppFunction_Component_GetName );
	
	cb->AddDistructor( &LuaCpp::Class_Unknown_Distructor_Release<CComponent> );
}


void iberbar::Paper2d::LuaCppClassBuilder_ComponentCustomAttributes( const char*, LuaCpp::CClassBuilder* cb )
{
	cb->AddMemberMethod( "AddAttribute", &LuaCppFunction_ComponentCustomAttributes_AddAttribute );
	cb->AddMemberMethod( "GetAttribute", &LuaCppFunction_ComponentCustomAttributes_GetAttribute );
	cb->AddMemberMethod( "GetAttributeString", &LuaCppFunction_ComponentCustomAttributes_GetAttributeString );
	cb->AddMemberMethod( "GetAttributeBoolean", &LuaCppFunction_ComponentCustomAttributes_GetAttributeBoolean );
	cb->AddMemberMethod( "GetAttributeInt", &LuaCppFunction_ComponentCustomAttributes_GetAttributeInt );
	cb->AddMemberMethod( "GetAttributeFloat", &LuaCppFunction_ComponentCustomAttributes_GetAttributeFloat );

	cb->AddConstructor( &LuaCpp::Class_Unknown_Constructor_New<CComponent_CustomAttributes> );
	cb->AddDistructor( &LuaCpp::Class_Unknown_Distructor_Release<CComponent_CustomAttributes> );

	cb->AddStaticMethod( "dynamic_cast", &LuaCpp::Class_Unknown_Dynamic_Cast<CComponent_CustomAttributes> );
}


void iberbar::Paper2d::LuaCppClassBuilder_ComponentUpdatable( const char*, LuaCpp::CClassBuilder* cb )
{
	cb->AddMemberMethod( "BindCallback", &LuaCppFunction_Component_BindCallback<CComponent_Updatable> );
	cb->AddMemberMethod( "BindCallbackOnAttach", &LuaCppFunction_Component_BindCallbackOnAttach<CComponent_Updatable> );
	cb->AddMemberMethod( "BindCallbackOnRemove", &LuaCppFunction_Component_BindCallbackOnRemove<CComponent_Updatable> );

	cb->AddConstructor( &LuaCpp::Class_Unknown_Constructor_New<CComponent_Updatable_ForLua> );
	cb->AddDistructor( &LuaCpp::Class_Unknown_Distructor_Release<CComponent_Updatable_ForLua> );

	cb->AddStaticMethod( "dynamic_cast", &LuaCpp::Class_Unknown_Dynamic_Cast<CComponent_Updatable_ForLua> );
}


void iberbar::Paper2d::LuaCppClassBuilder_ComponentHandleMouseInput( const char*, LuaCpp::CClassBuilder* cb )
{
	cb->AddMemberMethod( "BindCallback", &LuaCppFunction_Component_BindCallback<CComponent_HandleMouseInput> );
	cb->AddMemberMethod( "BindCallbackOnAttach", &LuaCppFunction_Component_BindCallbackOnAttach<CComponent_HandleMouseInput> );
	cb->AddMemberMethod( "BindCallbackOnRemove", &LuaCppFunction_Component_BindCallbackOnRemove<CComponent_HandleMouseInput> );

	cb->AddConstructor( &LuaCpp::Class_Unknown_Constructor_New<CComponent_HandleMouseInput_ForLua> );
	cb->AddDistructor( &LuaCpp::Class_Unknown_Distructor_Release<CComponent_HandleMouseInput_ForLua> );

	cb->AddStaticMethod( "dynamic_cast", &LuaCpp::Class_Unknown_Dynamic_Cast<CComponent_HandleMouseInput_ForLua> );
}


void iberbar::Paper2d::LuaCppClassBuilder_ComponentHandleKeyboardInput( const char*, LuaCpp::CClassBuilder* cb )
{
	cb->AddMemberMethod( "BindCallback", &LuaCppFunction_Component_BindCallback<CComponent_HandleKeyboardInput> );
	cb->AddMemberMethod( "BindCallbackOnAttach", &LuaCppFunction_Component_BindCallbackOnAttach<CComponent_HandleKeyboardInput> );
	cb->AddMemberMethod( "BindCallbackOnRemove", &LuaCppFunction_Component_BindCallbackOnRemove<CComponent_HandleKeyboardInput> );

	cb->AddConstructor( &LuaCpp::Class_Unknown_Constructor_New<CComponent_HandleKeyboardInput_ForLua> );
	cb->AddDistructor( &LuaCpp::Class_Unknown_Distructor_Release<CComponent_HandleKeyboardInput_ForLua> );

	cb->AddStaticMethod( "dynamic_cast", &LuaCpp::Class_Unknown_Dynamic_Cast<CComponent_HandleKeyboardInput_ForLua> );
}


void iberbar::Paper2d::LuaCppClassBuilder_ComponentTransform( const char*, LuaCpp::CClassBuilder* cb )
{
	cb->AddMemberMethod( "BindCallback", &LuaCppFunction_Component_BindCallback<CComponent_Transform> );
	cb->AddMemberMethod( "BindCallbackOnAttach", &LuaCppFunction_Component_BindCallbackOnAttach<CComponent_Transform> );
	cb->AddMemberMethod( "BindCallbackOnRemove", &LuaCppFunction_Component_BindCallbackOnRemove<CComponent_Transform> );

	cb->AddConstructor( &LuaCpp::Class_Unknown_Constructor_New<CComponent_Transform_ForLua> );
	cb->AddDistructor( &LuaCpp::Class_Unknown_Distructor_Release<CComponent_Transform_ForLua> );

	cb->AddStaticMethod( "dynamic_cast", &LuaCpp::Class_Unknown_Dynamic_Cast<CComponent_Transform_ForLua> );
}


void iberbar::Paper2d::LuaCppClassBuilder_ComponentBoundingBox( const char*, LuaCpp::CClassBuilder* cb )
{
	cb->AddMemberMethod( "Contains", &LuaCppFunction_ComponentBoundingBox_Contains );
	cb->AddMemberMethod( "ContainsPoint", &LuaCppFunction_ComponentBoundingBox_ContainsPoint );
	cb->AddMemberMethod( "GetBox", &LuaCppFunction_ComponentBoundingBox_GetBox );

	cb->AddConstructor( &LuaCpp::Class_Unknown_Constructor_New<CComponent_BoundingBox> );
	cb->AddDistructor( &LuaCpp::Class_Unknown_Distructor_Release<CComponent_BoundingBox> );

	cb->AddStaticMethod( "dynamic_cast", &LuaCpp::Class_Unknown_Dynamic_Cast<CComponent_BoundingBox> );
}








void iberbar::Paper2d::CComponent_Updatable_ForLua::OnAttach()
{
	CComponent_Updatable::OnAttach();

	LuaCall_OnAttachOrRemove( m_pLuaCallbackHandler_OnAttach, LuaCppNames::ComponentUpdatable_FullName );
}


void iberbar::Paper2d::CComponent_Updatable_ForLua::OnRemove()
{
	CComponent_Updatable::OnRemove();

	LuaCall_OnAttachOrRemove( m_pLuaCallbackHandler_OnRemove, LuaCppNames::ComponentUpdatable_FullName );
}


void iberbar::Paper2d::CComponent_Updatable_ForLua::Update( float nDelta )
{
	if ( m_pLuaCallbackHandler && m_pLuaCallbackHandler->PushFunction() == true )
	{
		lua_State* pLuaState = m_pLuaCallbackHandler->GetLuaState();

		int nArg = 2;
		if ( m_pLuaCallbackHandler->HasExt() == true )
		{
			if ( m_pLuaCallbackHandler->PushExt() == true )
				nArg++;
		}

		lua_pushcppref( pLuaState, LuaCppNames::ComponentUpdatable_FullName, this );

		lua_pushnumber( pLuaState, (lua_Number)nDelta );

		int ret = lua_pcall( pLuaState, nArg, LUA_MULTRET, 0 );
		if ( ret != 0 )
		{
			int type = lua_type( pLuaState, -1 );
			const char* err = lua_tostring( pLuaState, -1 );
			lua_pop( pLuaState, 1 );
		}
	}
}






void iberbar::Paper2d::CComponent_HandleMouseInput_ForLua::OnAttach()
{
	CComponent_HandleMouseInput::OnAttach();

	LuaCall_OnAttachOrRemove( m_pLuaCallbackHandler_OnAttach, LuaCppNames::ComponentHandleMouseInput_FullName );
}


void iberbar::Paper2d::CComponent_HandleMouseInput_ForLua::OnRemove()
{
	CComponent_HandleMouseInput::OnRemove();

	LuaCall_OnAttachOrRemove( m_pLuaCallbackHandler_OnRemove, LuaCppNames::ComponentHandleMouseInput_FullName );
}


void iberbar::Paper2d::CComponent_HandleMouseInput_ForLua::OnMouseEvent( const UComponentMouseEventData* EventData )
{
	if ( m_pLuaCallbackHandler != nullptr && m_pLuaCallbackHandler->PushFunction() == true )
	{
		lua_State* pLuaState = m_pLuaCallbackHandler->GetLuaState();

		int nArg = 2;
		if ( m_pLuaCallbackHandler->HasExt() == true )
		{
			if ( m_pLuaCallbackHandler->PushExt() == true )
				nArg++;
		}

		lua_pushcppref( pLuaState, LuaCppNames::ComponentHandleMouseInput_FullName, this );

		lua_newtable( pLuaState );
		int nTableRoot = lua_gettop( pLuaState );

		lua_pushstring( pLuaState, "event" );
		lua_pushinteger( pLuaState, (lua_Integer)EventData->nEvent );
		lua_settable( pLuaState, nTableRoot );

		lua_pushstring( pLuaState, "mouseid" );
		lua_pushinteger( pLuaState, (lua_Integer)EventData->nMouseId );
		lua_settable( pLuaState, nTableRoot );

		lua_pushstring( pLuaState, "wheel" );
		lua_pushinteger( pLuaState, (lua_Integer)EventData->nMouseWheel );
		lua_settable( pLuaState, nTableRoot );

		int nTableForPoint;

		lua_newtable( pLuaState );
		nTableForPoint = lua_gettop( pLuaState );

		lua_pushstring( pLuaState, "x" );
		lua_pushinteger( pLuaState, (lua_Integer)EventData->MousePoint.x );
		lua_settable( pLuaState, nTableForPoint );

		lua_pushstring( pLuaState, "y" );
		lua_pushinteger( pLuaState, (lua_Integer)EventData->MousePoint.y );
		lua_settable( pLuaState, nTableForPoint );

		lua_pushstring( pLuaState, "mousePoint" );
		lua_rotate( pLuaState, -2, 1 );
		lua_settable( pLuaState, nTableRoot );

		lua_newtable( pLuaState );
		nTableForPoint = lua_gettop( pLuaState );

		lua_pushstring( pLuaState, "x" );
		lua_pushinteger( pLuaState, (lua_Integer)EventData->WorldPoint.x );
		lua_settable( pLuaState, nTableForPoint );

		lua_pushstring( pLuaState, "y" );
		lua_pushinteger( pLuaState, (lua_Integer)EventData->WorldPoint.y );
		lua_settable( pLuaState, nTableForPoint );

		lua_pushstring( pLuaState, "worldPoint" );
		lua_rotate( pLuaState, -2, 1 );
		lua_settable( pLuaState, nTableRoot );

		int ret = lua_pcall( pLuaState, nArg, LUA_MULTRET, 0 );
		if ( ret != 0 )
		{
			int type = lua_type( pLuaState, -1 );
			const char* err = lua_tostring( pLuaState, -1 );
			lua_pop( pLuaState, 1 );
		}
	}
}








void iberbar::Paper2d::CComponent_HandleKeyboardInput_ForLua::OnAttach()
{
	CComponent_HandleKeyboardInput::OnAttach();

	LuaCall_OnAttachOrRemove( m_pLuaCallbackHandler_OnAttach, LuaCppNames::ComponentHandleKeyboardInput_FullName );
}


void iberbar::Paper2d::CComponent_HandleKeyboardInput_ForLua::OnRemove()
{
	CComponent_HandleKeyboardInput::OnRemove();

	LuaCall_OnAttachOrRemove( m_pLuaCallbackHandler_OnRemove, LuaCppNames::ComponentHandleKeyboardInput_FullName );
}


void iberbar::Paper2d::CComponent_HandleKeyboardInput_ForLua::OnKeyboardEvent( const UComponentKeyboardEventData* EventData )
{
	if ( m_pLuaCallbackHandler != nullptr && m_pLuaCallbackHandler->PushFunction() == true )
	{
		lua_State* pLuaState = m_pLuaCallbackHandler->GetLuaState();

		int nArg = 2;
		if ( m_pLuaCallbackHandler->HasExt() == true )
		{
			if ( m_pLuaCallbackHandler->PushExt() == true )
				nArg++;
		}

		lua_pushcppref( pLuaState, LuaCppNames::ComponentHandleKeyboardInput_FullName, this );

		lua_newtable( pLuaState );
		int nLuaTable = lua_gettop( pLuaState );

		lua_pushstring( pLuaState, "event" );
		lua_pushinteger( pLuaState, (lua_Integer)EventData->nEvent );
		lua_settable( pLuaState, nLuaTable );

		lua_pushstring( pLuaState, "keycode" );
		lua_pushinteger( pLuaState, (lua_Integer)EventData->nKeyCode );
		lua_settable( pLuaState, nLuaTable );

		int ret = lua_pcall( pLuaState, nArg, LUA_MULTRET, 0 );
		if ( ret != 0 )
		{
			int type = lua_type( pLuaState, -1 );
			const char* err = lua_tostring( pLuaState, -1 );
			lua_pop( pLuaState, 1 );
		}
	}
}







void iberbar::Paper2d::CComponent_Transform_ForLua::OnAttach()
{
	CComponent_Transform::OnAttach();

	LuaCall_OnAttachOrRemove( m_pLuaCallbackHandler_OnAttach, LuaCppNames::ComponentTransform_FullName );
}


void iberbar::Paper2d::CComponent_Transform_ForLua::OnRemove()
{
	CComponent_Transform::OnRemove();

	LuaCall_OnAttachOrRemove( m_pLuaCallbackHandler_OnRemove, LuaCppNames::ComponentTransform_FullName );
}


void iberbar::Paper2d::CComponent_Transform_ForLua::OnUpdateTransform()
{
	if ( m_pLuaCallbackHandler != nullptr && m_pLuaCallbackHandler->PushFunction() == true )
	{
		lua_State* pLuaState = m_pLuaCallbackHandler->GetLuaState();

		int nArg = 1;
		if ( m_pLuaCallbackHandler->HasExt() == true )
		{
			if ( m_pLuaCallbackHandler->PushExt() == true )
				nArg++;
		}

		lua_pushcppref( pLuaState, LuaCppNames::ComponentTransform_FullName, this );

		int ret = lua_pcall( pLuaState, nArg, LUA_MULTRET, 0 );
		if ( ret != 0 )
		{
			int type = lua_type( pLuaState, -1 );
			const char* err = lua_tostring( pLuaState, -1 );
			lua_pop( pLuaState, 1 );
		}
	}
}
