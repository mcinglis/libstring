
#include <stdio.h>

#include <libmacro/assert.h>

#include "../string.h"


static
void
test_properties( void )
{
    StringC const hello = STRINGC( "hello" );
    StringC const world = STRINGC0( "world" );
    StringM const mhello = STRINGM( "hello" );
    StringM const mworld = STRINGM0( "world" );
    ASSERT( hello.length == 5,
            world.length == 6,
            !stringc__equal( hello, world ),
            stringc__equal( hello, mhello ),
            stringc__equal( world, mworld ) );
}


static
void
test_nullterm( void )
{
    StringM foo = stringm__copy( "foo" );
    ASSERT( foo.length == 3,
            foo.e[ 0 ] == 'f', foo.e[ 1 ] == 'o', foo.e[ 2 ] == 'o',
            stringm__last_isnt_null( foo ) );
    stringm__nullterm( &foo );
    ASSERT( foo.length == 4,
            stringm__equal( foo, ( StringC ) STRINGC0( "foo" ) ),
            stringm__last_is_null( foo ) );
    stringm__nullterm( &foo );
    ASSERT( foo.length == 4,
            stringm__equal( foo, ( StringC ) STRINGC0( "foo" ) ),
            stringm__last_is_null( foo ) );
}


static
void
test_replace( void )
{
    StringC const a = STRINGC( "hopeful FOLLY" );
    StringM al = stringc__replaced( a, 'L', 'X' );
    ASSERT( stringm__equal( al, "hopeful FOXXY" ) );
    StringM ali = stringc__replaced_i( a, 'l', 'Z' );
    ASSERT( stringm__equal( ali, "hopefuZ FOZZY" ) );
    stringm__free( &al );
    stringm__free( &ali );

    StringM const b = STRINGM( "truthful RUTH" );
    stringm__replace( b, 'u', 'x' );
    ASSERT( stringm__equal( b, "trxthfxl RUTH" ) );
    stringm__replace_i( b, 'H', 'z' );
    ASSERT( stringm__equal( b, "trxtzfxl RUTz" ) );
}


int
main( void )
{
    puts( "Running tests..." );
    test_properties();
    puts( "  property tests passed" );
    test_nullterm();
    puts( "  nullterm tests passed" );
    test_replace();
    puts( "  replace tests passed" );
    puts( "All tests passed!" );

}


