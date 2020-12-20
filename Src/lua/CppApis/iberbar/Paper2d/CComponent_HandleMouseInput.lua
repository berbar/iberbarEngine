


--- 鼠标输入组件
---@class iberbar.Paper2d.CComponent_HandleMouseInput : iberbar.Paper2d.CComponent
local m = {};


--- 绑定鼠标输入回调方法
---@param func function 函数
---@param ext any 附加参数，原样返回，可用于self传递
function m:BindCallback( func, ext ) end


--- 导出
if ( iberbar.Paper2d == nil ) then
    iberbar.Paper2d = {};
end
iberbar.Paper2d.CComponent_HandleMouseInput = m;

