


---@class iberbar.Paper2d.CCamera
local m = {};


--- 设置摄像机的位置
--- @param x number
--- @param y number
--- @return void
function m:SetPosition( x, y ) end


---设置摄像机的视口尺寸
---@param w number
---@param h number
---@return void
function m:SetOrthographicProjection( w, h ) end


--- 刷新矩阵
---@return void
function m:Build() end


--- 导出
if ( iberbar.Paper2d == nil ) then
    iberbar.Paper2d = {};
end
iberbar.Paper2d.CCamera = m;
