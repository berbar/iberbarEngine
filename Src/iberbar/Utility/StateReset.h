

#ifndef __iberbar_BaseUnity_StateReset_h__
#define __iberbar_BaseUnity_StateReset_h__

#include "iberbar\Base\Unknown.h"
#include <vector>

namespace iberbar
{
	typedef void (CUnknown::*PUnknownStateOnLost)();
	typedef bool (CUnknown::*PUnknownStateOnReset)();


	class CStateResetQue 
	{
	public:
		struct StateResetNode 
		{
			PTR_CUnknown         pObject;
			PUnknownStateOnLost  pOnLost;
			PUnknownStateOnReset pOnReset;
		};


	public:
		void onLost();
		bool onReset();

		void add( PTR_CUnknown pObject, PUnknownStateOnLost pLost, PUnknownStateOnReset pReset );
		void remove( PTR_CUnknown pObject );
		void removeAll();


	private:
		int find( PTR_CUnknown pObject );


	private:
		std::vector< StateResetNode > m_Que;
	};
}

#endif
