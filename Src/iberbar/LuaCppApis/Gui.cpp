
#include <iberbar/LuaCppApis/Gui.h>
#include <iberbar/LuaCppApis/Gui_Names.h>
#include <iberbar/LuaCppApis/Utility.h>
#include <iberbar/LuaCppApis/Utility_Names.h>
#include <iberbar/Lua/LuaError.h>
#include <iberbar/Lua/LuaCppInstantiateDef.h>
#include <iberbar/Lua/LuaCppBuilder.h>
#include <iberbar/Lua/LuaCallback.h>
#include <iberbar/Lua/FunctionHelper.h>

#include <iberbar/Gui/Engine.h>
#include <iberbar/Gui/Dialog.h>
#include <iberbar/Gui/Commands.h>
#include <iberbar/Gui/Widgets/Button.h>
#include <iberbar/Gui/Widgets/CheckBox.h>
#include <iberbar/Gui/Widgets/RadioBox.h>
#include <iberbar/Gui/Widgets/ListBox.h>
#include <iberbar/Gui/Widgets/EditBox.h>
#include <iberbar/Gui/Widgets/ProgressBar.h>
#include <iberbar/Gui/Element/ElemColorRect.h>
#include <iberbar/Gui/Element/ElemStateLabel.h>
#include <iberbar/Gui/Element/ElemStateTexture.h>
#include <iberbar/Gui/Xml/XmlState.h>

#include <iberbar/RHI/Texture.h>
#include <iberbar/Renderer/Font.h>




#define LuaCppNamespace "iberbar.Gui"

#define LuaCppName_Button "CButton"
#define LuaCppName_Button_FullName LuaCppNamespace "." LuaCppName_Button

#define LuaCppName_Engine "CEngine"
#define LuaCppName_Engine_FullName LuaCppNamespace "." LuaCppName_Engine

#define LuaCppName_XmlParser "CXmlParser"
#define LuaCppName_XmlParser_FullName LuaCppNamespace "." LuaCppName_XmlParser


namespace iberbar
{
	namespace Gui
	{

		//class CVariantDictionaryForLua
		//	: private CVariantDictionary
		//{
		//public:
		//	void ToLua( lua_State* pLuaState, int n ) const;
		//	void FromLua( lua_State* pLuaState, int n );
		//};

		//typedef TListBox<CVariantDictionaryForLua> CListBoxDictionaryForLua;

		void ListBoxItemDataDictionary_ToLua( const CVariantDictionary& Dict, lua_State* pLuaState, int n );
		void ListBoxItemDataDictionary_FromLua( CVariantDictionary& Dict, lua_State* pLuaState, int n );


		int LuaCppFunction_Object_SetId( lua_State* L );
		int LuaCppFunction_Object_GetId( lua_State* L );
		int LuaCppFunction_Object_SetPosition( lua_State* L );
		int LuaCppFunction_Object_GetPosition( lua_State* L );
		int LuaCppFunction_Object_SetSize( lua_State* L );
		int LuaCppFunction_Object_GetSize( lua_State* L );
		int LuaCppFunction_Object_SetPaddings( lua_State* L );
		int LuaCppFunction_Object_GetPaddings( lua_State* L );
		int LuaCppFunction_Object_SetVisible( lua_State* L );
		int LuaCppFunction_Object_GetVisible( lua_State* L );
		int LuaCppFunction_Object_SetAlignHorizental( lua_State* pLuaState );
		int LuaCppFunction_Object_GetAlignHorizental( lua_State* pLuaState );
		int LuaCppFunction_Object_SetAlignVertical( lua_State* pLuaState );
		int LuaCppFunction_Object_GetAlignVertical( lua_State* pLuaState );
		int LuaCppFunction_Object_SetPercentX( lua_State* pLuaState );
		int LuaCppFunction_Object_SetPercentY( lua_State* pLuaState );
		int LuaCppFunction_Object_SetPercentW( lua_State* pLuaState );
		int LuaCppFunction_Object_SetPercentH( lua_State* pLuaState );
		int LuaCppFunction_Object_Refresh( lua_State* pLuaState );
		int LuaCppFunction_Object_SetDebugId( lua_State* pLuaState );
		int LuaCppFunction_Object_GetBounding( lua_State* pLuaState );

		int LuaCppFunction_Dialog_SetWidgetRoot( lua_State* pLuaState );
		int LuaCppFunction_Dialog_GetWidgetRoot( lua_State* pLuaState );
		int LuaCppFunction_Dialog_RequestTop( lua_State* pLuaState );
		int LuaCppFunction_Dialog_SetModal( lua_State* pLuaState );
		int LuaCppFunction_Dialog_IsModal( lua_State* pLuaState );

		int LuaCppFunction_Widget_AddWidget( lua_State* pLuaState );
		int LuaCppFunction_Widget_FindWidget( lua_State* pLuaState );
		int LuaCppFunction_Widget_FindElement( lua_State* pLuaState );
		int LuaCppFunction_Widget_AddEventCallback( lua_State* pLuaState );
		int LuaCppFunction_Widget_RemoveEventCallback( lua_State* pLuaState );
		int LuaCppFunction_Widget_RemoveEventCallbacksAll( lua_State* pLuaState );
		int LuaCppFunction_Widget_SetRenderElement( lua_State* pLuaState );
		int LuaCppFunction_Widget_RequestFocus( lua_State* pLuaState );
		int LuaCppFunction_Widget_SetNeedClip( lua_State* pLuaState );
		int LuaCppFunction_Widget_Clone( lua_State* pLuaState );

		int LuaCppFunction_WidgetButton_SetHotKey( lua_State* pLuaState );

		int LuaCppFunction_WidgetRadioBox_SetGroup( lua_State* pLuaState );
		int LuaCppFunction_WidgetRadioBox_GetGroup( lua_State* pLuaState );

		int LuaCppFunction_WidgetListBox_SetDragStyle( lua_State* pLuaState );
		int LuaCppFunction_WidgetListBox_SetSelectType( lua_State* pLuaState );
		int LuaCppFunction_WidgetListBox_SetItemSize( lua_State* pLuaState );
		int LuaCppFunction_WidgetListBox_SetItemMargin( lua_State* pLuaState );
		int LuaCppFunction_WidgetListBox_SetCallback_ItemElementAllocate( lua_State* pLuaState );
		int LuaCppFunction_WidgetListBox_SetCallback_ItemElementRelease( lua_State* pLuaState );
		int LuaCppFunction_WidgetListBox_SetCallback_ItemElementCreate( lua_State* pLuaState );
		int LuaCppFunction_WidgetListBox_SetCallback_ItemElementUpdate( lua_State* pLuaState );
		int LuaCppFunction_WidgetListBox_AddItem( lua_State* pLuaState );
		int LuaCppFunction_WidgetListBox_UpdateItem( lua_State* pLuaState );
		int LuaCppFunction_WidgetListBox_UpdateItemFields( lua_State* pLuaState );
		int LuaCppFunction_WidgetListBox_RemoveItemOne( lua_State* pLuaState );
		int LuaCppFunction_WidgetListBox_RemoveItems( lua_State* pLuaState );
		int LuaCppFunction_WidgetListBox_RemoveItemsAll( lua_State* pLuaState );
		int LuaCppFunction_WidgetListBox_GetItem( lua_State* pLuaState );
		int LuaCppFunction_WidgetListBox_FindItemOne( lua_State* pLuaState );
		int LuaCppFunction_WidgetListBox_FindItems( lua_State* pLuaState );
		int LuaCppFunction_WidgetListBox_IsSelect( lua_State* pLuaState );
		int LuaCppFunction_WidgetListBox_GetSelectFirst( lua_State* pLuaState );
		int LuaCppFunction_WidgetListBox_GetSelectAll( lua_State* pLuaState );
		int LuaCppFunction_WidgetListBox_GetItemContextMenu( lua_State* pLuaState );
		int LuaCppFunction_WidgetListBox_GetItemContextMenuAtPoint( lua_State* pLuaState );
		int LuaCppFunction_WidgetListBox_GetItemBounding( lua_State* pLuaState );
		int LuaCppFunction_WidgetListBox_SetItemEnable( lua_State* pLuaState );
		int LuaCppFunction_WidgetListBox_IsItemEnable( lua_State* pLuaState );
		CRenderElement* LuaCppCallbackExecute_WidgetListBox_ItemElementAllocate( Lua::PTR_CLuaCallbackHandler pLuaCallback );
		void LuaCppCallbackExecute_WidgetListBox_ItemElementRelease( Lua::PTR_CLuaCallbackHandler pLuaCallback, CRenderElement* pElement );
		void LuaCppCallbackExecute_WidgetListBox_ItemElementCreate( Lua::PTR_CLuaCallbackHandler pLuaCallback, CRenderElement* pElement, const CVariantDictionary* pData, int nIndex );
		void LuaCppCallbackExecute_WidgetListBox_ItemElementUpdate( Lua::PTR_CLuaCallbackHandler pLuaCallback, CRenderElement* pElement, const CVariantDictionary* pData, int nIndex, int nUpdateEvent );
		bool LuaCppCallbackExecute_WidgetListBox_FindItem( Lua::PTR_CLuaCallbackHandler pLuaCallback, const CVariantDictionary* pData );

		int LuaCppFunction_WidgetEditBox_SetValueText( lua_State* pLuaState );
		int LuaCppFunction_WidgetEditBox_GetValueText( lua_State* pLuaState );

		int LuaCppFunction_WidgetProgressBar_SetProgressValueMax( lua_State* pLuaState );
		int LuaCppFunction_WidgetProgressBar_SetProgressValue( lua_State* pLuaState );
		int LuaCppFunction_WidgetProgressBar_GetProgressValueMax( lua_State* pLuaState );
		int LuaCppFunction_WidgetProgressBar_GetProgressValue( lua_State* pLuaState );

		int LuaCppFunction_Element_Init( lua_State* pLuaState );
		int LuaCppFunction_Element_SetZOrder( lua_State* pLuaState );
		int LuaCppFunction_Element_Clone( lua_State* pLuaState );
		int LuaCppFunction_Element_FindElement( lua_State* pLuaState );
		int LuaCppFunction_Element_AddElement( lua_State* pLuaState );

		int LuaCppFunction_ElementColorRect_SetColor( lua_State* pLuaState );

		int LuaCppFunction_ElementStateLabel_SetFont( lua_State* pLuaState );
		int LuaCppFunction_ElementStateLabel_SetText( lua_State* pLuaState );
		int LuaCppFunction_ElementStateLabel_SetColor( lua_State* pLuaState );
		int LuaCppFunction_ElementStateLabel_SetTextAlignHorizental( lua_State* pLuaState );
		int LuaCppFunction_ElementStateLabel_SetTextAlignVertical( lua_State* pLuaState );

		int LuaCppFunction_ElementStateTexture_SetTexture( lua_State* pLuaState );
		int LuaCppFunction_ElementStateTexture_SetColor( lua_State* pLuaState );
		int LuaCppFunction_ElementStateTexture_SetUV( lua_State* pLuaState );

		int LuaCppFunction_Engine_CreateDialog( lua_State* L );
		int LuaCppFunction_Engine_GetDialog( lua_State* L );
		int LuaCppFunction_Engine_DestroyDialog( lua_State* L );

		int LuaCppFunction_XmlParser_ReadFile( lua_State* pLuaState );
		int LuaCppFunction_XmlParser_SetCallbackGetTexture( lua_State* pLuaState );
		int LuaCppFunction_XmlParser_SetCallbackGetFont( lua_State* pLuaState );

		void LuaCppCallbackExecute_WidetEvent( Lua::PTR_CLuaCallbackHandler pLuaCallback, CWidget* pWidget, uint64 nEvent, uint64 nValueUint );
		CResult LuaCppCallbackExecute_XmlParser_CallbackGetTexture( Lua::PTR_CLuaCallbackHandler pLuaCallback, const char* strText, RHI::ITexture** ppOutTexture );
		CResult LuaCppCallbackExecute_XmlParser_CallbackGetFont( Lua::PTR_CLuaCallbackHandler pLuaCallback, Renderer::CFont** ppOutFont, const UFontDesc& FontDesc );




		static const Lua::UClassDefinition Defs[] = {
			{

			}
		};
	}
}





