

--- 关键任务，本身不做任何加载处理，只是作为任务列表中的一个关键节点
--- @class iberbar.Game.CLoadingTask_KeyTask : iberbar.Game.CLoadingTask
local m = {};


--- @return iberbar.Game.CLoadingTask_KeyTask
function m.new() end


--- @param func function
--- @param ext any
--- @return void
function m:SetCallback( func, ext ) end


--- export
iberbar.Game.CLoadingTask_KeyTask = m;
