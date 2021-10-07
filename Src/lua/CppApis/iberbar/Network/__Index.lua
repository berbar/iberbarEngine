

iberbar.IO = {};



--- @param url string
--- @param protocol any 协议
--- @return iberbar.IO.CSocketClient
function iberbar.IO.CreateSocketClient( url, protocol ) end


--- @param type number 协议，1=包头，2=包头和包尾
--- @return any
function iberbar.IO.NewSocketProtocol( type, ... ) end