
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


#ifndef LIBSTRING_DEF_STRING_H
#define LIBSTRING_DEF_STRING_H


#include <libtypes/types.h>
#include <libmacro/logic.h>


typedef struct stringc {
    char const * e;
    size_t length;
} StringC;

#define STRINGC_INVARIANTS( S ) \
    IMPLIES( ( S ).e == NULL, ( S ).length == 0 )

#define STRINGC( STR ) \
    { .e      = ( STR ), \
      .length = ( sizeof ( STR ) ) - 1 }

#define STRINGC0( STR ) \
    { .e      = ( STR ), \
      .length = sizeof ( STR ) }


typedef struct stringm {
    char * e;
    size_t length;
    size_t capacity;
} StringM;

#define STRINGM_INVARIANTS( S ) \
    IMPLIES( ( S ).e == NULL, ( S ).length == 0 ), \
    ( S ).length <= ( S ).capacity

#define STRINGM( STR ) \
    { .e        = ( char[] ){ STR }, \
      .length   = ( sizeof ( STR ) ) - 1, \
      .capacity = sizeof ( STR ) }

#define STRINGM0( STR ) \
    { .e        = ( char[] ){ STR }, \
      .length   = sizeof ( STR ), \
      .capacity = sizeof ( STR ) }


#endif

