--[[ Generated with https://github.com/TypeScriptToLua/TypeScriptToLua ]]
function TestBindLuaFunction(self, a, b)
    print(_G, "-------TestBindLuaFunction")
    print(
        _G,
        "a=" .. tostring(a)
    )
    print(
        _G,
        "b=" .. tostring(b)
    )
    print(_G, "-------TestBindLuaFunction")
    print(_G, "\n")
end
function Do_1(self, ...)
    do
        local i = 0
        while i < 100 do
            print(
                _G,
                "do_1:" .. tostring(i)
            )
            i = i + 1
        end
    end
end
function Main(self)
    print(_G, "test")
    print(_G, lrTest)
    lrTest:lrTest()
    local s = {5, 8, 955, 444}
    print("print list")
    for ____, i in ipairs(s) do
        print(i)
    end
    local foo = Biubiu.CFoo:new()
    local foo2 = Biubiu.CFoo:new()
    local foo_metatable = getmetatable(_G, foo)
    local fooNumbers = CFooNumbers:new()
    foo:SetNumbers(fooNumbers)
    fooNumbers = foo:GetNumbers()
    fooNumbers:Set(4, 7)
    fooNumbers:BindCallback(TestBindLuaFunction)
    fooNumbers:Print()
    print(foo)
    print(
        _G,
        foo:Add(5, 3)
    )
    print(
        _G,
        foo:Sub(5, 3)
    )
    foo:Hello("mary")
    fooNumbers = nil
    collectgarbage(_G, "restart")
    print(_G, "--------------GC------------------")
    print(
        _G,
        collectgarbage(_G, "count")
    )
    print(
        _G,
        collectgarbage(_G, "collect")
    )
    print(
        _G,
        collectgarbage(_G, "count")
    )
    local fooNumbers_2 = foo:GetNumbers()
    foo = nil
    print(_G, "--------------GC------------------")
    print(
        _G,
        collectgarbage(_G, "count")
    )
    print(
        _G,
        collectgarbage(_G, "collect")
    )
    print(
        _G,
        collectgarbage(_G, "count")
    )
end
