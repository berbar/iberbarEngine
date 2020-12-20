
#pragma once

#include <iberbar/Utility/Result.h>
#include <iberbar/Utility/Unknown.h>


namespace iberbar
{
	namespace Xml
	{
		template < typename Ch >
		class TDocument;

		template < typename Ch >
		class TNode;

		template < typename Ch >
		class TNodeList;

		typedef TDocument<char> CDocumentA;
		typedef TDocument<wchar_t> CDocumentW;

		typedef TNode<char> CNodeA;
		typedef TNode<wchar_t> CNodeW;

		typedef TNodeList<char> CNodeListA;
		typedef TNodeList<wchar_t> CNodeListW;

		IBERBAR_UNKNOWN_PTR_DECLARE( CDocumentA );
		IBERBAR_UNKNOWN_PTR_DECLARE( CNodeA );
		IBERBAR_UNKNOWN_PTR_DECLARE( CNodeListA );

		IBERBAR_UNKNOWN_PTR_DECLARE( CDocumentW );
		IBERBAR_UNKNOWN_PTR_DECLARE( CNodeW );
		IBERBAR_UNKNOWN_PTR_DECLARE( CNodeListW );

		template < typename Ch >
		class TDocument abstract
			: public CRef
		{
		public:
			TDocument() : CRef(), m_hasLoaded( false ) {}

		public:
			virtual CResult Create( const Ch* rootnode ) = 0;
			virtual CResult LoadFromFile( const Ch* filename ) = 0;
			virtual CResult Load( const Ch* source ) = 0;
			virtual CResult Save( const Ch* filename ) = 0;
			virtual void GetRoot( TNode<Ch>** ppRootNode ) = 0;

			bool HasLoaded() { return m_hasLoaded; }

		protected:
			bool m_hasLoaded;
		};

		template < typename Ch >
		class TNode abstract
			: public CRef
		{
		public:
			TNode( void ) : CRef() {}

			virtual void SelectSingleNode( const Ch* tagName, TNode<Ch>** ppNode ) = 0;
			virtual void SelectNodes( const Ch* tagName, TNodeList<Ch>** ppNodeList ) = 0;

			virtual void AppendChild( const Ch* tagName, TNode<Ch>** ppNode ) = 0;
			virtual void DeleteChild( TNode<Ch>* pNode ) = 0;

			virtual void SetAttribute( const Ch* attrName, const Ch* attrValue ) = 0;
			virtual const Ch* GetAttribute( const Ch* attrName ) = 0;

			virtual void SetValueText( const Ch* value ) = 0;
			virtual const Ch* GetValueText() = 0;

			virtual const Ch* NameText() = 0;
		};

		template < typename Ch >
		class TNodeList abstract
			: public CRef
		{

		public:
			TNodeList( void ) : CRef() {}

		public:
			virtual void GetNodeAt( int index, TNode<Ch>** ppNode ) = 0;
			virtual int  GetNodeCount() = 0;
		};


		template class __iberbarUtilityApi__ TDocument<char>;
		template class __iberbarUtilityApi__ TDocument<wchar_t>;
		template class __iberbarUtilityApi__ TNode<char>;
		template class __iberbarUtilityApi__ TNode<wchar_t>;
		template class __iberbarUtilityApi__ TNodeList<char>;
		template class __iberbarUtilityApi__ TNodeList<wchar_t>;


		int32 GetAttributeInt32( CNodeA* pNode, const char* strAttrName, int32 nDef );
		int64 GetAttributeInt64( CNodeA* pNode, const char* strAttrName, int64 nDef );
		uint32 GetAttributeUInt32( CNodeA* pNode, const char* strAttrName, uint32 nDef );
		uint64 GetAttributeUInt64( CNodeA* pNode, const char* strAttrName, uint64 nDef );
		bool GetAttributeBoolean( CNodeA* pNode, const char* strAttrName, bool bDef, const char* strTrue, const char* strFalse );
	}
}


inline int32 iberbar::Xml::GetAttributeInt32( CNodeA* pNode, const char* strAttrName, int32 nDef )
{
	const char* strAttrValue = pNode->GetAttribute( strAttrName );
	if ( strAttrValue == nullptr || strAttrValue[ 0 ] == 0 )
		return nDef;
	if ( (strAttrValue[ 0 ] == '0' && strAttrValue[ 1 ] == 'x') ||
		(strAttrValue[ 1 ] == '0' && strAttrValue[ 2 ] == 'x') )
		return strtol( strAttrValue, nullptr, 16 );
	return strtol( strAttrValue, nullptr, 10 );
}

inline int64 iberbar::Xml::GetAttributeInt64( CNodeA* pNode, const char* strAttrName, int64 nDef )
{
	const char* strAttrValue = pNode->GetAttribute( strAttrName );
	if ( strAttrValue == nullptr || strAttrValue[ 0 ] == 0 )
		return nDef;
	if ( (strAttrValue[ 0 ] == '0' && strAttrValue[ 1 ] == 'x') ||
		(strAttrValue[ 1 ] == '0' && strAttrValue[ 2 ] == 'x') )
		return strtoll( strAttrValue, nullptr, 16 );
	return strtoll( strAttrValue, nullptr, 10 );
}

inline uint32 iberbar::Xml::GetAttributeUInt32( CNodeA* pNode, const char* strAttrName, uint32 nDef )
{
	const char* strAttrValue = pNode->GetAttribute( strAttrName );
	if ( strAttrValue == nullptr || strAttrValue[ 0 ] == 0 )
		return nDef;
	if ( strAttrValue[ 0 ] == '0' && strAttrValue[ 1 ] == 'x' )
		return strtoul( strAttrValue, nullptr, 16 );
	return strtoul( strAttrValue, nullptr, 10 );
}

inline uint64 iberbar::Xml::GetAttributeUInt64( CNodeA* pNode, const char* strAttrName, uint64 nDef )
{
	const char* strAttrValue = pNode->GetAttribute( strAttrName );
	if ( strAttrValue == nullptr || strAttrValue[ 0 ] == 0 )
		return nDef;
	if ( strAttrValue[ 0 ] == '0' && strAttrValue[ 1 ] == 'x' )
		return strtoull( strAttrValue, nullptr, 16 );
	return strtoull( strAttrValue, nullptr, 10 );
}

inline bool iberbar::Xml::GetAttributeBoolean( CNodeA* pNode, const char* strAttrName, bool bDef, const char* strTrue, const char* strFalse )
{
	const char* strAttrValue = pNode->GetAttribute( strAttrName );
	if ( strAttrValue == nullptr || strAttrValue[ 0 ] == 0 )
		return bDef;
	if ( strcmp( strAttrValue, strTrue ) == 0 )
		return true;
	if ( strcmp( strAttrValue, strFalse ) == 0 )
		return false;
	return bDef;
}


