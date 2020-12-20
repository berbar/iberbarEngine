// function __G__TRACKBACK__(msg) --引用通知，全局变量  跟踪绑定执行函数发生错误的信息并输出
// local errText = tostring(msg).."\n"..debug.traceback();
// if ( iberbar.Game.LogError ~= nil ) then
//     iberbar.Game.LogError( "Lua", tostring(msg) );
//     iberbar.Game.LogError( "Lua", debug.traceback() );
// else
//     print( "LUA ERROR: "..tostring(msg) );
//     print( debug.traceback() );
// end
// end
function __G__TRACKBACK__(msg) {
    var errText = msg + "\n" + debug.traceback();
    if (iberbar.Game.LogError != null) {
        iberbar.Game.LogError("Lua", tostring(msg));
        iberbar.Game.LogError("Lua", debug.traceback());
    }
    else {
        console.log("" + msg);
        console.log(debug.traceback());
    }
}
var CPopo = /** @class */ (function () {
    function CPopo() {
        this.m_image = null;
    }
    CPopo.sxx = function () {
    };
    CPopo.prototype.Init = function (scene) {
        var texture = iberbar.Game.GetTexture("Images/Popo.png");
        this.m_image = iberbar.Paper2d.CImage["new"]();
        this.m_image.Init();
        this.m_image.SetContentSize(44, 41);
        this.m_image.SetZOrder(100);
        this.m_image.SetTexture(texture);
        this.m_image.SetUV(0, 0, 1 / 3, 1);
        scene.InsertChild(this.m_image);
        this.NewComponent();
    };
    CPopo.prototype.NewComponent = function () {
        var componentUpdatable = iberbar.Paper2d.CComponent_Updatable["new"]();
        componentUpdatable.SetName("TestUpdate");
        componentUpdatable.BindCallback(this.Update, this);
        this.m_image.AddComponent(componentUpdatable);
        var componentKeyboard = iberbar.Paper2d.CComponent_HandleKeyboardInput["new"]();
        componentKeyboard.SetName("TestKeyboard");
        componentKeyboard.BindCallback(this.OnKeyboard, this);
        this.m_image.AddComponent(componentKeyboard);
    };
    CPopo.prototype.Update = function (delta) {
        //iberbar.Game.LogDebug( "Lua", "update delta=%".format( delta ) );
    };
    CPopo.prototype.OnKeyboard = function (event) {
        iberbar.Game.LogDebug("Lua", "keycode=%d".format(event.keycode));
    };
    return CPopo;
}());
function Main() {
    var dlg = iberbar.Gui.CreateDialog(0);
    dlg.SetId("MainMenu");
    dlg.SetPosition(0, 0);
    dlg.SetSize(800, 600);
    var guiXmlParser = iberbar.Game.GetGuiXmlParser();
    guiXmlParser.ReadFile("Scripts/MainMenu.xml", null);
    var btn = dlg.FindWidget("Btn-StartGame");
    if (btn != null) {
        btn.AddEventCallback(0, function (widget, event, valueInt) {
            "widget id=%s, event=%d".format(widget.GetId(), event);
        });
    }
    var listbox = iberbar.Gui.CListBox["new"]();
    listbox.SetPosition(0, 0);
    listbox.SetSize(200, 100);
    listbox.SetEnableDrag(true);
    listbox.SetSelectType(2);
    listbox.SetItemSize(200, 40);
    listbox.SetCallback_CreateItemElements(function (element, item, index) {
        var elementLabel = iberbar.Gui.CElementStateLabel["new"]();
        elementLabel.SetId("" + item.id);
        elementLabel.SetFont(iberbar.Game.GetFont());
        elementLabel.SetSize(200, 40);
        elementLabel.SetText(item.text);
        elementLabel.SetColor(-1, 0xffffffff);
        elementLabel.SetColor(1, 0xff0000ff);
        elementLabel.SetColor(2, 0xffff0000);
        elementLabel.SetColor(3, 0xff00ff00);
        element.AddElement(elementLabel);
    });
    listbox.AddItem({ id: 1, text: "gogo" });
    listbox.AddItem({ id: 343, text: "lady" });
    listbox.AddItem({ id: 22, text: "gaga" });
    listbox.AddItem({ id: 22, text: "我" });
    listbox.AddItem({ id: 22, text: "好可爱哟" });
    listbox.AddEventCallback(iberbar.Gui.UEvents.ValueChanged, function (widget, event, valueInt) {
        var listboxTemp = iberbar.Gui.CListBox.dynamic_cast(widget);
        iberbar.Game.LogDebug("Lua", "valuechanged: index=%d, selected=".format(valueInt, listboxTemp.IsSelect(valueInt)));
        var selectList = listboxTemp.GetSelectAll();
        if (selectList != null) {
            var text = "[";
            for (var _i = 0, selectList_1 = selectList; _i < selectList_1.length; _i++) {
                var selected = selectList_1[_i];
                text = text + selected + ",";
            }
            text = text + "]";
        }
        else {
            iberbar.Game.LogDebug("Lua", "[]");
        }
    });
    dlg.AddWidget(listbox);
    var find = listbox.FindItemOne(function (item) {
        if (item.id == 22)
            return true;
        return false;
    });
    var finds = listbox.FindItems(function (item) {
        if (item.id == 22)
            return true;
        return false;
    });
    var itemFind = listbox.GetItem(find);
    var scene = iberbar.Paper2d.CScene["new"]();
    scene.Init();
    var camera = scene.GetCamera();
    camera.SetPosition(0, 0);
    camera.SetOrthographicProjection(800, 600);
    camera.Build();
    var terrain = iberbar.Paper2d.CGridTerrain["new"]();
    scene.InsertChild(terrain);
    terrain.Init();
    terrain.SetZOrder(99);
    terrain.SetPosition(100, 0);
    terrain.SetAnchor(0.5, 0.5);
    terrain.SetContentSize(380, 400);
    terrain.SetGridSize(30, 30);
    iberbar.Game.Paper2dLoadGridTerrain("Scripts/Terrain_1.json", terrain);
    var popo = new CPopo();
    CPopo.sxx();
    popo.Init(scene);
    iberbar.Paper2d.RunScene(scene);
}
