



---@class iberbar.Gui.CWidget : iberbar.Gui.CObject
local m1;


---@return iberbar.Gui.CWidget
function m1.new() end


--- 插入子部件
--- @param widget iberbar.Gui.CWidget
function m1:AddChild( widget ) end


--- 查找子部件
--- @param id string
--- @return iberbar.Gui.CWidget
function m1:FindChild( id ) end


--- @param id string
--- @return iberbar.Gui.CRenderElement
function m1:FindElement( id ) end


--- 设置事件回调
--- @param func function
--- @param ext any
--- @return void
function m1:SetEventProc( func, ext ) end


--- @return iberbar.Gui.CWidget
function m1:Clone() end



iberbar.Gui.CWidget = m1;