void iberbar::Gui::RegisterLuaCpp( lua_State* L )
{
	Lua::CBuilder builder( L );
	builder.ResolveScope(
		[]( Lua::CScopeBuilder* scope )
		{
			scope->AddClass( LuaCppNames::s_ClassName_Object,
				[]( const char*, Lua::CClassBuilder* pClass )
				{
					pClass->AddMemberMethod( "SetId", &LuaCppFunction_Object_SetId );
					pClass->AddMemberMethod( "GetId", &LuaCppFunction_Object_GetId );
					pClass->AddMemberMethod( "SetPosition", &LuaCppFunction_Object_SetPosition );
					pClass->AddMemberMethod( "GetPosition", &LuaCppFunction_Object_GetPosition );
					pClass->AddMemberMethod( "SetSize", &LuaCppFunction_Object_SetSize );
					pClass->AddMemberMethod( "GetSize", &LuaCppFunction_Object_GetSize );
					pClass->AddMemberMethod( "SetPaddings", &LuaCppFunction_Object_SetPaddings );
					pClass->AddMemberMethod( "GetPaddings", &LuaCppFunction_Object_GetPaddings );
					pClass->AddMemberMethod( "SetVisible", &LuaCppFunction_Object_SetVisible );
					pClass->AddMemberMethod( "GetVisible", &LuaCppFunction_Object_GetVisible );
					pClass->AddMemberMethod( "SetAlignHorizental", &LuaCppFunction_Object_SetAlignHorizental );
					pClass->AddMemberMethod( "GetAlignHorizental", &LuaCppFunction_Object_GetAlignHorizental );
					pClass->AddMemberMethod( "SetAlignVertical", &LuaCppFunction_Object_SetAlignVertical );
					pClass->AddMemberMethod( "GetAlignVertical", &LuaCppFunction_Object_GetAlignVertical );
					pClass->AddMemberMethod( "SetPercentX", &LuaCppFunction_Object_SetPercentX );
					pClass->AddMemberMethod( "SetPercentY", &LuaCppFunction_Object_SetPercentY );
					pClass->AddMemberMethod( "SetPercentW", &LuaCppFunction_Object_SetPercentW );
					pClass->AddMemberMethod( "SetPercentH", &LuaCppFunction_Object_SetPercentH );
					pClass->AddMemberMethod( "Refresh", &LuaCppFunction_Object_Refresh );
					pClass->AddMemberMethod( "SetDebugId", &LuaCppFunction_Object_SetDebugId );
					pClass->AddMemberMethod( "GetBounding", &LuaCppFunction_Object_GetBounding );
					pClass->AddDistructor( &Lua::Class_Unknown_Distructor_Release<CObject> );
				}, iberbar::LuaCppNames::s_ClassName_Ref_FullName );

			scope->AddClass( LuaCppNames::s_ClassName_Widget,
				[]( const char*, Lua::CClassBuilder* pClass )
				{
					pClass->AddMemberMethod( "AddWidget", &LuaCppFunction_Widget_AddWidget );
					pClass->AddMemberMethod( "FindWidget", &LuaCppFunction_Widget_FindWidget );
					pClass->AddMemberMethod( "FindElement", &LuaCppFunction_Widget_FindElement );
					pClass->AddMemberMethod( "AddEventCallback", &LuaCppFunction_Widget_AddEventCallback );
					pClass->AddMemberMethod( "RemoveEventCallback", &LuaCppFunction_Widget_RemoveEventCallback );
					pClass->AddMemberMethod( "RemoveEventCallbacksAll", &LuaCppFunction_Widget_RemoveEventCallbacksAll );
					pClass->AddMemberMethod( "SetRenderElement", &LuaCppFunction_Widget_SetRenderElement );
					pClass->AddMemberMethod( "Clone", &LuaCppFunction_Widget_Clone );
					pClass->AddMemberMethod( "RequestFocus", &LuaCppFunction_Widget_RequestFocus );
					pClass->AddMemberMethod( "SetNeedClip", &LuaCppFunction_Widget_SetNeedClip );
					pClass->AddDistructor( &Lua::Class_Unknown_Distructor_Release<CWidget> );
				}, LuaCppNames::s_ClassName_Object_FullName );

			scope->AddClass( LuaCppNames::s_ClassName_WidgetButton,
				[]( const char*, Lua::CClassBuilder* pClass )
				{
					pClass->AddConstructor( &Lua::Class_Unknown_Constructor_New<CButton> );
					pClass->AddDistructor( &Lua::Class_Unknown_Distructor_Release<CButton> );
					pClass->AddStaticMethod( "dynamic_cast", &Lua::Class_Unknown_Dynamic_Cast<CButton> );
					pClass->AddMemberMethod( "SetHotKey", &LuaCppFunction_WidgetButton_SetHotKey );
				}, LuaCppNames::s_ClassName_Widget_FullName );

			scope->AddClass( LuaCppNames::s_ClassName_WidgetCheckBox,
				[]( const char*, Lua::CClassBuilder* pClass )
				{
					pClass->AddConstructor( &Lua::Class_Unknown_Constructor_New<CCheckBox> );
					pClass->AddDistructor( &Lua::Class_Unknown_Distructor_Release<CCheckBox> );
					pClass->AddStaticMethod( "dynamic_cast", &Lua::Class_Unknown_Dynamic_Cast<CCheckBox> );
				}, LuaCppNames::s_ClassName_WidgetButton_FullName );

			scope->AddClass( LuaCppNames::s_ClassName_WidgetRadioBox,
				[]( const char*, Lua::CClassBuilder* pClass )
				{
					pClass->AddConstructor( &Lua::Class_Unknown_Constructor_New<CRadioBox> );
					pClass->AddDistructor( &Lua::Class_Unknown_Distructor_Release<CRadioBox> );
					pClass->AddStaticMethod( "dynamic_cast", &Lua::Class_Unknown_Dynamic_Cast<CRadioBox> );
					pClass->AddMemberMethod( "SetGroup", &LuaCppFunction_WidgetRadioBox_SetGroup );
					pClass->AddMemberMethod( "GetGroup", &LuaCppFunction_WidgetRadioBox_GetGroup );
				}, LuaCppNames::s_ClassName_WidgetCheckBox_FullName );

			scope->AddClass( LuaCppNames::s_ClassName_WidgetListBox,
				[]( const char*, Lua::CClassBuilder* pClass )
				{
					pClass->AddConstructor( &Lua::Class_Unknown_Constructor_New<CListBoxDictionary> );
					pClass->AddDistructor( &Lua::Class_Unknown_Distructor_Release<CListBoxDictionary> );
					pClass->AddStaticMethod( "dynamic_cast", &Lua::Class_Unknown_Dynamic_Cast<CListBoxDictionary> );
					pClass->AddMemberMethod( "SetDragStyle", &LuaCppFunction_WidgetListBox_SetDragStyle );
					pClass->AddMemberMethod( "SetSelectType", &LuaCppFunction_WidgetListBox_SetSelectType );
					pClass->AddMemberMethod( "SetItemSize", &LuaCppFunction_WidgetListBox_SetItemSize );
					pClass->AddMemberMethod( "SetItemMargin", &LuaCppFunction_WidgetListBox_SetItemMargin );
					pClass->AddMemberMethod( "SetCallback_ItemElementAllocate", &LuaCppFunction_WidgetListBox_SetCallback_ItemElementAllocate );
					pClass->AddMemberMethod( "SetCallback_ItemElementRelease", &LuaCppFunction_WidgetListBox_SetCallback_ItemElementRelease );
					pClass->AddMemberMethod( "SetCallback_ItemElementCreate", &LuaCppFunction_WidgetListBox_SetCallback_ItemElementCreate );
					pClass->AddMemberMethod( "SetCallback_ItemElementUpdate", &LuaCppFunction_WidgetListBox_SetCallback_ItemElementUpdate );
					pClass->AddMemberMethod( "AddItem", &LuaCppFunction_WidgetListBox_AddItem );
					pClass->AddMemberMethod( "UpdateItem", &LuaCppFunction_WidgetListBox_UpdateItem );
					pClass->AddMemberMethod( "UpdateItemFields", &LuaCppFunction_WidgetListBox_UpdateItemFields );
					pClass->AddMemberMethod( "RemoveItemOne", &LuaCppFunction_WidgetListBox_RemoveItemOne );
					pClass->AddMemberMethod( "RemoveItems", &LuaCppFunction_WidgetListBox_RemoveItems );
					pClass->AddMemberMethod( "RemoveItemsAll", &LuaCppFunction_WidgetListBox_RemoveItemsAll );
					pClass->AddMemberMethod( "GetItem", &LuaCppFunction_WidgetListBox_GetItem );
					pClass->AddMemberMethod( "FindItemOne", &LuaCppFunction_WidgetListBox_FindItemOne );
					pClass->AddMemberMethod( "FindItems", &LuaCppFunction_WidgetListBox_FindItems );
					pClass->AddMemberMethod( "IsSelect", &LuaCppFunction_WidgetListBox_IsSelect );
					pClass->AddMemberMethod( "GetSelectFirst", &LuaCppFunction_WidgetListBox_GetSelectFirst );
					pClass->AddMemberMethod( "GetSelectAll", &LuaCppFunction_WidgetListBox_GetSelectAll );
					pClass->AddMemberMethod( "GetItemContextMenu", &LuaCppFunction_WidgetListBox_GetItemContextMenu );
					pClass->AddMemberMethod( "GetItemContextMenuAtPoint", &LuaCppFunction_WidgetListBox_GetItemContextMenuAtPoint );
					pClass->AddMemberMethod( "GetItemBounding", &LuaCppFunction_WidgetListBox_GetItemBounding );
					pClass->AddMemberMethod( "SetItemEnable", &LuaCppFunction_WidgetListBox_SetItemEnable );
					pClass->AddMemberMethod( "IsItemEnable", &LuaCppFunction_WidgetListBox_IsItemEnable );
				}, LuaCppNames::s_ClassName_Widget_FullName );

			scope->AddClass( LuaCppNames::s_ClassName_WidgetEditBox,
				[]( const char*, Lua::CClassBuilder* pClass )
				{
					pClass->AddConstructor( &Lua::Class_Unknown_Constructor_New<CEditBox> );
					pClass->AddDistructor( &Lua::Class_Unknown_Distructor_Release<CEditBox> );
					pClass->AddStaticMethod( "dynamic_cast", &Lua::Class_Unknown_Dynamic_Cast<CEditBox> );
					pClass->AddMemberMethod( "SetValueText", &LuaCppFunction_WidgetEditBox_SetValueText );
					pClass->AddMemberMethod( "GetValueText", &LuaCppFunction_WidgetEditBox_GetValueText );
				}, LuaCppNames::s_ClassName_Widget_FullName );

			scope->AddClass( LuaCppNames::s_ClassName_WidgetProgressBar,
				[]( const char*, Lua::CClassBuilder* pClass )
				{
					pClass->AddConstructor( &Lua::Class_Unknown_Constructor_New<CProgressBar> );
					pClass->AddDistructor( &Lua::Class_Unknown_Distructor_Release<CProgressBar> );
					pClass->AddStaticMethod( "dynamic_cast", &Lua::Class_Unknown_Dynamic_Cast<CProgressBar> );
					pClass->AddMemberMethod( "SetProgressValueMax", &LuaCppFunction_WidgetProgressBar_SetProgressValueMax );
					pClass->AddMemberMethod( "SetProgressValue", &LuaCppFunction_WidgetProgressBar_SetProgressValue );
					pClass->AddMemberMethod( "GetProgressValueMax", &LuaCppFunction_WidgetProgressBar_GetProgressValueMax );
					pClass->AddMemberMethod( "GetProgressValue", &LuaCppFunction_WidgetProgressBar_GetProgressValue );
				}, LuaCppNames::s_ClassName_Widget_FullName );

			scope->AddClass( LuaCppNames::s_ClassName_Dialog,
				[]( const char*, Lua::CClassBuilder* pClass )
				{
					pClass->AddDistructor( &Lua::Class_Unknown_Distructor_Release<CDialog> );
					pClass->AddMemberMethod( "SetWidgetRoot", &LuaCppFunction_Dialog_SetWidgetRoot );
					pClass->AddMemberMethod( "GetWidgetRoot", &LuaCppFunction_Dialog_GetWidgetRoot );
					pClass->AddMemberMethod( "RequestTop", &LuaCppFunction_Dialog_RequestTop );
					pClass->AddMemberMethod( "SetModal", &LuaCppFunction_Dialog_SetModal );
					pClass->AddMemberMethod( "IsModal", &LuaCppFunction_Dialog_IsModal );
				}, LuaCppNames::s_ClassName_Object_FullName );

			scope->AddClass( LuaCppNames::s_ClassName_Element,
				[]( const char*, Lua::CClassBuilder* pClass )
				{
					pClass->AddConstructor( &Lua::Class_Unknown_Constructor_New<CRenderElement> );
					pClass->AddDistructor( &Lua::Class_Unknown_Distructor_Release<CRenderElement> );
					pClass->AddMemberMethod( "SetZOrder", &LuaCppFunction_Element_SetZOrder );
					pClass->AddMemberMethod( "Init", &LuaCppFunction_Element_Init );
					pClass->AddMemberMethod( "Clone", &LuaCppFunction_Element_Clone );
					pClass->AddMemberMethod( "FindElement", &LuaCppFunction_Element_FindElement );
					pClass->AddMemberMethod( "AddElement", &LuaCppFunction_Element_AddElement );
				}, LuaCppNames::s_ClassName_Object_FullName );

			scope->AddClass( LuaCppNames::s_ClassName_ElementColorRect,
				[]( const char*, Lua::CClassBuilder* pClass )
				{
					pClass->AddStaticMethod( "dynamic_cast", &Lua::Class_Unknown_Dynamic_Cast<CElementColorRect> );
					pClass->AddConstructor( &Lua::Class_Unknown_Constructor_New<CElementColorRect> );
					pClass->AddDistructor( &Lua::Class_Unknown_Distructor_Release<CElementColorRect> );
					pClass->AddMemberMethod( "SetColor", &LuaCppFunction_ElementColorRect_SetColor );
				}, LuaCppNames::s_ClassName_Element_FullName );

			scope->AddClass( LuaCppNames::s_ClassName_ElementStateLabel,
				[]( const char*, Lua::CClassBuilder* pClass )
				{
					pClass->AddStaticMethod( "dynamic_cast", &Lua::Class_Unknown_Dynamic_Cast<CElementStateLabel> );
					pClass->AddConstructor( &Lua::Class_Unknown_Constructor_New<CElementStateLabel> );
					pClass->AddDistructor( &Lua::Class_Unknown_Distructor_Release<CElementStateLabel> );
					pClass->AddMemberMethod( "SetFont", &LuaCppFunction_ElementStateLabel_SetFont );
					pClass->AddMemberMethod( "SetColor", &LuaCppFunction_ElementStateLabel_SetColor );
					pClass->AddMemberMethod( "SetText", &LuaCppFunction_ElementStateLabel_SetText );
					pClass->AddMemberMethod( "SetTextAlignHorizental", &LuaCppFunction_ElementStateLabel_SetTextAlignHorizental );
					pClass->AddMemberMethod( "SetTextAlignVertical", &LuaCppFunction_ElementStateLabel_SetTextAlignVertical );
				}, LuaCppNames::s_ClassName_Element_FullName );

			scope->AddClass( LuaCppNames::s_ClassName_ElementStateTexture,
				[]( const char*, Lua::CClassBuilder* pClass )
				{
					pClass->AddStaticMethod( "dynamic_cast", &Lua::Class_Unknown_Dynamic_Cast<CElementStateTexture> );
					pClass->AddConstructor( &Lua::Class_Unknown_Constructor_New<CElementStateTexture> );
					pClass->AddDistructor( &Lua::Class_Unknown_Distructor_Release<CElementStateTexture> );
					pClass->AddMemberMethod( "SetTexture", &LuaCppFunction_ElementStateTexture_SetTexture );
					pClass->AddMemberMethod( "SetColor", &LuaCppFunction_ElementStateTexture_SetColor );
					pClass->AddMemberMethod( "SetUV", &LuaCppFunction_ElementStateTexture_SetUV );
				}, LuaCppNames::s_ClassName_Element_FullName );

			scope->AddClass( LuaCppName_XmlParser,
				[]( const char*, Lua::CClassBuilder* pClass )
				{
					pClass->AddMemberMethod( "ReadFile", &LuaCppFunction_XmlParser_ReadFile );
					pClass->AddMemberMethod( "SetCallbackGetTexture", &LuaCppFunction_XmlParser_SetCallbackGetTexture );
					pClass->AddMemberMethod( "SetCallbackGetFont", &LuaCppFunction_XmlParser_SetCallbackGetFont );
				} );

			scope->AddEnum( "UEvents",
				[]( Lua::CEnumBuilder* pEnum )
				{
					pEnum->AddValueInt( "Clicked", (lua_Integer)BaseEvent::nClicked );
					pEnum->AddValueInt( "ValueChanged", (lua_Integer)BaseEvent::nValueChanged );
					pEnum->AddValueInt( "ContextMenu", (lua_Integer)BaseEvent::nContextMenu );
					pEnum->AddValueInt( "FocusOut", (lua_Integer)BaseEvent::nFocusOut );
					pEnum->AddValueInt( "FocusIn", (lua_Integer)BaseEvent::nFocusIn );
				} );

			scope->AddEnum( "UWidgetState",
				[]( Lua::CEnumBuilder* pEnum )
				{
					pEnum->AddValueInt( "Normal", (lua_Integer)UWidgetState::Normal );
					pEnum->AddValueInt( "MouseOver", (lua_Integer)UWidgetState::MouseOver );
					pEnum->AddValueInt( "Pressed", (lua_Integer)UWidgetState::Pressed );
					pEnum->AddValueInt( "Focus", (lua_Integer)UWidgetState::Focus );
					pEnum->AddValueInt( "Disabled", (lua_Integer)UWidgetState::Disabled );
					pEnum->AddValueInt( "Hidden", (lua_Integer)UWidgetState::Hidden );
				} );

			scope->AddEnum( "UDialogStyle",
				[]( Lua::CEnumBuilder* pEnum )
				{
					pEnum->AddValueInt( "Overlapped", (lua_Integer)UDialogStyle::Overlapped );
					pEnum->AddValueInt( "Child", (lua_Integer)UDialogStyle::Child );
				} );

			scope->AddEnum( "EListBoxDragStyle",
				[]( Lua::CEnumBuilder* pEnum )
				{
					pEnum->AddValueInt( "None", (lua_Integer)EListBoxDragStyle::None );
					pEnum->AddValueInt( "DragView", (lua_Integer)EListBoxDragStyle::DragView );
					pEnum->AddValueInt( "DragItem", (lua_Integer)EListBoxDragStyle::DragItem );
				} );

			scope->AddFunctionOne( "CreateDialog", &LuaCppFunction_Engine_CreateDialog );
			scope->AddFunctionOne( "GetDialog", &LuaCppFunction_Engine_GetDialog );
			scope->AddFunctionOne( "DestroyDialog", &LuaCppFunction_Engine_DestroyDialog );
			
		}, LuaCppNamespace );
}








