#pragma once

#include <iberbar/Poster/Elements/XmlCommon.h>

namespace iberbar
{
	namespace Poster
	{
		void __iberbarExports__ XmlReadProc_ElementBase( Xml::PTR_CNodeA node, CElementBase* pElement );
		CResult __iberbarExports__ XmlReadProc_ElementLabel( UXmlReadProcContext& context );
		CResult __iberbarExports__ XmlReadProc_ElementImage( UXmlReadProcContext& context );
	}
}

