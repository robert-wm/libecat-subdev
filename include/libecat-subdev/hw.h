/**
 * \file hw.h
 *
 * \author Robert Burger <robert@burger-system.de>
 *
 * \date 25 Oct 2023
 *
 * \brief ECSD hardware header.
 *
 * ECSD hardware header.
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

#include <libecat-base/types.h>

// forward declarations
struct ecsd;

struct hw {
    ec_uint8_t *ecsd_base;

    ec_bool_t sync0_mapped_to_al_status;
    ec_bool_t sync1_mapped_to_al_status;
};

//! \brief copy bytes from ECSD memory to local memory
/*!
 * \param[in]   pecsd        Pointer to ECSD struture.
 * \param[in]   adr         ECSD memory address to copy from.
 * \param[out]  buf         Target buffer to copy to.
 * \param[in]   buf_len     Length of target buffer.
 *
 * \return Number of bytes copied.
 */
ec_size_t hw_read(struct ecsd *pecsd, ec_off_t adr, ec_uint8_t *buf, ec_size_t buf_len);

//! \brief copy bytes from local memory to ECSD memory
/*!
 * \param[in]   pecsd        Pointer to ECSD struture.
 * \param[in]   adr         ECSD memory address to copy to.
 * \param[in]   buf         Target buffer to copy from.
 * \param[in]   buf_len     Length of target buffer.
 *
 * \return Number of bytes copied.
 */
ec_size_t hw_write(struct ecsd *pecsd, ec_off_t adr, ec_uint8_t *buf, ec_size_t buf_len);

//! \brief Opens and initializes ECSD hardware
/*!
 * \param[in]   pecsd        Pointer to ECSD struture.
 * \param[in]   ecsd_base    Base address of ECSD memory.
 * \param[in]   pdi_type    Type of PDI waiting for ECSD to become ready.
 *
 * \retval  OK on success.
 */
ec_uint32_t hw_open(struct ecsd *pecsd, void *ecsd_base, ec_uint16_t pdi_type);

#endif // LIBECAT_SUBDEV_HW_H

