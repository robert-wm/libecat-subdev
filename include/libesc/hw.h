/**
 * \file hw.h
 *
 * \author Robert Burger <robert@burger-system.de>
 *
 * \date 25 Oct 2023
 *
 * \brief ESC hardware header.
 *
 * ESC hardware header.
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

#ifndef LIBESC_HW_H
#define LIBESC_HW_H

#include <libesc/types.h>

typedef struct hw {
    osal_uint8_t *esc_base;
} hw_t;

//! \brief copy bytes from ESC memory to local memory
/*!
 * \param[in]   pesc        Pointer to ESC struture.
 * \param[in]   adr         ESC memory address to copy from.
 * \param[out]  buf         Target buffer to copy to.
 * \param[in]   buf_len     Length of target buffer.
 *
 * \return Number of bytes copied.
 */
osal_size_t hw_read(esc_t *pesc, osal_off_t adr, osal_uint8_t *buf, osal_size_t buf_len);

//! \brief copy bytes from local memory to ESC memory
/*!
 * \param[in]   pesc        Pointer to ESC struture.
 * \param[in]   adr         ESC memory address to copy to.
 * \param[in]   buf         Target buffer to copy from.
 * \param[in]   buf_len     Length of target buffer.
 *
 * \return Number of bytes copied.
 */
osal_size_t hw_write(esc_t *pesc, osal_off_t adr, osal_uint8_t *buf, osal_size_t buf_len);

//! \brief Opens and initializes ESC hardware
/*!
 * \param[in]   pesc        Pointer to ESC struture.
 * \param[in]   esc_base    Base address of ESC memory.
 * \param[in]   pdi_type    Type of PDI waiting for ESC to become ready.
 *
 * \retval  OK on success.
 */
osal_uint32_t hw_open(esc_t *pesc, void *esc_base, osal_uint16_t pdi_type);

#endif // LIBESC_HW_H

