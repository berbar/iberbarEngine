#pragma once

#include <iberbar/GameEngine/Headers.h>
#include <iberbar/RHI/Shader.h>
#include <iberbar/RHI/Types.h>


namespace iberbar
{
	namespace RHI
	{
		class IDevice;
	}

	namespace Game
	{
		class __iberbarGameEngineApi__ CShaderManager
		{
		public:
			CShaderManager( RHI::IDevice* pRHIDevice );
			~CShaderManager();

		public:
			void SetRootDir( const char* strRootDir ) { m_strRootDir = strRootDir; }
			CResult GetOrCreateShader( RHI::EShaderType eShaderType, const char* strName, RHI::IShader** ppOutShader );
			bool FindShader( RHI::EShaderType eShaderType, const char* strName, RHI::IShader** ppOutShader );

		private:
			struct _ShaderNode
			{
				RHI::EShaderType eShaderType;
				RHI::IShader* pShader;
				std::string strName;
			};
			std::string CombineShaderFilePath( RHI::EShaderType eShaderType, const char* strName );

		private:
			RHI::IDevice* m_pRHIDevice;
			RHI::UApiType m_nApiType;
			std::string m_strRootDir;
			std::vector<_ShaderNode> m_ShaderNodeList;
		};
	}
}

