
// Copyright 2015  Malcolm Inglis <http://minglis.id.au>
//
// This file is part of Libstring.
//
// Libstring is free software: you can redistribute it and/or modify it under
// the terms of the GNU Affero General Public License as published by the
// Free Software Foundation, either version 3 of the License, or (at your
// option) any later version.
//
// Libstring is distributed in the hope that it will be useful, but WITHOUT ANY
// WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
// FOR A PARTICULAR PURPOSE. See the GNU Affero General Public License for
// more details.
//
// You should have received a copy of the GNU Affero General Public License
// along with Libstring. If not, see <https://gnu.org/licenses/>.


#include "string.h"

#include <ctype.h>
#include <errno.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>     // strlen

#include <libmacro/assert.h>    // ASSERT
#include <libmacro/logic.h>     // IMPLIES
#include <libmacro/minmax.h>    // MIN, MAX

#include <libbase/size.h>       // size__*
#include <libarray/array-char.h>
#include <libvec/vec-char.h>


static
size_t
strlen_null(
        char const * const str )
{
    return ( str == NULL ) ? 0 : strlen( str );
}


static
void
vasprintf_err(
        char * * const strp,
        char const * const format,
        va_list ap )
{
    ASSERT( strp != NULL, format != NULL );

    int const r = vasprintf( strp, format, ap );
    if ( r < 0 && !errno ) {
        errno = EIO;
    }
}


static
bool
char_equal(
        char const x,
        char const y )
{
    return x == y;
}


static
bool
char_equal_i(
        char const x,
        char const y )
{
    return tolower( x ) == tolower( y );
}




///////////////////////////////////
/// STRINGC FUNCTIONS
///////////////////////////////////


bool
stringc__is_valid(
        StringC const s )
{
    return ALL( STRINGC_INVARIANTS( s ) );
}


StringC
stringc__new(
        char const * const str,
        size_t const length )
{
    ASSERT( IMPLIES( str == NULL, length == 0 ) );

    return ( StringC ){ .e = str, .length = length };
}


StringC
stringc__view_stringm(
        StringM const s )
{
    ASSERT( stringm__is_valid( s ) );

    return stringc__new( s.e, s.length );
}


StringC
stringc__view_arrayc(
        ArrayC_char const xs )
{
    ASSERT( arrayc_char__is_valid( xs ) );

    return stringc__new( xs.e, xs.length );
}


StringC
stringc__view_arraym(
        ArrayM_char const xs )
{
    ASSERT( arraym_char__is_valid( xs ) );

    return stringc__new( xs.e, xs.length );
}


StringC
stringc__view_vec(
        Vec_char const v )
{
    ASSERT( vec_char__is_valid( v ) );

    return stringc__new( v.e, v.length );
}


StringC
stringc__view_str(
        char const * const str )
{
    ASSERT( str != NULL );

    return stringc__new( str, strlen_null( str ) );
}


StringC
stringc__view_str0(
        char const * const str )
{
    ASSERT( str != NULL );

    return stringc__new( str, strlen_null( str ) + 1 );
}


char const *
stringc__elements(
        StringC const s )
{
    ASSERT( stringc__is_valid( s ) );

    return s.e;
}


size_t
stringc__length(
        StringC const s )
{
    ASSERT( stringc__is_valid( s ) );

    return s.length;
}


bool
stringc__same(
        StringC const s,
        StringC const t )
{
    ASSERT( stringc__is_valid( s ), stringc__is_valid( t ) );

    return arrayc_char__same( arrayc_char__view_stringc( s ),
                              arrayc_char__view_stringc( t ) );
}


bool
stringc__is_empty(
        StringC const s )
{
    ASSERT( stringc__is_valid( s ) );

    return stringc__length( s ) == 0;
}


bool
stringc__isnt_empty(
        StringC const s )
{
    ASSERT( stringc__is_valid( s ) );

    return !stringc__is_empty( s );
}


bool
stringc__is_empty0(
        StringC const s )
{
    ASSERT( stringc__is_valid( s ) );

    return stringc__is_empty( s )
        || stringc__equal( s, stringc__view_str0( "" ) );
}


