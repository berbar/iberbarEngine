#pragma once

#include <iberbar/GameEngine/Headers.h>
#include <iberbar/RHI/Types.h>


namespace iberbar
{
	namespace RHI
	{
		class IDevice;
		class IShaderProgram;
	}

	namespace Game
	{
		class __iberbarGameEngineApi__ CShaderLoader
		{
		public:
			CShaderLoader( RHI::IDevice* pRHIDevice );

		public:
			void SetRootDir( const char* strRootDir ) { m_strRootDir = strRootDir; }
			CResult LoadShaderProgram( const char* pstrName, RHI::IShaderProgram** ppOutShaderProgram );

		private:
			std::string CombineShaderFilePath( RHI::EShaderType eShaderType, const char* strName );

		protected:
			RHI::IDevice* m_pRHIDevice;
			RHI::UApiType m_nApiType;
			std::string m_strRootDir;
		};


		class __iberbarGameEngineApi__ CShaderManager
		{
		public:
			CShaderManager();
			~CShaderManager();

		public:
			void AddShaderProgram( const char* pstrName, RHI::IShaderProgram* pShaderProgram );
			bool GetShaderProgram( const char* pstrName, RHI::IShaderProgram** ppOutShaderProgram );

		private:
			struct _Node
			{
				RHI::IShaderProgram* pShaderProgram;
				std::string strName;
			};

		private:
			std::vector<_Node> m_ShaderProgramNodeList;
		};
	}
}

