
#include <stdio.h>

#include <libmacro/assert.h>

#include "../string.h"


int
main( void )
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
    printf( "Property and equality assertions passed!\n" );

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
    printf( "Null termination assertions passed!\n" );

}


