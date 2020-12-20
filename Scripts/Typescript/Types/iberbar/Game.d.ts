
declare namespace iberbar.Game
{
    function GetGuiXmlParser( this: void ): iberbar.Gui.CXmlParser;

    function GetFont( this: void ): any;

    function GetTexture( this: void,  name: string ): iberbar.Rhi.CTexture;

    function Paper2dLoadGridTerrain( this: void, name: string, terrain: Paper2d.CGridTerrain ): void;

    function LogTrace( this: void,  tag: string, text: string ): void;
    function LogDebug( this: void,  tag: string, text: string ): void;
    function LogInfo( this: void,  tag: string, text: string ): void;
    function LogWarn( this: void,  tag: string, text: string ): void;
    function LogError( this: void,  tag: string, text: string ): void;
    function LogCritical( this: void,  tag: string, text: string ): void;
    function LogFatal( this: void,  tag: string, text: string ): void;
}