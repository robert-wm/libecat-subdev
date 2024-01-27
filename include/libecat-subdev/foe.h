/**
 * \file foe.h
 *
 * \author Robert Burger <robert@burger-system.de>
 *
 * \date 15 Nov 2023
 *
 * \brief ECSD FoE header.
 *
 * ECSD FoE header.
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

#ifndef LIBECAT_SUBDEV_FOE_H
#define LIBECAT_SUBDEV_FOE_H

#include <stdint.h>
#include <libecat-subdev/mailbox.h>
#include <libecat-subdev/ecsd.h>
#include <libecat-base/types.h>
//#include <libecat-base/foe.h>

#define FOE_STATE_OFF       ((uint8_t)  0u)
#define FOE_STATE_IDLE      ((uint8_t)  1u)
#define FOE_STATE_DWLS      ((uint8_t)  2u)
#define FOE_STATE_UPLS      ((uint8_t)  3u)
#define FOE_STATE_UWUPD     ((uint8_t)  4u)
#define FOE_STATE_WUPD      ((uint8_t)  5u)
#define FOE_STATE_DWUPD     ((uint8_t)  6u)
#define FOE_STATE_DWSEG     ((uint8_t)  7u)
#define FOE_STATE_UWSEG     ((uint8_t)  8u)
#define FOE_STATE_ERR       ((uint8_t)255u)

struct ecsd_foe {
    ec_uint8_t state;
};

struct ec_foe_service_data {
};

//! \brief 
/*!
 * Refer to Service Definition ETG.1000.5
 */
ec_retval_t ecsd_foe_mailbox_write_ind(struct ecsd *pecsd, struct ec_foe_service_data *mbx);

//! \brief 
/*!
 * Refer to Service Definition ETG.1000.5
 */
ec_retval_t ecsd_foe_mailbox_read_ind(struct ecsd *pecsd, struct ec_foe_service_data *mbx);

//! \brief 
/*!
 * Refer to Service Definition ETG.1000.5
 */
ec_retval_t ecsd_foe_mailbox_data_ind(struct ecsd *pecsd, struct ec_foe_service_data *mbx);

//! \brief 
/*!
 * Refer to Service Definition ETG.1000.5
 */
ec_retval_t ecsd_foe_mailbox_ack_ind(struct ecsd *pecsd, struct ec_foe_service_data *mbx);

//! \brief 
/*!
 * Refer to Service Definition ETG.1000.5
 */
ec_retval_t ecsd_foe_mailbox_error_ind(struct ecsd *pecsd, struct ec_foe_service_data *mbx);

//! \brief
/*!
 */
ec_retval_t ecsd_foe_mailbox_start(struct ecsd *pecsd);

//! \brief
/*!
 */
ec_retval_t ecsd_foe_mailbox_stop(struct ecsd *pecsd);

#endif /* LIBECAT_SUBDEV_FOE_H */


