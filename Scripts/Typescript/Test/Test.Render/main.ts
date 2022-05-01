

function __G__TRACKBACK__( msg: object )
{
    //let errText = msg + "\n" + debug.traceback();
    if ( iberbar.Game.LogError != null )
    {
        iberbar.Game.LogError( "Lua", "" + msg );
        iberbar.Game.LogError( "Lua", debug.traceback() );
    }
    else
    {
        console.log( "" + msg );
        console.log( debug.traceback() );
    }
}


type UListBoxItem = { id: number, text: string };

class CPopo
{
    protected m_image: iberbar.Paper2d.CImage = null;

    public static sxx()
    {

    }

    public Init( scene: iberbar.Paper2d.CScene ): void
    {
        let texture = iberbar.Game.GetTexture( "Images/Popo.png" );
        this.m_image = iberbar.Paper2d.CImage.new();
        this.m_image.Init();
        this.m_image.SetContentSize( 44, 41 );
        this.m_image.SetZOrder( 100 );
        this.m_image.SetTexture( texture );
        this.m_image.SetUV( 0, 0, 1/3, 1 );
        scene.InsertChild( this.m_image );

        this.NewComponent();
    }

    protected NewComponent(): void
    {
        let componentUpdatable = iberbar.Paper2d.CComponent_Updatable.new();
        componentUpdatable.SetName( "TestUpdate" );
        componentUpdatable.BindCallback( this.Update, this );
        this.m_image.AddComponent( componentUpdatable );

        let componentKeyboard = iberbar.Paper2d.CComponent_HandleKeyboardInput.new();
        componentKeyboard.SetName( "TestKeyboard" );
        componentKeyboard.BindCallback( this.OnKeyboard, this );
        this.m_image.AddComponent( componentKeyboard );
    }

    protected Update( delta: number ): void
    {
        //iberbar.Game.LogDebug( "Lua", .format( delta ) );
    }

    protected OnKeyboard( event: iberbar.Paper2d.UComponent_HandleKeyboardInput_Event ): void
    {
        iberbar.Game.LogDebug( "Lua", string.format( "keycode=%d", event.keycode ) );
    }
}

// function ON()
// {

// }

function Main(): void
{
    let dlg = iberbar.Gui.CreateDialog();
    dlg.SetId( "MainMenu" );
    dlg.SetPosition( 0, 0 );
    dlg.SetSize( 800, 600 );

    let dlgPop = iberbar.Gui.CreateDialog();
    dlgPop.SetId( "PopMenu" );
    dlgPop.SetPosition( -50, -50 );
    dlgPop.SetSize( 400, 200 );
    dlgPop.SetAlignHorizental( iberbar.UAlignHorizontal.Center );
    dlgPop.SetAlignVertical( iberbar.UAlignVertical.Center );
    dlgPop.SetModal( false );

    let guiXmlParser = iberbar.Game.GetGuiXmlParser();
    guiXmlParser.ReadFile( "Scripts/MainMenu.xml", dlg );
    guiXmlParser.ReadFile( "Scripts/PopMenu.xml", dlgPop );

    let btn = dlg.GetWidgetRoot().FindWidget( "Btn-StartGame" );
    if ( btn != null )
    {
        btn.AddEventCallback( 0, function( widget, event, valueInt )
        {
            string.format( "widget id=%s, event=%d", widget.GetId(), event );
        });
    }


    let listbox = iberbar.Gui.CListBox.new<UListBoxItem>();
    listbox.SetDebugId( 15 );
    listbox.SetPosition( 0, 0 );
    listbox.SetSize( 200, 100 );
    listbox.SetDragStyle( iberbar.Gui.EListBoxDragStyle.DragView );
    listbox.SetSelectType( 2 );
    listbox.SetItemSize( 200, 40 );
    listbox.SetCallback_ItemElementCreate( function( element, item, index )
    {
        let elementLabel = iberbar.Gui.CElementStateLabel.new();
        elementLabel.SetId( "" + item.id );
        elementLabel.SetFont( iberbar.Game.GetFont() );
        elementLabel.SetSize( 200, 40 );
        elementLabel.SetText( item.text );
        elementLabel.SetColor( -1, 0xffffffff );
        elementLabel.SetColor( 1, 0xff0000ff );
        elementLabel.SetColor( 2, 0xffff0000 );
        elementLabel.SetColor( 3, 0xff00ff00 );
        elementLabel.SetColor( iberbar.Gui.UWidgetState.Disabled, 0xff808080 );
        element.AddElement( elementLabel );
    });
    listbox.AddItem( { id: 1, text : "gogo" } );
    listbox.AddItem( { id: 343, text : "lady" } );
    listbox.AddItem( { id: 22, text : "gaga" } );
    listbox.AddItem( { id: 22, text : "我" } );
    listbox.AddItem( { id: 22, text : "好可爱哟" } );
    listbox.SetItemEnable( 2, false );
    listbox.AddEventCallback( iberbar.Gui.UEvents.ValueChanged, function( widget, event, valueInt )
    {
        let listboxTemp = iberbar.Gui.CListBox.dynamic_cast( widget );
        iberbar.Game.LogDebug( "Lua", string.format( "valuechanged: index=%d, selected=", valueInt, listboxTemp.IsSelect( valueInt ) ) );
        let selectList = listboxTemp.GetSelectAll();
        if ( selectList != null )
        {
            let text = "[";
            for ( const selected of selectList )
            {
                text = text + selected + ",";
            }
            text = text + "]";
        }
        else
        {
            iberbar.Game.LogDebug( "Lua","[]" );
        }
    } );
    dlg.GetWidgetRoot().AddWidget( listbox );

    let find = listbox.FindItemOne( function( item )
    {
        if ( item.id == 22 )
            return true;
        return false;
    });
    let finds = listbox.FindItems( function( item )
    {
        if ( item.id == 22 )
            return true;
        return false;
    });
    let itemFind = listbox.GetItem( find );


    let scene = iberbar.Paper2d.CScene.new();
    scene.Init();
    let camera = scene.GetCamera();
    camera.SetPosition( 0, 0 )
    camera.SetOrthographicProjection( 800, 600 );
    camera.Build();

    let terrain = iberbar.Paper2d.CGridTerrain.new();
    scene.InsertChild( terrain );
    terrain.Init();
    terrain.SetZOrder( 99 );
    terrain.SetPosition( 100, 0 );
    terrain.SetAnchor( 0.5, 0.5 );
    terrain.SetContentSize( 380, 400 );
    terrain.SetGridSize( 30, 30 );
    iberbar.Game.Paper2dLoadGridTerrain( "Scripts/Terrain_1.json", terrain );


    let popo = new CPopo();
    CPopo.sxx();
    popo.Init( scene );

    iberbar.Paper2d.RunScene( scene );
}







// declare namespace iberbar.Gui
// {
//     export type UCallbackWidgetEvent2 = ( this: void, widget: iberbar.Gui.CWidget, event: number, valueInt: number ) => void;
// }


// class CFoo
// {
//     public Go( func: iberbar.Gui.UCallbackWidgetEvent ): void
//     {
    
//     }
// }

// function Main()
// {
//     // let btn: iberbar.Gui.CWidget = null;
//     // btn.AddEventCallback( 0, function()
//     // {

//     // } );

//     new CFoo().Go( function( widget, event, vint )
//     {

//     });
// }
// iberbar.Gui.CListBox.new().AddEventCallback( 0, function()
// {

// });
