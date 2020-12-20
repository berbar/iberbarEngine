

#ifndef __iberbar__Base__UnknownObject__hhh__
#define __iberbar__Base__UnknownObject__hhh__


#include <iberbar\Utility\Unknown.h>
#include <string>
#include <list>
#include <vector>

namespace iberbar
{

	struct UnknownEventParams;
	class  CUnknownEventDelegate;
	class  CUnknownObject;


	IBERBAR_UNKNOWN_PTR_DECLARE( CUnknownObject );

	// 事件参数结构体
	struct UnknownEventParams
	{
		uint32      nEventId;
		uint64      wParam;
		int64       lParam;
		void*       pData;
		std::string cParam;
	};
	typedef void ( CUnknownObject::*PUNOBJECT_EVENT_PROC )( PTR_CUnknownObject ptrSender, const UnknownEventParams& refEventParams ); // UnknownObject 事件处理回调函数





#define MAX_UNOBJECT_TAG_SIZE    256

	class CUnknownObject abstract
		: public CRef
	{
	public:
		struct _TAGNODE 
		{
			std::tstring key;
			std::tstring tag;
		};
		typedef std::list< _TAGNODE >  _TAGMAP;
	public:
		CUnknownObject( void );
		virtual ~CUnknownObject() override;

		IBERBAR_UNKNOWN_CLONE_ABSTRACT( CUnknownObject );


	public:
		void addEventProc( CUnknownObject* pEventReceiver, PUNOBJECT_EVENT_PROC pEventProc, uint32 nSpecialEventId = 0 );
		void sendEvent( const UnknownEventParams& msgParam );
		void sendEventLong( uint32 nEvent, uint64 wParam = 0, int64 lParam = 0, void* data = NULL, const char* cParam = NULL );
		void setTag( const TCHAR* tag )          { setTag( TEXT(""), tag ); }
		void setTag( const TCHAR* key, const TCHAR* tag );
		const TCHAR* getTag()                    { return getTag( TEXT("") ); }
		const TCHAR* getTag( const TCHAR* key );


	private:
		_TAGNODE* getTagNode( const TCHAR* key );


	private:
		CUnknownEventDelegate* m_EventDelegate;
		_TAGMAP m_TagsMap;
	};
}



#endif
