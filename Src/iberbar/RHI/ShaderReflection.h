#pragma once

#include <iberbar/RHI/Headers.h>
#include <iberbar/RHI/Types.h>

namespace iberbar
{
	namespace RHI
	{
		class IShaderReflectionBuffer;
		class IShaderReflectionVariable;
		class IShaderReflectionType;
		class IShaderReflectionMember;
		class IShaderReflection;


		struct UShaderReflectionVariableDesc
		{
			UShaderVariableType nVarType;
			UShaderVariableClass nVarClass;
			uint32 nOffset;
		};

		struct UShaderReflectionTypeDesc
		{
			UShaderVariableType nVarType;
			UShaderVariableClass nVarClass;
			uint32 nElements;
			uint32 nRows;
			uint32 nColumns;
			const char* pstrName;
		};

		struct UShaderReflectionMemberDesc
		{
			UShaderVariableType nVarType;
			uint32 nOffset;
			const char* pstrName;
		};


		class __iberbarRHIApi__ IShaderReflectionBuffer abstract
		{
		public:
			virtual ~IShaderReflectionBuffer() {}
			virtual int GetVariableCount() const = 0;
			virtual const IShaderReflectionVariable* GetVariableByIndex( int nIndex ) const = 0;
			virtual const IShaderReflectionVariable* GetVariableByName( const char* pstrName ) const = 0;
		};


		class __iberbarRHIApi__ IShaderReflectionVariable abstract
		{
		public:
			virtual ~IShaderReflectionVariable() {}
			virtual const IShaderReflectionType* GetType() const = 0;
			virtual void GetDesc( UShaderReflectionVariableDesc* pOutDesc ) const = 0;
		};


		class __iberbarRHIApi__ IShaderReflectionType abstract
		{
		public:
			virtual ~IShaderReflectionType() {}
			virtual int GetMemberCount() const = 0;
			virtual const IShaderReflectionMember* GetMemberByIndex( int nIndex ) const = 0;
			virtual const IShaderReflectionMember* GetMemberByName( const char* pstrName ) const = 0;
			virtual void GetDesc( UShaderReflectionTypeDesc* pOutDesc ) const = 0;
		};

		class __iberbarRHIApi__ IShaderReflectionMember abstract
		{
		public:
			virtual ~IShaderReflectionMember() {}
			virtual IShaderReflectionType* GetType() = 0;
			virtual void GetDesc( UShaderReflectionMemberDesc* pOutDesc ) const = 0;
		};

		class __iberbarRHIApi__ IShaderReflection abstract
		{
		public:
			virtual ~IShaderReflection() {}
			virtual int GetBufferCount() const = 0;
			virtual const IShaderReflectionBuffer* GetBufferByIndex( int nIndex ) const = 0;
			virtual const IShaderReflectionBuffer* GetBufferByName( const char* pstrName ) const = 0;
		};
	}
}