bool
stringc__isnt_empty0(
        StringC const s )
{
    ASSERT( stringc__is_valid( s ) );

    return !stringc__is_empty0( s );
}


char
stringc__get(
        StringC const s,
        size_t const index )
{
    ASSERT( stringc__is_valid( s ), index < stringc__length( s ) );

    return *( stringc__get_ptr( s, index ) );
}


char const *
stringc__get_ptr(
        StringC const s,
        size_t const index )
{
    ASSERT( stringc__is_valid( s ), index < stringc__length( s ) );

    return stringc__elements( s ) + index;
}


char
stringc__first(
        StringC const s )
{
    ASSERT( stringc__is_valid( s ), stringc__isnt_empty( s ) );

    return *( stringc__first_ptr( s ) );
}


char const *
stringc__first_ptr(
        StringC const s )
{
    ASSERT( stringc__is_valid( s ) );

    return stringc__is_empty( s ) ? NULL : stringc__elements( s );
}


char
stringc__last(
        StringC const s )
{
    ASSERT( stringc__is_valid( s ), stringc__isnt_empty( s ) );

    return *( stringc__last_ptr( s ) );
}


char const *
stringc__last_ptr(
        StringC const s )
{
    ASSERT( stringc__is_valid( s ) );

    return stringc__is_empty( s )
               ? NULL
               : stringc__get_ptr( s, stringc__length( s ) - 1 );
}


bool
stringc__last_is_null(
        StringC const s )
{
    ASSERT( stringc__is_valid( s ) );

    return stringc__isnt_empty( s ) && stringc__last( s ) == '\0';
}


bool
stringc__last_isnt_null(
        StringC const s )
{
    ASSERT( stringc__is_valid( s ) );

    return !stringc__last_is_null( s );
}


bool
stringc__equal_stringc(
        StringC const x,
        StringC const y )
{
    ASSERT( stringc__is_valid( x ), stringc__is_valid( y ) );

    return arrayc_char__equal( arrayc_char__view_stringc( x ),
                               arrayc_char__view_stringc( y ) );
}


bool
stringc__equal_stringm(
        StringC const x,
        StringM const y )
{
    ASSERT( stringc__is_valid( x ), stringm__is_valid( y ) );

    return stringc__equal_stringc( x, stringc__view( y ) );
}


bool
stringc__equal_arrayc(
        StringC const x,
        ArrayC_char const y )
{
    ASSERT( stringc__is_valid( x ), arrayc_char__is_valid( y ) );

    return stringc__equal_stringc( x, stringc__view( y ) );
}


bool
stringc__equal_arraym(
        StringC const x,
        ArrayM_char const y )
{
    ASSERT( stringc__is_valid( x ), arraym_char__is_valid( y ) );

    return stringc__equal_stringc( x, stringc__view( y ) );
}


bool
stringc__equal_vec(
        StringC const x,
        Vec_char const y )
{
    ASSERT( stringc__is_valid( x ), vec_char__is_valid( y ) );

    return stringc__equal_stringc( x, stringc__view( y ) );
}


bool
stringc__equal_str(
        StringC const x,
        char const * const y )
{
    ASSERT( stringc__is_valid( x ), y != NULL );

    return stringc__equal_stringc( x, stringc__view( y ) );
}


StringM
stringc__replaced_by(
        StringC const xs,
        char const el,
        char const repl,
        bool ( * const eq )( char x, char el ) )
{
    ASSERT( stringc__is_valid( xs ), eq != NULL );

    return stringm__view(
               arrayc_char__replaced_by( arrayc_char__view_stringc( xs ),
                                         el, repl, eq ) );
}


StringM
stringc__replaced(
        StringC const xs,
        char const el,
        char const repl )
{
    ASSERT( stringc__is_valid( xs ) );

    return stringc__replaced_by( xs, el, repl, char_equal );
}


StringM
stringc__replaced_i(
        StringC const xs,
        char const el,
        char const repl )
{
    ASSERT( stringc__is_valid( xs ) );

    return stringc__replaced_by( xs, el, repl, char_equal_i );
}


