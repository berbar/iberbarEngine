




#pragma once

#include <iberbar/Utility/Platform.h>
#include <optional>
#include <unordered_map>

namespace iberbar
{
	enum class UVariantType {
		VT_Null,
		VT_Boolean,
		VT_Int,
		VT_UInt,
		VT_String,
		VT_Float
	};

	class __iberbarUtilityApi__ CVariant final
	{
	private:
		union {
			bool _v_boolean;
			std::string* _v_string;
			int64 _v_int64;
			uint64 _v_uint64;
			float _v_float;
		} m_v;
		UVariantType m_t;

	public:
		CVariant();
		CVariant( bool value );
		CVariant( const char* value );
		CVariant( int32 value );
		CVariant( int64 value );
		CVariant( uint32 value );
		CVariant( uint64 value );
		CVariant( float value );
		CVariant( const CVariant& Other );
		~CVariant();

	public:
		UVariantType GetValueType() const;
		const char* ToString() const;
		std::optional<bool> ToBoolean() const;
		std::optional<int64> ToInt() const;
		std::optional<uint64> ToUInt() const;
		std::optional<float> ToFloat() const;
		CVariant& operator=( const CVariant& Other );
	};


	class __iberbarUtilityApi__ CVariantDictionary final
	{
	public:
		CVariantDictionary();
		CVariantDictionary( const CVariantDictionary& Other );

	public:
		const CVariant* Get( const char* strName ) const;
		void SetString( const char* strName, const char* value );
		void SetBoolean( const char* strName, bool value );
		void SetInt( const char* strName, int64 value );
		void SetUInt( const char* strName, uint64 value );
		void SetFloat( const char* strName, float value );

		const std::unordered_map<std::string, CVariant>& GetMap() const { return m_AttrsMap; }

		CVariantDictionary& operator=( const CVariantDictionary& Other );

		void Merge( const CVariantDictionary& Other, int nMergeFlag = 0 );

	protected:
		std::unordered_map<std::string, CVariant> m_AttrsMap;
	};
}


inline iberbar::CVariant::CVariant()
	: m_v()
	, m_t( UVariantType::VT_Null )
{
}


inline iberbar::CVariant::CVariant( bool value )
	: m_v()
	, m_t( UVariantType::VT_Boolean )
{
	m_v._v_boolean = value;
}


inline iberbar::CVariant::CVariant( const char* value )
	: m_v()
	, m_t( UVariantType::VT_String )
{
	if ( value == nullptr )
	{
		m_t = UVariantType::VT_Null;
	}
	else
	{
		m_v._v_string = new std::string( value );
	}
}


inline iberbar::CVariant::CVariant( int32 value )
	: m_v()
	, m_t( UVariantType::VT_Int )
{
	m_v._v_int64 = (int64)value;
}

inline iberbar::CVariant::CVariant( int64 value )
	: m_v()
	, m_t( UVariantType::VT_Int )
{
	m_v._v_int64 = value;
}


inline iberbar::CVariant::CVariant( uint32 value )
	: m_v()
	, m_t( UVariantType::VT_UInt )
{
	m_v._v_uint64 = (uint32)value;
}


inline iberbar::CVariant::CVariant( uint64 value )
	: m_v()
	, m_t( UVariantType::VT_UInt )
{
	m_v._v_uint64 = value;
}


inline iberbar::CVariant::CVariant( float value )
	: m_v()
	, m_t( UVariantType::VT_Float )
{
	m_v._v_float = value;
}


inline iberbar::CVariant::CVariant( const CVariant& Other )
	: m_v()
	, m_t( Other.m_t )
{
	if ( m_t == UVariantType::VT_String )
	{
		m_v._v_string = new std::string( Other.m_v._v_string->c_str() );
	}
	else
	{
		memcpy_s( &m_v, sizeof( m_v ), &Other.m_v, sizeof( Other.m_v ) );
	}
}


inline iberbar::CVariant::~CVariant()
{
	if ( m_t == UVariantType::VT_String )
	{
		SAFE_DELETE( m_v._v_string );
	}
}


