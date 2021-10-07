

--- @class iberbar.Gui.CDialog : iberbar.Gui.CObject
local m = {};


--- 添加部件
--- @param widget iberbar.Gui.CWidget
--- @return void
function m:AddWidget( widget )
end


--- 查找部件
--- @param id string
--- @return iberbar.Gui.CWidget
function m:FindWidget( id )
end


--- 窗口置顶
--- @return void
function m:RequestTop() end


--- 创建窗口
--- @param style number 风格, 0=overlapped, 1=child
--- @param dlgParent iberbar.Gui.CDialog 父级窗口
--- @return iberbar.Gui.CDialog
function iberbar.Gui.CreateDialog( style, dlgParent )
end


--- 销毁窗口
--- @param dlg iberbar.Gui.CDialog
--- @return void
function iberbar.Gui.DestroyDialog( dlg )
end


--- 查找窗口
--- @param id string
--- @return iberbar.Gui.CDialog
function iberbar.Gui.GetDialog( id )
end