StringM
stringc__replacedf(
        StringC const xs,
        bool ( * const f )( char x ),
        char const repl )
{
    ASSERT( stringc__is_valid( xs ), f != NULL );

    return stringm__view(
               arrayc_char__replacedf( arrayc_char__view_stringc( xs ),
                                       f, repl ) );
}





///////////////////////////////////
/// STRINGM FUNCTIONS
///////////////////////////////////


bool
stringm__is_valid(
        StringM const s )
{
    return ALL( STRINGM_INVARIANTS( s ) );
}


void
stringm__free(
        StringM * const s )
{
    ASSERT( s != NULL, stringm__is_valid( *s ) );

    Vec_char v = vec_char__view_stringm( *s );
    vec_char__free( &v );
    *s = stringm__view_vec( v );
}


void
stringm__freev(
        StringM const s )
{
    ASSERT( stringm__is_valid( s ) );

    vec_char__freev( vec_char__view_stringm( s ) );
}


StringM
stringm__new(
        char const * const str,
        size_t const length,
        size_t const capacity )
{
    ASSERT( IMPLIES( str == NULL, length == 0 ), length <= capacity );

    return stringm__view_vec( vec_char__new( str, length, capacity ) );
}


StringM
stringm__new_empty(
        size_t const capacity )
{
    return stringm__view_vec( vec_char__new_empty( capacity ) );
}


StringM
stringm__new_fmt(
        char const * const format,
        ... )
{
    ASSERT( format != NULL );

    StringM s = ( StringM ){ 0 };
    va_list ap;
    va_start( ap, format );
    stringm__extend_fmtv( &s, format, ap );
    int const err = errno;
    va_end( ap );
    if ( err ) { errno = err; }
    return s;
}


void
stringm__fmt_into(
        StringM * const to,
        char const * const format,
        ... )
{
    ASSERT( to != NULL, stringm__is_valid( *to ), format != NULL );

    errno = 0;
    va_list ap;
    va_start( ap, format );
    stringm__empty( to );
    stringm__extend_fmtv( to, format, ap );
    int const err = errno;
    va_end( ap );
    if ( err ) { errno = err; }
}


StringM
stringm__view_strm(
        char * const str )
{
    ASSERT( str != NULL );

    size_t const len = strlen_null( str );
    return ( StringM ){ .e = str, .length = len, .capacity = len };
}


StringM
stringm__view_arraym(
        ArrayM_char const xs )
{
    ASSERT( arraym_char__is_valid( xs ) );

    return stringm__view_vec( vec_char__view_arraym( xs ) );
}


StringM
stringm__view_vec(
        Vec_char const v )
{
    ASSERT( vec_char__is_valid( v ) );

    return ( StringM ){ .e = v.e, .length = v.length, .capacity = v.capacity };
}


StringM
stringm__view_strm0(
        char * const str )
{
    ASSERT( str != NULL );

    size_t const len = strlen_null( str );
    return ( StringM ){ .e = str, .length = len + 1, .capacity = len + 1 };
}


StringM
stringm__copy_stringc(
        StringC const s )
{
    ASSERT( stringc__is_valid( s ) );

    return stringm__new( s.e, s.length, s.length );
}


StringM
stringm__copy_stringm(
        StringM const s )
{
    ASSERT( stringm__is_valid( s ) );

    return stringm__new( s.e, s.length, s.capacity );
}


StringM
stringm__copy_arrayc(
        ArrayC_char const xs )
{
    ASSERT( arrayc_char__is_valid( xs ) );

    return stringm__new( xs.e, xs.length, xs.length );
}


StringM
stringm__copy_arraym(
        ArrayM_char const xs )
{
    ASSERT( arraym_char__is_valid( xs ) );

    return stringm__new( xs.e, xs.length, xs.length );
}


StringM
stringm__copy_vec(
        Vec_char const v )
{
    ASSERT( vec_char__is_valid( v ) );

    return stringm__new( v.e, v.length, v.capacity );
}


