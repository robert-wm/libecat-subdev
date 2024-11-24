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

//! \brief copy bytes from ECSD memory to local memory
/*!
 * \param[in]   hw_hdl      Pointer to hardware handle.
 * \param[in]   adr         ECSD memory address to copy from.
 * \param[out]  buf         Target buffer to copy to.
 * \param[in]   buf_len     Length of target buffer.
 *
 * \return Number of bytes copied.
 */
typedef ec_size_t (*ecsd_hw_read)(void *hw_hdl, ec_off_t adr, ec_uint8_t *buf, ec_size_t buf_len);

//! \brief copy bytes from local memory to ECSD memory
/*!
 * \param[in]   hw_hdl      Pointer to hardware handle.
 * \param[in]   adr         ECSD memory address to copy to.
 * \param[in]   buf         Target buffer to copy from.
 * \param[in]   buf_len     Length of target buffer.
 *
 * \return Number of bytes copied.
 */
typedef ec_size_t (*ecsd_hw_write)(void *hw_hdl, ec_off_t adr, ec_uint8_t *buf, ec_size_t buf_len);

//! \brief Enable PDI interrupt in hardware.
/*!
 * \param[in]   hw_hdl      Pointer to hardware handle.
 * \param[in]   enable      Flag indicating if irq should be enabled or disabled.
 *
 * \return Old enable state.
 */
typedef ec_uint32_t (*ecsd_hw_enable_int)(void *hw_hdl, ec_bool_t enable);

//! \brief Opens and initializes ECSD hardware
/*!
 * \param[in]   pecsd        Pointer to ECSD struture.
 * \param[in]   ecsd_base    Base address of ECSD memory.
 * \param[in]   pdi_type    Type of PDI waiting for ECSD to become ready.
 *
 * \retval  OK on success.
 */
typedef ec_uint32_t (*ecsd_hw_open)(struct ecsd *pecsd, void *ecsd_base, ec_uint16_t pdi_type);

//! \brief Enable SyncX interrupt in hardware.
/*!
 * \param[in]   pecsd       Pointer to ECSD struture.
 * \param[in]   enable      Flag indicating if irq should be enabled or disabled.
 */
typedef void (*ecsd_hw_enable_syncx)(struct ecsd *pecsd, ec_bool_t enable);

struct hw {
    void *hdl;
    ecsd_hw_read read;
    ecsd_hw_write write;
    ecsd_hw_enable_int enable_int;
    ecsd_hw_enable_syncx enable_sync0;
    ecsd_hw_enable_syncx enable_sync1;

    ec_bool_t sync0_mapped_to_al_status;
    ec_bool_t sync1_mapped_to_al_status;
};

#endif // LIBECAT_SUBDEV_HW_H

