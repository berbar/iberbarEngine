
#include <iberbar/Javascript/JsModuleLoader.h>
#include <iberbar/Utility/FileHelper.h>
#include <iberbar/Utility/Paths.h>


bool iberbar::iJavascript::CDefaultModuleLoader::Search(
	const std::string& RequiringDir, const std::string& ModuleName,
	std::string& OutPath, std::string& OutAbsolutePath )
{
	if ( SearchModuleInDir( RequiringDir, ModuleName, OutPath, OutAbsolutePath ) )
		return true;

	return false;
}

bool iberbar::iJavascript::CDefaultModuleLoader::Load( const std::string& Path, std::string& OutData )
{
	CFileHelper FileHelper;
	if ( FileHelper.OpenFileA( Path.c_str(), "r" ) == false )
		return false;
	OutData = FileHelper.ReadAsText();
	return true;
}

bool iberbar::iJavascript::CDefaultModuleLoader::SearchModuleInDir(
	const std::string& Dir, const std::string& RequiredModule,
	std::string& OutPath, std::string& OutAbsolutePath )
{

	if ( Paths::GetExtensionA( RequiredModule ) == "" )
	{
		return SearchModuleWithExtInDir( Dir, RequiredModule + ".js", OutPath, OutAbsolutePath )
			|| SearchModuleWithExtInDir( Dir, RequiredModule + "/index.js", OutPath, OutAbsolutePath )
			|| SearchModuleWithExtInDir( Dir, RequiredModule + "/package.json", OutPath, OutAbsolutePath );
	}
	else
	{
		return SearchModuleWithExtInDir( Dir, RequiredModule, OutPath, OutAbsolutePath );
	}
}

bool iberbar::iJavascript::CDefaultModuleLoader::SearchModuleWithExtInDir(
	const std::string& Dir, const std::string& RequiredModule,
	std::string& OutPath, std::string& OutAbsolutePath )
{
	if ( CheckExists( Paths::CombineA( Dir, RequiredModule ), OutPath, OutAbsolutePath ) )
		return true;

	if ( RequiredModule.rfind( "node_modules" ) == std::string::npos &&
		CheckExists( Paths::CombineA( Dir, "/node_modules/" + RequiredModule ), OutPath, OutAbsolutePath ) )
		return true;

	return false;
}

bool iberbar::iJavascript::CDefaultModuleLoader::CheckExists( const std::string& InPath, std::string& OutPath, std::string& OutAbsolutePath )
{
	std::string NormalizePath = InPath;
	if ( FileApi::IsExistFileA( NormalizePath.c_str() ) )
	{
		OutPath = NormalizePath;
		return true;
	}

	return false;
}