StringM
stringm__copy_str(
        char const * str )
{
    ASSERT( str != NULL );

    size_t const len = strlen_null( str );
    return stringm__new( str, len, len );
}


void
stringm__copy_stringc_into(
        StringC const from,
        StringM * const to )
{
    ASSERT( to != NULL, stringm__is_valid( *to ),
            stringc__is_valid( from ) );

    stringm__empty( to );
    stringm__extend( to, from );
}


void
stringm__copy_stringm_into(
        StringM const from,
        StringM * const to )
{
    ASSERT( to != NULL, stringm__is_valid( *to ),
            stringm__is_valid( from ) );

    stringm__copy_stringc_into( stringc__view( from ), to );
}


void
stringm__copy_arrayc_into(
        ArrayC_char const from,
        StringM * const to )
{
    ASSERT( to != NULL, stringm__is_valid( *to ),
            arrayc_char__is_valid( from ) );

    stringm__copy_stringc_into( stringc__view( from ), to );
}


void
stringm__copy_arraym_into(
        ArrayM_char const from,
        StringM * const to )
{
    ASSERT( to != NULL, stringm__is_valid( *to ),
            arraym_char__is_valid( from ) );

    stringm__copy_stringc_into( stringc__view( from ), to );
}


void
stringm__copy_vec_into(
        Vec_char const from,
        StringM * const to )
{
    ASSERT( to != NULL, stringm__is_valid( *to ),
            vec_char__is_valid( from ) );

    stringm__copy_stringc_into( stringc__view( from ), to );
}


void
stringm__copy_str_into(
        char const * const from,
        StringM * const to )
{
    ASSERT( to != NULL, stringm__is_valid( *to ), from != NULL );

    stringm__copy_stringc_into( stringc__view( from ), to );
}


void
stringm__into_stringm(
        StringM const from,
        StringM * const to )
{
    ASSERT( stringm__is_valid( from ), to != NULL, stringm__is_valid( *to ) );

    Vec_char to_vec = vec_char__view_stringm( *to );
    vec_char__into_vec( vec_char__view_stringm( from ), &to_vec );
    *to = stringm__view( to_vec );
}


void
stringm__into_vec(
        StringM const from,
        Vec_char * const to )
{
    ASSERT( stringm__is_valid( from ), to != NULL, vec_char__is_valid( *to ) );

    vec_char__into_vec( vec_char__view_stringm( from ), to );
}


void
stringm__realloc(
        StringM * const s,
        size_t const new_capacity )
{
    ASSERT( s != NULL, stringm__is_valid( *s ) );

    Vec_char v = vec_char__view_stringm( *s );
    vec_char__realloc( &v, new_capacity );
    *s = stringm__view_vec( v );
}


void
stringm__grow_capacity(
        StringM * const s )
{
    ASSERT( s != NULL, stringm__is_valid( *s ) );

    Vec_char v = vec_char__view_stringm( *s );
    vec_char__grow_capacity( &v );
    *s = stringm__view_vec( v );
}


void
stringm__grow_capacity_by(
        StringM * const s,
        size_t const to_grow )
{
    ASSERT( s != NULL, stringm__is_valid( *s ) );

    Vec_char v = vec_char__view_stringm( *s );
    vec_char__grow_capacity_by( &v, to_grow );
    *s = stringm__view_vec( v );
}


void
stringm__grow_capacity_for(
        StringM * const s,
        size_t const req_space )
{
    ASSERT( s != NULL, stringm__is_valid( *s ) );

    Vec_char v = vec_char__view_stringm( *s );
    vec_char__grow_capacity_for( &v, req_space );
    *s = stringm__view_vec( v );
}


void
stringm__ensure_capacity(
        StringM * const s,
        size_t const min_capacity )
{
    ASSERT( s != NULL, stringm__is_valid( *s ) );

    Vec_char v = vec_char__view_stringm( *s );
    vec_char__ensure_capacity( &v, min_capacity );
    *s = stringm__view_vec( v );
}


void
stringm__shrink_capacity(
        StringM * const s )
{
    ASSERT( s != NULL, stringm__is_valid( *s ) );

    Vec_char v = vec_char__view_stringm( *s );
    vec_char__shrink_capacity( &v );
    *s = stringm__view_vec( v );
}


