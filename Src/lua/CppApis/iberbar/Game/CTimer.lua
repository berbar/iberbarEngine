iberbar.Game = iberbar.Game or {};

--- @class iberbar.Game.CTimer
local m = {};


--- 开始计时
--- @param timeout number 时间，单位秒
--- @param loop boolean 是否重复执行
--- @return void
function m:Start( timeout, loop ) end


--- @return void
function m:Stop() end


--- @return void
function m:Resume() end


--- @return void
function m:Pause() end


--- 绑定执行回调
--- @param func function
--- @param ext any
--- @return void
function m:SetCallbackExecute( func, ext ) end


--- 设置id
--- @param id string
--- @return void
function m:SetId( id ) end


--- 获取id
--- @return string
function m:GetId() end



--- 创建新的计时器
--- @return iberbar.Game.CTimer
function iberbar.Game.CreateTimer() end


--- 销毁计时器
--- @param timer iberbar.Game.CTimer
--- @return void
function iberbar.Game.DestroyTimer( timer ) end


--- 查找计时器
--- @param id string
--- @return iberbar.Game.CTimer
function iberbar.Game.FindTimer( id ) end

