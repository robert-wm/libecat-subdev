/**
 * \file types.h
 *
 * \author Robert Burger <robert@burger-system.de>
 *
 * \date 25 Oct 2023
 *
 * \brief ESC types header.
 *
 * ESC types header.
 */

/*
 * This file is part of libecat-subdev.
 *
 * libecat-subdev is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 3 of the License, or (at your option) any later version.
 * 
 * libecat-subdev is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public License
 * along with libecat-subdev; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */


#ifndef LIBECAT_SUBDEV_TYPES_H
#define LIBECAT_SUBDEV_TYPES_H

#include <stdint.h>

typedef int8_t      ecsd_int8_t;
typedef int16_t     ecsd_int16_t;
typedef int32_t     ecsd_int32_t;
typedef uint8_t     ecsd_uint8_t;
typedef uint16_t    ecsd_uint16_t;
typedef uint32_t    ecsd_uint32_t;
typedef char        ecsd_char_t;

typedef uint64_t    ecsd_off_t;

#define PACKED      __attribute__((__packed__))

typedef int32_t     ecsd_retval_t;

typedef uint64_t    ecsd_size_t;

typedef uint8_t     ecsd_bool_t;

typedef uint16_t    ecsd_state_transition_t;

typedef void        ecsd_void_t;

typedef uint8_t     ecsd_data_t;

#define ECSD_TRUE   1
#define ECSD_FALSE  0
#define ECSD_OK      0

#endif
