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

#ifndef LIBECAT_SUBDEV_HW_H
#define LIBECAT_SUBDEV_HW_H

#include <libecat-subdev/types.h>

typedef struct hw {
    ecsd_uint8_t *ecsd_base;
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
ecsd_size_t hw_read(ecsd_t *pesc, ecsd_off_t adr, ecsd_uint8_t *buf, ecsd_size_t buf_len);

//! \brief copy bytes from local memory to ESC memory
/*!
 * \param[in]   pesc        Pointer to ESC struture.
 * \param[in]   adr         ESC memory address to copy to.
 * \param[in]   buf         Target buffer to copy from.
 * \param[in]   buf_len     Length of target buffer.
 *
 * \return Number of bytes copied.
 */
ecsd_size_t hw_write(ecsd_t *pesc, ecsd_off_t adr, ecsd_uint8_t *buf, ecsd_size_t buf_len);

//! \brief Opens and initializes ESC hardware
/*!
 * \param[in]   pesc        Pointer to ESC struture.
 * \param[in]   ecsd_base    Base address of ESC memory.
 * \param[in]   pdi_type    Type of PDI waiting for ESC to become ready.
 *
 * \retval  OK on success.
 */
ecsd_uint32_t hw_open(ecsd_t *pesc, void *ecsd_base, ecsd_uint16_t pdi_type);

#endif // LIBECAT_SUBDEV_HW_H

