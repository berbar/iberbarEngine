
#pragma once

#include <iberbar\Utility\Platform.h>



#ifndef UNKNOWN_SAFE_RELEASE_NULL
#define UNKNOWN_SAFE_RELEASE_NULL(p)         { if((p)!=nullptr) { (p)->Release(); (p)=nullptr; } }
#endif
#ifndef UNKNOWN_SAFE_ADDREF
#define UNKNOWN_SAFE_ADDREF(p)               { if((p)!=nullptr) { (p)->AddRef(); } }
#endif
/*#endif*/


#ifndef IBERBAR_UNKNOWN_DECLARE_CLASS
//#define IBERBAR_UNKNOWN_PTR_DECLARE( type )      typedef TSmartRefPtr< type >  type##Ptr; typedef TSmartRefPtr< type > PTR_##type;
#define IBERBAR_UNKNOWN_PTR_DECLARE( type )       typedef TSmartRefPtr< type > PTR_##type;
#endif

namespace iberbar
{
	class CRef;



	template < typename T >
	class TSmartRefPtr
	{
#ifdef _DEBUG
	public:
		class TNoAddRefReleaseOnUnknownPtr
			: public T
		{
		private:
			uint32_t AddRef() { return 0; }
			uint32_t Release() { return 0; }
			//CUnknown* clone() const OVERRIDE { return NULL; }
		};
#endif
	public:

#ifdef WIN32
		typename typedef TSmartRefPtr< T > _Myt;
		typename typedef T                 _InterfaceT;
#else
		typedef TSmartRefPtr< T > _Myt;
		typedef T                 _InterfaceT;
#endif


	public:
		TSmartRefPtr( void ) throw()
			: m_ptr( NULL )
		{
		}

		TSmartRefPtr( const _Myt& smartPtr ) throw()
			: m_ptr( smartPtr.m_ptr )
		{
			if ( m_ptr )
				m_ptr->AddRef();
		}

		TSmartRefPtr( _InterfaceT* ptr ) throw()
			: m_ptr( ptr )
		{
			if ( m_ptr )
				m_ptr->AddRef();
		}

		~TSmartRefPtr() throw()
		{
			if ( m_ptr )
				m_ptr->Release();
		}


	public:
		// no add ref
		void attach( _InterfaceT* ptr ) throw( )
		{
			if ( m_ptr )
			{
				uint32 ref = m_ptr->Release();
				//( ref );
				// Attaching to the same object only works if duplicate references are being coalesced.  Otherwise
				// re-attaching will cause the pointer to be released and may cause a crash on a subsequent dereference.
				assert( ref != 0 || ptr != m_ptr );
			}
			m_ptr = ptr;
		}

		// no release
		_InterfaceT* detach()
		{
			_InterfaceT* pt = m_ptr;
			m_ptr = NULL;
			return pt;
		}


	public:
		operator _InterfaceT*( ) const throw()
		{
			return m_ptr;
		}

	
#ifdef _DEBUG
		TNoAddRefReleaseOnUnknownPtr* operator->( ) throw()
		{
			assert( m_ptr );
			return (TNoAddRefReleaseOnUnknownPtr*)m_ptr;
		}

		const TNoAddRefReleaseOnUnknownPtr* operator->( ) const throw()
		{
			assert( m_ptr );
			return (TNoAddRefReleaseOnUnknownPtr*)m_ptr;
		}
#else
		T* operator->( ) throw()
		{
			assert( m_ptr );
			return m_ptr;
		}

		const T* operator->( ) const throw()
		{
			assert( m_ptr );
			return m_ptr;
		}
#endif

		_InterfaceT** operator&() throw()
		{
			if ( m_ptr )
				m_ptr->Release();
			m_ptr = NULL;
			return &m_ptr;
		}

		_Myt& operator=( const _Myt& SmartPtr ) throw()
		{
			if ( m_ptr )
				m_ptr->Release();
			m_ptr = SmartPtr.m_ptr;
			if ( m_ptr )
				m_ptr->AddRef();
			return *this;
		}

		bool operator!() throw()
		{
			if ( m_ptr )
				return false;
			return true;
		}

		bool operator==( const T* ptr ) const
		{
			return m_ptr == ptr;
		}

		bool operator==( const _Myt& SmartPtr ) const
		{
			return SmartPtr.m_ptr == m_ptr;
		}

		bool operator!=( const T* ptr ) const
		{
			return m_ptr != ptr;
		}

		bool operator!=( const _Myt& SmartPtr ) const
		{
			return SmartPtr.m_ptr != m_ptr;
		}

		bool equal( _InterfaceT* p )
		{
			if ( m_ptr == p )
				return true;
			return false;
		}


	public:
		template< typename... Ts >
		static _Myt _sNew( Ts&&... xs ) throw()
		{
			TSmartRefPtr lc_instance;
			lc_instance.attach( new T( std::forward<Ts>( xs )... ) );
			return lc_instance;
		}

		static _Myt _sClone( const T* pSrc )
		{
			if ( pSrc == NULL )
				return _Myt( NULL );
			_Myt lc_clone;
			lc_clone.attach( (T*)pSrc->Clone() );
			return lc_clone;
		}


	private:
		_InterfaceT* m_ptr;
	};


	template < typename T >
	inline TSmartRefPtr< T > tCreateUnknownSingleton()
	{
		return T::CreateSharedInstance();
	}


	template < typename T >
	inline TSmartRefPtr< T > tGetUnknownSingleton()
	{
		return T::GetSharedInstance();
	}

	template < typename T >
	inline void tReleaseUnknownSingleton()
	{
		T::ReleaseSharedInstance();
	}




	class __iberbarUtilityApi__ CRef abstract
	{
	public:
		CRef( void );
		explicit CRef( const CRef& unknown );
		virtual ~CRef();


	public:
		uint32 AddRef();
		uint32 Release();
		uint32 Refcount() const { return m_nRef; }

	private:
		CRef& operator=( const CRef& SrcObject ) { return *this; }


	private:
		uint32 m_nRef;
	};



	template < typename T >
	void SafeReleaseStdVector( std::vector<T*>& data )
	{
		auto iter = data.begin();
		auto end = data.end();
		for ( ; iter != end; iter++ )
		{
			UNKNOWN_SAFE_RELEASE_NULL( *iter );
		}
		data.clear();
	}
}