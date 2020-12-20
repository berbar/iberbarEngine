




function TestBindLuaFunction( a: number, b: number )
{
    print( "-------TestBindLuaFunction" );
    print( `a=${a}` );
    print( `b=${b}` );
    print( "-------TestBindLuaFunction" );
    print( "\n" );
}

function Do_1( ...args: any[] ): void
{
    for ( let i = 0; i < 100; i ++ )
    {
        print( `do_1:${i}` )
    }
}



function Main()
{
    print( "test" );

    // local success, ret = JsonDeserialize("[1,[5,6]]");
    // if ( success == false ) then
    //     print( ret )
    // else
    //     print( ret[1] + ret[2][1] * ret[2][2] );
    // end
    print( lrTest );
    lrTest.lrTest();

    let s = [ 5, 8, 955, 444 ];
    console.log( "print list" );
    for ( let i of  s )
    {
        console.log( i );
    }

    let foo = Biubiu.CFoo.new();
    let foo2 = Biubiu.CFoo.new();
    let foo_metatable = getmetatable(foo);
    let fooNumbers = CFooNumbers.new();
    foo.SetNumbers( fooNumbers );
    fooNumbers = foo.GetNumbers();
    fooNumbers.Set( 4, 7 );
    fooNumbers.BindCallback(TestBindLuaFunction);
    fooNumbers.Print();
    console.log( foo );
    print( foo.Add(5,3) );
    print( foo.Sub(5,3) );
    foo.Hello( "mary" );

    fooNumbers = null;
    collectgarbage("restart");
    print("--------------GC------------------");
    print(collectgarbage("count"));
    print(collectgarbage("collect"));
    print(collectgarbage("count"));

    let fooNumbers_2 = foo.GetNumbers();

    foo = null;
    print("--------------GC------------------");
    print(collectgarbage("count"));
    print(collectgarbage("collect"));
    print(collectgarbage("count"));
}

