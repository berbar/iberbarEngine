

#ifndef __iberbar_datamap_h__
#define __iberbar_datamap_h__

#include <iberbar\Base\Platform.h>
#include <iberbar\Base\data\Vector_SortDichotomy.h>
#include <string>

namespace iberbar
{
#define IBERBAR_DATAMAP_DECLARE_TYPE( type )          DataUnionType_##type
#define IBERBAR_DATAMAP_VALUE_NAME( type )    data_##type
#define IBERBAR_DATAMAP_DECLARE_VALUE( type )         type IBERBAR_DATAMAP_VALUE_NAME( type )
#define IBERBAR_DATAMAP_DECLARE_VALUE_PTR( type )     type* IBERBAR_DATAMAP_VALUE_NAME( type )



	// Old
// #define IBERBAR_DATAMAP_DECLARE_FUNC( dt )                                  \
// 	void add_data_##dt( const _Key& key, dt data, bool bIsCover )           \
// 	{                                                                       \
// 		DataUnion* lc_old = NULL;                                           \
// 		DataUnion* lc_union = new DataUnion;                                \
// 		lc_union->IBERBAR_DATAMAP_VALUE_NAME( dt ) = data;                  \
// 		lc_union->type = IBERBAR_DATAMAP_DECLARE_TYPE( dt );                \
// 		getMap()->add( key, lc_union, &lc_old, true );                      \
// 		SAFE_DELETE( lc_old );                                              \
// 	}                                                                       \
// 	bool has_data_##dt( const _Key& key ) const                             \
// 	{                                                                       \
// 		DataUnion* lc_union;                                                \
// 		if ( getMap()->find( key, &lc_union ) == true &&                    \
// 			lc_union &&                                                     \
// 			lc_union->type == IBERBAR_DATAMAP_DECLARE_TYPE( dt ) )          \
// 			return true;                                                    \
// 		return false;                                                       \
// 	}                                                                       \
// 	const dt& get_data_##dt( const _Key& key, dt def ) const                \
// 	{                                                                       \
// 		DataUnion* lc_union;                                                \
// 		if ( getMap()->find( key, &lc_union ) == true &&                    \
// 			lc_union &&                                                     \
// 			lc_union->type == IBERBAR_DATAMAP_DECLARE_TYPE( dt ) )          \
// 			return lc_union->IBERBAR_DATAMAP_VALUE_NAME( dt );              \
// 		return def;                                                         \
// 	}

#define IBERBAR_DATAMAP_DECLARE_FUNC( dt )                                  \
	void add_data_##dt( const _Key& key, dt data, bool bIsCover )           \
	{                                                                       \
		DataUnion* lc_old = NULL;                                           \
		DataUnion* lc_union = new DataUnion;                                \
		lc_union->IBERBAR_DATAMAP_VALUE_NAME( dt ) = data;                  \
		lc_union->type = IBERBAR_DATAMAP_DECLARE_TYPE( dt );                \
		getMap()->add( key, lc_union, &lc_old, true );                      \
		SAFE_DELETE( lc_old );                                              \
	}                                                                       \
	bool has_data_##dt( const _Key& key ) const                             \
	{                                                                       \
		DataUnion* lc_union;                                                \
		if ( getMap()->find( key, &lc_union ) == true &&                    \
			lc_union &&                                                     \
			lc_union->type == IBERBAR_DATAMAP_DECLARE_TYPE( dt ) )          \
			return true;                                                    \
		return false;                                                       \
	}                                                                       \
	dt get_data_##dt( const _Key& key, dt def ) const                \
	{                                                                       \
		DataUnion* lc_union;                                                \
		if ( getMap()->find( key, &lc_union ) == true &&                    \
			lc_union &&                                                     \
			lc_union->type == IBERBAR_DATAMAP_DECLARE_TYPE( dt ) )          \
			return lc_union->IBERBAR_DATAMAP_VALUE_NAME( dt );              \
		return def;                                                         \
	}


#define IBERBAR_DATAMAP_DECLARE_FUNC_STRING( dt, c )                                  \
	void add_data_##dt( const _Key& key, const c* data, bool bIsCover )       \
	{                                                                              \
		DataUnion* lc_old = NULL;                                                  \
		DataUnion* lc_union = new DataUnion;                                       \
		lc_union->IBERBAR_DATAMAP_VALUE_NAME( dt ) = new dt( data );             \
		lc_union->type = IBERBAR_DATAMAP_DECLARE_TYPE( dt );                       \
		getMap()->add( key, lc_union, &lc_old, true );                             \
		SAFE_DELETE( lc_old );                                                     \
	}                                                                              \
	const c* get_data_##dt( const _Key& key, const c* def ) const                 \
	{                                                                              \
		DataUnion* lc_union;                                                       \
		if ( getMap()->find( key, &lc_union ) == true &&                           \
			lc_union &&                                                            \
			lc_union->type == IBERBAR_DATAMAP_DECLARE_TYPE( dt ) )                 \
			return lc_union->IBERBAR_DATAMAP_VALUE_NAME( dt )->c_str();            \
		return def;                                                                \
	}

//type get_data_##type( const _Key& key, type def ) const;

//#define IBERBAR_DATAMAP_DECLARE_FUNC( type )          void aad_data_##type( const _Key& key, type data, bool bIsCover ); type get_data_##type( const _Key& key, type def ) const;
//#define IBERBAR_DATAMAP_DECLARE_FUNC_PTR( type )      void aad_data_##type( const _Key& key, const type& data, bool bIsCover ); type get_data_##type( const _Key& key, const type& def ) const;
	
	
	
