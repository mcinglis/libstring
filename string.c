
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
strlen_null( char const * const str )
{
    return ( str == NULL ) ? 0 : strlen( str );
}


static
void
vasprintf_err( char * * const strp,
               char const * const format,
               va_list ap )
{
    int const r = vasprintf( strp, format, ap );
    if ( r < 0 && !errno ) {
        errno = EIO;
    }
}




///////////////////////////////////
/// STRINGC FUNCTIONS
///////////////////////////////////


bool
stringc__is_valid( StringC const s )
{
    return ALL( STRINGC_INVARIANTS( s ) );
}


StringC
stringc__new( char const * const str,
              size_t const length )
{
    ASSERT( IMPLIES( str == NULL, length == 0 ) );

    return ( StringC ){ .e = str, .length = length };
}


StringC stringc__view_stringm( StringM const s )
    { return stringc__new( s.e, s.length ); }

StringC stringc__view_arrayc( ArrayC_char const xs )
    { return stringc__new( xs.e, xs.length ); }

StringC stringc__view_arraym( ArrayM_char const xs )
    { return stringc__new( xs.e, xs.length ); }

StringC stringc__view_vec( Vec_char const v )
    { return stringc__new( v.e, v.length ); }

StringC stringc__view_str( char const * const str )
    { return stringc__new( str, strlen_null( str ) ); }


char const *
stringc__elements( StringC const s )
{
    ASSERT( stringc__is_valid( s ) );

    return s.e;
}


size_t
stringc__length( StringC const s )
{
    ASSERT( stringc__is_valid( s ) );

    return s.length;
}


bool
stringc__is_empty( StringC const s )
{
    ASSERT( stringc__is_valid( s ) );

    return stringc__length( s ) == 0;
}


bool
stringc__isnt_empty( StringC const s )
{
    ASSERT( stringc__is_valid( s ) );

    return !stringc__is_empty( s );
}


char
stringc__get( StringC const s,
              size_t const index )
{
    ASSERT( stringc__is_valid( s ), index < stringc__length( s ) );

    return *( stringc__get_ptr( s, index ) );
}


char const *
stringc__get_ptr( StringC const s,
                  size_t const index )
{
    ASSERT( stringc__is_valid( s ), index < stringc__length( s ) );

    return stringc__elements( s ) + index;
}


char
stringc__first( StringC const s )
{
    ASSERT( stringc__is_valid( s ), stringc__isnt_empty( s ) );

    return *( stringc__first_ptr( s ) );
}


char const *
stringc__first_ptr( StringC const s )
{
    ASSERT( stringc__is_valid( s ) );

    return stringc__is_empty( s ) ? NULL : stringc__elements( s );
}


char
stringc__last( StringC const s )
{
    ASSERT( stringc__is_valid( s ), stringc__isnt_empty( s ) );

    return *( stringc__last_ptr( s ) );
}


char const *
stringc__last_ptr( StringC const s )
{
    ASSERT( stringc__is_valid( s ) );

    return stringc__is_empty( s )
               ? NULL
               : stringc__get_ptr( s, stringc__length( s ) - 1 );
}


bool
stringc__last_is_null( StringC const s )
{
    ASSERT( stringc__is_valid( s ) );

    return stringc__isnt_empty( s ) && stringc__last( s ) == '\0';
}


bool
stringc__last_isnt_null( StringC const s )
{
    ASSERT( stringc__is_valid( s ) );

    return !stringc__last_is_null( s );
}


bool
stringc__equal_stringc( StringC const x,
                        StringC const y )
{
    return arrayc_char__equal( x, y );
}


bool
stringc__equal_stringm( StringC const x,
                        StringM const y )
{
    return stringc__equal_stringc( x, stringc__view( y ) );
}


bool
stringc__equal_arrayc( StringC const x,
                       ArrayC_char const y )
{
    return stringc__equal_stringc( x, stringc__view( y ) );
}


bool
stringc__equal_arraym( StringC const x,
                       ArrayM_char const y )
{
    return stringc__equal_stringc( x, stringc__view( y ) );
}


bool
stringc__equal_vec( StringC const x,
                    Vec_char const y )
{
    return stringc__equal_stringc( x, stringc__view( y ) );
}


bool
stringc__equal_str( StringC const x,
                    char const * const y )
{
    return stringc__equal_stringc( x, stringc__view( y ) );
}





///////////////////////////////////
/// STRINGM FUNCTIONS
///////////////////////////////////


bool
stringm__is_valid( StringM const s )
{
    return vec_char__is_valid( s );
}


