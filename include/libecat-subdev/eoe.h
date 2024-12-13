/**
 * \file eoe.h
 *
 * \author Robert Burger <robert@burger-system.de>
 *
 * \date 15 Nov 2023
 *
 * \brief ECSD CoE header.
 *
 * ECSD CoE header.
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

#ifndef LIBECAT_SUBDEV_EOE_H
#define LIBECAT_SUBDEV_EOE_H

#include <stdint.h>
#include <libecat-subdev/mailbox.h>
#include <libecat-subdev/ecsd.h>
#include <libecat-base/types.h>
//#include <libecat-base/eoe.h>

#define EOE_STATE_OFF       ((uint8_t)0x00u)
#define EOE_STATE_IDLE      ((uint8_t)0x01u)
#define EOE_STATE_DWLE      ((uint8_t)0x02u)
#define EOE_STATE_DWLN      ((uint8_t)0x03u)
#define EOE_STATE_DWLS      ((uint8_t)0x04u)
#define EOE_STATE_UPLE      ((uint8_t)0x05u)
#define EOE_STATE_UPLN      ((uint8_t)0x06u)
#define EOE_STATE_UPLS      ((uint8_t)0x07u)
#define EOE_STATE_INF       ((uint8_t)0x08u)
#define EOE_STATE_WUPD      ((uint8_t)0x09u)
#define EOE_STATE_WSEG      ((uint8_t)0x0Au)
#define EOE_STATE_ERR       ((uint8_t)0xFFu)

//! \brief 
/*!
 * Refer to Mailbox Read Upd Service Definition ETG.1000.3
 */
ec_retval_t ecsd_eoe_mailbox_read_upd_req(struct ecsd *pecsd, struct ec_mailbox *mbx);

//! \brief 
/*!
 * Refer to Service Definition ETG.1000.3
 */
ec_retval_t ecsd_eoe_mailbox_write_ind(struct ecsd *pecsd, struct ec_mailbox *mbx);

//! \brief 
/*!
 * Refer to Service Definition ETG.1000.3
 */
ec_retval_t ecsd_eoe_mailbox_read_upd_cnf(struct ecsd *pecsd, ec_bool_t success);

//! \brief
/*!
 * Refer to Service Definition ETG.1000.3
 */
ec_retval_t ecsd_eoe_mailbox_start(struct ecsd *pecsd);

//! \brief
/*!
 * Refer to Service Definition ETG.1000.3
 */
ec_retval_t ecsd_eoe_mailbox_stop(struct ecsd *pecsd);

#endif // LIBECAT_SUBDEV_EOE_H