void
stringm__shrink_capacity_to(
        StringM * const s,
        size_t const max_capacity )
{
    ASSERT( s != NULL, stringm__is_valid( *s ) );

    Vec_char v = vec_char__view_stringm( *s );
    vec_char__shrink_capacity_to( &v, max_capacity );
    *s = stringm__view_vec( v );
}


void
stringm__shrink_capacity_by(
        StringM * const s,
        size_t const to_shrink )
{
    ASSERT( s != NULL, stringm__is_valid( *s ) );

    Vec_char v = vec_char__view_stringm( *s );
    vec_char__shrink_capacity_by( &v, to_shrink );
    *s = stringm__view_vec( v );
}


void
stringm__free_spare_capacity(
        StringM * const s )
{
    ASSERT( s != NULL, stringm__is_valid( *s ) );

    Vec_char v = vec_char__view_stringm( *s );
    vec_char__free_spare_capacity( &v );
    *s = stringm__view( v );
}


char *
stringm__elements(
        StringM const s )
{
    ASSERT( stringm__is_valid( s ) );

    return s.e;
}


size_t
stringm__length(
        StringM const s )
{
    ASSERT( stringm__is_valid( s ) );

    return s.length;
}


bool
stringm__same(
        StringM const s,
        StringM const t )
{
    ASSERT( stringm__is_valid( s ), stringm__is_valid( t ) );

    return vec_char__same( vec_char__view_stringm( s ),
                           vec_char__view_stringm( t ) );
}


bool
stringm__is_empty(
        StringM const s )
{
    ASSERT( stringm__is_valid( s ) );

    return stringm__length( s ) == 0;
}


bool
stringm__isnt_empty(
        StringM const s )
{
    ASSERT( stringm__is_valid( s ) );

    return !stringm__is_empty( s );
}


bool
stringm__is_empty0(
        StringM const s )
{
    ASSERT( stringm__is_valid( s ) );

    return stringm__is_empty( s )
        || stringm__equal( s, stringc__view_str0( "" ) );
}


bool
stringm__isnt_empty0(
        StringM const s )
{
    ASSERT( stringm__is_valid( s ) );

    return !stringm__is_empty0( s );
}


char
stringm__get(
        StringM const s,
        size_t const index )
{
    ASSERT( stringm__is_valid( s ), index < stringm__length( s ) );

    return *( stringm__get_ptr( s, index ) );
}


char *
stringm__get_ptr(
        StringM const s,
        size_t const index )
{
    ASSERT( stringm__is_valid( s ), index < stringm__length( s ) );

    return stringm__elements( s ) + index;
}


char
stringm__first(
        StringM const s )
{
    ASSERT( stringm__is_valid( s ), stringm__isnt_empty( s ) );

    return *( stringm__first_ptr( s ) );
}


char *
stringm__first_ptr(
        StringM const s )
{
    ASSERT( stringm__is_valid( s ) );

    return stringm__is_empty( s ) ? NULL : stringm__elements( s );
}


char
stringm__last(
        StringM const s )
{
    ASSERT( stringm__is_valid( s ), stringm__isnt_empty( s ) );

    return *( stringm__last_ptr( s ) );
}


char *
stringm__last_ptr(
        StringM const s )
{
    ASSERT( stringm__is_valid( s ) );

    return stringm__is_empty( s )
               ? NULL
               : stringm__get_ptr( s, stringm__length( s ) - 1 );
}


bool
stringm__last_is_null(
        StringM const s )
{
    ASSERT( stringm__is_valid( s ) );

    return stringm__isnt_empty( s ) && stringm__last( s ) == '\0';
}


bool
stringm__last_isnt_null(
        StringM const s )
{
    ASSERT( stringm__is_valid( s ) );

    return !stringm__last_is_null( s );
}


void
stringm__append(
        StringM * const s,
        char const c )
{
    ASSERT( s != NULL, stringm__is_valid( *s ) );

    Vec_char v = vec_char__view_stringm( *s );
    vec_char__append( &v, c );
    *s = stringm__view( v );
}


