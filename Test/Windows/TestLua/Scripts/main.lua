print("test")

local success, ret = JsonDeserialize("[1,[5,6]]");
if ( success == false ) then
    print( ret )
else
    print( ret[1] + ret[2][1] * ret[2][2] );
end
print( lrTest );
lrTest.lrTest();


function TestBindLuaFunction( a, b )
    print( "-------TestBindLuaFunction" );
    print( "a="..a );
    print( "b="..b );
    print( "-------TestBindLuaFunction" );
    print( "\n" );
end


local foo = Biubiu.CFoo.new();
local foo2 = Biubiu.CFoo.new();
local foo_metatable = getmetatable(foo);
local fooNumbers= CFooNumbers.new();
foo:SetNumbers(fooNumbers);
fooNumbers = foo:GetNumbers();
fooNumbers:Set( 4, 7 );
--fooNumbers:BindCallback(TestBindLuaFunction);
fooNumbers:Print();
print( foo );
print( foo:Add(5,3) );
print( foo:Sub(5,3) );
foo:Hello( "mary" );

fooNumbers = nil;
collectgarbage("restart");
print("--------------GC------------------");
print(collectgarbage("count"));
print(collectgarbage("collect"));
print(collectgarbage("count"));

local fooNumbers_2 = foo:GetNumbers();

foo = nil;
print("--------------GC------------------");
print(collectgarbage("count"));
print(collectgarbage("collect"));
print(collectgarbage("count"));




function Do_1(...)
    print( "do_1" );
    for i=0, 100 do
        print( "do_1:"..i );
        coroutine.yield();
    end
end

function Do_2(...)
    print( "do_2" );
    for i=0, 100 do
        print( "do_2:"..i );
        coroutine.yield();
    end
end


--BindLuaFunction( TestBindLuaFunction );

--print( lrTest.lrTest[1] );

--TestArgumentsNotMatch( 5 );