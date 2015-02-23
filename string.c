
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


bool
stringc__is_valid( StringC const s )
{
    return arrayc_char__is_valid( s );
}


bool
stringm__is_valid( StringM const s )
{
    return vec_char__is_valid( s );
}


StringC
stringc__new( char const * const str,
              size_t const length )
{
    ASSERT( IMPLIES( str == NULL, length == 0 ) );

    return ( StringC ){ .e = str, .length = length };
}


StringC stringc__from_stringm( StringM const s )
    { return stringc__new( s.e, s.length ); }

StringC stringc__from_arrayc( ArrayC_char const xs )
    { return stringc__new( xs.e, xs.length ); }

StringC stringc__from_arraym( ArrayM_char const xs )
    { return stringc__new( xs.e, xs.length ); }

StringC stringc__from_vec( Vec_char const v )
    { return stringc__new( v.e, v.length ); }

StringC stringc__from_str( char const * const str )
    { return stringc__new( str, strlen_null( str ) ); }


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
stringm__from_mutstr( char * const str )
{
    size_t const len = strlen_null( str );
    return ( StringM ){ .e = str, .length = len, .capacity = len };
}


StringM
stringm__from_arraym( ArrayM_char const xs )
{
    return vec_char__from_arraym( xs );
}


StringM
stringm__from_vec( Vec_char const v )
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
stringm__free( StringM * const s )
{
    vec_char__free( s );
}


