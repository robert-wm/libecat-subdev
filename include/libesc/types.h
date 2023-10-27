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
 * This file is part of libesc.
 *
 * libesc is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 3 of the License, or (at your option) any later version.
 * 
 * libesc is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public License
 * along with libesc; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */


#ifndef LIBESC_TYPES_H
#define LIBESC_TYPES_H

typedef uint8_t     esc_uint8_t;
typedef uint16_t    esc_uint16_t;
typedef uint16_t    osal_uint16_t;
typedef uint8_t     osal_uint8_t;
typedef uint32_t    osal_uint32_t;

#define PACKED      __attribute__((__packed__))

typedef int32_t     esc_retval_t;

typedef uint64_t    osal_size_t;
typedef uint64_t    osal_off_t;

typedef uint8_t     osal_bool_t;
typedef uint8_t     esc_bool_t;

typedef uint16_t ec_state_transition_t;

typedef void        osal_void_t;
typedef void        esc_void_t;

#define OSAL_TRUE   1
#define OSAL_FALSE  0

#define ESC_TRUE   1
#define ESC_FALSE  0
#define ESC_OK      0

#endif
