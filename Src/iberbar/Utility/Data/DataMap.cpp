

// ≤‚ ‘
#ifdef _DEBUG

#include "iberbar/Base/data/DataMap.h"

using namespace iberbar;

TDataUnionVector< int > g_TDataUnionVector;

void TDataUnionVector_cpp_Test()
{
	g_TDataUnionVector.add_data_handle(  DataUnionType::DataUnionType_handle, NULL, true );
	g_TDataUnionVector.add_data_int32(   DataUnionType::DataUnionType_int32, 0, true );
	g_TDataUnionVector.add_data_int64(   DataUnionType::DataUnionType_int64, 0, true );
	g_TDataUnionVector.add_data_uint32(  DataUnionType::DataUnionType_uint32, 0, true );
	g_TDataUnionVector.add_data_uint64(  DataUnionType::DataUnionType_uint64, 0, true );
	g_TDataUnionVector.add_data_float(   DataUnionType::DataUnionType_float, 0.0f, true );
	g_TDataUnionVector.add_data_double(  DataUnionType::DataUnionType_double, 0.0, true );

	g_TDataUnionVector.get_data_string8( 0, "" );
	g_TDataUnionVector.has_data_bool( 0 );
}

#endif