void
stringm__set_capacity( StringM * const s,
                       size_t const new_capacity )
{
    vec_char__set_capacity( s, new_capacity );
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


void
stringm__append( StringM * const s,
                 char const c )
{
    vec_char__append( s, c );
}


void stringm__extend_stringc( StringM * const s, StringC const ext )
    { vec_char__extend( s, ext ); }

void stringm__extend_stringm( StringM * const s, StringM const ext )
    { vec_char__extend( s, ext ); }

void stringm__extend_arrayc( StringM * const s, ArrayC_char const ext )
    { vec_char__extend( s, ext ); }

void stringm__extend_arraym( StringM * const s, ArrayM_char const ext )
    { vec_char__extend( s, ext ); }

void stringm__extend_vec( StringM * const s, Vec_char const ext )
    { vec_char__extend( s, ext ); }

void stringm__extend_str( StringM * const s, char const * const ext )
    { vec_char__extend( s, stringc__from( ext ) ); }


void
stringm__extend_fmt_stringc( StringM * const s,
                             StringC const format,
                             ... )
{
    ASSERT( s != NULL, stringm__is_valid( *s ), stringc__is_valid( format ) );

    va_list ap;
    va_start( ap, format );
    stringm__extend_fmtv( s, format, ap );
    int const err = errno;
    va_end( ap );
    if ( err ) { errno = err; }
}


void
stringm__extend_fmt_stringm( StringM * const s,
                             StringM const format,
                             ... )
{
    ASSERT( s != NULL, stringm__is_valid( *s ), stringm__is_valid( format ) );

    va_list ap;
    va_start( ap, format );
    stringm__extend_fmtv( s, format, ap );
    int const err = errno;
    va_end( ap );
    if ( err ) { errno = err; }
}


void
stringm__extend_fmt_arrayc( StringM * const s,
                            ArrayC_char const format,
                            ... )
{
    ASSERT( s != NULL,
            stringm__is_valid( *s ),
            arrayc_char__is_valid( format ) );

    va_list ap;
    va_start( ap, format );
    stringm__extend_fmtv( s, format, ap );
    int const err = errno;
    va_end( ap );
    if ( err ) { errno = err; }
}


void
stringm__extend_fmt_arraym( StringM * const s,
                            ArrayM_char const format,
                            ... )
{
    ASSERT( s != NULL,
            stringm__is_valid( *s ),
            arraym_char__is_valid( format ) );

    va_list ap;
    va_start( ap, format );
    stringm__extend_fmtv( s, format, ap );
    int const err = errno;
    va_end( ap );
    if ( err ) { errno = err; }
}


void
stringm__extend_fmt_vec( StringM * const s,
                         Vec_char const format,
                         ... )
{
    ASSERT( s != NULL, stringm__is_valid( *s ), vec_char__is_valid( format ) );

    va_list ap;
    va_start( ap, format );
    stringm__extend_fmtv( s, format, ap );
    int const err = errno;
    va_end( ap );
    if ( err ) { errno = err; }
}


void
stringm__extend_fmt_str( StringM * const s,
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
stringm__extend_fmtv_stringc( StringM * const s,
                              StringC const format,
                              va_list ap )
{
    ASSERT( s != NULL, stringm__is_valid( *s ), stringc__is_valid( format ) );

    errno = 0;
    char * const format_str = mutstr__from_stringc( format );
    if ( errno ) { return; }
    stringm__extend_fmtv( s, format_str, ap );
    int const err = errno;
    free( format_str );
    if ( err ) { errno = err; }
}


void
stringm__extend_fmtv_stringm( StringM * const s,
                              StringM const format,
                              va_list ap )
{
    stringm__extend_fmtv( s, stringc__from( format ), ap );
}


void
stringm__extend_fmtv_arrayc( StringM * const s,
                             ArrayC_char const format,
                             va_list ap )
{
    stringm__extend_fmtv( s, stringc__from( format ), ap );
}


void
stringm__extend_fmtv_arraym( StringM * const s,
                             ArrayM_char const format,
                             va_list ap )
{
    stringm__extend_fmtv( s, stringc__from( format ), ap );
}


void
stringm__extend_fmtv_vec( StringM * const s,
                          Vec_char const format,
                          va_list ap )
{
    stringm__extend_fmtv( s, stringc__from( format ), ap );
}


void
stringm__extend_fmtv_str( StringM * const s,
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
stringc__equal_stringc( StringC const x,
                        StringC const y )
{
    return arrayc_char__equal( x, y );
}


bool
stringc__equal_stringm( StringC const x,
                        StringM const y )
{
    return stringc__equal( x, stringc__from( y ) );
}


bool
stringc__equal_arrayc( StringC const x,
                       ArrayC_char const y )
{
    return stringc__equal( x, stringc__from( y ) );
}


bool
stringc__equal_arraym( StringC const x,
                       ArrayM_char const y )
{
    return stringc__equal( x, stringc__from( y ) );
}


bool
stringc__equal_vec( StringC const x,
                    Vec_char const y )
{
    return stringc__equal( x, stringc__from( y ) );
}


bool
stringc__equal_str( StringC const x,
                    char const * const y )
{
    return stringc__equal( x, stringc__from( y ) );
}


bool
stringm__equal_stringc( StringM const x,
                        StringC const y )
{
    return stringc__equal( stringc__from( x ), y );
}


bool
stringm__equal_stringm( StringM const x,
                        StringM const y )
{
    return stringm__equal( x, stringc__from( y ) );
}


bool
stringm__equal_arrayc( StringM const x,
                       ArrayC_char const y )
{
    return stringm__equal( x, stringc__from( y ) );
}


bool
stringm__equal_arraym( StringM const x,
                       ArrayM_char const y )
{
    return stringm__equal( x, stringc__from( y ) );
}


bool
stringm__equal_vec( StringM const x,
                    Vec_char const y )
{
    return stringm__equal( x, stringc__from( y ) );
}


bool
stringm__equal_str( StringM const x,
                    char const * const y )
{
    return stringm__equal( x, stringc__from( y ) );
}


char *
mutstr__from_stringc( StringC const string )
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
mutstr__from_stringm( StringM const string )
{
    ASSERT( stringm__is_valid( string ) );

    return mutstr__from_stringc( stringc__from_stringm( string ) );
}


ArrayC_char
arrayc_char__from_stringc( StringC const s )
{
    return s;
}


ArrayC_char
arrayc_char__from_stringm( StringM const s )
{
    return arrayc_char__from_stringc( stringc__from_stringm( s ) );
}


ArrayM_char
arraym_char__from_stringm( StringM const s )
{
    return arraym_char__from_vec( s );
}


Vec_char
vec_char__from_stringm( StringM const s )
{
    return s;
}


