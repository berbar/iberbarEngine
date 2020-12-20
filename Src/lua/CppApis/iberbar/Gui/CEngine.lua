

--- @class iberbar.Gui.CEngine
local m = {};


--- 添加UI
--- @param widget iberbar.Gui.CWidget UI主节点
--- @return void
function m:AddUI( widget ) end


--- 查找UI
--- @param id string id值
--- @return iberbar.Gui.CWidget
function m:FindUI( id ) end


--- 移除UI
--- @param id string id值
--- @return void
function m:RemoveUI( id ) end


--- 设置当前展示的UI
--- @param id string id值
function m:SetMainUI( id ) end


--- export
-- if ( iberbar.Gui == nil ) then
--     iberbar.Gui = {};
-- end
iberbar.Gui.CEngine = m;
