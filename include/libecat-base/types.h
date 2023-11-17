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

typedef char        ec_char_t;
typedef int8_t      ec_int8_t;
typedef int16_t     ec_int16_t;
typedef int32_t     ec_int32_t;
typedef int32_t     ec_retval_t;

typedef uint8_t     ec_data_t;
typedef uint8_t     ec_uint8_t;
typedef uint8_t     ec_bool_t;
typedef uint16_t    ec_uint16_t;
typedef uint16_t    ec_state_transition_t;
typedef uint32_t    ec_uint32_t;
typedef uint64_t    ec_off_t;
typedef uint64_t    ec_size_t;

#define PACKED      __attribute__((__packed__))

typedef void        ec_void_t;
typedef ec_uint32_t    ec_abort_code;

#define EC_TRUE   1
#define EC_FALSE  0
#define EC_OK      0

#endif
