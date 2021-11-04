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


		class __iberbarRHIApi__ IShaderReflectionBuffer abstract
		{
		public:
			IShaderReflectionBuffer();

			FORCEINLINE uint32 GetBindPoint() const { return m_nBindPoint; }
			FORCEINLINE uint32 GetOffset() const { return m_nOffset; }
			FORCEINLINE uint32 GetSize() const { return m_nSize; }
			FORCEINLINE uint32 GetVariableCount() const { return m_nVariableCount; }
			FORCEINLINE const char* GetName() const { return m_strName.c_str(); }

			virtual const IShaderReflectionVariable* GetVariableByIndex( int nIndex ) const = 0;
			virtual const IShaderReflectionVariable* GetVariableByName( const char* pstrName ) const = 0;

		protected:
			uint32 m_nBindPoint;
			uint32 m_nOffset;
			uint32 m_nSize;
			uint32 m_nVariableCount;
			std::string m_strName;
		};


		class __iberbarRHIApi__ IShaderReflectionVariable abstract
		{
		public:
			IShaderReflectionVariable();

			FORCEINLINE uint32 GetOffsetInBuffer() const { return m_nOffsetInBuffer; }
			FORCEINLINE uint32 GetOffset() const { return m_nOffset; }
			FORCEINLINE uint32 GetSize() const { return m_nTotalSize; }
			FORCEINLINE const char* GetName() const { return m_strName.c_str(); }

			virtual const IShaderReflectionType* GetType() const = 0;

		protected:
			uint32 m_nOffsetInBuffer;
			uint32 m_nOffset;
			uint32 m_nTotalSize;
			std::string m_strName;
		};


		class __iberbarRHIApi__ IShaderReflectionBindResource
		{
		public:
			IShaderReflectionBindResource();

			FORCEINLINE uint32 GetBindSlot() const { return m_nBindSlot; }
			FORCEINLINE uint32 GetBindCount() const { return m_nBindCount; }
			FORCEINLINE const char* GetName() const { return m_strName.c_str(); }

		protected:
			UShaderVariableType m_nVarType;
			uint32 m_nBindSlot;
			uint32 m_nBindCount;
			std::string m_strName;
		};


		class __iberbarRHIApi__ IShaderReflectionType abstract
		{
		public:
			IShaderReflectionType();

			FORCEINLINE UShaderVariableType GetVariableType() const { return m_nVarType; }
			FORCEINLINE UShaderVariableClass GetVariableClass() const { return m_nVarClass; }
			FORCEINLINE uint32 GetElementCount() const { return m_nElementCount; }
			FORCEINLINE uint32 GetRowCount() const { return m_nRowCount; }
			FORCEINLINE uint32 GetColumnCount() const { return m_nColumnCount; }
			FORCEINLINE uint32 GetMemberCount() const { return m_nMemberCount; }
			FORCEINLINE const char* GetName() const { return m_strName.c_str(); }

			virtual const IShaderReflectionMember* GetMemberByIndex( int nIndex ) const = 0;
			virtual const IShaderReflectionMember* GetMemberByName( const char* pstrName ) const = 0;

		protected:
			UShaderVariableType m_nVarType;
			UShaderVariableClass m_nVarClass;
			uint32 m_nElementCount;
			uint32 m_nRowCount;
			uint32 m_nColumnCount;
			uint32 m_nMemberCount;
			std::string m_strName;
		};

		class __iberbarRHIApi__ IShaderReflectionMember abstract
		{
		public:
			IShaderReflectionMember();

			FORCEINLINE uint32 GetOffset() const { return m_nOffset; }
			FORCEINLINE const char* GetName() const { return m_strName.c_str(); }

			virtual const IShaderReflectionType* GetType() const = 0;

		protected:
			uint32 m_nOffset;
			std::string m_strName;
		};

		class __iberbarRHIApi__ IShaderReflection abstract
		{
		public:
			IShaderReflection();

			FORCEINLINE uint32 GetBufferCount() const { return m_nBufferCount; }
			FORCEINLINE uint32 GetBufferSizeTotal() const { return m_nBufferSizeTotal; }
			FORCEINLINE uint32 GetTextureCountTotal() const { return m_nTextureCountTotal; }
			FORCEINLINE uint32 GetSamplerStateCountTotal() const { return m_nSamplerStateCountTotal; }

			virtual const IShaderReflectionBuffer* GetBufferByIndex( int nIndex ) const = 0;
			virtual const IShaderReflectionBuffer* GetBufferByName( const char* pstrName ) const = 0;
			virtual const IShaderReflectionVariable* GetVariableByName( const char* pstrName ) const = 0;
			virtual const IShaderReflectionBindResource* GetTextureByName( const char* pstrName ) const = 0;
			virtual const IShaderReflectionBindResource* GetSamplerStateByName( const char* pstrName ) const = 0;

		protected:
			uint32 m_nBufferSizeTotal;
			uint32 m_nBufferCount;
			uint32 m_nTextureCountTotal;
			uint32 m_nSamplerStateCountTotal;
		};
	}
}




