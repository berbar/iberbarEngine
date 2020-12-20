#pragma once

#include <iberbar/Utility/Platform.h>

namespace iberbar
{
	class __iberbarUtilityApi__ IClonable
	{
	public:
		virtual IClonable* Clone() const = 0;

		virtual ~IClonable() {}
	};

	template < typename T >
	inline T* CloneImplements_Default( const T* src )
	{
		T* instance = new T( *src );
		instance->OnCloned( src );
		return instance;
	}

//#if defined(__GNUC__) && ((__GNUC__ >= 5) || ((__GNUG__ == 4) && (__GNUC_MINOR__ >= 4))) \
//    || (defined(__clang__) && (__clang_major__ >= 3)) || (_MSC_VER >= 1800)
//#define IBERBAR_DISALLOW_COPY_AND_ASSIGN(TypeName) \
//    TypeName(const TypeName &) = delete; \
//    TypeName &operator =(const TypeName &) = delete;
//#else
//#define CC_DISALLOW_COPY_AND_ASSIGN(TypeName) \
//    TypeName(const TypeName &); \
//    TypeName &operator =(const TypeName &);
//#endif
//#define iberbarDisallowCopyAndAssign
}
