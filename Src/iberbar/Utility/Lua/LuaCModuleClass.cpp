
#include <iberbar/Base/Lua/LuaCModuleClass.h>


void iberbar::CLuaCModuleClass::onRegister( lua_State* pLuaState, int tb )
{
	if ( tb < 0 )
	{

	}
	else
	{

	}

	PTR_CLuaCppClass pLuaCppCls;
	pLuaCppCls.attach( new CLuaCppClass() );

	const Reg* cls = this->classes;
	while ( cls->clsname )
	{
		pLuaCppCls->declareStart( pLuaState, cls->clsname );
		pLuaCppCls->setFunctionConstruct( cls->fn_constructor );
		pLuaCppCls->setFunctionImportant( "__gc", cls->fn_gc );
		pLuaCppCls->setFunctionImportant( "__tostring", cls->fn_tostring );
		pLuaCppCls->setFunction( cls->fns );
		pLuaCppCls->declareEnd();
		++ cls;
	}
}