void iberbar::Gui::ListBoxItemDataDictionary_ToLua( const CVariantDictionary& Dict, lua_State* pLuaState, int n )
{
	for ( auto& node : Dict.GetMap() )
	{
		lua_pushstring( pLuaState, node.first.c_str() );
		switch ( node.second.GetValueType() )
		{
			case UVariantType::VT_Boolean:
				lua_pushboolean( pLuaState, node.second.ToBoolean().value() );
				lua_rawset( pLuaState, n );
				break;

			case UVariantType::VT_String:
				lua_pushstring( pLuaState, node.second.ToString() );
				lua_rawset( pLuaState, n );
				break;

			case UVariantType::VT_Int:
				lua_pushinteger( pLuaState, node.second.ToInt().value() );
				lua_rawset( pLuaState, n );
				break;

			case UVariantType::VT_UInt:
				lua_pushinteger( pLuaState, node.second.ToUInt().value() );
				lua_rawset( pLuaState, n );
				break;

			case UVariantType::VT_Float:
				lua_pushnumber( pLuaState, node.second.ToFloat().value() );
				lua_rawset( pLuaState, n );
				break;

			default:
				lua_pop( pLuaState, 1 );
				break;
		}
	}
}


void iberbar::Gui::ListBoxItemDataDictionary_FromLua( CVariantDictionary& Dict, lua_State* pLuaState, int n )
{
	lua_pushnil( pLuaState );
	const char* strKey;
	size_t nKeyLen;
	int nLuaType;
	while ( lua_next( pLuaState, n ) )
	{

		// 现在的栈：-1 => value; -2 => key; index => table
		// 拷贝一份 key 到栈顶，然后对它做 lua_tostring 就不会改变原始的 key 值了
		lua_pushvalue( pLuaState, -2 );

		// 现在的栈：-1 => key; -2 => value; -3 => key; index => table
		strKey = lua_tolstring( pLuaState, -1, &nKeyLen );

		// 打包
		nLuaType = lua_type( pLuaState, -2 );
		switch ( nLuaType )
		{
			case LUA_TNUMBER:
			{
				if ( lua_isinteger( pLuaState, -2 ) == 1 )
				{
					Dict.SetInt( strKey, (int)lua_tointeger( pLuaState, -2 ) );
				}
				else
				{
					Dict.SetFloat( strKey, (float)lua_tonumber( pLuaState, -2 ) );
				}
				break;
			}

			case LUA_TBOOLEAN:
			{
				Dict.SetBoolean( strKey, (lua_toboolean( pLuaState, -2 ) == 0) ? false : true );
				break;
			}

			case LUA_TSTRING:
			{
				Dict.SetString( strKey, lua_tostring( pLuaState, -2 ) );
				break;
			}

			default:
				break;
		}

		// 弹出 value 和拷贝的 key，留下原始的 key 作为下一次 lua_next 的参数
		lua_pop( pLuaState, 2 );

		// 现在的栈：-1 => key; index => table
	}
}








int iberbar::Gui::LuaCppFunction_Object_SetId( lua_State* pLuaState )
{
	int top = lua_gettop( pLuaState );
	if ( top != 2 )
		return iberbar_LuaError_ArgumentsCountIsNotMatch( pLuaState, 2 );

	CObject* pObject = lua_self_tocppobject( pLuaState, CObject );
	const char* strId = lua_tostring( pLuaState, 2 );

	if ( pObject != nullptr && strId != nullptr )
	{
		pObject->SetId( strId );
	}

	return 0;
}


int iberbar::Gui::LuaCppFunction_Object_GetId( lua_State* pLuaState )
{
	int top = lua_gettop( pLuaState );
	if ( top != 1 )
		return iberbar_LuaError_ArgumentsCountIsNotMatch( pLuaState, 1 );

	CObject* obj = lua_self_tocppobject( pLuaState, CObject );

	if ( obj != nullptr )
	{
		const char* strId = obj->GetId().c_str();
		lua_pushstring( pLuaState, strId );
	}
	else
	{
		lua_pushnil( pLuaState );
	}

	return 1;
}


int iberbar::Gui::LuaCppFunction_Object_SetPosition( lua_State* pLuaState )
{
	int top = lua_gettop( pLuaState );
	if ( top != 3 )
		return iberbar_LuaError_ArgumentsCountIsNotMatch( pLuaState, 3 );

	CObject* pObject = lua_self_tocppobject( pLuaState, CObject );

	if ( pObject != nullptr )
	{
		int nx = (int)lua_tointeger( pLuaState, 2 );
		int ny = (int)lua_tointeger( pLuaState, 3 );
		pObject->SetPosition( CPoint2i( nx, ny ) );
	}

	return 0;
}


int iberbar::Gui::LuaCppFunction_Object_GetPosition( lua_State* pLuaState )
{
	int top = lua_gettop( pLuaState );
	if ( top != 1 )
		return iberbar_LuaError_ArgumentsCountIsNotMatch( pLuaState, 1 );

	CObject* pObject = lua_self_tocppobject( pLuaState, CObject );

	if ( pObject != nullptr )
	{
		const CPoint2i& ptPosition = pObject->GetPosition();
		lua_newtable( pLuaState );
		int nLuaTable = lua_gettop( pLuaState );
		lua_pushstring( pLuaState, "x" );
		lua_pushinteger( pLuaState, (lua_Integer)ptPosition.x );
		lua_settable( pLuaState, nLuaTable );
		lua_pushstring( pLuaState, "y" );
		lua_pushinteger( pLuaState, (lua_Integer)ptPosition.y );
		lua_settable( pLuaState, nLuaTable );
	}
	else
	{
		lua_pushnil( pLuaState );
	}

	return 1;
}


int iberbar::Gui::LuaCppFunction_Object_SetSize( lua_State* pLuaState )
{
	int top = lua_gettop( pLuaState );
	if ( top != 3 )
		return iberbar_LuaError_ArgumentsCountIsNotMatch( pLuaState, 3 );

	CObject* pObject = lua_self_tocppobject( pLuaState, CObject );

	if ( pObject != nullptr )
	{
		int nx = (int)lua_tointeger( pLuaState, 2 );
		int ny = (int)lua_tointeger( pLuaState, 3 );
		pObject->SetSize( CSize2i( nx, ny ) );
	}

	return 0;
}


int iberbar::Gui::LuaCppFunction_Object_GetSize( lua_State* pLuaState )
{
	int top = lua_gettop( pLuaState );
	if ( top != 1 )
		return iberbar_LuaError_ArgumentsCountIsNotMatch( pLuaState, 1 );

	CObject* pObject = lua_self_tocppobject( pLuaState, CObject );

	if ( pObject != nullptr )
	{
		const CSize2i& size = pObject->GetSize();
		lua_newtable( pLuaState );
		int nLuaTable = lua_gettop( pLuaState );
		lua_pushstring( pLuaState, "w" );
		lua_pushinteger( pLuaState, (lua_Integer)size.w );
		lua_settable( pLuaState, nLuaTable );
		lua_pushstring( pLuaState, "h" );
		lua_pushinteger( pLuaState, (lua_Integer)size.h );
		lua_settable( pLuaState, nLuaTable );
	}
	else
	{
		lua_pushnil( pLuaState );
	}

	return 1;
}


int iberbar::Gui::LuaCppFunction_Object_SetPaddings( lua_State* pLuaState )
{
	int nTop = lua_gettop( pLuaState );
	iberbar_LuaCheckArguments2( pLuaState, nTop, 2, 5 );

	CObject* pObject = lua_self_tocppobject( pLuaState, CObject );

	if ( pObject == nullptr )
		return 0;

	CRect2i Paddings;
	if ( nTop == 2 )
	{
		int nPaddingAll = (int)lua_tointeger( pLuaState, 2 );
		Paddings = CRect2i( nPaddingAll, nPaddingAll, nPaddingAll, nPaddingAll );
	}
	else if ( nTop == 5 )
	{
		int nPaddingLeft = (int)lua_tointeger( pLuaState, 2 );
		int nPaddingTop = (int)lua_tointeger( pLuaState, 3 );
		int nPaddingRight = (int)lua_tointeger( pLuaState, 4 );
		int nPaddingBottom = (int)lua_tointeger( pLuaState, 5 );
		Paddings = CRect2i( nPaddingLeft, nPaddingTop, nPaddingRight, nPaddingBottom );
	}

	pObject->SetPaddings( Paddings );

	return 0;
}


