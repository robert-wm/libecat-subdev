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
#include <libecat-subdev/coe.h>

#include <assert.h>
#include <stdlib.h>
#include <string.h>

//! \brief 
/*!
 * Refer to Service Definition ETG.1000.3
 */
ec_retval_t ecsd_mailbox_write_ind(struct ecsd *pecsd, struct ec_mailbox *mbx) {
    assert (pecsd != NULL);
    ec_retval_t ret = 0u;
    ec_uint8_t type = EC_MBX_HDR__TYPE(&mbx->mailbox_hdr);

    switch (type) {
#ifdef ECSD_MAILBOX_COE_SUPPORT
        case EC_MBX_HDR__TYPE__COE:
            ret = ecsd_coe_mailbox_write_ind(pecsd, (struct ec_coe_service_data *)mbx);
            break;
#endif
#ifdef ECSD_MAILBOX_FOE_SUPPORT
        case EC_MBX_HDR__TYPE__FOE:
            ret = ecsd_foe_mailbox_write_ind(pecsd, (struct ec_foe_service_data *)mbx);
            break;
#endif
#ifdef ECSD_MAILBOX_EOE_SUPPORT
        case EC_MBX_HDR__TYPE__EOE:
            ret = ecsd_eoe_mailbox_write_ind(pecsd, (struct ec_eoe_service_data *)mbx);
            break;
#endif
#ifdef ECSD_MAILBOX_SOE_SUPPORT
        case EC_MBX_HDR__TYPE__SOE:
            ret = ecsd_soe_mailbox_write_ind(pecsd, (struct ec_soe_service_data *)mbx);
            break;
#endif
#ifdef ECSD_MAILBOX_VOE_SUPPORT
        case EC_MBX_HDR__TYPE__VOE:
            ret = ecsd_voe_mailbox_write_ind(pecsd, (struct ec_voe_service_data *)mbx);
            break;
#endif
        default:
            break;
    }

    return ret;
}

//! \brief 
/*!
 * Refer to Service Definition ETG.1000.3
 */
ec_retval_t ecsd_mailbox_read_upd_cnf(struct ecsd *pecsd, ec_bool_t success) {
    assert (pecsd != NULL);
    ec_retval_t ret = 0u;

#ifdef ECSD_MAILBOX_COE_SUPPORT
    ret = ecsd_coe_mailbox_read_upd_cnf(pecsd, success);
#endif
#ifdef ECSD_MAILBOX_FOE_SUPPORT
    ret = ecsd_foe_mailbox_read_upd_cnf(pecsd, success);
#endif
#ifdef ECSD_MAILBOX_EOE_SUPPORT
    ret = ecsd_eoe_mailbox_read_upd_cnf(pecsd, success);
#endif
#ifdef ECSD_MAILBOX_SOE_SUPPORT
    ret = ecsd_soe_mailbox_read_upd_cnf(pecsd, success);
#endif
#ifdef ECSD_MAILBOX_VOE_SUPPORT
    ret = ecsd_voe_mailbox_read_upd_cnf(pecsd, success);
#endif

    return ret;
}

// 
ec_retval_t ecsd_mailbox_read_upd_req(struct ecsd *pesc, struct ec_mailbox *mbx) {
    int counter = pesc->mbx.counter++;
    if (pesc->mbx.counter > 7) {
        pesc->mbx.counter = 1;
    }

    EC_MBX_HDR__CNT__SET(&mbx->mailbox_hdr, counter);
    pesc->hw.write(pesc->hw.hdl, pesc->mbx.sm_send.address, (void *)mbx, mbx->mailbox_hdr.length + 6);

    if (mbx->mailbox_hdr.length < pesc->mbx.sm_send.length) {
        uint16_t dummy = 0;
        pesc->hw.write(pesc->hw.hdl, pesc->mbx.sm_send.address + pesc->mbx.sm_send.length - 2, (void *)&dummy, 2u);
    }

    return 0;
}

