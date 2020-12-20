
CColor4B = class();

function CColor4B:ctor( ... )
    local s = {...};
    local n = select("#",...);
    if n == 4 then
        self.a = s[1];
        self.r = s[2];
        self.g = s[3];
        self.b = s[4];
    elseif n == 1 then
        self.a = 0;
        self.r = 0;
        self.g = 0;
        self.b = 0;
    else
        self.a = 0;
        self.r = 0;
        self.g = 0;
        self.b = 0;
    end
end
