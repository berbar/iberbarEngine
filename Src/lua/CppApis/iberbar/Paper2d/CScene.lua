

---@class iberbar.Paper2d.CScene : iberbar.Paper2d.CNode
local m = {};


--- 获取摄像机对象
---@return iberbar.Paper2d.CCamera
function m:GetCamera() end


--- 导出CScene
if ( iberbar.Paper2d == nil ) then
    iberbar.Paper2d = {};
end
iberbar.Paper2d.CScene = m;

