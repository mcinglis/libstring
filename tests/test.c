
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

    printf( "All assertions passed!\n" );
}


