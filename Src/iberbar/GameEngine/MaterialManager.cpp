
#include <iberbar/GameEngine/MaterialManager.h>
#include <iberbar/GameEngine/ShaderManager.h>
#include <iberbar/Renderer/Material.h>
#include <iberbar/Utility/JsonConvert.h>


iberbar::CResult iberbar::Game::LoadMaterialFromJson( Renderer::CMaterial** ppOutMaterial, const char* pstrJsonText )
{
	//nlohmann::json Json = nlohmann::json::parse( pstrJsonText );
	//
	//auto strShaderState = JsonGetString( Json, "ShaderState" );
	//if ( strShaderState.has_value() == false )
	//	return MakeResult( ResultCode::Bad, "" );

	//CShaderManager::GetOrCreateShader();
	//RHI::IShaderState* pShaderState = nullptr;

	//TSmartRefPtr<Renderer::CMaterial> pMaterial = TSmartRefPtr<Renderer::CMaterial>::_sNew();

	//pMaterial->SetShaderState( pShaderState );

	return MakeResult( ResultCode::Bad, "" );
}
