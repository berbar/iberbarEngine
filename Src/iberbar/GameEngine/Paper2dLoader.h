#pragma once


#include <iberbar/GameEngine/Headers.h>
#include <iberbar/Utility/Result.h>


namespace iberbar
{
	namespace Paper2d
	{
		class CAnimationController;
		class CGridTerrain;
	}


	namespace Game
	{
		class __iberbarGameEngineApi__ CPaper2dLoader
		{
		public:
			CPaper2dLoader();
			~CPaper2dLoader();

		public:
			CResult LoadAnimations( const char* strFile, std::vector<Paper2d::CAnimationController*>& outAnimList );
			CResult LoadGridTerrain( const char* strFile, Paper2d::CGridTerrain* pTerrain );

		private:
			static CPaper2dLoader* sm_pSharedInstance;
		public:
			static CPaper2dLoader* GetShared() { return sm_pSharedInstance; }
		};
	}
}
