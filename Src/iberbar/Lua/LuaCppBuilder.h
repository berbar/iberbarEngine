#pragma once

#include <iberbar/Lua/LuaBase.h>
#include <iberbar/Utility/Result.h>

namespace iberbar
{
	namespace LuaCpp
	{
		class CClassBuilder;
		class CEnumBuilder;
		class CVariableBuilder;
		class CBuilder;
		class CScopeBuilder;

		typedef void (*PHowToBuildClass)(const char* moduleName, CClassBuilder* classBuilder);
		typedef void (*PHowToBuildVariable)(const char* moduleName, CVariableBuilder* classBuilder);
		typedef void (*PHowToBuildEnum)( CEnumBuilder* pEnum );
		typedef void (*PHowToBuildScope)(CScopeBuilder* scope);
		typedef void (*PHowToBuildTable)(const char* moduleName);

		enum UClassStandardMethodFlag
		{
			ToString,
			OperatorAdd
		};

		//extern const char ClassStandardMethod_ToString[] = "__tostring";
		//// 运算符 '+'
		//extern const char ClassStandardMethod_Operator_Add[] = "__add";
		//// 运算符 '-'
		//extern const char ClassStandardMethod_Operator_Sub[] = "__sub";
		//// 运算符 '*'
		//extern const char ClassStandardMethod_Operator_Mul[] = "__mul";
		//// 运算符 '/'
		//extern const char ClassStandardMethod_Operator_Div[] = "__div";
		//// 运算符 '%'
		//extern const char ClassStandardMethod_Operator_Mod[] = "__mod";
		//// 运算符 '-'，取反
		//extern const char ClassStandardMethod_Operator_Unm[] = "__unm";
		//// 运算符 '..'
		//extern const char ClassStandardMethod_Operator_Concat[] = "__concat";
		//// 运算符 '=='
		//extern const char ClassStandardMethod_Operator_Eq[] = "__eq";
		//// 运算符 '<'
		//extern const char ClassStandardMethod_Operator_Lt[] = "__lt";
		//// 运算符 '<='
		//extern const char ClassStandardMethod_Operator_Le[] = "__le";

		class __iberbarLuaApi__ CClassBuilder
		{
		public:
			enum __iberbarLuaApi__ UpValueType
			{
				UpValue_Boolean,
				UpValue_Integer,
				UpValue_Number,
				UpValue_String,
				UpValue_Pointer
			};

			struct __iberbarLuaApi__ UpValue
			{
				union
				{
					bool v_b;
					lua_Integer v_i;
					lua_Number v_n;
					const char* v_s;
					void* v_p;
				} v;
				UpValueType t;
			};

		public:
			CClassBuilder( lua_State* pLuaState, const char* classNameFull, const char* className, int metatable, int methods );

		public:
			// 添加构造函数
			CClassBuilder* AddConstructor( lua_CFunction func );

			// 添加析构函数
			CClassBuilder* AddDistructor( lua_CFunction func );

			// 添加成员方法
			CClassBuilder* AddMemberMethod( const char* name, lua_CFunction func, UpValue* upvalues = nullptr, int upvaluesCount = 0 );

			// 添加静态方法
			CClassBuilder* AddStaticMethod( const char* name, lua_CFunction func );

			// 重载标准方法，方法对应的字段参考 ClassStandardMethod_xxx
			CClassBuilder* AddStandardMethod( const char* name, lua_CFunction func );

			const char* ClassName() { return m_className.c_str(); }

		private:
			lua_State* m_pLuaState;
			std::string m_classNameFull;
			std::string m_className;
			int m_methods;
			int m_metatable;
		};



		class __iberbarLuaApi__ CEnumBuilder
		{
		public:
			CEnumBuilder( lua_State* pLuaState, int nValueCount );
			void AddValueInt( const char* strKey, lua_Integer Value );
			void AddValueString( const char* strKey, const char* Value );

		private:
			lua_State* m_pLuaState;
			int m_nTable;
		};


		//class __iberbarLuaApi__ CVariableBuilder
		//{
		//public:
		//	void AddInteger( const char* name, lua_Integer value );
		//	void SetNumber( const char* name, lua_Number value );
		//	void SetString( const char* name, const char* value );
		//	void SetTable( const char* name, lua_Integer value );
		//};

		class __iberbarLuaApi__ CScopeBuilder abstract
		{
		protected:
			CScopeBuilder( lua_State* pLuaState, const char* moduleName );

		public:
			// 注册class类型
			// @extends 继承的父类
			virtual void AddClass( const char* className, PHowToBuildClass pHowToAddClass, const char* extends = nullptr ) = 0;

			virtual void AddEnum( const char* strEnumTypeName, PHowToBuildEnum pHow, int nCount = -1 ) = 0;

			virtual void AddFunctionOne( const char* functionName, lua_CFunction pFunction ) = 0;

			virtual void AddFunctions( const luaL_Reg* pFunctionRegs ) = 0;
			//virtual void AddVariables( PHowToBuildVariable pHowToBuildVars );

			const std::string& GetModuleName() const { return m_name; }

		protected:
			lua_State* m_pLuaState;
			// 域名，空字符串为全局，不然为table路径名称
			std::string m_name;
		};

		class __iberbarLuaApi__ CBuilder
		{
		public:
			CBuilder( lua_State* pLuaState );

			void ResolveScope( PHowToBuildScope pHowToBuildScope, const char* moduleName = nullptr );

		private:
			lua_State* m_pLuaState;
		};
	}
}