int iberbar::Gui::LuaCppFunction_Object_GetPaddings( lua_State* pLuaState )
{
	int top = lua_gettop( pLuaState );
	if ( top != 1 )
		return iberbar_LuaError_ArgumentsCountIsNotMatch( pLuaState, 1 );

	CObject* pObject = lua_self_tocppobject( pLuaState, CObject );

	if ( pObject != nullptr )
	{
		LuaApiUtil_PushRect2i( pLuaState, pObject->GetPaddings() );
	}
	else
	{
		lua_pushnil( pLuaState );
	}

	return 1;
}


int iberbar::Gui::LuaCppFunction_Object_SetVisible( lua_State* pLuaState )
{
	int top = lua_gettop( pLuaState );
	iberbar_LuaCheckArguments( pLuaState, top, 2 );

	CObject* pObject = lua_tocppobject( pLuaState, 1, CObject );

	if ( pObject )
	{
		pObject->SetVisible( lua_toboolean( pLuaState, 2 ) == 0 ? false : true );
	}

	return 0;
}


int iberbar::Gui::LuaCppFunction_Object_GetVisible( lua_State* pLuaState )
{
	int top = lua_gettop( pLuaState );
	iberbar_LuaCheckArguments( pLuaState, top, 1 );

	CObject* pObject = lua_tocppobject( pLuaState, 1, CObject );

	if ( pObject )
	{
		lua_pushboolean( pLuaState, pObject->GetVisible() );
	}
	else
	{
		lua_pushboolean( pLuaState, false );
	}

	return 1;
}


int iberbar::Gui::LuaCppFunction_Object_SetAlignHorizental( lua_State* pLuaState )
{
	int nTop = lua_gettop( pLuaState );
	iberbar_LuaCheckArguments( pLuaState, nTop, 2 );

	CObject* pObject = lua_tocppobject( pLuaState, 1, CObject );

	if ( pObject )
	{
		pObject->SetAlignHorizental( (UAlignHorizental)lua_tointeger( pLuaState, 2 ) );
	}

	return 0;
}


int iberbar::Gui::LuaCppFunction_Object_GetAlignHorizental( lua_State* pLuaState )
{
	int nTop = lua_gettop( pLuaState );
	iberbar_LuaCheckArguments( pLuaState, nTop, 1 );

	CObject* pObject = lua_tocppobject( pLuaState, 1, CObject );

	if ( pObject )
	{
		lua_pushinteger( pLuaState, (int)pObject->GetAlignHorizental() );
	}
	else
	{
		lua_pushinteger( pLuaState, 0 );
	}

	return 1;
}


int iberbar::Gui::LuaCppFunction_Object_SetAlignVertical( lua_State* pLuaState )
{
	int nTop = lua_gettop( pLuaState );
	iberbar_LuaCheckArguments( pLuaState, nTop, 2 );

	CObject* pObject = lua_tocppobject( pLuaState, 1, CObject );

	if ( pObject )
	{
		pObject->SetAlignVertical( (UAlignVertical)lua_tointeger( pLuaState, 2 ) );
	}

	return 0;
}


int iberbar::Gui::LuaCppFunction_Object_GetAlignVertical( lua_State* pLuaState )
{
	int nTop = lua_gettop( pLuaState );
	iberbar_LuaCheckArguments( pLuaState, nTop, 1 );

	CObject* pObject = lua_tocppobject( pLuaState, 1, CObject );

	if ( pObject )
	{
		lua_pushinteger( pLuaState, (int)pObject->GetAlignVertical() );
	}
	else
	{
		lua_pushinteger( pLuaState, 0 );
	}

	return 1;
}


int iberbar::Gui::LuaCppFunction_Object_SetPercentX( lua_State* pLuaState )
{
	int nTop = lua_gettop( pLuaState );
	iberbar_LuaCheckArguments( pLuaState, nTop, 2 );

	CObject* pObject = lua_tocppobject( pLuaState, 1, CObject );

	if ( pObject )
	{
		pObject->SetPercentX( lua_toboolean( pLuaState, 2 ) == 0 ? false : true );
	}

	return 0;
}


int iberbar::Gui::LuaCppFunction_Object_SetPercentY( lua_State* pLuaState )
{
	int nTop = lua_gettop( pLuaState );
	iberbar_LuaCheckArguments( pLuaState, nTop, 2 );

	CObject* pObject = lua_tocppobject( pLuaState, 1, CObject );

	if ( pObject )
	{
		pObject->SetPercentY( lua_toboolean( pLuaState, 2 ) == 0 ? false : true );
	}

	return 0;
}

int iberbar::Gui::LuaCppFunction_Object_SetPercentW( lua_State* pLuaState )
{
	int nTop = lua_gettop( pLuaState );
	iberbar_LuaCheckArguments( pLuaState, nTop, 2 );

	CObject* pObject = lua_tocppobject( pLuaState, 1, CObject );

	if ( pObject )
	{
		pObject->SetPercentW( lua_toboolean( pLuaState, 2 ) == 0 ? false : true );
	}

	return 0;
}

int iberbar::Gui::LuaCppFunction_Object_SetPercentH( lua_State* pLuaState )
{
	int nTop = lua_gettop( pLuaState );
	iberbar_LuaCheckArguments( pLuaState, nTop, 2 );

	CObject* pObject = lua_tocppobject( pLuaState, 1, CObject );

	if ( pObject )
	{
		pObject->SetPercentH( lua_toboolean( pLuaState, 2 ) == 0 ? false : true );
	}

	return 0;
}


int iberbar::Gui::LuaCppFunction_Object_Refresh( lua_State* pLuaState )
{
	int top = lua_gettop( pLuaState );
	iberbar_LuaCheckArguments( pLuaState, top, 1 );

	CObject* pObject = lua_tocppobject( pLuaState, 1, CObject );
	if ( pObject == nullptr )
		return 0;

	pObject->Refresh();

	return 0;
}


int iberbar::Gui::LuaCppFunction_Object_SetDebugId( lua_State* pLuaState )
{
#ifdef _DEBUG
	int top = lua_gettop( pLuaState );
	iberbar_LuaCheckArguments( pLuaState, top, 2 );

	CObject* pObject = lua_tocppobject( pLuaState, 1, CObject );
	if ( pObject == nullptr )
		return 0;

	int nDebugId = lua_tointeger( pLuaState, 2 );
	pObject->SetDebugId( nDebugId );
#endif

	return 0;
}


int iberbar::Gui::LuaCppFunction_Object_GetBounding( lua_State* pLuaState )
{
	int nTop = lua_gettop( pLuaState );
	iberbar_LuaCheckArguments( pLuaState, nTop, 1 );

	CObject* pObject = lua_tocppobject( pLuaState, 1, CObject );
	if ( pObject == nullptr )
	{
		lua_pushnil( pLuaState );
		return 1;
	}


	const CRect2i& RectBounding = pObject->GetBounding();

	LuaApiUtil_PushRect2i( pLuaState, RectBounding );

	return 1;
}








int iberbar::Gui::LuaCppFunction_Dialog_SetWidgetRoot( lua_State* pLuaState )
{
	int top = lua_gettop( pLuaState );
	iberbar_LuaCheckArguments( pLuaState, top, 2 );

	CDialog* pDlg = lua_tocppobject( pLuaState, 1, CDialog );
	CWidget* pWidget = lua_tocppobject( pLuaState, 2, CWidget );

	if ( pDlg == nullptr || pWidget == nullptr )
		return 0;

	pDlg->SetWidgetRoot( pWidget );

	return 0;
}


int iberbar::Gui::LuaCppFunction_Dialog_GetWidgetRoot( lua_State* pLuaState )
{
	int top = lua_gettop( pLuaState );
	iberbar_LuaCheckArguments( pLuaState, top, 1 );

	CDialog* pDialog = lua_tocppobject( pLuaState, 1, CDialog );

	if ( pDialog != nullptr )
	{
		CWidget* pWidgetFind = pDialog->GetWidgetRoot();

		if ( pWidgetFind == nullptr )
		{
			lua_pushnil( pLuaState );
		}
		else
		{
			lua_pushcppref( pLuaState, LuaCppNames::s_ClassName_Widget_FullName, pWidgetFind );
		}
	}
	else
	{
		lua_pushnil( pLuaState );
	}

	return 1;
}


int iberbar::Gui::LuaCppFunction_Dialog_RequestTop( lua_State* pLuaState )
{
	int top = lua_gettop( pLuaState );
	iberbar_LuaCheckArguments( pLuaState, top, 1 );

	CDialog* pDlg = lua_tocppobject( pLuaState, 1, CDialog );
	if ( pDlg == nullptr )
		return 0;

	pDlg->RequestTop();

	return 0;
}


int iberbar::Gui::LuaCppFunction_Dialog_SetModal( lua_State* pLuaState )
{
	int nTop = lua_gettop( pLuaState );
	iberbar_LuaCheckArguments( pLuaState, nTop, 2 );

	CDialog* pDlg = lua_tocppobject( pLuaState, 1, CDialog );
	if ( pDlg == nullptr )
		return 0;

	pDlg->SetModal( lua_toboolean( pLuaState, 2 ) == 1 ? true : false );

	return 0;
}


int iberbar::Gui::LuaCppFunction_Dialog_IsModal( lua_State* pLuaState )
{
	int nTop = lua_gettop( pLuaState );
	iberbar_LuaCheckArguments( pLuaState, nTop, 2 );

	CDialog* pDlg = lua_tocppobject( pLuaState, 1, CDialog );
	if ( pDlg == nullptr )
	{
		lua_pushboolean( pLuaState, 0 );
	}
	else
	{
		lua_pushboolean( pLuaState, pDlg->IsModal() == true ? 1 : 0 );
	}

	return 1;
}










int iberbar::Gui::LuaCppFunction_Widget_AddWidget( lua_State* pLuaState )
{
	int top = lua_gettop( pLuaState );
	iberbar_LuaCheckArguments( pLuaState, top, 2 );

	CWidget* pWidget = lua_tocppobject( pLuaState, 1, CWidget );
	CWidget* pWidgetChild = lua_tocppobject( pLuaState, 2, CWidget );

	if ( pWidget == nullptr || pWidgetChild == nullptr )
		return 0;

	pWidget->AddWidget( pWidgetChild );

	return 0;
}


int iberbar::Gui::LuaCppFunction_Widget_FindWidget( lua_State* pLuaState )
{
	int top = lua_gettop( pLuaState );
	iberbar_LuaCheckArguments( pLuaState, top, 2 );

	CWidget* pWidget = lua_tocppobject( pLuaState, 1, CWidget );
	const char* strId = lua_tostring( pLuaState, 2 );
	if ( pWidget != nullptr && StringIsNullOrEmpty( strId ) == false )
	{
		CWidget* pWidgetFind = pWidget->FindWidget( strId );

		if ( pWidgetFind == nullptr )
		{
			lua_pushnil( pLuaState );
		}
		else
		{
			lua_pushcppref( pLuaState, LuaCppNames::s_ClassName_Widget_FullName, pWidgetFind );
		}
	}
	else
	{
		lua_pushnil( pLuaState );
	}

	return 1;
}


int iberbar::Gui::LuaCppFunction_Widget_FindElement( lua_State* pLuaState )
{
	int top = lua_gettop( pLuaState );
	if ( top != 2 )
		return iberbar_LuaError_ArgumentsCountIsNotMatch( pLuaState, 2 );

	CWidget* pWidget = lua_self_tocppobject( pLuaState, CWidget );
	const char* strId = lua_tostring( pLuaState, 2 );

	if ( pWidget != nullptr && StringIsNullOrEmpty( strId ) == false )
	{
		PTR_CRenderElement pElement = nullptr;
		if ( pWidget->FindElement( strId, &pElement ) == false )
		{
			lua_pushnil( pLuaState );
		}
		else
		{
			lua_pushcppref( pLuaState, LuaCppNames::s_ClassName_Element_FullName, pElement );
		}
	}
	else
	{
		lua_pushnil( pLuaState );
	}

	return 1;
}


int iberbar::Gui::LuaCppFunction_Widget_AddEventCallback( lua_State* pLuaState )
{
	int nTop = lua_gettop( pLuaState );
	iberbar_LuaCheckArguments3( pLuaState, nTop, 3, 4, 5 );

	CWidget* pWidget = lua_tocppobject( pLuaState, 1, CWidget );
	if ( pWidget == nullptr )
	{
		lua_pushinteger( pLuaState, 0 );
		return 1;
	}

	uint64 nEvent = (uint64)lua_tointeger( pLuaState, 2 );
	lua_Integer nCallback = lua_toluacallback( pLuaState, 3 );
	lua_Integer nExt = 0;
	bool bDefer = true;
	if ( nTop == 4 )
	{
		if ( lua_isboolean( pLuaState, 4 ) )
			bDefer = lua_toboolean( pLuaState, 4 ) == 0 ? false : true;
		else
			nExt = lua_toluacallback_extparam( pLuaState, 4 );
	}
	else if ( nTop == 5 )
	{
		nExt = lua_toluacallback_extparam( pLuaState, 4 );
		bDefer = lua_toboolean( pLuaState, 5 ) == 0 ? false : true;
	}
		

	if ( nCallback == 0 )
	{
		lua_pushinteger( pLuaState, 0 );
		return 1;
	}

	Lua::PTR_CLuaCallbackHandler pCallbackHandler = Lua::PTR_CLuaCallbackHandler::_sNew( pLuaState, nCallback, nExt );
	int nHandle = pWidget->AddEventCallback(
		[pCallbackHandler]( CWidget* pWidget, uint64 nEvent, uint64 nValueUint, const void* pValueExt )
		{
			LuaCppCallbackExecute_WidetEvent( pCallbackHandler, pWidget, nEvent, nValueUint );
		},
		nEvent, bDefer
	);

	lua_pushinteger( pLuaState, nHandle );
	return 1;
}