inline iberbar::UVariantType iberbar::CVariant::GetValueType() const
{
	return m_t;
}


inline const char* iberbar::CVariant::ToString() const
{
	if ( m_t == UVariantType::VT_String )
		return m_v._v_string->c_str();
	return nullptr;
}


inline std::optional<bool> iberbar::CVariant::ToBoolean() const
{
	if ( m_t == UVariantType::VT_Boolean )
		return m_v._v_boolean;
	return std::optional<bool>();
}


inline std::optional<int64> iberbar::CVariant::ToInt() const
{
	if ( m_t == UVariantType::VT_Int )
		return m_v._v_int64;
	return std::optional<int64>();
}


inline std::optional<uint64> iberbar::CVariant::ToUInt() const
{
	if ( m_t == UVariantType::VT_UInt )
		return m_v._v_uint64;
	return std::optional<uint64>();
}


inline std::optional<float> iberbar::CVariant::ToFloat() const
{
	if ( m_t == UVariantType::VT_Boolean )
		return m_v._v_float;
	return std::optional<float>();
}


inline iberbar::CVariant& iberbar::CVariant::operator=( const CVariant& Other )
{
	if ( m_t == UVariantType::VT_String && Other.m_t == UVariantType::VT_String )
	{
		int nStrLen = strlen( Other.m_v._v_string->c_str() );
		m_v._v_string->resize( nStrLen + 1 );
		memcpy_s( &m_v._v_string->front(), nStrLen + 1, Other.m_v._v_string->c_str(), nStrLen );
		( &m_v._v_string->front() )[nStrLen] = 0;
	}
	else
	{
		if ( m_t == UVariantType::VT_String )
		{
			SAFE_DELETE( m_v._v_string );
		}

		m_t = Other.m_t;

		if ( m_t == UVariantType::VT_String )
		{
			m_v._v_string = new std::string( Other.m_v._v_string->c_str() );
		}
		else
		{
			memcpy_s( &m_v, sizeof( m_v ), &Other.m_v, sizeof( Other.m_v ) );
		}
	}

	return *this;
}









inline const iberbar::CVariant* iberbar::CVariantDictionary::Get( const char* strName ) const
{
	auto iter = m_AttrsMap.find( strName );
	if ( iter == m_AttrsMap.end() )
		return nullptr;
	return &iter->second;
}


inline void iberbar::CVariantDictionary::SetString( const char* strName, const char* value )
{
	if ( Get( strName ) != nullptr )
	{
		m_AttrsMap[ strName ] = CVariant( value );
	}
	else
	{
		m_AttrsMap.insert( std::make_pair( strName, CVariant( value ) ) );
	}
}


inline void iberbar::CVariantDictionary::SetBoolean( const char* strName, bool value )
{
	if ( Get( strName ) != nullptr )
	{
		m_AttrsMap[ strName ] = CVariant( value );
	}
	else
	{
		m_AttrsMap.insert( std::make_pair( strName, CVariant( value ) ) );
	}
}


inline void iberbar::CVariantDictionary::SetInt( const char* strName, int64 value )
{
	if ( Get( strName ) != nullptr )
	{
		m_AttrsMap[ strName ] = CVariant( value );
	}
	else
	{
		m_AttrsMap.insert( std::make_pair( strName, CVariant( value ) ) );
	}
}


inline void iberbar::CVariantDictionary::SetUInt( const char* strName, uint64 value )
{
	if ( Get( strName ) != nullptr )
	{
		m_AttrsMap[ strName ] = CVariant( value );
	}
	else
	{
		m_AttrsMap.insert( std::make_pair( strName, CVariant( value ) ) );
	}
}


inline void iberbar::CVariantDictionary::SetFloat( const char* strName, float value )
{
	if ( Get( strName ) != nullptr )
	{
		m_AttrsMap[ strName ] = CVariant( value );
	}
	else
	{
		m_AttrsMap.insert( std::make_pair( strName, CVariant( value ) ) );
	}
}







