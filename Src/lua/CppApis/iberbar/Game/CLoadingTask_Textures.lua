iberbar.Game = iberbar.Game or {};

--- 批量纹理加载任务
--- @class iberbar.Game.CLoadingTask_Textures : iberbar.Game.CLoadingTask
local m = {};


--- @return iberbar.Game.CLoadingTask_Textures
function m.new() end


--- 设置每加载一张纹理后线程休眠时间，单位毫秒
--- @param milliseconds number
--- @return void
function m:SetSleepFor( milliseconds ) end


--- 添加待加载的纹理
--- @param texName string
--- @return void
function m:AddTexture( texName ) end


--- 设置纹理加载回调，加载完成一张纹理调用一次
--- @param func function
--- @param ext any
--- @return void
function m:SetCallbackOnStep( func, ext ) end


--- 设置任务结束回调
--- @param func function
--- @param ext any
--- @return void
function m:SetCallbackOnFinal( func, ext ) end


--- export
iberbar.Game.CLoadingTask_Textures = m;
