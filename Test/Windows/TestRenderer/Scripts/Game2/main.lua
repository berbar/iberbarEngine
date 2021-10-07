--[[ Generated with https://github.com/TypeScriptToLua/TypeScriptToLua ]]
require("lualib_bundle");
function __G__TRACKBACK__(self, msg)
    if iberbar.Game.LogError ~= nil then
        iberbar.Game.LogError(
            "Lua",
            "" .. tostring(msg)
        )
        iberbar.Game.LogError(
            "Lua",
            debug.traceback()
        )
    else
        print(
            "" .. tostring(msg)
        )
        print(
            debug.traceback()
        )
    end
end
CPopo = __TS__Class()
CPopo.name = "CPopo"
function CPopo.prototype.____constructor(self)
    self.m_image = nil
end
function CPopo.sxx(self)
end
function CPopo.prototype.Init(self, scene)
    local texture = iberbar.Game.GetTexture("Images/Popo.png")
    self.m_image = iberbar.Paper2d.CImage:new()
    self.m_image:Init()
    self.m_image:SetContentSize(44, 41)
    self.m_image:SetZOrder(100)
    self.m_image:SetTexture(texture)
    self.m_image:SetUV(0, 0, 1 / 3, 1)
    scene:InsertChild(self.m_image)
    self:NewComponent()
end
function CPopo.prototype.NewComponent(self)
    local componentUpdatable = iberbar.Paper2d.CComponent_Updatable:new()
    componentUpdatable:SetName("TestUpdate")
    componentUpdatable:BindCallback(self.Update, self)
    self.m_image:AddComponent(componentUpdatable)
    local componentKeyboard = iberbar.Paper2d.CComponent_HandleKeyboardInput:new()
    componentKeyboard:SetName("TestKeyboard")
    componentKeyboard:BindCallback(self.OnKeyboard, self)
    self.m_image:AddComponent(componentKeyboard)
end
function CPopo.prototype.Update(self, delta)
end
function CPopo.prototype.OnKeyboard(self, event)
    iberbar.Game.LogDebug(
        "Lua",
        string.format("keycode=%d", event.keycode)
    )
end
function Main(self)
    local dlg = iberbar.Gui.CreateDialog()
    dlg:SetId("MainMenu")
    dlg:SetPosition(0, 0)
    dlg:SetSize(800, 600)
    local dlgPop = iberbar.Gui.CreateDialog()
    dlgPop:SetId("PopMenu")
    dlgPop:SetPosition(-50, -50)
    dlgPop:SetSize(400, 200)
    dlgPop:SetAlignHorizental(iberbar.UAlignHorizontal.Center)
    dlgPop:SetAlignVertical(iberbar.UAlignVertical.Center)
    dlgPop:SetModal(false)
    local guiXmlParser = iberbar.Game.GetGuiXmlParser()
    guiXmlParser:ReadFile("Scripts/MainMenu.xml", dlg)
    guiXmlParser:ReadFile("Scripts/PopMenu.xml", dlgPop)
    local btn = dlg:GetWidgetRoot():FindWidget("Btn-StartGame")
    if btn ~= nil then
        btn:AddEventCallback(
            0,
            function(widget, event, valueInt)
                string.format(
                    "widget id=%s, event=%d",
                    widget:GetId(),
                    event
                )
            end
        )
    end
    local listbox = iberbar.Gui.CListBox:new()
    listbox:SetDebugId(15)
    listbox:SetPosition(0, 0)
    listbox:SetSize(200, 100)
    listbox:SetDragStyle(iberbar.Gui.EListBoxDragStyle.DragView)
    listbox:SetSelectType(2)
    listbox:SetItemSize(200, 40)
    listbox:SetCallback_ItemElementCreate(
        function(element, item, index)
            local elementLabel = iberbar.Gui.CElementStateLabel:new()
            elementLabel:SetId(
                "" .. tostring(item.id)
            )
            elementLabel:SetFont(
                iberbar.Game.GetFont()
            )
            elementLabel:SetSize(200, 40)
            elementLabel:SetText(item.text)
            elementLabel:SetColor(-1, 4294967295)
            elementLabel:SetColor(1, 4278190335)
            elementLabel:SetColor(2, 4294901760)
            elementLabel:SetColor(3, 4278255360)
            elementLabel:SetColor(iberbar.Gui.UWidgetState.Disabled, 4286611584)
            element:AddElement(elementLabel)
        end
    )
    listbox:AddItem({id = 1, text = "gogo"})
    listbox:AddItem({id = 343, text = "lady"})
    listbox:AddItem({id = 22, text = "gaga"})
    listbox:AddItem({id = 22, text = "我"})
    listbox:AddItem({id = 22, text = "好可爱哟"})
    listbox:SetItemEnable(2, false)
    listbox:AddEventCallback(
        iberbar.Gui.UEvents.ValueChanged,
        function(widget, event, valueInt)
            local listboxTemp = iberbar.Gui.CListBox:dynamic_cast(widget)
            iberbar.Game.LogDebug(
                "Lua",
                string.format(
                    "valuechanged: index=%d, selected=",
                    valueInt,
                    listboxTemp:IsSelect(valueInt)
                )
            )
            local selectList = listboxTemp:GetSelectAll()
            if selectList ~= nil then
                local text = "["
                for ____, selected in ipairs(selectList) do
                    text = (tostring(text) .. tostring(selected)) .. ","
                end
                text = tostring(text) .. "]"
            else
                iberbar.Game.LogDebug("Lua", "[]")
            end
        end
    )
    dlg:GetWidgetRoot():AddWidget(listbox)
    local find = listbox:FindItemOne(
        function(item)
            if item.id == 22 then
                return true
            end
            return false
        end
    )
    local finds = listbox:FindItems(
        function(item)
            if item.id == 22 then
                return true
            end
            return false
        end
    )
    local itemFind = listbox:GetItem(find)
    local scene = iberbar.Paper2d.CScene:new()
    scene:Init()
    local camera = scene:GetCamera()
    camera:SetPosition(0, 0)
    camera:SetOrthographicProjection(800, 600)
    camera:Build()
    local terrain = iberbar.Paper2d.CGridTerrain:new()
    scene:InsertChild(terrain)
    terrain:Init()
    terrain:SetZOrder(99)
    terrain:SetPosition(100, 0)
    terrain:SetAnchor(0.5, 0.5)
    terrain:SetContentSize(380, 400)
    terrain:SetGridSize(30, 30)
    iberbar.Game.Paper2dLoadGridTerrain("Scripts/Terrain_1.json", terrain)
    local popo = __TS__New(CPopo)
    CPopo:sxx()
    popo:Init(scene)
    iberbar.Paper2d.RunScene(scene)
end
