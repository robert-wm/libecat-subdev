/**
 * \file hw.c
 *
 * \author Robert Burger <robert@burger-system.de>
 *
 * \date 12 Oct 2026
 *
 * \brief Weak default implementations of the ECSD hardware hooks.
 *
 * Weak default implementations of the ECSD hardware hooks.
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

#include <stddef.h>

#include <libecat-base/types.h>
#include <libecat-subdev/hw.h>

/*
 * All hooks have weak no-op defaults so the library links on its own.
 * An application provides strong definitions (in its board abstraction)
 * for ecsd_hw_read(), ecsd_hw_write() and ecsd_hw_enable_int(); the
 * sync line hooks are optional for boards without dedicated SYNC
 * interrupt lines. An override replaces the no-op default at link time.
 */

//! \brief copy bytes from ECSD memory to local memory (weak default: no-op).
/*!
 * \param[in]   hw_hdl      Pointer to hardware handle.
 * \param[in]   adr         ECSD memory address to copy from.
 * \param[out]  buf         Target buffer to copy to.
 * \param[in]   buf_len     Length of target buffer.
 *
 * \return Number of bytes copied.
 */
ec_size_t __attribute__((weak)) ecsd_hw_read(void *hw_hdl, ec_off_t adr, ec_uint8_t *buf, ec_size_t buf_len) {
    (void)hw_hdl;
    (void)adr;
    (void)buf;
    (void)buf_len;

    return 0;
}

//! \brief copy bytes from local memory to ECSD memory (weak default: no-op).
/*!
 * \param[in]   hw_hdl      Pointer to hardware handle.
 * \param[in]   adr         ECSD memory address to copy to.
 * \param[in]   buf         Target buffer to copy from.
 * \param[in]   buf_len     Length of target buffer.
 *
 * \return Number of bytes copied.
 */
ec_size_t __attribute__((weak)) ecsd_hw_write(void *hw_hdl, ec_off_t adr, ec_uint8_t *buf, ec_size_t buf_len) {
    (void)hw_hdl;
    (void)adr;
    (void)buf;
    (void)buf_len;

    return 0;
}

//! \brief Enable PDI interrupt in hardware (weak default: no-op).
/*!
 * \param[in]   hw_hdl      Pointer to hardware handle.
 * \param[in]   enable      Flag indicating if irq should be enabled or disabled.
 *
 * \return Old enable state.
 */
ec_uint32_t __attribute__((weak)) ecsd_hw_enable_int(void *hw_hdl, ec_bool_t enable) {
    (void)hw_hdl;
    (void)enable;

    return 0u;
}

//! \brief Enable the SYNC0 interrupt line in hardware (weak default: no-op).
/*!
 * \param[in]   pecsd       Pointer to ECSD struture.
 * \param[in]   enable      Flag indicating if irq should be enabled or disabled.
 */
void __attribute__((weak)) ecsd_hw_enable_sync0(struct ecsd *pecsd, ec_bool_t enable) {
    (void)pecsd;
    (void)enable;
}

//! \brief Enable the SYNC1 interrupt line in hardware (weak default: no-op).
/*!
 * \param[in]   pecsd       Pointer to ECSD struture.
 * \param[in]   enable      Flag indicating if irq should be enabled or disabled.
 */
void __attribute__((weak)) ecsd_hw_enable_sync1(struct ecsd *pecsd, ec_bool_t enable) {
    (void)pecsd;
    (void)enable;
}