	typedef void* handle;
	typedef std::string     string8;
	typedef std::wstring    string16;
	//typedef std::vector< DataUnion > DataUnionArray;


	enum DataUnionType
	{
		IBERBAR_DATAMAP_DECLARE_TYPE( null ),
		IBERBAR_DATAMAP_DECLARE_TYPE( handle ),
		IBERBAR_DATAMAP_DECLARE_TYPE( int32 ),
		IBERBAR_DATAMAP_DECLARE_TYPE( int64 ),
		IBERBAR_DATAMAP_DECLARE_TYPE( uint32 ),
		IBERBAR_DATAMAP_DECLARE_TYPE( uint64 ),
		IBERBAR_DATAMAP_DECLARE_TYPE( float ),
		IBERBAR_DATAMAP_DECLARE_TYPE( double ),
		IBERBAR_DATAMAP_DECLARE_TYPE( string8 ),
		IBERBAR_DATAMAP_DECLARE_TYPE( string16 ),
		IBERBAR_DATAMAP_DECLARE_TYPE( bool ),
		//IBERBAR_DATAMAP_DECLARE_TYPE( DataUnionArray ),
	};



	struct DataUnion
	{
		
		union
		{
			IBERBAR_DATAMAP_DECLARE_VALUE( handle );
			IBERBAR_DATAMAP_DECLARE_VALUE( int32 );
			IBERBAR_DATAMAP_DECLARE_VALUE( int64 );
			IBERBAR_DATAMAP_DECLARE_VALUE( uint32 );
			IBERBAR_DATAMAP_DECLARE_VALUE( uint64 );
			IBERBAR_DATAMAP_DECLARE_VALUE( float );
			IBERBAR_DATAMAP_DECLARE_VALUE( double );
			IBERBAR_DATAMAP_DECLARE_VALUE_PTR( string8 );
			IBERBAR_DATAMAP_DECLARE_VALUE_PTR( string16 );
			IBERBAR_DATAMAP_DECLARE_VALUE( bool );
			//IBERBAR_DATAMAP_DECLARE_VALUE_PTR( DataUnionArray );
		};
		
		DataUnionType type;

		DataUnion()
			: type( IBERBAR_DATAMAP_DECLARE_TYPE( null ) )
		{
		}

		DataUnion( const DataUnion& old )
			: type( old.type )
		{
			if ( type == IBERBAR_DATAMAP_DECLARE_TYPE( string8 ) )
			{
				IBERBAR_DATAMAP_VALUE_NAME( string8 ) = new string8( old.IBERBAR_DATAMAP_VALUE_NAME( string8 )->c_str() );
			}
			else if ( type == IBERBAR_DATAMAP_DECLARE_TYPE( string16 ) )
			{
				IBERBAR_DATAMAP_VALUE_NAME( string16 ) = new string16( old.IBERBAR_DATAMAP_VALUE_NAME( string16 )->c_str() );
			}
// 			else if ( type == IBERBAR_DATAMAP_DECLARE_TYPE( DataUnionArray ) )
// 			{
// 				IBERBAR_DATAMAP_VALUE_NAME( DataUnionArray ) = new DataUnionArray( *(old.IBERBAR_DATAMAP_VALUE_NAME( DataUnionArray ) ) );
// 			}
		};

