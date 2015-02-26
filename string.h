
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


#ifndef LIBSTRING_STRING_H
#define LIBSTRING_STRING_H


#include <stdarg.h>

#include <libtypes/types.h>
#include <libbase/char.h>

#include "string-def.h" // StringC, StringM


// TODO: abstract this to a library / header file some day?
#ifdef HAVE_ATTRIBUTE_FORMAT
#define ATTR_FORMAT __attribute__((format(printf, 2, 3)))
#else
#define ATTR_FORMAT
#endif


#define STRINGC( STR ) \
    { .e = ( STR ), \
      .length = ( sizeof ( STR ) ) - 1 }


bool
stringc__is_valid( StringC );


bool
stringm__is_valid( StringM );


StringC
stringc__new( char const * str,
              size_t length );


StringC stringc__from_stringm( StringM );
StringC stringc__from_arrayc ( ArrayC_char );
StringC stringc__from_arraym ( ArrayM_char );
StringC stringc__from_vec    ( Vec_char );
StringC stringc__from_str    ( char const * str );

#define stringc__from( X ) \
    _Generic( ( X ), \
        ArrayC_char: stringc__from_arrayc, \
        ArrayM_char: stringc__from_arraym, \
        Vec_char:    stringc__from_vec, \
        default:     stringc__from_str \
    )( X )


void
stringm__free( StringM * );


StringM
stringm__new( char const * str,
              size_t length,
              size_t capacity );


StringM
stringm__new_empty( size_t capacity );


StringM
stringm__new_fmt( char const * format,
                  ... )
    ATTR_FORMAT;


void
stringm__fmt_into( StringM *,
                   char const * format,
                   ... );


StringM stringm__from_mutstr( char * str );
StringM stringm__from_arraym( ArrayM_char );
StringM stringm__from_vec   ( Vec_char );

#define stringm__from( X ) \
    _Generic( ( X ), \
        ArrayM_char: stringm__from_arraym, \
        Vec_char:    stringm__from_vec, \
        default:     stringm__from_mutstr \
    )( X )


StringM stringm__copy_stringc( StringC );
StringM stringm__copy_stringm( StringM );
StringM stringm__copy_arrayc ( ArrayC_char );
StringM stringm__copy_arraym ( ArrayM_char );
StringM stringm__copy_vec    ( Vec_char );
StringM stringm__copy_str    ( char const * str );

#define stringm__copy( X ) \
    _Generic( ( X ), \
        ArrayC_char: stringm__copy_arrayc, \
        ArrayM_char: stringm__copy_arraym, \
        Vec_char:    stringm__copy_vec, \
        default:     stringm__copy_str \
    )( X )


void stringm__copy_stringc_into( StringM *, StringC );
void stringm__copy_stringm_into( StringM *, StringM );
void stringm__copy_arrayc_into( StringM *, ArrayC_char );
void stringm__copy_arraym_into( StringM *, ArrayM_char );
void stringm__copy_vec_into( StringM *, Vec_char );
void stringm__copy_str_into( StringM *, char const * );

#define stringm__copy_into( S, FROM ) \
    _Generic( ( FROM ), \
        ArrayC_char: stringm__copy_arrayc_into, \
        ArrayM_char: stringm__copy_arraym_into, \
        Vec_char:    stringm__copy_vec_into, \
        default:     stringm__copy_str_into \
    )( S, FROM )


void
stringm__set_capacity( StringM *,
                       size_t new_capacity );


void
stringm__grow_capacity( StringM * );


void
stringm__shrink_capacity( StringM * );


void
stringm__append( StringM *,
                 char );


void stringm__extend_stringc( StringM *, StringC );
void stringm__extend_stringm( StringM *, StringM );
void stringm__extend_arrayc ( StringM *, ArrayC_char );
void stringm__extend_arraym ( StringM *, ArrayM_char );
void stringm__extend_vec    ( StringM *, Vec_char );
void stringm__extend_str    ( StringM *, char const * str );

#define stringm__extend( STRING, EXT ) \
    _Generic( ( EXT ), \
        ArrayC_char: stringm__extend_arrayc, \
        ArrayM_char: stringm__extend_arraym, \
        Vec_char:    stringm__extend_vec, \
        default:     stringm__extend_str \
    )( STRING, EXT )


void
stringm__extend_fmt( StringM *,
                     char const * format,
                     ... )
    ATTR_FORMAT;


void
stringm__extend_fmtv( StringM *,
                      char const * format,
                      va_list );


bool stringc__equal_stringc( StringC, StringC );
bool stringc__equal_stringm( StringC, StringM );
bool stringc__equal_arrayc ( StringC, ArrayC_char );
bool stringc__equal_arraym ( StringC, ArrayM_char );
bool stringc__equal_vec    ( StringC, Vec_char );
bool stringc__equal_str    ( StringC, char const * str );

#define stringc__equal( STRING, X ) \
    _Generic( ( X ), \
        ArrayC_char: stringc__equal_arrayc, \
        ArrayM_char: stringc__equal_arraym, \
        Vec_char:    stringc__equal_vec, \
        default:     stringc__equal_str \
    )( STRING, X )


bool stringm__equal_stringc( StringM, StringC );
bool stringm__equal_stringm( StringM, StringM );
bool stringm__equal_arrayc( StringM, ArrayC_char );
bool stringm__equal_arraym( StringM, ArrayM_char );
bool stringm__equal_vec( StringM, Vec_char );
bool stringm__equal_str( StringM, char const * str );

#define stringm__equal( STRING, X ) \
    _Generic( ( X ), \
        ArrayC_char: stringm__equal_arrayc, \
        ArrayM_char: stringm__equal_arraym, \
        Vec_char:    stringm__equal_vec, \
        default:     stringm__equal_str \
    )( STRING, X )


char *
mutstr__from_stringc( StringC );


char *
mutstr__from_stringm( StringM );


ArrayC_char
arrayc_char__from_stringc( StringC );


ArrayC_char
arrayc_char__from_stringm( StringM );


ArrayM_char
arraym_char__from_stringm( StringM );


Vec_char
vec_char__from_stringm( StringM );


#undef ATTR_FORMAT


#endif

