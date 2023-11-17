/**
 * \file mailbox.c
 *
 * \author Robert Burger <robert@burger-system.de>
 *
 * \date 25 Oct 2023
 *
 * \brief ESC mailbox source.
 *
 * ESC mailbox source.
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

#include <libesc/mailbox.h>

//! \brief 
/*!
 * Refer to Service Definition ETG.1000.3
 */
osal_uint32_t esc_mailbox_write_ind(esc_t *pesc, esc_mbx_service_data_t *sd) {
    assert (pesc != NULL);
    osal_uint32_t ret = 0u;

    return ret;
}

//! \brief 
/*!
 * Refer to Service Definition ETG.1000.3
 */
osal_uint32_t esc_mailbox_read_upd_cnf(esc_t *pesc, esc_bool_t success) {
    assert (pesc != NULL);
    osal_uint32_t ret = 0u;

    return ret;
}


osal_uint32_t esc_mailbox_written(esc_t *pesc) {
    osal_uint32_t ret = 0u;

    // Get receive mailbox from ESC.

    switch (type) {
        case ESC_MAILBOX_COE:
            // TODO ...
            break;
        case ESC_MAILBOX_FOE:
            // TODO
            break;
        case ESC_MAILBOX_EOE:
            // TODO
            break;
        case ESC_MAILBOX_SOE:
            // TODO
            break;
        case ESC_MAILBOX_VOE:
            // TODO
            break;
        default:
            break;
    }

    return ret;
}
