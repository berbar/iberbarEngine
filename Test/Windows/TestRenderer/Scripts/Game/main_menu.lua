


local CMainMenu = {};
function CMainMenu:New()
end
function CMainMenu:Initial()
    local guiEngine = iberbar.Game.GetGuiEngine();
    local guiXmlParser = iberbar.Game.GetGuiXmlParser();
    self.Container = iberbar.Gui.CContainer:new();

    self.Container:SetEngine( guiEngine );
    self.Container:SetId( "MainMenu" );
    self.Container:SetPosition( 0, 0 );
    self.Container:SetSize( 800, 600 );

    guiXmlParser:PushWidget( self.Container );
    guiXmlParser:ReadFile( "Scripts/MainMenu.xml" );
    guiXmlParser:PopWidget();

    guiEngine:AddUI( self.Container );
    guiEngine:SetMainUI( "MainMenu" );

    guiEngine = nil;
    guiXmlParser = nil;
end

return CMainMenu
