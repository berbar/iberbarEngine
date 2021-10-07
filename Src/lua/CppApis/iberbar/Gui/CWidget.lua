



---@class iberbar.Gui.CWidget : iberbar.Gui.CObject
local m1;


---@return iberbar.Gui.CWidget
function m1.new() end


--- @param id string
--- @return iberbar.Gui.CRenderElement
function m1:FindElement( id ) end


--- 设置事件回调
--- @param event number 事件id，0=所有事件
--- @param func function
--- @param ext any
--- @return void
function m1:AddEventCallback( event, func, ext )
end


--- 获取焦点
--- @return void
function m1:RequestFocus()
end


--- @return iberbar.Gui.CWidget
function m1:Clone() end



iberbar.Gui.CWidget = m1;

