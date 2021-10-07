#pragma once

#include <iberbar/Lua/LuaBase.h>

namespace iberbar
{
	namespace Lua
	{
		// Class 类名，如 CFoo
		extern const char __iberbarLuaApi__ uClassReflection_ClassName[];

		// Class 所属的模块名，如 iberbar.Test
		extern const char __iberbarLuaApi__ uClassReflection_ModuleName[];

		// Class 全名，如 iberbar.Test.CFoo
		extern const char __iberbarLuaApi__ uClassReflection_FullName[];

		// Class 定义来源，1=C++，2=Lua
		extern const char __iberbarLuaApi__ uClassReflection_CType[];

		extern const char __iberbarLuaApi__ uWeakTable_ForUserData[];

		enum class UClassReflectionCType
		{
			Cpp = 1,
			Lua = 2
		};


		static const char s_ClassStandardMethod_ToString[] = "__tostring";
		// 运算符 '+'
		static const char s_ClassStandardMethod_Operator_Add[] = "__add";
		// 运算符 '-'
		static const char s_ClassStandardMethod_Operator_Sub[] = "__sub";
		// 运算符 '*'
		static const char s_ClassStandardMethod_Operator_Mul[] = "__mul";
		// 运算符 '/'
		static const char s_ClassStandardMethod_Operator_Div[] = "__div";
		// 运算符 '%'
		static const char s_ClassStandardMethod_Operator_Mod[] = "__mod";
		// 运算符 '-'，取反
		static const char s_ClassStandardMethod_Operator_Unm[] = "__unm";
		// 运算符 '..'
		static const char s_ClassStandardMethod_Operator_Concat[] = "__concat";
		// 运算符 '=='
		static const char s_ClassStandardMethod_Operator_Eq[] = "__eq";
		// 运算符 '<'
		static const char s_ClassStandardMethod_Operator_Lt[] = "__lt";
		// 运算符 '<='
		static const char s_ClassStandardMethod_Operator_Le[] = "__le";
	}
}


// Class 类名，如 CFoo
#define iberbar_LuaCpp_ClassReflection_ClassName "__cpp_classname"

// Class 所属的模块名，如 iberbar.Test
#define iberbar_LuaCpp_ClassReflection_ModuleName "__cpp_modulename"

// Class 全名，如 iberbar.Test.CFoo
#define iberbar_LuaCpp_ClassReflection_FullName "__cpp_fullname"

// Class 定义来源，1=C++，2=Lua
#define iberbar_LuaCpp_ClassReflection_CType "__ctype"