int iberbar::Gui::LuaCppFunction_Widget_RemoveEventCallback( lua_State* pLuaState )
{
	int nTop = lua_gettop( pLuaState );
	iberbar_LuaCheckArguments( pLuaState, nTop, 2 );

	CWidget* pWidget = lua_tocppobject( pLuaState, 1, CWidget );
	if ( pWidget == nullptr )
		return 0;

	int nHandle = (int)lua_tointeger( pLuaState, 2 );
	pWidget->RemoveEventCallback( nHandle );

	return 0;
}


int iberbar::Gui::LuaCppFunction_Widget_RemoveEventCallbacksAll( lua_State* pLuaState )
{
	int nTop = lua_gettop( pLuaState );
	iberbar_LuaCheckArguments( pLuaState, nTop, 1 );

	CWidget* pWidget = lua_tocppobject( pLuaState, 1, CWidget );
	if ( pWidget == nullptr )
		return 0;

	pWidget->RemoveEventCallbacksAll();

	return 0;
}


int iberbar::Gui::LuaCppFunction_Widget_SetRenderElement( lua_State* pLuaState )
{
	int top = lua_gettop( pLuaState );
	iberbar_LuaCheckArguments( pLuaState, top, 2 );

	CWidget* pWidget = lua_self_tocppobject( pLuaState, CWidget );
	if ( pWidget == nullptr )
		return 0;

	CRenderElement* pElement = lua_tocppobject( pLuaState, 2, CRenderElement );
	
	pWidget->SetRenderElement( pElement );

	return 0;
}


int iberbar::Gui::LuaCppFunction_Widget_RequestFocus( lua_State* pLuaState )
{
	int top = lua_gettop( pLuaState );
	iberbar_LuaCheckArguments( pLuaState, top, 1 );

	CWidget* pWidget = lua_self_tocppobject( pLuaState, CWidget );
	if ( pWidget == nullptr )
		return 0;

	pWidget->RequestFocus();

	return 0;
}


int iberbar::Gui::LuaCppFunction_Widget_SetNeedClip( lua_State* pLuaState )
{
	int top = lua_gettop( pLuaState );
	iberbar_LuaCheckArguments( pLuaState, top, 2 );

	CWidget* pWidget = lua_tocppobject( pLuaState, 1, CWidget );
	if ( pWidget == nullptr )
		return 0;

	pWidget->SetNeedClip( lua_toboolean( pLuaState, 2 ) == 0 ? false : true );

	return 0;
}


int iberbar::Gui::LuaCppFunction_Widget_Clone( lua_State* pLuaState )
{
	int top = lua_gettop( pLuaState );
	iberbar_LuaCheckArguments( pLuaState, top, 1 );

	CWidget* pWidget = lua_tocppobject( pLuaState, 1, CWidget );
	if ( pWidget == nullptr )
	{
		lua_pushnil( pLuaState );
		return 1;
	}

	CWidget* pWidgetClone = CWidget::sCloneWidgetEx( pWidget, false );
	if ( pWidgetClone == nullptr )
	{
		lua_pushnil( pLuaState );
	}
	else
	{
		lua_pushcppref( pLuaState, LuaCppNames::s_ClassName_Widget_FullName, pWidgetClone );
		pWidgetClone->Release();
	}

	return 1;
}








int iberbar::Gui::LuaCppFunction_WidgetButton_SetHotKey( lua_State* pLuaState )
{
	int top = lua_gettop( pLuaState );
	iberbar_LuaCheckArguments( pLuaState, top, 2 );

	CButton* pWidget = lua_tocppobject( pLuaState, 1, CButton );
	if ( pWidget != nullptr )
		return 0;

	pWidget->SetHotKey( (int)lua_tointeger( pLuaState, 2 ) );

	return 0;
}







int iberbar::Gui::LuaCppFunction_WidgetRadioBox_SetGroup( lua_State* pLuaState )
{
	int top = lua_gettop( pLuaState );
	iberbar_LuaCheckArguments( pLuaState, top, 2 );

	CRadioBox* pWidget = lua_tocppobject( pLuaState, 1, CRadioBox );
	if ( pWidget != nullptr )
		return 0;

	pWidget->SetGroup( (int)lua_tointeger( pLuaState, 2 ) );

	return 0;
}


int iberbar::Gui::LuaCppFunction_WidgetRadioBox_GetGroup( lua_State* pLuaState )
{
	int top = lua_gettop( pLuaState );
	iberbar_LuaCheckArguments( pLuaState, top, 2 );

	CRadioBox* pWidget = lua_tocppobject( pLuaState, 1, CRadioBox );
	if ( pWidget == nullptr )
	{
		lua_pushinteger( pLuaState, 0 );
		return 1;
	}

	lua_pushinteger( pLuaState, pWidget->GetGroup() );

	return 1;
}










int iberbar::Gui::LuaCppFunction_WidgetListBox_SetDragStyle( lua_State* pLuaState )
{
	int top = lua_gettop( pLuaState );
	iberbar_LuaCheckArguments( pLuaState, top, 2 );

	CListBoxDictionary* pListBox = lua_tocppobject( pLuaState, 1, CListBoxDictionary );
	if ( pListBox == nullptr )
		return 0;

	pListBox->SetDragStyle( (EListBoxDragStyle)lua_tointeger( pLuaState, 2 ) );

	return 0;
}


int iberbar::Gui::LuaCppFunction_WidgetListBox_SetSelectType( lua_State* pLuaState )
{
	int top = lua_gettop( pLuaState );
	iberbar_LuaCheckArguments( pLuaState, top, 2 );

	CListBoxDictionary* pListBox = lua_tocppobject( pLuaState, 1, CListBoxDictionary );
	if ( pListBox == nullptr )
		return 0;

	UListBoxSelectType nSelectType = (UListBoxSelectType)lua_tointeger( pLuaState, 2 );
	pListBox->SetSelectType( nSelectType );

	return 0;
}


int iberbar::Gui::LuaCppFunction_WidgetListBox_SetItemSize( lua_State* pLuaState )
{
	int top = lua_gettop( pLuaState );
	iberbar_LuaCheckArguments( pLuaState, top, 3 );

	CListBoxDictionary* pListBox = lua_tocppobject( pLuaState, 1, CListBoxDictionary );
	if ( pListBox == nullptr )
		return 0;

	int nW = lua_tointeger( pLuaState, 2 );
	int nH = lua_tointeger( pLuaState, 3 );
	pListBox->SetItemSize( nW, nH );

	return 0;
}


int iberbar::Gui::LuaCppFunction_WidgetListBox_SetItemMargin( lua_State* pLuaState )
{
	int top = lua_gettop( pLuaState );
	iberbar_LuaCheckArguments( pLuaState, top, 2 );

	CListBoxDictionary* pListBox = lua_tocppobject( pLuaState, 1, CListBoxDictionary );
	if ( pListBox == nullptr )
		return 0;

	int nMargin = lua_tointeger( pLuaState, 2 );
	pListBox->SetItemMargin( nMargin );

	return 0;
}


int iberbar::Gui::LuaCppFunction_WidgetListBox_SetCallback_ItemElementAllocate( lua_State* pLuaState )
{
	int top = lua_gettop( pLuaState );
	iberbar_LuaCheckArguments2( pLuaState, top, 2, 3 );

	CListBoxDictionary* pListBox = lua_tocppobject( pLuaState, 1, CListBoxDictionary );
	if ( pListBox == nullptr )
		return 0;

	lua_Integer nFunc = lua_toluacallback( pLuaState, 2 );
	lua_Integer nExt = 0;
	if ( top == 3 )
		nExt = lua_toluacallback_extparam( pLuaState, 3 );
	if ( nFunc == 0 )
		return 0;

	Lua::PTR_CLuaCallbackHandler pLuaCallback = Lua::PTR_CLuaCallbackHandler::_sNew( pLuaState, nFunc, nExt );
	pListBox->SetCallback_ItemElementAllocate(
		[pLuaCallback]()
		{
			return LuaCppCallbackExecute_WidgetListBox_ItemElementAllocate( pLuaCallback );
		}
	);

	return 0;
}


int iberbar::Gui::LuaCppFunction_WidgetListBox_SetCallback_ItemElementRelease( lua_State* pLuaState )
{
	int top = lua_gettop( pLuaState );
	iberbar_LuaCheckArguments2( pLuaState, top, 2, 3 );

	CListBoxDictionary* pListBox = lua_tocppobject( pLuaState, 1, CListBoxDictionary );
	if ( pListBox == nullptr )
		return 0;

	lua_Integer nFunc = lua_toluacallback( pLuaState, 2 );
	lua_Integer nExt = 0;
	if ( top == 3 )
		nExt = lua_toluacallback_extparam( pLuaState, 3 );
	if ( nFunc == 0 )
		return 0;

	Lua::PTR_CLuaCallbackHandler pLuaCallback = Lua::PTR_CLuaCallbackHandler::_sNew( pLuaState, nFunc, nExt );
	pListBox->SetCallback_ItemElementRelease(
		[pLuaCallback]( CRenderElement* pElement )
		{
			LuaCppCallbackExecute_WidgetListBox_ItemElementRelease( pLuaCallback, pElement );
		}
	);

	return 0;
}


int iberbar::Gui::LuaCppFunction_WidgetListBox_SetCallback_ItemElementCreate( lua_State* pLuaState )
{
	int top = lua_gettop( pLuaState );
	iberbar_LuaCheckArguments2( pLuaState, top, 2, 3 );

	CListBoxDictionary* pListBox = lua_tocppobject( pLuaState, 1, CListBoxDictionary );
	if ( pListBox == nullptr )
		return 0;

	lua_Integer nFunc = lua_toluacallback( pLuaState, 2 );
	lua_Integer nExt = 0;
	if ( top == 3 )
		nExt = lua_toluacallback_extparam( pLuaState, 3 );
	if ( nFunc == 0 )
		return 0;

	Lua::PTR_CLuaCallbackHandler pLuaCallback = Lua::PTR_CLuaCallbackHandler::_sNew( pLuaState, nFunc, nExt );
	pListBox->SetCallback_ItemElementCreate(
		[pLuaCallback]( CRenderElement* pElement, const CVariantDictionary* pData, int nIndex )
		{
			LuaCppCallbackExecute_WidgetListBox_ItemElementCreate( pLuaCallback, pElement, pData, nIndex );
		}
	);

	return 0;
}


int iberbar::Gui::LuaCppFunction_WidgetListBox_SetCallback_ItemElementUpdate( lua_State* pLuaState )
{
	int top = lua_gettop( pLuaState );
	iberbar_LuaCheckArguments2( pLuaState, top, 2, 3 );

	CListBoxDictionary* pListBox = lua_tocppobject( pLuaState, 1, CListBoxDictionary );
	if ( pListBox == nullptr )
		return 0;

	lua_Integer nFunc = lua_toluacallback( pLuaState, 2 );
	lua_Integer nExt = 0;
	if ( top == 3 )
		nExt = lua_toluacallback_extparam( pLuaState, 3 );
	if ( nFunc == 0 )
		return 0;

	Lua::PTR_CLuaCallbackHandler pLuaCallback = Lua::PTR_CLuaCallbackHandler::_sNew( pLuaState, nFunc, nExt );
	pListBox->SetCallback_ItemElementUpdate(
		[pLuaCallback]( CRenderElement* pElement, const CVariantDictionary* pData, int nIndex, int nUpdateEvent )
		{
			LuaCppCallbackExecute_WidgetListBox_ItemElementUpdate( pLuaCallback, pElement, pData, nIndex, nUpdateEvent );
		}
	);

	return 0;
}


int iberbar::Gui::LuaCppFunction_WidgetListBox_AddItem( lua_State* pLuaState )
{
	int top = lua_gettop( pLuaState );
	iberbar_LuaCheckArguments( pLuaState, top, 2 );

	CListBoxDictionary* pListBox = lua_tocppobject( pLuaState, 1, CListBoxDictionary );
	if ( pListBox == nullptr )
	{
		lua_pushinteger( pLuaState, -1 );
		return 1;
	}

	if ( lua_istable( pLuaState, 2 ) == false )
	{
		lua_pushinteger( pLuaState, -1 );
		return 1;
	}

	CVariantDictionary Data;
	ListBoxItemDataDictionary_FromLua( Data, pLuaState, 2 );

	int nIndex = pListBox->AddItem( Data );

	lua_pushinteger( pLuaState, nIndex );
	return 1;
}


int iberbar::Gui::LuaCppFunction_WidgetListBox_UpdateItem( lua_State* pLuaState )
{
	int nTop = lua_gettop( pLuaState );
	iberbar_LuaCheckArguments( pLuaState, nTop, 3 );

	CListBoxDictionary* pListBox = lua_tocppobject( pLuaState, 1, CListBoxDictionary );
	if ( pListBox == nullptr )
	{
		return 0;
	}

	if ( lua_istable( pLuaState, 3 ) == false )
	{
		return 0;
	}

	int nIndex = lua_tointeger( pLuaState, 2 );
	CVariantDictionary Data;
	ListBoxItemDataDictionary_FromLua( Data, pLuaState, 3 );

	pListBox->UpdateItem( nIndex, Data );

	return 0;
}