		~DataUnion()
		{
			assert( type != IBERBAR_DATAMAP_DECLARE_TYPE( null ) );

			if ( type == IBERBAR_DATAMAP_DECLARE_TYPE( string8 ) )
			{
				delete IBERBAR_DATAMAP_VALUE_NAME( string8 );
			}
			else if ( type == IBERBAR_DATAMAP_DECLARE_TYPE( string16 ) )
			{
				delete IBERBAR_DATAMAP_VALUE_NAME( string16 );
			}
// 			else if ( type == IBERBAR_DATAMAP_DECLARE_TYPE( DataUnionArray ) )
// 			{
// 				delete IBERBAR_DATAMAP_VALUE_NAME( DataUnionArray );
// 			}
		}
	};




	template < typename _Key >
	class TDataUnionVector
	{
	public:


	public:
		TDataUnionVector( void ) {}
		TDataUnionVector( const TDataUnionVector& v );
		~TDataUnionVector();

	public:
		IBERBAR_DATAMAP_DECLARE_FUNC( handle );
		IBERBAR_DATAMAP_DECLARE_FUNC( int32 );
		IBERBAR_DATAMAP_DECLARE_FUNC( int64 );
		IBERBAR_DATAMAP_DECLARE_FUNC( uint32 );
		IBERBAR_DATAMAP_DECLARE_FUNC( uint64 );
		IBERBAR_DATAMAP_DECLARE_FUNC( float );
		IBERBAR_DATAMAP_DECLARE_FUNC( double );
		IBERBAR_DATAMAP_DECLARE_FUNC_STRING( string8, char );
		IBERBAR_DATAMAP_DECLARE_FUNC_STRING( string16, wchar_t );
		IBERBAR_DATAMAP_DECLARE_FUNC( bool );

// 		void aad_data_handle( const _Key& key, void* data, bool bIsCover ); // void*
// 		void add_data_int32( const _Key& key, int32 data, bool bIsCover );  // int32
// 		void add_data_int64( const _Key& key, int64 data, bool bIsCover );  // int64
// 		void add_data_uint32( const _Key& key, uint32 data, bool bIsCover );  // unsigned int32
// 		void add_data_uint64( const _Key& key, uint64 data, bool bIsCover );  // unsigned int64
// 		void add_data_float( const _Key& key, float data, bool bIsCover );  // float
// 		void add_data_double( const _Key& key, double data, bool bIsCover );  // double
// 		void add_data_wstring( const _Key& key, const wchar_t* data, bool bIsCover );  // string wide char
// 		void add_data_string( const _Key& key, const char* data, bool bIsCover ); // string multi byte
// 		void add_data_boolean( const _Key& key, bool data, bool bIsCover );  // boolean
// 
// 		int32          get_data_int32( const _Key& key, int32 def ) const;
// 		int64          get_data_int64( const _Key& key, int64 def ) const;
// 		uint32         get_data_uint32( const _Key& key, uint32 def ) const;
// 		uint64         get_data_uint64( const _Key& key, uint64 def ) const;
// 		void*          get_data_handle( const _Key& key, void* def ) const;
// 		float          get_data_float( const _Key& key, float def ) const;
// 		double         get_data_double( const _Key& key, double def ) const;
// 		const char*    get_data_string( const _Key& key, const char* def ) const;
// 		const wchar_t* get_data_wstring( const _Key& key, const wchar_t* def ) const;
// 		bool           get_data_boolean( const _Key& key, bool def ) const;

		TVector_SortDichotomy< _Key, DataUnion* >* getMap() { return &m_Map; }
		const TVector_SortDichotomy< _Key, DataUnion* >* getMap() const { return &m_Map; }

	private:
		TVector_SortDichotomy< _Key, DataUnion* > m_Map;
	};


	template < typename _Key >
	TDataUnionVector< _Key >::TDataUnionVector( const TDataUnionVector& old )
	{
		if ( old.m_Map.empty() == false )
		{
			DataUnion* lc_union;
			typename TVector_SortDichotomy< _Key, DataUnion* >::node_iterator lc_iter = old.m_Map.begin();
			typename TVector_SortDichotomy< _Key, DataUnion* >::node_iterator lc_end  = old.m_Map.end();
			for ( ; lc_iter != lc_end; ++ lc_iter )
			{
				this->m_Map.add( ( *lc_iter )->getKey(), ( *lc_iter )->data, NULL, true );
			}
		}
	}

