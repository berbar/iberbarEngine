#pragma once

#include <iberbar/RHI/Headers.h>
#include <iberbar/Utility/Result.h>


namespace iberbar
{
	namespace RHI
	{
		enum class UResourceType
		{
			Unknown,
			Texture,
			VertexBuffer,
			IndexBuffer,
			Shader,
			VertexDeclaration,
			Font,
		};

		class __iberbarRHIApi__ IResource
			: public CRef
		{
		public:
			IResource( UResourceType nResourceType );
			virtual ~IResource();

		public:
			inline UResourceType GetResourceType() const { return m_nResourceType; }

		public:
			virtual void OnLost() {}
			virtual CResult OnReset() { return CResult(); }

		protected:
			UResourceType m_nResourceType;
		};
	}
}