int iberbar::Gui::LuaCppFunction_WidgetListBox_UpdateItemFields( lua_State* pLuaState )
{
	int nTop = lua_gettop( pLuaState );
	iberbar_LuaCheckArguments( pLuaState, nTop, 4 );

	CListBoxDictionary* pListBox = lua_tocppobject( pLuaState, 1, CListBoxDictionary );
	if ( pListBox == nullptr )
	{
		return 0;
	}

	if ( lua_istable( pLuaState, 3 ) == false )
	{
		return 0;
	}

	int nIndex = lua_tointeger( pLuaState, 2 );
	CVariantDictionary Data;
	ListBoxItemDataDictionary_FromLua( Data, pLuaState, 3 );
	int nUpdateEvent = lua_tointeger( pLuaState, 4 );

	pListBox->UpdateItem( nIndex, [&Data]( CVariantDictionary* pItem )
		{
			pItem->Merge( Data );
		}, nUpdateEvent );

	return 0;
}


int iberbar::Gui::LuaCppFunction_WidgetListBox_RemoveItemOne( lua_State* pLuaState )
{
	int top = lua_gettop( pLuaState );
	iberbar_LuaCheckArguments( pLuaState, top, 2 );

	CListBoxDictionary* pListBox = lua_tocppobject( pLuaState, 1, CListBoxDictionary );
	if ( pListBox == nullptr )
	{
		lua_pushinteger( pLuaState, 0 );
		return 1;
	}

	int nIndex = lua_tointeger( pLuaState, 2 );
	int nRet = pListBox->RemoveItemOne( nIndex );

	lua_pushinteger( pLuaState, nRet );
	return 1;
}


int iberbar::Gui::LuaCppFunction_WidgetListBox_RemoveItems( lua_State* pLuaState )
{
	int top = lua_gettop( pLuaState );
	iberbar_LuaCheckArguments( pLuaState, top, 2 );

	CListBoxDictionary* pListBox = lua_tocppobject( pLuaState, 1, CListBoxDictionary );
	if ( pListBox == nullptr )
	{
		lua_pushinteger( pLuaState, 0 );
		return 1;
	}

	if ( lua_istable( pLuaState, 2 ) == false )
	{
		lua_pushinteger( pLuaState, 0 );
		return 1;
	}

	lua_len( pLuaState, 2 );
	int nLen = (int)lua_tointeger( pLuaState, -1 );
	lua_pop( pLuaState, 1 );

	std::vector<int> Indices;
	for ( int i = 1; i <= nLen; i++ )
	{
		lua_rawgeti( pLuaState, 2, i );
		Indices.push_back( (int)lua_tointeger( pLuaState, -1 ) );
		lua_pop( pLuaState, 1 );
	}

	int nRet = pListBox->RemoveItems( Indices.data(), Indices.size() );

	lua_pushinteger( pLuaState, nRet );
	return 1;
}


int iberbar::Gui::LuaCppFunction_WidgetListBox_RemoveItemsAll( lua_State* pLuaState )
{
	int top = lua_gettop( pLuaState );
	iberbar_LuaCheckArguments( pLuaState, top, 1 );

	CListBoxDictionary* pListBox = lua_tocppobject( pLuaState, 1, CListBoxDictionary );
	if ( pListBox == nullptr )
	{
		return 0;
	}

	pListBox->RemoveItemsAll();

	return 0;
}


int iberbar::Gui::LuaCppFunction_WidgetListBox_GetItem( lua_State* pLuaState )
{
	int top = lua_gettop( pLuaState );
	iberbar_LuaCheckArguments( pLuaState, top, 2 );

	CListBoxDictionary* pListBox = lua_tocppobject( pLuaState, 1, CListBoxDictionary );
	if ( pListBox == nullptr )
	{
		lua_pushnil( pLuaState );
		return 1;
	}

	int nIndex = lua_tointeger( pLuaState, 2 );

	const CVariantDictionary* pData = pListBox->GetItem( nIndex );
	if ( pData == nullptr )
	{
		lua_pushnil( pLuaState );
		return 1;
	}

	lua_createtable( pLuaState, 0, 0 );
	ListBoxItemDataDictionary_ToLua( *pData, pLuaState, lua_gettop( pLuaState ) );

	return 1;
}


int iberbar::Gui::LuaCppFunction_WidgetListBox_FindItemOne( lua_State* pLuaState )
{
	int top = lua_gettop( pLuaState );
	iberbar_LuaCheckArguments( pLuaState, top, 2 );

	CListBoxDictionary* pListBox = lua_tocppobject( pLuaState, 1, CListBoxDictionary );
	if ( pListBox == nullptr )
	{
		lua_pushinteger( pLuaState, -1 );
		return 1;
	}

	lua_Integer nFunc = lua_toluacallback( pLuaState, 2 );
	lua_Integer nExt = 0;
	if ( top == 3 )
		nExt = lua_toluacallback_extparam( pLuaState, 3 );
	if ( nFunc == 0 )
	{
		lua_pushinteger( pLuaState, -1 );
		return 1;
	}

	Lua::PTR_CLuaCallbackHandler pLuaCallback = Lua::PTR_CLuaCallbackHandler::_sNew( pLuaState, nFunc, nExt );
	int nFind = pListBox->FindItem(
		[pLuaCallback]( const CVariantDictionary* pData )
		{
			return LuaCppCallbackExecute_WidgetListBox_FindItem( pLuaCallback, pData );
		}
	);

	lua_pushinteger( pLuaState, nFind );
	return 1;
}


int iberbar::Gui::LuaCppFunction_WidgetListBox_FindItems( lua_State* pLuaState )
{
	int top = lua_gettop( pLuaState );
	iberbar_LuaCheckArguments( pLuaState, top, 2 );

	CListBoxDictionary* pListBox = lua_tocppobject( pLuaState, 1, CListBoxDictionary );
	if ( pListBox == nullptr )
	{
		lua_pushinteger( pLuaState, -1 );
		return 1;
	}

	lua_Integer nFunc = lua_toluacallback( pLuaState, 2 );
	lua_Integer nExt = 0;
	if ( top == 3 )
		nExt = lua_toluacallback_extparam( pLuaState, 3 );
	if ( nFunc == 0 )
	{
		lua_pushinteger( pLuaState, -1 );
		return 1;
	}

	Lua::PTR_CLuaCallbackHandler pLuaCallback = Lua::PTR_CLuaCallbackHandler::_sNew( pLuaState, nFunc, nExt );
	auto callbackCompare = [pLuaCallback]( const CVariantDictionary* pData )
	{
		return LuaCppCallbackExecute_WidgetListBox_FindItem( pLuaCallback, pData );
	};
	std::vector<int> FindIdxs;
	int nOffset = 0;
	while ( ( nOffset = pListBox->FindItem( callbackCompare, nOffset ) ) >= 0 )
	{
		FindIdxs.push_back( nOffset );
		nOffset++;
	}

	if ( FindIdxs.empty() == true )
	{
		lua_pushnil( pLuaState );
	}
	else
	{
		lua_createtable( pLuaState, FindIdxs.size(), 0 );

		for ( size_t i = 0, s = FindIdxs.size(); i < s; i++ )
		{
			lua_pushinteger( pLuaState, (lua_Integer)FindIdxs[ i ] );
			lua_rawseti( pLuaState, -2, (lua_Integer)i + 1 );
		}
	}
	
	return 1;
}


int iberbar::Gui::LuaCppFunction_WidgetListBox_IsSelect( lua_State* pLuaState )
{
	int top = lua_gettop( pLuaState );
	iberbar_LuaCheckArguments( pLuaState, top, 2 );

	CListBoxDictionary* pListBox = lua_tocppobject( pLuaState, 1, CListBoxDictionary );
	if ( pListBox == nullptr )
	{
		lua_pushboolean( pLuaState, 0 );
		return 1;
	}

	int nIndex = (int)lua_tointeger( pLuaState, 2 );
	bool bIsSelect = pListBox->IsSelect( nIndex );

	lua_pushboolean( pLuaState, bIsSelect );
	return 1;
}


int iberbar::Gui::LuaCppFunction_WidgetListBox_GetSelectFirst( lua_State* pLuaState )
{
	int top = lua_gettop( pLuaState );
	iberbar_LuaCheckArguments( pLuaState, top, 1 );

	CListBoxDictionary* pListBox = lua_tocppobject( pLuaState, 1, CListBoxDictionary );
	if ( pListBox == nullptr )
	{
		lua_pushinteger( pLuaState, -1 );
		return 1;
	}

	int nSel = pListBox->GetSelect();

	lua_pushinteger( pLuaState, nSel );
	return 1;
}


int iberbar::Gui::LuaCppFunction_WidgetListBox_GetSelectAll( lua_State* pLuaState )
{
	int top = lua_gettop( pLuaState );
	iberbar_LuaCheckArguments( pLuaState, top, 1 );

	CListBoxDictionary* pListBox = lua_tocppobject( pLuaState, 1, CListBoxDictionary );
	if ( pListBox == nullptr )
	{
		lua_pushnil( pLuaState );
		return 1;
	}

	lua_createtable( pLuaState, 0, 0 );
	int nOffset = 0;
	int nSel = -1;
	int nCount = 0;
	while ( ( nSel = pListBox->GetSelect( nOffset ) ) >= 0 )
	{
		nCount++;
		lua_pushinteger( pLuaState, nSel );
		lua_rawseti( pLuaState, -2, nCount );
		nOffset = nSel + 1;
	}

	return 1;
}


int iberbar::Gui::LuaCppFunction_WidgetListBox_GetItemContextMenu( lua_State* pLuaState )
{
	int nTop = lua_gettop( pLuaState );
	iberbar_LuaCheckArguments( pLuaState, nTop, 1 );

	CListBoxDictionary* pListBox = lua_tocppobject( pLuaState, 1, CListBoxDictionary );
	if ( pListBox == nullptr )
	{
		lua_pushinteger( pLuaState, -1 );
		return 1;
	}

	lua_pushinteger( pLuaState, (lua_Integer)pListBox->GetItemContextMenu() );

	return 1;
}


int iberbar::Gui::LuaCppFunction_WidgetListBox_GetItemContextMenuAtPoint( lua_State* pLuaState )
{
	int nTop = lua_gettop( pLuaState );
	iberbar_LuaCheckArguments( pLuaState, nTop, 1 );

	CListBoxDictionary* pListBox = lua_tocppobject( pLuaState, 1, CListBoxDictionary );
	if ( pListBox == nullptr )
	{
		lua_pushnil( pLuaState );
		return 1;
	}

	CPoint2i Point = pListBox->GetItemContextMenuAtPoint();
	LuaApiUtil_PushPoint2i( pLuaState, Point );

	return 1;
}


int iberbar::Gui::LuaCppFunction_WidgetListBox_GetItemBounding( lua_State* pLuaState )
{
	int nTop = lua_gettop( pLuaState );
	iberbar_LuaCheckArguments( pLuaState, nTop, 2 );

	CListBoxDictionary* pListBox = lua_tocppobject( pLuaState, 1, CListBoxDictionary );
	if ( pListBox == nullptr )
	{
		lua_pushnil( pLuaState );
		return 1;
	}

	int nIndex = lua_tointeger( pLuaState, 2 );
	CRect2i Rect = pListBox->GetItemBounding( nIndex );
	LuaApiUtil_PushRect2i( pLuaState, Rect );

	return 1;
}


int iberbar::Gui::LuaCppFunction_WidgetListBox_SetItemEnable( lua_State* pLuaState )
{
	int nTop = lua_gettop( pLuaState );
	iberbar_LuaCheckArguments( pLuaState, nTop, 3 );

	CListBoxDictionary* pListBox = lua_tocppobject( pLuaState, 1, CListBoxDictionary );
	if ( pListBox == nullptr )
		return 0;

	int nIndex = lua_tointeger( pLuaState, 2 );
	bool bEnable = lua_toboolean( pLuaState, 3 );
	pListBox->SetItemEnable( nIndex, bEnable );

	return 0;
}


int iberbar::Gui::LuaCppFunction_WidgetListBox_IsItemEnable( lua_State* pLuaState )
{
	int nTop = lua_gettop( pLuaState );
	iberbar_LuaCheckArguments( pLuaState, nTop, 2 );

	CListBoxDictionary* pListBox = lua_tocppobject( pLuaState, 1, CListBoxDictionary );
	if ( pListBox == nullptr )
	{
		lua_pushboolean( pLuaState, false );
		return 1;
	}

	int nIndex = lua_tointeger( pLuaState, 2 );
	lua_pushboolean( pLuaState, pListBox->IsItemEnable( nIndex ) );

	return 1;
}


iberbar::Gui::CRenderElement* iberbar::Gui::LuaCppCallbackExecute_WidgetListBox_ItemElementAllocate( Lua::PTR_CLuaCallbackHandler pLuaCallback )
{
	if ( pLuaCallback->PushFunction() == true )
	{
		lua_State* pLuaState = pLuaCallback->GetLuaState();
		int nArg = 0;
		if ( pLuaCallback->HasExt() == true )
		{
			pLuaCallback->PushExt();
			nArg++;
		}

		Lua::CFunctionHelper::sExecuteFunction( pLuaState, nArg, 1 );
		CRenderElement* pElement = lua_tocppobject( pLuaState, -1, CRenderElement );
		if ( pElement )
		{
			pElement->AddRef();
		}
		return pElement;
	}

	return nullptr;
}