void
stringm__nullterm(
        StringM * const s )
{
    ASSERT( s != NULL, stringm__is_valid( *s ) );

    if ( stringm__last_isnt_null( *s ) ) {
        stringm__append( s, '\0' );
    }
}


char
stringm__pop(
        StringM * const s )
{
    ASSERT( s != NULL, stringm__is_valid( *s ), stringm__isnt_empty( *s ) );

    Vec_char v = vec_char__view_stringm( *s );
    char const r = vec_char__pop( &v );
    *s = stringm__view( v );
    return r;
}


void
stringm__popn(
        StringM * const s,
        size_t const n )
{
    ASSERT( s != NULL, stringm__is_valid( *s ) );

    Vec_char v = vec_char__view_stringm( *s );
    vec_char__popn( &v, n );
    *s = stringm__view( v );
}


bool
stringm__pop_null(
        StringM * const s )
{
    ASSERT( s != NULL, stringm__is_valid( *s ), stringm__isnt_empty( *s ) );

    if ( stringm__last_is_null( *s ) ) {
        stringm__pop( s );
        return true;
    } else {
        return false;
    }
}


void
stringm__empty(
        StringM * const s )
{
    ASSERT( s != NULL, stringm__is_valid( *s ), stringm__isnt_empty( *s ) );

    Vec_char v = vec_char__view_stringm( *s );
    vec_char__empty( &v );
    *s = stringm__view( v );
}


void
stringm__extend_stringc(
        StringM * const s,
        StringC const ext )
{
    ASSERT( s != NULL, stringm__is_valid( *s ), stringc__is_valid( ext ) );

    stringm__extend_arrayc( s, arrayc_char__view_stringc( ext ) );
}


void
stringm__extend_stringm(
        StringM * const s,
        StringM const ext )
{
    ASSERT( s != NULL, stringm__is_valid( *s ), stringm__is_valid( ext ) );

    stringm__extend_arrayc( s, arrayc_char__view_stringm( ext ) );
}


void
stringm__extend_arrayc(
        StringM * const s,
        ArrayC_char const ext )
{
    ASSERT( s != NULL, stringm__is_valid( *s ), arrayc_char__is_valid( ext ) );

    Vec_char v = vec_char__view_stringm( *s );
    vec_char__extend_arrayc( &v, ext );
    *s = stringm__view_vec( v );
}


void
stringm__extend_arraym(
        StringM * const s,
        ArrayM_char const ext )
{
    ASSERT( s != NULL, stringm__is_valid( *s ), arraym_char__is_valid( ext ) );

    stringm__extend_arrayc( s, arrayc_char__view_arraym( ext ) );
}


void
stringm__extend_vec(
        StringM * const s,
        Vec_char const ext )
{
    ASSERT( s != NULL, stringm__is_valid( *s ), vec_char__is_valid( ext ) );

    stringm__extend_arrayc( s, arrayc_char__view_vec( ext ) );
}


void
stringm__extend_str(
        StringM * const s,
        char const * const ext )
{
    ASSERT( s != NULL, stringm__is_valid( *s ), ext != NULL );

    stringm__extend_stringc( s, stringc__view( ext ) );
}


void
stringm__extend_fmt(
        StringM * const s,
        char const * const format,
        ... )
{
    ASSERT( s != NULL, stringm__is_valid( *s ), format != NULL );

    va_list ap;
    va_start( ap, format );
    stringm__extend_fmtv( s, format, ap );
    int const err = errno;
    va_end( ap );
    if ( err ) { errno = err; }
}


void
stringm__extend_fmtv(
        StringM * const s,
        char const * const format,
        va_list ap )
{
    ASSERT( s != NULL, stringm__is_valid( *s ), format != NULL );

    errno = 0;
    // TODO: asprintf is a non-standard GNU/BSD extension to libc; replace
    char * ext;
    vasprintf_err( &ext, format, ap );
    if ( errno ) { return; }
    stringm__extend_str( s, ext );
    free( ext );
}


