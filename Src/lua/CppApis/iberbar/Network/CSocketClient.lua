


--- @class iberbar.IO.CSocketClient
local m = {};


--- @param func function
--- @param ext any
--- @return void
function m:Listen( func, ext ) end


--- @param data string
--- @return void
function m:Send( data ) end


--- @return void
function m:Reconnect() end


--- @return void
function m:Disconnect() end

