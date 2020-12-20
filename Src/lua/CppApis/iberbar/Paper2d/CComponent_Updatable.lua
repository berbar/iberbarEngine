


--- 可刷新组件
---@class iberbar.Paper2d.CComponent_Updatable : iberbar.Paper2d.CComponent
local m = {};


--- new实例
--- @return iberbar.Paper2d.CComponent_Updatable
function m.new() end


--- new实例
--- @param component iberbar.Paper2d.CComponent
--- @return iberbar.Paper2d.CComponent_Updatable
function m.dynamic_cast( component ) end


--- 绑定刷新回调方法
---@param func function 函数
---@param ext any 附加参数，原样返回，可用于self传递
function m:BindCallback( func, ext ) end


--- 导出 CComponent_Updatable
if ( iberbar.Paper2d == nil ) then
    iberbar.Paper2d = {};
end
iberbar.Paper2d.CComponent_Updatable = m;

