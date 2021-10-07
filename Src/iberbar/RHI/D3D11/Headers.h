#pragma once

#include <d3d11.h>
#include <d3d11_1.h>
#include <dxgi.h>
#include <d3dcompiler.h>
#include <iberbar/RHI/D3DCommon/Macros.h>
#include <iberbar/Utility/Result.h>


#define HR( exp, error ) if ( FAILED( hResult = ( exp ) ) ) return MakeResult( ResultCode::Bad, error );
#define HR( exp, error, ... ) if ( FAILED( hResult = ( exp ) ) ) return MakeResult( ResultCode::Bad, error, __VA_ARGS__ );
