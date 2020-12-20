

--local CMainMenu = require( "Scripts/Game/main_menu" )
function __G__TRACKBACK__(msg) --引用通知，全局变量  跟踪绑定执行函数发生错误的信息并输出
    local errText = tostring(msg).."\n"..debug.traceback();
    if ( iberbar.Game.LogError ~= nil ) then
        iberbar.Game.LogError( "Lua", tostring(msg) );
        iberbar.Game.LogError( "Lua", debug.traceback() );
    else
        print( "LUA ERROR: "..tostring(msg) );
        print( debug.traceback() );
    end
end

-- local ssss = nil;
-- ssss:dodo();

local popo = {};
function popo:Init( scene )
    local texture = iberbar.Game.GetTexture( "Images/Popo.png" );
    self.image = iberbar.Paper2d.CImage.new();
    self.image:Init();
    self.image:SetContentSize( 44, 41 );
    self.image:SetZOrder( 100 );
    self.image:SetAnchor( 0.5, 0.5 );
    self.image:SetTexture( texture );
    self.image:SetUV( 0, 0, 1/3, 1 );
    scene:InsertChild( self.image );

    self.animList = iberbar.Game.Paper2dLoadAnimations( "Images/Popo.json" );
    local animListSize = #self.animList;
    for i = 1,animListSize do
        print(self.animList[i])
    end
    self.animList[1]:Play( true );

    self:NewComponent();
end

function popo:NewComponent()
    local componentUpdatable = iberbar.Paper2d.CComponent_Updatable.new();
    componentUpdatable:SetName( "TestUpdate" );
    componentUpdatable:BindCallback( popo.Update, self );
    self.image:AddComponent( componentUpdatable )
    componentUpdatable = nil;

    local componentKeyboard = iberbar.Paper2d.CComponent_HandleKeyboardInput.new();
    componentKeyboard:SetName( "TestKeyboard" );
    componentKeyboard:BindCallback( popo.OnKeyboard, self );
    self.image:AddComponent( componentKeyboard )
    componentKeyboard = nil;
end

function popo:Update( delta )
    local anim = self.animList[1];
    anim:Update( delta );
    local uv = anim:GetCurrentUV();
    if uv ~= nil then
        self.image:SetUV( uv.l, uv.t, uv.r, uv.b );
    end
end

function popo:OnKeyboard( evt )
    print( evt.keycode );
end




function Main()
    -- local ssss = nil;
    -- ssss:dodo();
    

    local guiXmlParser = iberbar.Game.GetGuiXmlParser();
    local dlg = iberbar.Gui.CreateDialog( 0, nil );

    dlg:SetId( "MainMenu" );
    dlg:SetPosition( 0, 0 );
    dlg:SetSize( 800, 600 );

    guiXmlParser:ReadFile( "Scripts/MainMenu.xml", dlg );
    guiXmlParser = nil;

    local btn = dlg:FindWidget( "Btn-StartGame" );
    if ( btn ~= nil ) then
        btn:AddEventCallback( 0, function( widget, event, valueInt )
            print( string.format( "widget id=%s, event=%d", widget:GetId(), event ) );
        end)
    end

    local listbox = iberbar.Gui.CListBox.new();
    listbox:SetPosition( 0, 0 );
    listbox:SetSize( 200, 100 );
    listbox:SetEnableDrag( true );
    listbox:SetSelectType( 2 );
    listbox:SetItemSize( 200, 40 );
    listbox:SetCallback_CreateItemElements( function( element, item, index )
        local elementLabel = iberbar.Gui.CElementStateLabel.new();
        elementLabel:SetId( item.id );
        elementLabel:SetFont( iberbar.Game.GetFont() );
        elementLabel:SetSize( 200, 40 );
        elementLabel:SetText( item.text );
        elementLabel:SetColor( -1, 0xffffffff );
        elementLabel:SetColor( 1, 0xff0000ff );
        elementLabel:SetColor( 2, 0xffff0000 );
        elementLabel:SetColor( 3, 0xff00ff00 );
        element:AddElement( elementLabel );
    end);
    listbox:AddItem( { id = 1, text = "gogo" } );
    listbox:AddItem( { id = 343, text = "lady" } );
    listbox:AddItem( { id = 22, text = "gaga" } );
    listbox:AddItem( { id = 22, text = "我" } );
    listbox:AddItem( { id = 22, text = "好可爱哟" } );
    listbox:AddEventCallback( iberbar.Gui.UEvents.ValueChanged, function( widget, event, valueInt )
        --print( event );
        local listboxTemp = iberbar.Gui.CListBox.dynamic_cast( widget );
        --local select = listboxTemp:GetSelectFirst();
        print( string.format( "valuechanged: index=%d, selected=", valueInt, listboxTemp:IsSelect( valueInt ) ) );
        local selectList = listboxTemp:GetSelectAll();
        if ( selectList ~= nil ) then
            local text = "[";
            for i =1, #selectList do
                text = text..selectList[i]..","
            end
            text = text.."]";
            print( text );
        else
            print( "[]" )''
        end
    end)
    dlg:AddWidget( listbox );

    local find = listbox:FindItemOne( function( item )
        if ( item.id == 22 ) then
            return true;
        end
        --return false;
    end);
    local finds = listbox:FindItems( function( item )
        if ( item.id == 22 ) then
            return true;
        end
        --return false;
    end);
    local itemFind = listbox:GetItem( find );


    local scene = iberbar.Paper2d.CScene.new();
    scene:Init();
    local camera = scene:GetCamera();
    camera:SetPosition( 0, 0 );
    camera:SetOrthographicProjection( 800, 600 );
    camera:Build();

    -- 新建地形
    local terrain = iberbar.Paper2d.CGridTerrain.new();
    scene:InsertChild( terrain );
    terrain:Init();
    terrain:SetZOrder( 99 );
    terrain:SetPosition( 100, 0 );
    terrain:SetAnchor( 0.5, 0.5 );
    terrain:SetContentSize( 380, 400 );
    terrain:SetGridSize( 30, 30 );
    iberbar.Game.Paper2dLoadGridTerrain( "Scripts/Terrain_1.json", terrain );
    
    terrain = nil;

    -- 新建泡泡动画
    popo:Init( scene );

    -- 运行场景
    iberbar.Paper2d.RunScene( scene );

    scene = nil;
    camera = nil;
end

