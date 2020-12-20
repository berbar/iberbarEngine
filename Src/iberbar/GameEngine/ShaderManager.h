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
			CResult GetOrCreateShader( const char* strName, RHI::IShader** ppOutShader );
			bool FindShader( const char* strName, RHI::IShader** ppOutShader );

		private:
			struct _ShaderNode
			{
				std::string strName;
				RHI::IShader* pShader;
			};
			void CombineShaderFilePath( const char* strRootDir, std::string& strVS, std::string& strPS );

		private:
			RHI::IDevice* m_pRHIDevice;
			RHI::UApiType m_nApiType;
			std::string m_strRootDir;
			std::vector<_ShaderNode> m_ShaderNodeList;
		};
	}
}

