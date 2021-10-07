
#include <iberbar/Utility/Variant.h>



iberbar::CVariantDictionary::CVariantDictionary()
	: m_AttrsMap()
{

}


iberbar::CVariantDictionary::CVariantDictionary( const CVariantDictionary& Other )
	: m_AttrsMap()
{
	for ( std::pair<std::string, CVariant> iter : Other.m_AttrsMap )
	{
		m_AttrsMap.insert( iter );
	}
}


iberbar::CVariantDictionary& iberbar::CVariantDictionary::operator=( const CVariantDictionary& Other )
{
	m_AttrsMap.clear();

	std::unordered_map<std::string, CVariant>::const_iterator iter = Other.m_AttrsMap.begin();
	std::unordered_map<std::string, CVariant>::const_iterator end = Other.m_AttrsMap.end();
	std::unordered_map<std::string, CVariant>::iterator iterFind;
	for ( ; iter != end; iter ++ )
	{
		m_AttrsMap.insert( std::make_pair( iter->first, iter->second ) );
	}
	return *this;
}


void iberbar::CVariantDictionary::Merge( const CVariantDictionary& Other, int nFlag )
{
	std::unordered_map<std::string, CVariant>::const_iterator iter = Other.m_AttrsMap.begin();
	std::unordered_map<std::string, CVariant>::const_iterator end = Other.m_AttrsMap.end();
	std::unordered_map<std::string, CVariant>::iterator iterFind;
	for ( ; iter != end; iter++ )
	{
		iterFind = m_AttrsMap.find( iter->first );
		if ( iterFind == m_AttrsMap.end() )
		{
			m_AttrsMap.insert( std::make_pair( iter->first, iter->second ) );
		}
		else
		{
			iterFind->second = iter->second;
		}
	}
}
