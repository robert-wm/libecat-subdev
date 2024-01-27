/**
 * \file foe.c
 *
 * \author Robert Burger <robert@burger-system.de>
 *
 * \date 27 Jan 2024
 *
 * \brief ECSD FoE source.
 *
 * ECSD FoE source.
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

#include <libecat-base/object.h>
#include <libecat-subdev/coe.h>

#include <assert.h>
#include <stddef.h>

//! \brief 
/*!
 * Refer to Service Definition ETG.1000.5
 */
ec_retval_t ecsd_foe_mailbox_write_ind(struct ecsd *pecsd, struct ec_foe_service_data *mbx) {
    ec_retval_t ret = EC_OK;

    return ret;
}

//! \brief 
/*!
 * Refer to Service Definition ETG.1000.5
 */
ec_retval_t ecsd_foe_mailbox_read_ind(struct ecsd *pecsd, struct ec_foe_service_data *mbx) {
    ec_retval_t ret = EC_OK;

    return ret;
}

//! \brief 
/*!
 * Refer to Service Definition ETG.1000.5
 */
ec_retval_t ecsd_foe_mailbox_data_ind(struct ecsd *pecsd, struct ec_foe_service_data *mbx) {
    ec_retval_t ret = EC_OK;

    return ret;
}

//! \brief 
/*!
 * Refer to Service Definition ETG.1000.5
 */
ec_retval_t ecsd_foe_mailbox_ack_ind(struct ecsd *pecsd, struct ec_foe_service_data *mbx) {
    ec_retval_t ret = EC_OK;

    return ret;
}

//! \brief 
/*!
 * Refer to Service Definition ETG.1000.5
 */
ec_retval_t ecsd_foe_mailbox_error_ind(struct ecsd *pecsd, struct ec_foe_service_data *mbx) {
    ec_retval_t ret = EC_OK;

    return ret;
}

//! \brief
/*!
 */
ec_retval_t ecsd_foe_mailbox_start(struct ecsd *pecsd) {
    ec_retval_t ret = EC_OK;

    switch (pecsd->mbx.foe.state) {
        case FOE_STATE_OFF: {
            // #1
            pecsd->mbx.foe.state = FOE_STATE_IDLE;
            break;
        }
        case FOE_STATE_IDLE: 
        case FOE_STATE_DWLS:
        case FOE_STATE_UPLS:
        case FOE_STATE_UWUPD:
        case FOE_STATE_WUPD:
        case FOE_STATE_DWUPD:
        case FOE_STATE_DWSEG:
        case FOE_STATE_UWSEG:
        case FOE_STATE_ERR: {
            // #3 | #13 | #18 | #24 | #28 | #32 | #36 | #40 | #47
            break;
        }
        default: {
            break;
        }
    }

    return ret;
}

//! \brief
/*!
 */
ec_retval_t ecsd_foe_mailbox_stop(struct ecsd *pecsd) {
    ec_retval_t ret = EC_OK;

    switch (pecsd->mbx.foe.state) {
        case FOE_STATE_OFF: {
            // #2
            break;
        }
        case FOE_STATE_IDLE: {
            // #4
            break;
        }
        case FOE_STATE_DWLS: {
            // #14
            // TODO
            // CANCEL SERVICE
            break;
        }
        case FOE_STATE_UPLS: {
            // #19
            // TODO
            // CANCEL SERVICE
            break;
        }
        case FOE_STATE_UWUPD: {
            // #25
            // TODO
            // RESET MAILBOX
            break;
        }
        case FOE_STATE_WUPD: {
            // #29
            // TODO
            // RESET MAILBOX
            break;
        }
        case FOE_STATE_DWUPD: {
            // #33
            // TODO
            // RESET MAILBOX
            break;
        }
        case FOE_STATE_DWSEG: {
            // #41
            break;
        }
        case FOE_STATE_UWSEG: {
            // #48
            break;
        }
        case FOE_STATE_ERR: {
            // #37
            // TODO
            // RESET MAILBOX
            break;
        }
        default: {
            break;
        }
    }

    pecsd->mbx.foe.state = FOE_STATE_OFF;

    return ret;
}