bool
stringm__equal_stringc(
        StringM const x,
        StringC const y )
{
    ASSERT( stringm__is_valid( x ), stringc__is_valid( y ) );

    return stringc__equal( stringc__view( x ), y );
}


bool
stringm__equal_stringm(
        StringM const x,
        StringM const y )
{
    ASSERT( stringm__is_valid( x ), stringm__is_valid( y ) );

    return stringm__equal( x, stringc__view( y ) );
}


bool
stringm__equal_arrayc(
        StringM const x,
        ArrayC_char const y )
{
    ASSERT( stringm__is_valid( x ), arrayc_char__is_valid( y ) );

    return stringm__equal( x, stringc__view( y ) );
}


bool
stringm__equal_arraym(
        StringM const x,
        ArrayM_char const y )
{
    ASSERT( stringm__is_valid( x ), arraym_char__is_valid( y ) );

    return stringm__equal( x, stringc__view( y ) );
}


bool
stringm__equal_vec(
        StringM const x,
        Vec_char const y )
{
    ASSERT( stringm__is_valid( x ), vec_char__is_valid( y ) );

    return stringm__equal( x, stringc__view( y ) );
}


bool
stringm__equal_str(
        StringM const x,
        char const * const y )
{
    ASSERT( stringm__is_valid( x ), y != NULL );

    return stringm__equal( x, stringc__view( y ) );
}


void
stringm__replace_by(
        StringM const xs,
        char const el,
        char const repl,
        bool ( * const eq )( char x, char el ) )
{
    ASSERT( stringm__is_valid( xs ), eq != NULL );

    arraym_char__replace_by( arraym_char__view_stringm( xs ), el, repl, eq );
}


void
stringm__replace(
        StringM const xs,
        char const el,
        char const repl )
{
    ASSERT( stringm__is_valid( xs ) );

    stringm__replace_by( xs, el, repl, char_equal );
}


void
stringm__replace_i(
        StringM const xs,
        char const el,
        char const repl )
{
    ASSERT( stringm__is_valid( xs ) );

    stringm__replace_by( xs, el, repl, char_equal_i );
}


void
stringm__replacef(
        StringM const xs,
        bool ( * const f )( char x ),
        char const repl )
{
    ASSERT( stringm__is_valid( xs ), f != NULL );

    arraym_char__replacef( arraym_char__view_stringm( xs ), f, repl );
}





char *
strm__copy_stringc(
        StringC const s )
{
    ASSERT( stringc__is_valid( s ) );

    errno = 0;
    if ( stringc__last_is_null( s ) ) {
        char * const str = malloc( s.length );
        if ( errno ) { return NULL; }
        memcpy( str, s.e, s.length );
        ASSERT( str[ s.length ] == '\0' );
        return str;
    } else {
        if ( s.length == SIZE_MAX ) {
            errno = ENOBUFS;
            return NULL;
        }
        char * const str = malloc( s.length + 1 );
        if ( errno ) { return NULL; }
        memcpy( str, s.e, s.length );
        str[ s.length ] = '\0';
        return str;
    }
}


char *
strm__copy_stringm(
        StringM const string )
{
    ASSERT( stringm__is_valid( string ) );

    return strm__copy_stringc( stringc__view_stringm( string ) );
}


ArrayC_char
arrayc_char__view_stringc(
        StringC const s )
{
    ASSERT( stringc__is_valid( s ) );

    return ( ArrayC_char ){ .e = s.e, .length = s.length };
}


ArrayC_char
arrayc_char__view_stringm(
        StringM const s )
{
    ASSERT( stringm__is_valid( s ) );

    return arrayc_char__view_vec( vec_char__view_stringm( s ) );
}


ArrayM_char
arraym_char__view_stringm(
        StringM const s )
{
    ASSERT( stringm__is_valid( s ) );

    return arraym_char__view_vec( vec_char__view_stringm( s ) );
}


Vec_char
vec_char__view_stringm(
        StringM const s )
{
    ASSERT( stringm__is_valid( s ) );

    return ( Vec_char ){ .e = s.e,
                         .length = s.length,
                         .capacity = s.capacity };
}