void
stringm__free( StringM * const s )
{
    vec_char__free( s );
}


StringM
stringm__new( char const * const str,
              size_t const length,
              size_t const capacity )
{
    ASSERT( IMPLIES( str == NULL, length == 0 ), length <= capacity );

    return vec_char__new( str, length, capacity );
}


StringM
stringm__new_empty( size_t const capacity )
{
    return vec_char__new_empty( capacity );
}


StringM
stringm__new_fmt( char const * const format,
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
stringm__fmt_into( StringM * const s,
                   char const * const format,
                   ... )
{
    ASSERT( s != NULL, stringm__is_valid( *s ), format != NULL );

    va_list ap;
    va_start( ap, format );
    s->length = 0;
    stringm__extend_fmtv( s, format, ap );
    int const err = errno;
    va_end( ap );
    if ( err ) { errno = err; }
}


StringM
stringm__view_strm( char * const str )
{
    size_t const len = strlen_null( str );
    return ( StringM ){ .e = str, .length = len, .capacity = len };
}


StringM
stringm__view_arraym( ArrayM_char const xs )
{
    return vec_char__view_arraym( xs );
}


StringM
stringm__view_vec( Vec_char const v )
{
    return v;
}


StringM stringm__copy_stringc( StringC const s )
    { return stringm__new( s.e, s.length, s.length ); }

StringM stringm__copy_stringm( StringM const s )
    { return stringm__new( s.e, s.length, s.capacity ); }

StringM stringm__copy_arrayc( ArrayC_char const xs )
    { return stringm__new( xs.e, xs.length, xs.length ); }

StringM stringm__copy_arraym( ArrayM_char const xs )
    { return stringm__new( xs.e, xs.length, xs.length ); }

StringM stringm__copy_vec( Vec_char const v )
    { return stringm__new( v.e, v.length, v.capacity ); }

StringM stringm__copy_str( char const * str )
    { size_t const len = strlen_null( str );
      return stringm__new( str, len, len ); }


void
stringm__copy_stringc_into( StringM * const s,
                            StringC const from )
{
    ASSERT( s != NULL, stringm__is_valid( *s ), stringc__is_valid( from ) );

    s->length = 0;
    stringm__extend( s, from );
}


void
stringm__copy_stringm_into( StringM * const s,
                            StringM const from )
{
    stringm__copy_stringc_into( s, stringc__view( from ) );
}


void
stringm__copy_arrayc_into( StringM * const s,
                           ArrayC_char const from )
{
    stringm__copy_stringc_into( s, stringc__view( from ) );
}


void
stringm__copy_arraym_into( StringM * const s,
                           ArrayM_char const from )
{
    stringm__copy_stringc_into( s, stringc__view( from ) );
}


void
stringm__copy_vec_into( StringM * const s,
                        Vec_char const from )
{
    stringm__copy_stringc_into( s, stringc__view( from ) );
}


void
stringm__copy_str_into( StringM * const s,
                        char const * const from )
{
    stringm__copy_stringc_into( s, stringc__view( from ) );
}


void
stringm__realloc( StringM * const s,
                  size_t const new_capacity )
{
    vec_char__realloc( s, new_capacity );
}


void
stringm__grow_capacity( StringM * const s )
{
    vec_char__grow_capacity( s );
}


void
stringm__shrink_capacity( StringM * const s )
{
    vec_char__shrink_capacity( s );
}


char *
stringm__elements( StringM const s )
{
    ASSERT( stringm__is_valid( s ) );

    return s.e;
}


size_t
stringm__length( StringM const s )
{
    ASSERT( stringm__is_valid( s ) );

    return s.length;
}


bool
stringm__is_empty( StringM const s )
{
    ASSERT( stringm__is_valid( s ) );

    return stringm__length( s ) == 0;
}


bool
stringm__isnt_empty( StringM const s )
{
    ASSERT( stringm__is_valid( s ) );

    return !stringm__is_empty( s );
}


char
stringm__get( StringM const s,
              size_t const index )
{
    ASSERT( stringm__is_valid( s ), index < stringm__length( s ) );

    return *( stringm__get_ptr( s, index ) );
}


char *
stringm__get_ptr( StringM const s,
                  size_t const index )
{
    ASSERT( stringm__is_valid( s ), index < stringm__length( s ) );

    return stringm__elements( s ) + index;
}


char
stringm__first( StringM const s )
{
    ASSERT( stringm__is_valid( s ), stringm__isnt_empty( s ) );

    return *( stringm__first_ptr( s ) );
}


char *
stringm__first_ptr( StringM const s )
{
    ASSERT( stringm__is_valid( s ) );

    return stringm__is_empty( s ) ? NULL : stringm__elements( s );
}


char
stringm__last( StringM const s )
{
    ASSERT( stringm__is_valid( s ), stringm__isnt_empty( s ) );

    return *( stringm__last_ptr( s ) );
}


char *
stringm__last_ptr( StringM const s )
{
    ASSERT( stringm__is_valid( s ) );

    return stringm__is_empty( s )
               ? NULL
               : stringm__get_ptr( s, stringm__length( s ) - 1 );
}


bool
stringm__last_is_null( StringM const s )
{
    ASSERT( stringm__is_valid( s ) );

    return stringm__isnt_empty( s ) && stringm__last( s ) == '\0';
}


bool
stringm__last_isnt_null( StringM const s )
{
    ASSERT( stringm__is_valid( s ) );

    return !stringm__last_is_null( s );
}


void
stringm__append( StringM * const s,
                 char const c )
{
    vec_char__append( s, c );
}


void
stringm__nullterm( StringM * const s )
{
    if ( stringm__last( *s ) != '\0' ) {
        stringm__append( s, '\0' );
    }
}


void stringm__extend_stringc( StringM * const s, StringC const ext )
    { vec_char__extend_arrayc( s, ext ); }

void stringm__extend_stringm( StringM * const s, StringM const ext )
    { vec_char__extend_vec( s, ext ); }

void stringm__extend_arrayc( StringM * const s, ArrayC_char const ext )
    { vec_char__extend_arrayc( s, ext ); }

void stringm__extend_arraym( StringM * const s, ArrayM_char const ext )
    { vec_char__extend_arraym( s, ext ); }

void stringm__extend_vec( StringM * const s, Vec_char const ext )
    { vec_char__extend_vec( s, ext ); }

void stringm__extend_str( StringM * const s, char const * const ext )
    { stringm__extend_stringc( s, stringc__view( ext ) ); }


void
stringm__extend_fmt( StringM * const s,
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
stringm__extend_fmtv( StringM * const s,
                      char const * const format,
                      va_list ap )
{
    ASSERT( s != NULL, stringm__is_valid( *s ), format != NULL );

    errno = 0;
    // TODO: asprintf is a non-standard GNU/BSD extension to libc; remove
    // this and then remove `_GNU_SOURCE` definition in Makefile:
    char * ext;
    vasprintf_err( &ext, format, ap );
    if ( errno ) { return; }
    stringm__extend_str( s, ext );
    int const extend_err = errno;
    free( ext );
    if ( extend_err ) { errno = extend_err; }
}


bool
stringm__equal_stringc( StringM const x,
                        StringC const y )
{
    return stringc__equal( stringc__view( x ), y );
}


bool
stringm__equal_stringm( StringM const x,
                        StringM const y )
{
    return stringm__equal( x, stringc__view( y ) );
}


bool
stringm__equal_arrayc( StringM const x,
                       ArrayC_char const y )
{
    return stringm__equal( x, stringc__view( y ) );
}


bool
stringm__equal_arraym( StringM const x,
                       ArrayM_char const y )
{
    return stringm__equal( x, stringc__view( y ) );
}


bool
stringm__equal_vec( StringM const x,
                    Vec_char const y )
{
    return stringm__equal( x, stringc__view( y ) );
}


bool
stringm__equal_str( StringM const x,
                    char const * const y )
{
    return stringm__equal( x, stringc__view( y ) );
}








char *
strm__copy_stringc( StringC const string )
{
    ASSERT( stringc__is_valid( string ) );

    if ( string.length == SIZE_MAX ) {
        errno = ENOBUFS;
        return NULL;
    }
    errno = 0;
    char * const str = malloc( string.length + 1 );
    if ( errno ) { return NULL; }
    memcpy( str, string.e, string.length );
    str[ string.length ] = '\0';
    return str;
}


char *
strm__copy_stringm( StringM const string )
{
    ASSERT( stringm__is_valid( string ) );

    return strm__copy_stringc( stringc__view_stringm( string ) );
}


ArrayC_char
arrayc_char__view_stringc( StringC const s )
{
    return s;
}


ArrayC_char
arrayc_char__view_stringm( StringM const s )
{
    return arrayc_char__view_stringc( stringc__view_stringm( s ) );
}


ArrayM_char
arraym_char__view_stringm( StringM const s )
{
    return arraym_char__view_vec( s );
}


Vec_char
vec_char__view_stringm( StringM const s )
{
    return s;
}


