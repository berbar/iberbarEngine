#pragma once


#include <iberbar/Paper2d/Component.h>
#include <iberbar/Utility/Variant.h>
#include <unordered_map>

namespace iberbar
{
	namespace Paper2d
	{
		// Ä¬ÈÏÃû³Æ _DefaultCustomAttributes
		class __iberbarPaper2dApis__ CComponent_CustomAttributes
			: public CComponent
		{
		public:
			CComponent_CustomAttributes();
			virtual ~CComponent_CustomAttributes();

		public:
			const CVariant* GetAttribute( const char* strName ) const;
			bool AddAttributeString( const char* strName, const char* value );
			bool AddAttributeBoolean( const char* strName, bool value );
			bool AddAttributeInt( const char* strName, int64 value );
			bool AddAttributeUInt( const char* strName, uint64 value );
			bool AddAttributeFloat( const char* strName, float value );

		private:
			std::unordered_map<std::string, CVariant*> m_AttrsMap;

		public:
			static const char sm_DefaultName[];
		};
	}
}



inline const iberbar::CVariant* iberbar::Paper2d::CComponent_CustomAttributes::GetAttribute( const char* strName ) const
{
	auto iter = m_AttrsMap.find( strName );
	if ( iter == m_AttrsMap.end() )
		return nullptr;
	return iter->second;
}


inline bool iberbar::Paper2d::CComponent_CustomAttributes::AddAttributeString( const char* strName, const char* value )
{
	if ( GetAttribute( strName ) != nullptr )
		return false;
	m_AttrsMap.insert( std::make_pair( strName, new CVariant( value ) ) );
	return true;
}


inline bool iberbar::Paper2d::CComponent_CustomAttributes::AddAttributeBoolean( const char* strName, bool value )
{
	if ( GetAttribute( strName ) != nullptr )
		return false;
	m_AttrsMap.insert( std::make_pair( strName, new CVariant( value ) ) );
	return true;
}


inline bool iberbar::Paper2d::CComponent_CustomAttributes::AddAttributeInt( const char* strName, int64 value )
{
	if ( GetAttribute( strName ) != nullptr )
		return false;
	m_AttrsMap.insert( std::make_pair( strName, new CVariant( value ) ) );
	return true;
}


inline bool iberbar::Paper2d::CComponent_CustomAttributes::AddAttributeUInt( const char* strName, uint64 value )
{
	if ( GetAttribute( strName ) != nullptr )
		return false;
	m_AttrsMap.insert( std::make_pair( strName, new CVariant( value ) ) );
	return true;
}


inline bool iberbar::Paper2d::CComponent_CustomAttributes::AddAttributeFloat( const char* strName, float value )
{
	if ( GetAttribute( strName ) != nullptr )
		return false;
	m_AttrsMap.insert( std::make_pair( strName, new CVariant( value ) ) );
	return true;
}
