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

#include <libecat-base/types.h>
#include <libecat-subdev/hw.h>
#include <libecat-subdev/mailbox.h>

#include <assert.h>
#include <stdlib.h>
#include <string.h>

//! \brief 
/*!
 * Refer to Service Definition ETG.1000.3
 */
ec_uint32_t esc_mailbox_write_ind(struct ecsd *pecsd, struct ec_mailbox *mbx) {
    assert (pecsd != NULL);
    ec_uint32_t ret = 0u;

    return ret;
}

//! \brief 
/*!
 * Refer to Service Definition ETG.1000.3
 */
ec_uint32_t esc_mailbox_read_upd_cnf(struct ecsd *pecsd, ec_bool_t success) {
    assert (pecsd != NULL);
    ec_uint32_t ret = 0u;

    return ret;
}

ec_uint32_t esc_mailbox_written(struct ecsd *pecsd) {
    ec_uint32_t ret = 0u;


    // Get receive mailbox from ESC.
    struct ec_mailbox mbx;
    ec_size_t ret_size = hw_read(pecsd, pecsd->mbx.sm_receive.address, (ec_uint8_t *)&mbx, pecsd->mbx.sm_receive.length);
    (void)ret_size;
    ec_uint8_t type = EC_MBX_HDR__TYPE(&mbx.mailbox_hdr);

    switch (type) {
        case EC_MBX_HDR__TYPE__COE:
            // TODO ...
            break;
        case EC_MBX_HDR__TYPE__FOE:
            // TODO
            break;
        case EC_MBX_HDR__TYPE__EOE:
            // TODO
            break;
        case EC_MBX_HDR__TYPE__SOE:
            // TODO
            break;
        case EC_MBX_HDR__TYPE__VOE:
            // TODO
            break;
        default:
            break;
    }

    return ret;
}

ec_retval_t ecsd_mbx_write(struct ecsd *pesc, struct ec_mailbox *mbx) {
    return 0;
}
