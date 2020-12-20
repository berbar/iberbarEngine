#pragma once

#include <iberbar/RHI/Resource.h>
#include <iberbar/Utility/Result.h>

namespace iberbar
{
	namespace RHI
	{
		class IDevice;

		class __iberbarRHIApi__ IShader
			: public IResource
		{
		public:
			IShader()
				: IResource( UResourceType::Shader )
			{
			}

		public:
			virtual CResult LoadFromSource( const char* strVertexSource, const char* strPixelSource ) = 0;
			virtual CResult LoadFromFile( const char* strVertexFile, const char* strPixelFile ) = 0;
		};
	}
}
