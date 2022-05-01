
// iberbar.IO = {};



// --- @param url string
// --- @param protocol any 协议
// --- @return iberbar.IO.CSocketClient
// function iberbar.IO.CreateSocketClient( url, protocol ) end


// --- @param type number 协议，1=包头，2=包头和包尾
// --- @return any
// function iberbar.IO.NewSocketProtocol( type, ... ) end

// --- @class iberbar.IO.CSocketClient
// local m = {};


// --- @param func function
// --- @param ext any
// --- @return void
// function m:Listen( func, ext ) end


// --- @param data string
// --- @return void
// function m:Send( data ) end


// --- @return void
// function m:Reconnect() end


// --- @return void
// function m:Disconnect() end



declare namespace iberbar.IO
{
    class CSocketClient
    {
        protected constructor();
        public Listen( func: any ): void;
        public Listen( func: any, ext: any ): void;
        public Send( bytes: string ): void;
        public Reconnect(): void;
        public Disconnect(): void;
    }

    function CreateSocketClient( this: void, url: string, head: string, tail: string ): CSocketClient;
}