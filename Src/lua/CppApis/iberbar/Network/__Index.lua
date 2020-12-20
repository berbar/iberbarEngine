

iberbar.Network = {};



--- @param url string
--- @param protocol any 协议
--- @return iberbar.Network.CSocketClient
function iberbar.Network.CreateSocketClient( url, protocol ) end


--- @param type number 协议，1=包头，2=包头和包尾
--- @return any
function iberbar.Network.NewSocketProtocol( type, ... ) end