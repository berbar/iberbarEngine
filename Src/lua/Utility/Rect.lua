

-- CRect = { l = 0, t = 0, r = 0, b = 0 };
-- CRect.__index = CRect;
-- function CRect:new(...)
--     local s = {...};
--     local n = select("#",...);
--     if ( n == 4 ) then
--         return setmetatable({
--             l = s[1],
--             t = s[2],
--             r = s[3],
--             b = s[4]
--         }, { __index = CRect } );
--     elseif n == 2 then
--         return setmetatable({
--             l = s[1].x,
--             t = s[1].y,
--             r = s[1].x + s[2].w,
--             b = s[1].y + s[2].h
--         }, { __index = CRect } );
--     end
--     return setmetatable({}, { __index = CRect } );
-- end
CRect = class();
function CRect:ctor( ... )
    local s = {...};
    local n = select("#",...);
    if ( n == 4 ) then
        self.l = s[1];
        self.t = s[2];
        self.r = s[3];
        self.b = s[4];
    elseif n == 2 then
        self.l = s[1].x;
        self.t = s[1].y;
        self.r = s[1].x + s[2].w;
        self.b = s[1].y + s[2].h;
    else
        self.l = 0;
        self.t = 0;
        self.r = 0;
        self.b = 0;
    end
end
function CRect:Width()
    return self.r - self.l;
end
function CRect:Height()
    return self.b - self.t;
end
function CRect:CenterX()
    return ( self.l + self.r ) / 2;
end
function CRect:CenterY()
    return ( self.t + self.b ) / 2;
end
function CRect:Size()
    return CSize.new( self:Width(), self:Height() );
end