void iberbar::Gui::LuaCppCallbackExecute_WidgetListBox_ItemElementRelease( Lua::PTR_CLuaCallbackHandler pLuaCallback, CRenderElement* pElement )
{
	if ( pLuaCallback->PushFunction() == true )
	{
		lua_State* pLuaState = pLuaCallback->GetLuaState();
		int nArg = 1;
		if ( pLuaCallback->HasExt() == true )
		{
			pLuaCallback->PushExt();
			nArg++;
		}

		lua_pushcppref( pLuaState, LuaCppNames::s_ClassName_Element, pElement );

		Lua::CFunctionHelper::sExecuteFunction( pLuaState, nArg );
	}
}


void iberbar::Gui::LuaCppCallbackExecute_WidgetListBox_ItemElementCreate( Lua::PTR_CLuaCallbackHandler pLuaCallback, CRenderElement* pElement, const CVariantDictionary* pData, int nIndex )
{
	if ( pLuaCallback->PushFunction() == true )
	{
		lua_State* pLuaState = pLuaCallback->GetLuaState();
		int nArg = 3;
		if ( pLuaCallback->HasExt() == true )
		{
			pLuaCallback->PushExt();
			nArg++;
		}

		lua_pushcppref( pLuaState, LuaCppNames::s_ClassName_Element_FullName, pElement );

		lua_createtable( pLuaState, 0, 0 );
		ListBoxItemDataDictionary_ToLua( *pData, pLuaState, lua_gettop( pLuaState ) );

		lua_pushinteger( pLuaState, nIndex );

		Lua::CFunctionHelper::sExecuteFunction( pLuaState, nArg );
	}
}


void iberbar::Gui::LuaCppCallbackExecute_WidgetListBox_ItemElementUpdate( Lua::PTR_CLuaCallbackHandler pLuaCallback, CRenderElement* pElement, const CVariantDictionary* pData, int nIndex, int nUpdateEvent )
{
	if ( pLuaCallback->PushFunction() == true )
	{
		lua_State* pLuaState = pLuaCallback->GetLuaState();
		int nArg = 4;
		if ( pLuaCallback->HasExt() == true )
		{
			pLuaCallback->PushExt();
			nArg++;
		}

		lua_pushcppref( pLuaState, LuaCppNames::s_ClassName_Element_FullName, pElement );

		lua_createtable( pLuaState, 0, 0 );
		ListBoxItemDataDictionary_ToLua( *pData, pLuaState, lua_gettop( pLuaState ) );

		lua_pushinteger( pLuaState, nIndex );
		lua_pushinteger( pLuaState, nUpdateEvent );

		Lua::CFunctionHelper::sExecuteFunction( pLuaState, nArg );
	}
}


bool iberbar::Gui::LuaCppCallbackExecute_WidgetListBox_FindItem( Lua::PTR_CLuaCallbackHandler pLuaCallback, const CVariantDictionary* pData )
{
	if ( pLuaCallback->PushFunction() == true )
	{
		lua_State* pLuaState = pLuaCallback->GetLuaState();
		int nArg = 1;
		if ( pLuaCallback->HasExt() == true )
		{
			pLuaCallback->PushExt();
			nArg++;
		}

		lua_createtable( pLuaState, 0, 0 );
		ListBoxItemDataDictionary_ToLua( *pData, pLuaState, lua_gettop( pLuaState ) );

		int ret = lua_pcallk( pLuaState, nArg, 1, 0, 0, 0 );
		if ( ret != 0 )
		{
			int type = lua_type( pLuaState, -1 );
			const char* err = lua_tostring( pLuaState, -1 );
			lua_pop( pLuaState, 1 );
			return false;
		}

		bool bFind = lua_toboolean( pLuaState, -1 );
		lua_pop( pLuaState, 1 );

		return bFind;
	}
	return false;
}









int iberbar::Gui::LuaCppFunction_WidgetEditBox_SetValueText( lua_State* pLuaState )
{
	int top = lua_gettop( pLuaState );
	iberbar_LuaCheckArguments( pLuaState, top, 2 );

	CEditBox* pEditBox = lua_tocppobject( pLuaState, 1, CEditBox );
	if ( pEditBox == nullptr )
		return 0;

	const char* strValueText = lua_tostring( pLuaState, 2 );
	if ( StringIsNullOrEmpty( strValueText ) )
	{
		pEditBox->SetValueText( L"" );
	}
	else
	{
		std::wstring strValueTextUnicode = Utf8ToUnicode( strValueText );
		pEditBox->SetValueText( strValueTextUnicode.c_str() );
	}

	return 0;
}


int iberbar::Gui::LuaCppFunction_WidgetEditBox_GetValueText( lua_State* pLuaState )
{
	int top = lua_gettop( pLuaState );
	iberbar_LuaCheckArguments( pLuaState, top, 1 );

	CEditBox* pEditBox = lua_tocppobject( pLuaState, 1, CEditBox );
	if ( pEditBox == nullptr )
	{
		lua_pushstring( pLuaState, "" );
		return 1;
	}

	const wchar_t* strValueTextUnicode = pEditBox->GetValueText();
	if ( StringIsNullOrEmpty( strValueTextUnicode ) )
	{
		lua_pushstring( pLuaState, "" );
	}
	else
	{
		std::string strValueTextUtf8 = UnicodeToUtf8( strValueTextUnicode );
		lua_pushstring( pLuaState, strValueTextUtf8.c_str() );
	}

	return 1;
}









int iberbar::Gui::LuaCppFunction_WidgetProgressBar_SetProgressValueMax( lua_State* pLuaState )
{
	CProgressBar* pProgressBar = lua_tocppobject( pLuaState, 1, CProgressBar );
	if ( pProgressBar == nullptr )
	{
		return 0;
	}

	int nValueMax = lua_tointeger( pLuaState, 2 );
	pProgressBar->SetProgressValueMax( nValueMax );
	return 0;
}


int iberbar::Gui::LuaCppFunction_WidgetProgressBar_SetProgressValue( lua_State* pLuaState )
{
	CProgressBar* pProgressBar = lua_tocppobject( pLuaState, 1, CProgressBar );
	if ( pProgressBar == nullptr )
	{
		return 0;
	}

	int nValue = lua_tointeger( pLuaState, 2 );
	pProgressBar->SetProgressValue( nValue );
	return 0;
}


int iberbar::Gui::LuaCppFunction_WidgetProgressBar_GetProgressValueMax( lua_State* pLuaState )
{
	CProgressBar* pProgressBar = lua_tocppobject( pLuaState, 1, CProgressBar );
	if ( pProgressBar == nullptr )
	{
		lua_pushinteger( pLuaState, 0 );
		return 1;
	}

	lua_pushinteger( pLuaState, pProgressBar->GetProgressValueMax() );
	return 1;
}


int iberbar::Gui::LuaCppFunction_WidgetProgressBar_GetProgressValue( lua_State* pLuaState )
{
	CProgressBar* pProgressBar = lua_tocppobject( pLuaState, 1, CProgressBar );
	if ( pProgressBar == nullptr )
	{
		lua_pushinteger( pLuaState, 0 );
		return 1;
	}

	lua_pushinteger( pLuaState, pProgressBar->GetProgressValue() );
	return 1;
}










int iberbar::Gui::LuaCppFunction_Element_Init( lua_State* pLuaState )
{
	int top = lua_gettop( pLuaState );
	iberbar_LuaCheckArguments( pLuaState, top, 1 );

	CRenderElement* pElement = lua_tocppobject( pLuaState, 1, CRenderElement );

	if ( pElement == nullptr )
		return 0;

	pElement->Init();

	return 0;
}


int iberbar::Gui::LuaCppFunction_Element_SetZOrder( lua_State* pLuaState )
{
	int top = lua_gettop( pLuaState );
	iberbar_LuaCheckArguments( pLuaState, top, 2 );

	CRenderElement* pElement = lua_tocppobject( pLuaState, 1, CRenderElement );

	if ( pElement == nullptr )
		return 0;

	int nZOrder = (int)lua_tointeger( pLuaState, 2 );
	pElement->SetZOrder( nZOrder );

	return 0;
}


int iberbar::Gui::LuaCppFunction_Element_Clone( lua_State* pLuaState )
{
	int top = lua_gettop( pLuaState );
	iberbar_LuaCheckArguments( pLuaState, top, 1 );

	CRenderElement* pElement = lua_tocppobject( pLuaState, 1, CRenderElement );

	if ( pElement == nullptr )
	{
		lua_pushnil( pLuaState );
		return 1;
	}

	CRenderElement* pElementClone = CRenderElement::sCloneRenderElementRecurse( pElement );
	if ( pElementClone == nullptr )
	{
		lua_pushnil( pLuaState );
	}
	else
	{
		lua_pushcppref( pLuaState, LuaCppNames::s_ClassName_Element_FullName, pElementClone );
		pElementClone->Release();
	}

	return 1;
}


int iberbar::Gui::LuaCppFunction_Element_FindElement( lua_State* pLuaState )
{
	int top = lua_gettop( pLuaState );
	if ( top != 2 )
		return iberbar_LuaError_ArgumentsCountIsNotMatch( pLuaState, 2 );

	CRenderElement* pElement = lua_tocppobject( pLuaState, 1, CRenderElement );
	const char* strId = lua_tostring( pLuaState, 2 );

	if ( pElement != nullptr && StringIsNullOrEmpty( strId ) == false )
	{
		PTR_CRenderElement pElementFind = nullptr;
		if ( pElement->FindElement( strId, &pElementFind ) == false )
		{
			lua_pushnil( pLuaState );
		}
		else
		{
			lua_pushcppref( pLuaState, LuaCppNames::s_ClassName_Element_FullName, pElementFind );
		}
	}
	else
	{
		lua_pushnil( pLuaState );
	}

	return 1;
}


int iberbar::Gui::LuaCppFunction_Element_AddElement( lua_State* pLuaState )
{
	int top = lua_gettop( pLuaState );
	if ( top != 2 )
		return iberbar_LuaError_ArgumentsCountIsNotMatch( pLuaState, 2 );

	CRenderElement* pElement = lua_tocppobject( pLuaState, 1, CRenderElement );

	if ( pElement == nullptr )
	{
		return 0;
	}

	CRenderElement* pElementChild = lua_tocppobject( pLuaState, 2, CRenderElement );
	pElement->AddChildElement( pElementChild );

	return 0;
}






int iberbar::Gui::LuaCppFunction_ElementColorRect_SetColor( lua_State* pLuaState )
{
	int top = lua_gettop( pLuaState );
	iberbar_LuaCheckArguments( pLuaState, top, 3 );

	CElementColorRect* pElement = lua_tocppobject( pLuaState, 1, CElementColorRect );

	if ( pElement == nullptr )
		return 0;

	int nState = lua_tointeger( pLuaState, 2 );
	CColor4B Color;
	if ( lua_isinteger( pLuaState, 3 ) )
	{
		Color = CColor4B( (uint32)lua_tointeger( pLuaState, 3 ) );
	}
	else if ( lua_istable( pLuaState, 3 ) )
	{
	}

	pElement->SetColor( nState, Color );

	return 0;
}








int iberbar::Gui::LuaCppFunction_ElementStateLabel_SetFont( lua_State* pLuaState )
{
	int top = lua_gettop( pLuaState );
	iberbar_LuaCheckArguments( pLuaState, top, 2 );

	CElementStateLabel* pElement = lua_tocppobject( pLuaState, 1, CElementStateLabel );

	if ( pElement == nullptr )
		return 0;

	Renderer::CFont* pFont = lua_tocppobject( pLuaState, 2, Renderer::CFont );
	pElement->SetFont( pFont );

	return 0;
}


int iberbar::Gui::LuaCppFunction_ElementStateLabel_SetColor( lua_State* pLuaState )
{
	int top = lua_gettop( pLuaState );
	iberbar_LuaCheckArguments( pLuaState, top, 3 );

	CElementStateLabel* pElement = lua_tocppobject( pLuaState, 1, CElementStateLabel );

	if ( pElement == nullptr )
		return 0;

	int nState = lua_tointeger( pLuaState, 2 );
	CColor4B Color;
	if ( lua_isinteger( pLuaState, 3 ) )
	{
		Color = CColor4B( (uint32)lua_tointeger( pLuaState, 3 ) );
	}
	else if ( lua_istable( pLuaState, 3 ) )
	{
	}

	pElement->SetColor( nState, Color );

	return 0;
}


int iberbar::Gui::LuaCppFunction_ElementStateLabel_SetText( lua_State* pLuaState )
{
	int top = lua_gettop( pLuaState );
	iberbar_LuaCheckArguments( pLuaState, top, 2 );

	CElementStateLabel* pElement = lua_tocppobject( pLuaState, 1, CElementStateLabel );

	if ( pElement == nullptr )
		return 0;

	const char* strText = lua_tostring( pLuaState, 2 );
	pElement->SetTextA( strText );

	return 0;
}


int iberbar::Gui::LuaCppFunction_ElementStateLabel_SetTextAlignHorizental( lua_State* pLuaState )
{
	int nTop = lua_gettop( pLuaState );
	iberbar_LuaCheckArguments( pLuaState, nTop, 2 );

	CElementStateLabel* pElement = lua_tocppobject( pLuaState, 1, CElementStateLabel );

	if ( pElement == nullptr )
		return 0;

	UAlignHorizental nAlign = (UAlignHorizental)lua_tointeger( pLuaState, 2 );
	pElement->SetTextAlignHorizental( nAlign );

	return 0;
}