	template < typename _Key >
	TDataUnionVector< _Key >::~TDataUnionVector()
	{
		if ( m_Map.empty() == false )
		{
			typename TVector_SortDichotomy< _Key, DataUnion* >::node_iterator lc_iter = m_Map.begin();
			typename TVector_SortDichotomy< _Key, DataUnion* >::node_iterator lc_end  = m_Map.end();
			for ( ; lc_iter != lc_end; ++ lc_iter )
			{
				SAFE_DELETE( (*lc_iter)->data );
			}
		}
	}
// 
// 
// 	template < typename _Key >
// 	void TDataUnionVector< _Key >::aad_data_handle( const _Key& key, void* data, bool bIsCover )
// 	{
// 		DataUnion* lc_old = NULL;
// 		DataUnion* lc_union = new DataUnion;
// 		lc_union->data_handle = data;
// 		lc_union->type = DataUnionType_handle;
// 		getMap()->add( key, lc_union, &lc_old, true );
// 		SAFE_DELETE( lc_old );
// 	}
// 
// 
// 	template < typename _Key >
// 	void TDataUnionVector< _Key >::add_data_uint32( const _Key& key, uint32 data, bool bIsCover )
// 	{
// 		DataUnion* lc_old = NULL;
// 		DataUnion* lc_union = new DataUnion;
// 		lc_union->data_uint32 = data;
// 		lc_union->type = DataUnionType_uint32;
// 		getMap()->add( key, lc_union, &lc_old, true );
// 		SAFE_DELETE( lc_old );
// 	}
// 
// 
// 	template < typename _Key >
// 	void TDataUnionVector< _Key >::add_data_int64( const _Key& key, int64 data, bool bIsCover )
// 	{
// 		DataUnion* lc_old = NULL;
// 		DataUnion* lc_union = new DataUnion;
// 		lc_union->data_int32 = data;
// 		lc_union->type = DataUnionType_int32;
// 		getMap()->add( key, lc_union, &lc_old, true );
// 		SAFE_DELETE( lc_old );
// 	}
// 
// 
// 	template < typename _Key >
// 	void TDataUnionVector< _Key >::add_data_uint64( const _Key& key, uint64 data, bool bIsCover )
// 	{
// 		DataUnion* lc_old = NULL;
// 		DataUnion* lc_union = new DataUnion;
// 		lc_union->data_uint64 = data;
// 		lc_union->type = DataUnionType_uint64;
// 		getMap()->add( key, lc_union, &lc_old, true );
// 		SAFE_DELETE( lc_old );
// 	}
// 
// 
// 	template < typename _Key >
// 	void TDataUnionVector< _Key >::add_data_int64( const _Key& key, int64 data, bool bIsCover )
// 	{
// 		DataUnion* lc_old = NULL;
// 		DataUnion* lc_union = new DataUnion;
// 		lc_union->data_int64 = data;
// 		lc_union->type = DataUnionType_int64;
// 		getMap()->add( key, lc_union, &lc_old, true );
// 		SAFE_DELETE( lc_old );
// 	}
// 
// 
// 	template < typename _Key >
// 	void TDataUnionVector< _Key >::add_data_float( const _Key& key, float data, bool bIsCover )
// 	{
// 		DataUnion* lc_old = NULL;
// 		DataUnion* lc_union = new DataUnion;
// 		lc_union->data_float = data;
// 		lc_union->type = DataUnionType_float;
// 		getMap()->add( key, lc_union, &lc_old, true );
// 		SAFE_DELETE( lc_old );
// 	}
// 
// 
// 	template < typename _Key >
// 	void TDataUnionVector< _Key >::add_data_double( const _Key& key, double data, bool bIsCover )
// 	{
// 		DataUnion* lc_old = NULL;
// 		DataUnion* lc_union = new DataUnion;
// 		lc_union->data_double = data;
// 		lc_union->type = DataUnionType_double;
// 		getMap()->add( key, lc_union, &lc_old, true );
// 		SAFE_DELETE( lc_old );
// 	}
// 
// 
// 	template < typename _Key >
// 	void TDataUnionVector< _Key >::add_data_wstring( const _Key& key, const wchar_t* data, bool bIsCover )
// 	{
// 		DataUnion* lc_old = NULL;
// 		DataUnion* lc_union = new DataUnion;
// 		lc_union->data_wstring = new std::string( data );
// 		lc_union->type = DataUnionType_string;
// 		getMap()->add( key, lc_union, &lc_old, true );
// 		SAFE_DELETE( lc_old );
// 	}
// 
// 
// 	template < typename _Key >
// 	void TDataUnionVector< _Key >::add_data_wstring( const _Key& key, const wchar_t* data, bool bIsCover )
// 	{
// 		DataUnion* lc_old = NULL;
// 		DataUnion* lc_union = new DataUnion;
// 		lc_union->data_wstring = new std::wstring( data );
// 		lc_union->type = DataUnionType_wstring;
// 		getMap()->add( key, lc_union, &lc_old, true );
// 		SAFE_DELETE( lc_old );
// 	}
// 
// 
// 	template < typename _Key >
// 	void TDataUnionVector< _Key >::add_data_boolean( const _Key& key, bool data, bool bIsCover )
// 	{
// 		DataUnion* lc_old = NULL;
// 		DataUnion* lc_union = new DataUnion;
// 		lc_union->data_boolean = data;
// 		lc_union->type = DataUnionType_boolean;
// 		getMap()->add( key, lc_union, &lc_old, true );
// 		SAFE_DELETE( lc_old );
// 	}
// 
// 
// 	template < typename _Key >
// 	void* TDataUnionVector< _Key >::get_data_handle( const _Key& key, void* def ) const
// 	{
// 		DataUnion* lc_union;
// 		if ( getMap()->find( key, &lc_union ) == true &&
// 			lc_union &&
// 			lc_union->type == DataUnionType_handle )
// 			return lc_union->data_handle;
// 
// 		return def;
// 	}
// 
// 
// 	template < typename _Key >
// 	uint64 TDataUnionVector< _Key >::get_data_uint64( const _Key& key, uint64 def ) const
// 	{
// 		DataUnion* lc_union;
// 		if ( getMap()->find( key, &lc_union ) == true &&
// 			lc_union &&
// 			lc_union->type == DataUnionType_uint64 )
// 			return lc_union->data_uint64;
// 
// 		return def;
// 	}
// 
// 
// 	template < typename _Key >
// 	int64 TDataUnionVector< _Key >::get_data_int64( const _Key& key, int64 def ) const
// 	{
// 		DataUnion* lc_union;
// 		if ( getMap()->find( key, &lc_union ) == true &&
// 			lc_union &&
// 			lc_union->type == DataUnionType_int64 )
// 			return lc_union->data_uint64;
// 
// 		return def;
// 	}
// 
// 
// 	template < typename _Key >
// 	float TDataUnionVector< _Key >::get_data_float( const _Key& key, float def ) const
// 	{
// 		DataUnion* lc_union;
// 		if ( getMap()->find( key, &lc_union ) == true &&
// 			lc_union &&
// 			lc_union->type == DataUnionType_float )
// 			return lc_union->data_float;
// 
// 		return def;
// 	}
// 
// 
// 	template < typename _Key >
// 	double TDataUnionVector< _Key >::get_data_double( const _Key& key, double def ) const
// 	{
// 		DataUnion* lc_union;
// 		if ( getMap()->find( key, &lc_union ) == true &&
// 			lc_union &&
// 			lc_union->type == DataUnionType_double )
// 			return lc_union->data_double;
// 
// 		return def;
// 	}
// 
// 
// 	template < typename _Key >
// 	const wchar_t* TDataUnionVector< _Key >::get_data_wstring( const _Key& key, const wchar_t* def ) const
// 	{
// 		DataUnion* lc_union;
// 		if ( getMap()->find( key, &lc_union ) == true &&
// 			lc_union &&
// 			lc_union->type == DataUnionType_wstring )
// 			return lc_union->data_wstring->c_str();
// 
// 		return def;
// 	}
// 
// 	template < typename _Key >
// 	bool TDataUnionVector< _Key >::get_data_boolean( const _Key& key, bool def ) const
// 	{
// 		DataUnion* lc_union;
// 		if ( getMap()->find( key, &lc_union ) == true &&
// 			lc_union &&
// 			lc_union->type == DataUnionType_boolean )
// 			return lc_union->data_boolean;
// 
// 		return def;
// 	}

}

#endif