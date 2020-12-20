#pragma once


#ifndef D3D_SAFE_ADDREF
#define D3D_SAFE_ADDREF( p ) if ( (p) != nullptr ) { (p)->AddRef(); }
#endif

#ifndef D3D_SAFE_RELEASE
#define D3D_SAFE_RELEASE( p ) if ( (p) != nullptr ) { (p)->Release(); (p) = nullptr; }
#endif

#ifndef __iberbarD3DApi__
#if defined ( __iberbarD3DApiExport__ )
#define __iberbarD3DApi__ __declspec(dllexport)
#else
#define __iberbarD3DApi__ __declspec(dllimport)
#endif
#endif