int iberbar::Gui::LuaCppFunction_ElementStateLabel_SetTextAlignVertical( lua_State* pLuaState )
{
	int nTop = lua_gettop( pLuaState );
	iberbar_LuaCheckArguments( pLuaState, nTop, 2 );

	CElementStateLabel* pElement = lua_tocppobject( pLuaState, 1, CElementStateLabel );

	if ( pElement == nullptr )
		return 0;

	UAlignVertical nAlign = (UAlignVertical)lua_tointeger( pLuaState, 2 );
	pElement->SetTextAlignVertical( nAlign );

	return 0;
}







int iberbar::Gui::LuaCppFunction_ElementStateTexture_SetTexture( lua_State* pLuaState )
{
	int nTop = lua_gettop( pLuaState );
	iberbar_LuaCheckArguments( pLuaState, nTop, 3 );

	CElementStateTexture* pElement = lua_tocppobject( pLuaState, 1, CElementStateTexture );

	if ( pElement == nullptr )
		return 0;

	int nState = (int)lua_tointeger( pLuaState, 2 );
	RHI::ITexture* pTexture = lua_tocppobject( pLuaState, 3, RHI::ITexture );
	pElement->SetTexture( nState, pTexture );

	return 0;
}


int iberbar::Gui::LuaCppFunction_ElementStateTexture_SetColor( lua_State* pLuaState )
{
	int nTop = lua_gettop( pLuaState );
	iberbar_LuaCheckArguments( pLuaState, nTop, 3 );

	CElementStateTexture* pElement = lua_tocppobject( pLuaState, 1, CElementStateTexture );

	if ( pElement == nullptr )
		return 0;

	int nState = (int)lua_tointeger( pLuaState, 2 );
	CColor4B Color;
	if ( lua_isinteger( pLuaState, 3 ) )
	{
		Color = CColor4B( (uint32)lua_tointeger( pLuaState, 3 ) );
	}
	else if ( lua_istable( pLuaState, 3 ) )
	{
	}
	pElement->SetColor( nState, Color );

	return 0;
}


int iberbar::Gui::LuaCppFunction_ElementStateTexture_SetUV( lua_State* pLuaState )
{
	int nTop = lua_gettop( pLuaState );
	iberbar_LuaCheckArguments( pLuaState, nTop, 6 );

	CElementStateTexture* pElement = lua_tocppobject( pLuaState, 1, CElementStateTexture );

	if ( pElement == nullptr )
		return 0;

	int nState = (int)lua_tointeger( pLuaState, 2 );
	float UV_Left = (float)lua_tonumber( pLuaState, 3 );
	float UV_Top = (float)lua_tonumber( pLuaState, 4 );
	float UV_Right = (float)lua_tonumber( pLuaState, 5 );
	float UV_Bottom = (float)lua_tonumber( pLuaState, 6 );
	pElement->SetUV( nState, CRect2f( UV_Left, UV_Top, UV_Right, UV_Bottom ) );

	return 0;
}







int iberbar::Gui::LuaCppFunction_Engine_CreateDialog( lua_State* pLuaState )
{
	int top = lua_gettop( pLuaState );
	iberbar_LuaCheckArguments2( pLuaState, top, 0, 1 );

	CDialog* pDlgParent = nullptr;
	if ( top == 1 )
	{
		pDlgParent = lua_tocppobject( pLuaState, 1, CDialog );;
	}

	TSmartRefPtr<CDialog> pDlg = nullptr;
	CResult ret = CDialog::sCreateDialog( &pDlg, pDlgParent );
	if ( ret.IsOK() == false )
	{
		lua_pushnil( pLuaState );
		return 1;
	}

	lua_pushcppref( pLuaState, LuaCppNames::s_ClassName_Dialog_FullName, pDlg );

	return 1;
}


int iberbar::Gui::LuaCppFunction_Engine_GetDialog( lua_State* pLuaState )
{
	int top = lua_gettop( pLuaState );
	iberbar_LuaCheckArguments( pLuaState, top, 1 );

	const char* strId = lua_tostring( pLuaState, 1 );
	if ( StringIsNullOrEmpty( strId ) == true )
	{
		lua_pushnil( pLuaState );
		return 1;
	}

	CDialog* pDlg = CEngine::sGetInstance()->GetDialog( strId );
	if ( pDlg == nullptr )
	{
		lua_pushnil( pLuaState );
	}
	else
	{
		lua_pushcppref( pLuaState, LuaCppNames::s_ClassName_Dialog_FullName, pDlg );
	}

	return 1;
}


int iberbar::Gui::LuaCppFunction_Engine_DestroyDialog( lua_State* pLuaState )
{
	int top = lua_gettop( pLuaState );
	iberbar_LuaCheckArguments( pLuaState, top, 1 );

	CDialog* pDlg = lua_tocppobject( pLuaState, 1, CDialog );
	if ( pDlg == nullptr )
		return 0;

	CDialog::sDestroyDialog( pDlg );

	return 0;
}








int iberbar::Gui::LuaCppFunction_XmlParser_ReadFile( lua_State* pLuaState )
{
	int top = lua_gettop( pLuaState );
	iberbar_LuaCheckArguments( pLuaState, top, 3 );

	CXmlParser* pXmlParser = lua_tocppobject( pLuaState, 1, CXmlParser );
	const char* strFile = lua_tostring( pLuaState, 2 );
	CDialog* pDlg = lua_tocppobject( pLuaState, 3, CDialog );

	if ( pXmlParser == nullptr ||
		StringIsNullOrEmpty( strFile ) ||
		pDlg == nullptr )
	{
		lua_pushstring( pLuaState, "invalid arguments" );
		return 1;
	}

	CResult ret = pXmlParser->ReadFile( strFile, pDlg );
	if ( ret.IsOK() == false )
	{
		lua_pushstring( pLuaState, ret.data.c_str() );
		return 1;
	}

	lua_pushnil( pLuaState );

	return 1;
}


int iberbar::Gui::LuaCppFunction_XmlParser_SetCallbackGetTexture( lua_State* pLuaState )
{
	int nTop = lua_gettop( pLuaState );
	iberbar_LuaCheckArguments2( pLuaState, nTop, 2, 3 );

	CXmlParser* pXmlParser = lua_tocppobject( pLuaState, 1, CXmlParser );
	if ( pXmlParser == nullptr )
		return 0;

	lua_Integer nFunc = lua_toluacallback( pLuaState, 2 );
	lua_Integer nExt = 0;
	if ( nTop == 3 )
	{
		nExt = lua_toluacallback_extparam( pLuaState, 3 );
	}
	if ( nFunc == 0 )
		return 0;
	
	Lua::PTR_CLuaCallbackHandler pLuaCallback = Lua::PTR_CLuaCallbackHandler::_sNew( pLuaState, nFunc, nExt );
	pXmlParser->RegisterGetTexture(
		[pLuaCallback]( const char* strText, RHI::ITexture** ppOutTexture )
		{
			return LuaCppCallbackExecute_XmlParser_CallbackGetTexture( pLuaCallback, strText, ppOutTexture );
		} );

	return 0;
}


int iberbar::Gui::LuaCppFunction_XmlParser_SetCallbackGetFont( lua_State* pLuaState )
{
	int nTop = lua_gettop( pLuaState );
	iberbar_LuaCheckArguments2( pLuaState, nTop, 2, 3 );

	CXmlParser* pXmlParser = lua_tocppobject( pLuaState, 1, CXmlParser );
	if ( pXmlParser == nullptr )
		return 0;

	lua_Integer nFunc = lua_toluacallback( pLuaState, 2 );
	lua_Integer nExt = 0;
	if ( nTop == 3 )
	{
		nExt = lua_toluacallback_extparam( pLuaState, 3 );
	}
	if ( nFunc == 0 )
		return 0;

	Lua::PTR_CLuaCallbackHandler pLuaCallback = Lua::PTR_CLuaCallbackHandler::_sNew( pLuaState, nFunc, nExt );
	pXmlParser->RegisterGetFont(
		[pLuaCallback]( Renderer::CFont** ppOutFont, const UFontDesc& FontDesc )
		{
			return LuaCppCallbackExecute_XmlParser_CallbackGetFont( pLuaCallback, ppOutFont, FontDesc );
		} );

	return 0;
}







FORCEINLINE void iberbar::Gui::LuaCppCallbackExecute_WidetEvent( Lua::PTR_CLuaCallbackHandler pLuaCallback, CWidget* pWidget, uint64 nEvent, uint64 nValueUint )
{
	if ( pLuaCallback->PushFunction() == true )
	{
		lua_State* pLuaState = pLuaCallback->GetLuaState();

		int nArg = 3;
		if ( pLuaCallback->HasExt() == true )
		{
			if ( pLuaCallback->PushExt() == true )
				nArg++;
		}

		if ( pWidget == nullptr )
		{
			lua_pushnil( pLuaState );
		}
		else
		{
			lua_pushcppref( pLuaState, LuaCppNames::s_ClassName_Widget_FullName, pWidget );
		}
		lua_pushinteger( pLuaState, nEvent );
		lua_pushinteger( pLuaState, nValueUint );

		Lua::CFunctionHelper::sExecuteFunction( pLuaState, nArg );
		//int ret = lua_pcall( pLuaState, nArg, 0, 0 );
		////lua_pcall( pLuaState, 3, 0, 0 );
		//if ( ret != 0 )
		//{
		//	int type = lua_type( pLuaState, -1 );
		//	const char* err = lua_tostring( pLuaState, -1 );
		//	lua_pop( pLuaState, 1 );
		//	//return MakeResult( ResultCode::Bad, err );
		//}
	}
}


iberbar::CResult iberbar::Gui::LuaCppCallbackExecute_XmlParser_CallbackGetTexture( Lua::PTR_CLuaCallbackHandler pLuaCallback, const char* strText, RHI::ITexture** ppOutTexture )
{
	UNKNOWN_SAFE_RELEASE_NULL( *ppOutTexture );

	if ( pLuaCallback->PushFunction() == false )
		return MakeResult( ResultCode::Bad, "error lua push callback" );

	lua_State* pLuaState = pLuaCallback->GetLuaState();
	int nArg = 1;
	if ( pLuaCallback->HasExt() )
	{
		if ( pLuaCallback->PushExt() )
			nArg++;
	}

	lua_pushstring( pLuaState, strText );

	if ( Lua::CFunctionHelper::sExecuteFunction( pLuaState, nArg, 1 ) != 1 )
		return MakeResult( ResultCode::Bad, "error lua return" );

	if ( lua_isnil( pLuaState, -1 ) )
		return CResult();

	if ( lua_isstring( pLuaState, -1 ) )
		return MakeResult( ResultCode::Bad, lua_tostring( pLuaState, -1 ) );

	if ( lua_isuserdata( pLuaState, -1 ) == 0 )
		return MakeResult( ResultCode::Bad, "error lua return" );

	*ppOutTexture = lua_tocppobject( pLuaState, -1, RHI::ITexture );
	UNKNOWN_SAFE_ADDREF( *ppOutTexture );

	return CResult();
}


iberbar::CResult iberbar::Gui::LuaCppCallbackExecute_XmlParser_CallbackGetFont( Lua::PTR_CLuaCallbackHandler pLuaCallback, Renderer::CFont** ppOutFont, const UFontDesc& FontDesc )
{
	UNKNOWN_SAFE_RELEASE_NULL( *ppOutFont );

	if ( pLuaCallback->PushFunction() == false )
		return MakeResult( ResultCode::Bad, "error lua push callback" );

	lua_State* pLuaState = pLuaCallback->GetLuaState();
	int nArg = 1;
	if ( pLuaCallback->HasExt() )
	{
		if ( pLuaCallback->PushExt() )
			nArg++;
	}

	lua_createtable( pLuaState, 0, 4 );
	lua_pushstring( pLuaState, "family" );
	lua_pushstring( pLuaState, FontDesc.FamilyName.c_str() );
	lua_settable( pLuaState, -3 );
	lua_pushstring( pLuaState, "size" );
	lua_pushinteger( pLuaState, FontDesc.Size );
	lua_settable( pLuaState, -3 );
	lua_pushstring( pLuaState, "weight" );
	lua_pushinteger( pLuaState, FontDesc.Weight );
	lua_settable( pLuaState, -3 );
	lua_pushstring( pLuaState, "italic" );
	lua_pushboolean( pLuaState, FontDesc.Italic );
	lua_settable( pLuaState, -3 );

	if ( Lua::CFunctionHelper::sExecuteFunction( pLuaState, nArg, 1 ) != 1 )
		return MakeResult( ResultCode::Bad, "error lua returns" );

	if ( lua_isnil( pLuaState, -1 ) )
		return CResult();

	if ( lua_isstring( pLuaState, -1 ) )
		return MakeResult( ResultCode::Bad, lua_tostring( pLuaState, -1 ) );

	if ( lua_isuserdata( pLuaState, -1 ) == 0 )
		return MakeResult( ResultCode::Bad, "error lua return" );

	*ppOutFont = lua_tocppobject( pLuaState, -1, Renderer::CFont );
	UNKNOWN_SAFE_ADDREF( *ppOutFont );

	return CResult();
}
