#pragma once


#include <iberbar/GameEngine/Headers.h>
#include <iberbar/Utility/Result.h>

namespace iberbar
{
	namespace Renderer
	{
		class CMaterial;
	}

	namespace Game
	{
		CResult LoadMaterialFromJson( Renderer::CMaterial** ppOutMaterial, const char* pstrJsonText );
	}
}
