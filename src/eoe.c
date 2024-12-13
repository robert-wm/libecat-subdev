/**
 * \file eoe.c
 *
 * \author Robert Burger <robert@burger-system.de>
 *
 * \date 15 Nov 2023
 *
 * \brief ESC CoE source.
 *
 * ESC CoE source.
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

#include <libecat-subdev/eoe.h>

/* include generated object dictionary */
#include "object.h"

#include <assert.h>
#include <stddef.h>
#include <string.h>

#ifdef ECSD_MAILBOX_EOE_SUPPORT

// forward declarations

ec_void_t ecsd_eoe_mailbox_error_reply(struct ecsd *pecsd, ec_uint8_t detail, struct ec_mailbox_error_reply *mbx) 
{
    mbx->mailbox_hdr.length  = 4u;
    EC_MBX_HDR__TYPE__SET(&mbx->mailbox_hdr, EC_MBX_HDR__TYPE__ERR);
    mbx->type                = EC_MBX_ERROR_REPLY__TYPE__MAILBOX_COMMAND;
    mbx->detail              = detail;

    ecsd_eoe_mailbox_read_upd_req(pecsd, (struct ec_mailbox *)mbx);

    pecsd->mbx.eoe.state = EOE_STATE_ERR;
}

//! \brief 
/*!
 * Refer to Mailbox Read Upd Service Definition ETG.1000.3
 */
ec_retval_t ecsd_eoe_mailbox_read_upd_req(struct ecsd *pecsd, struct ec_mailbox *mbx) {
    assert (pecsd != NULL);
    ec_retval_t ret = 0u;

    // Send update request to MainDevice.
    ecsd_mailbox_read_upd_req(pecsd, mbx);

    // Store for repeat or continuation.
    memcpy(&pecsd->mbx.eoe.repeat_mbx_buffer[0], (void *)mbx, ECSD_MAILBOX_READ__MAX_LENGTH);

    return ret;
}

//! \brief 
/*!
 * Refer to Service Definition ETG.1000.3
 */
ec_retval_t ecsd_eoe_mailbox_write_ind(struct ecsd *pecsd, struct ec_eoe_service_data *mbx) {
    assert(pecsd != NULL);

    ec_retval_t ret = 0;

    switch (pecsd->mbx.eoe.state) {
        default:
            break;
    }

    return ret;
}

//! \brief CoE Mailbox read update confirmation.
/*!
 * \details This function is called when the MainDevice has read the "read" mailbox.
 *
 * Refer to Service Definition ETG.1000.3
 */
ec_retval_t ecsd_eoe_mailbox_read_upd_cnf(struct ecsd *pecsd, ec_bool_t success) {
    assert (pecsd != NULL);
    ec_retval_t ret = 0u;

    if (success == EC_TRUE) {
        switch (pecsd->mbx.eoe.state) {
            default:
                break;
        }
    } else {
        // send again
        ecsd_eoe_mailbox_read_upd_req(pecsd, (struct ec_mailbox *)&pecsd->mbx.eoe.repeat_mbx_buffer[0]);
    }

    return ret;
}

//! \brief 
/*!
 * Refer to Service Definition ETG.1000.3
 */
ec_retval_t ecsd_eoe_mailbox_start(struct ecsd *pecsd) {
    switch (pecsd->mbx.eoe.state) {
        case EOE_STATE_OFF: {
            pecsd->mbx.eoe.seg   = 0u;
            pecsd->mbx.eoe.state = EOE_STATE_IDLE;
            break;
        }
        case EOE_STATE_DWLE:
        case EOE_STATE_DWLN:
        case EOE_STATE_DWLS:
        case EOE_STATE_UPLE:
        case EOE_STATE_UPLN:
        case EOE_STATE_UPLS:
        case EOE_STATE_INF: {
            // Ignore
            break;
        }
        case EOE_STATE_IDLE:
        case EOE_STATE_WSEG: {
            break;
        }
        case EOE_STATE_ERR:
        case EOE_STATE_WUPD: {
            // Ignore
            break;
        }

    }

    return 0;
}

//! \brief 
/*!
 * Refer to Service Definition ETG.1000.3
 */
ec_retval_t ecsd_eoe_mailbox_stop(struct ecsd *pecsd) {
    switch (pecsd->mbx.eoe.state) {
        case EOE_STATE_DWLE:
        case EOE_STATE_DWLN:
        case EOE_STATE_DWLS:
        case EOE_STATE_UPLE:
        case EOE_STATE_UPLN:
        case EOE_STATE_UPLS:
        case EOE_STATE_INF: {
            // TODO 
            // CANCEL SERVICE
            break;
        }
        case EOE_STATE_OFF:
        case EOE_STATE_IDLE:
        case EOE_STATE_WSEG: {
            break;
        }
        case EOE_STATE_ERR:
        case EOE_STATE_WUPD: {
            // TODO
            // RESET MAILBOX
            break;
        }

    }

    pecsd->mbx.eoe.state = EOE_STATE_OFF;

    return 0;
}

#endif /* ECSD_MAILBOX_EOE_SUPPORT */


