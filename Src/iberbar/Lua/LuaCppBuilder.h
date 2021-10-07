#pragma once

#include <iberbar/Lua/LuaBase.h>
#include <iberbar/Lua/LuaCppCommon.h>
#include <iberbar/Utility/Result.h>
#include <functional>

namespace iberbar
{
	namespace Lua
	{
		class CClassBuilder;
		class CEnumBuilder;
		class CVariableBuilder;
		class CBuilder;
		class CScopeBuilder;

		typedef void (PHowToBuildClass)(const char* moduleName, CClassBuilder* classBuilder);
		typedef void (*PHowToBuildVariable)(const char* moduleName, CVariableBuilder* classBuilder);
		typedef void (*PHowToBuildEnum)( CEnumBuilder* pEnum );
		typedef void (*PHowToBuildScope)(CScopeBuilder* scope);
		typedef void (*PHowToBuildTable)(const char* moduleName);

		enum UClassStandardMethodFlag
		{
			ToString,
			OperatorAdd
		};





		struct UClassDefinition
		{
			const char* classname;
			const char* classnamefull;
			const char* classname_extends;
			lua_CFunction __constructor;
			lua_CFunction __distructor;
			const luaL_Reg* __methods;
			//lua_CFunction __tostring;
			//lua_CFunction __add;
			//lua_CFunction __sub;
			//lua_CFunction __mul;
			//lua_CFunction __div;
			//lua_CFunction __mod;
			//lua_CFunction __unm;
			//lua_CFunction __concat;
			//lua_CFunction __eq;
			//lua_CFunction __lt;
			//lua_CFunction __le;
		};


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
			CClassBuilder( lua_State* pLuaState, const char* classNameFull, const char* strClassNameExtends, int metatable );

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

			const char* ClassName() { return m_classNameFull.c_str(); }
			const char* ClassName_Extends() { return m_strClassName_Extends.c_str(); }

		private:
			lua_State* m_pLuaState;
			std::string m_classNameFull;
			std::string m_strClassName_Extends;
			int m_metatable;
			int m_methods;
			//int m_nLuaMetatable_Extends;
			//int m_nLuaMethods_Extends;
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


		class __iberbarLuaApi__ CScopeBuilder
		{
		public:
			CScopeBuilder( lua_State* pLuaState );
			CScopeBuilder( lua_State* pLuaState, const char* strModuleName, int nLuaModule );

		public:
			// 注册class类型
			// @extends 继承的父类
			void AddClass( const char* strClassName, std::function<PHowToBuildClass> pHowToAddClass, const char* extends = nullptr );
			void AddClass( const char* strClassName, const char* strClassNameFull, std::function<PHowToBuildClass> pHowToAddClass, const char* extends = nullptr );
			void AddClass( const UClassDefinition& Definition );
			void AddEnum( const char* strEnumTypeName, PHowToBuildEnum pHow, int nCount = -1 );
			void AddFunctionOne( const char* functionName, lua_CFunction pFunction );
			void AddFunctions( const luaL_Reg* pFunctionRegs );
			const std::string& GetModuleName() const { return m_name; }

		protected:
			lua_State* m_pLuaState;
			// 域名，空字符串为全局，不然为table路径名称
			std::string m_name;
			int m_nLuaModule;
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
