/**
 * \file coe.c
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

#include <libesc/coe.h>
#include <libesc/object.h>

#include <assert.h>
#include <stddef.h>

esc_retval_t esc_coe_abort_sdo_transfer_req(esc_t *pesc, esc_coe_abort_sdo_t *mbx, esc_uint32_t abort_code) {
    
    // mailbox header
    mbx->mailbox_hdr.length = 10u;
    ESC_MBX_HDR__TYPE_SET(&mbx->mailbox_hdr, ESC_MBX_TYPE__COE);

    // coe header 
    ESC_COE_HDR__SERVICE_SET(&mbx->coe_hdr, ESC_COE_HDR__SERVICE__SDO_REQUEST);
    
    // sdo header
    ESC_COE_SDO_HDR__COMMAND_SET(&mbx->sdo_hdr, ESC_COE_SDO_HDR__COMMAND__ABORT_REQUEST);
    mbx->abort_code = abort_code;

    esc_mbx_write(pesc, (esc_mailbox_t *)mbx);
}

esc_retval_t esc_coe_sdo_download_expedited_ind(esc_t *pesc, esc_coe_sdo_download_expedited_t *mbx) {
    assert(pesc != NULL);
    esc_retval_t ret = ESC_OK;
    esc_uint32_t abort_code = 0u;
    esc_object_t *obj = NULL;

    esc_bool_t   complete_access = ESC_COE_SDO_HDR__COMPLETE_ACCESS(&mbx->sdo_hdr);
    esc_uint16_t index           = mbx->sdo_hdr.index;
    esc_uint8_t  sub_index       = mbx->sdo_hdr.sub_index;
    esc_size_t   data_set_size   = 4u - ESC_COE_SDO_HDR__DATA_SET_SIZE(&mbx->sdo_hdr);
    esc_uint8_t *data            = &mbx->data[0];

    obj = esc_obj_get_object(pesc, index);
    if (obj == NULL) {
        // ERROR object index not found
        abort_code = ESC_SDO_ABORT_CODE__OBJECT_DOES_NOT_EXIST;
    } else {
        // write downloaded SDO to dictionary
        if (complete_access == ESC_TRUE) {
            if (sub_index != 0) {
                // ERROR unsupported access
                abort_code = ESC_SDO_ABORT_CODE__UNSUPPORTED_ACCESS;
            } else {
                abort_code = esc_obj_write_object(obj, pesc->al_state, data, data_set_size);
            }
        } else {
            abort_code = esc_obj_write_object_entry(obj, sub_index, pesc->al_state, data, data_set_size);
        }
    }
                
    if (abort_code != ESC_OK) {
        // #28
        // ERROR return obj_ret error code.
        esc_coe_abort_sdo_transfer_req(pesc, (esc_coe_abort_sdo_t *)mbx, abort_code);
    }

    mbx->coe_hdr.hdr = 0u;
    ESC_COE_HDR__SERVICE_SET(&mbx->coe_hdr, ESC_COE_HDR__SERVICE__SDO_RESPONSE);
    
    mbx->sdo_hdr.hdr = 0u;
    ESC_COE_SDO_HDR__COMMAND_SET(&mbx->sdo_hdr, ESC_COE_SDO_HDR__COMMAND__DOWNLOAD_RESPONSE);

    return ret;
}

esc_retval_t esc_coe_sdo_download_normal_ind(esc_t *pesc, esc_coe_sdo_download_normal_t *mbx) {
    assert(pesc != NULL);
    esc_retval_t ret = ESC_OK;

    esc_bool_t   complete_access = ESC_COE_SDO_HDR__COMPLETE_ACCESS(&mbx->sdo_hdr);
    esc_uint16_t index           = mbx->sdo_hdr.index;
    esc_uint8_t  sub_index       = mbx->sdo_hdr.sub_index;
    esc_size_t   complete_size   = mbx->complete_size;
    esc_uint8_t *data            = &mbx->data[4];
    esc_size_t   data_length     = mbx->mailbox_hdr.length - 10;

    // write downloaded SDO to dictionary
    esc_uint32_t abort_code = esc_obj_write_object(esc, index, sub_index, complete_access, *data, data_length);

    if (abort_code != 0u) {
        mbx->coe_hdr.hdr = 0u;
        ESC_COE_HDR__SERVICE_SET(&mbx->coe_hdr, ESC_COE_HDR__SERVICE__SDO_REQUEST);

        mbx->sdo_hdr.hdr = 0u;
        ESC_COE_SDO_HDR__COMMAND_SET(&mbx->sdo_hdr, ESC_COE_SDO_HDR__COMMAND__ABORT_REQUEST);

        mbx->sdo_hdr.index = index;
        mbx->sdo_hdr.sub_index = sub_index;
        *(esc_uint32_t *)&mbx->data[0] = abort_code;
    } else {
        mbx->coe_hdr.hdr = 0u;
        ESC_COE_HDR__SERVICE_SET(&mbx->coe_hdr, ESC_COE_HDR__SERVICE__SDO_RESPONSE);

        mbx->sdo_hdr.hdr = 0u;
        ESC_COE_SDO_HDR__COMMAND_SET(&mbx->sdo_hdr, ESC_COE_SDO_HDR__COMMAND__DOWNLOAD_RESPONSE);
    }

    return ret;
}


esc_retval_t esc_coe_sdo_request(esc_t *pesc, esc_coe_sdo_request_t *mbx) {
    assert(pesc != NULL);
    esc_retval_t ret = ESC_OK;

    switch (ESC_COE_SDO_HDR__COMMAND(*mbx)) {
        case ESC_COE_SDO_HDR__COMMAND__DOWNLOAD_SEGMENT_REQUEST:
            // #11 | #12
            if (mbx->mailbox_hdr.length < 10) {
                // #11
                esc_coe_mailbox_error_reply(pesc, (esc_mailbox_error_reply_t *)mbx, ESC_MBXERR_INVALIDSIZE);
            } else {
                // #12
            }
            break;
        case ESC_COE_SDO_HDR__COMMAND__DOWNLOAD_REQUEST:
            // #7 | #8 | #9 | #10 
            if (ESC_COE_SDO_HDR__HDR_TRANSFER_TYPE(*mbx) == ESC_COE_SDO_HDR__TRANSFER_TYPE__EXPEDITED) {
                // #7 | #8
                if (mbx->mailbox_hdr.length != 10) {
                    // #7
                    esc_coe_mailbox_error_reply(pesc, (esc_mailbox_error_reply_t *)mbx, ESC_MBXERR_INVALIDSIZE);
                } else {
                    // #8
                    esc_coe_sdo_download_expedited_ind(esc, (esc_coe_sdo_download_expedited_t *)mbx);
                }
            } else {
                // #9 | #10
                if (mbx->mailbox_hdr.length <= 10) {
                    // #9
                    esc_coe_mailbox_error_reply(pesc, (esc_mailbox_error_reply_t *)mbx, ESC_MBXERR_INVALIDSIZE);
                } else {
                    // #10
                }
            }
            break;
        case ESC_COE_SDO_HDR__COMMAND__UPLOAD_REQUEST:
            // #13 | #14 | #15 | #16
            break;
        case ESC_COE_SDO_HDR__COMMAND__UPLOAD_SEGMENT_REQUEST:
            break;
            // #17 | #18
        default:
            // #6
            esc_coe_mailbox_error_reply(pesc, ESC_MBXERR_INVALIDHEADER, (esc_mailbox_error_reply_t *)sd);
            break;
    }

    if ( 
            (sd->mailbox_hdr.length != 10) && 
            (ESC_COE_SDO_HDR__COMMAND(*sdo_hdr) == ESC_COE_SDO_HDR__COMMAND__DOWNLOAD_REQUEST) && 
            (ESC_COE_SDO_HDR__HDR_TRANSFER_TYPE(*sdo_hdr) == ESC_COE_SDO_HDR__TRANSFER_TYPE__EXPEDITED)     ) 
    {
        // #7
        esc_coe_mailbox_error_reply(pesc, ESC_MBXERR_INVALIDSIZE, (esc_mailbox_error_reply_t *)sd);
    } else if (
            (sd->mailbox_hdr.length == 10) &&
            (ESC_COE_SDO_HDR__COMMAND(*sdo_hdr) == ESC_COE_SDO_HDR__COMMAND__DOWNLOAD_REQUEST) &&
            (ESC_COE_SDO_HDR__HDR_TRANSFER_TYPE(*sdo_hdr) == ESC_COE_SDO_HDR__TRANSFER_TYPE__EXPEDITED) ) {
        // #8
        // TODO
    } else if (
            (sd->mailbox_hdr.length >= 10) &&
            (ESC_COE_SDO_HDR__COMMAND(*sdo_hdr) == ESC_COE_SDO_HDR__COMMAND__DOWNLOAD_REQUEST) &&
            (ESC_COE_SDO_HDR__HDR_TRANSFER_TYPE(*sdo_hdr) == ESC_COE_SDO_HDR__TRANSFER_TYPE__NORMAL)    ) {
        // #9
        // TODO
    }

    return ret;
}

esc_retval_t esc_coe_sdo_info_service_ind(esc_t *pesc, esc_coe_sdo_information_t *mbx) {
    assert(pesc != NULL);
    esc_retval_t ret = ESC_OK;

    switch (ESC_COE_SDO_INFO_HDR__OPCODE(*mbx)) {
        case ESC_COE_SDO_INFO_HDR__GET_OD_LIST_REQUEST:
        case ESC_COE_SDO_INFO_HDR__GET_OBJECT_DESCRIPTION_REQUEST:
            // #19 | #20
            if (mbx->mailbox_hdr.length != 8) {
                // #19
                esc_coe_mailbox_error_reply(pesc, ESC_MBXERR_INVALIDSIZE, (esc_mailbox_error_reply_t *)mbx);
            } else {
                // #20
                pesc->mailbox.coe.state = MAILBOX_STATE_INF;
            }
            break;
        case ESC_COE_SDO_INFO_HDR__GET_ENTRY_DESCRIPTION_REQUEST:
            // #21 | #22
            if (mbx->mailbox_hdr.length != 10) {
                // #21
                esc_coe_mailbox_error_reply(pesc, ESC_MBXERR_INVALIDSIZE, (esc_mailbox_error_reply_t *)mbx);
            } else {
                // #22
                pesc->mailbox.coe.state = MAILBOX_STATE_INF;
            }
            break;
        default:
            break;
    }
    return ESC_OK;
}

esc_void_t esc_coe_mailbox_error_reply(esc_t *pesc, esc_uint8_t detail, esc_mailbox_error_reply_t *mbx) 
{
    sd_mbx_err->mailbox_hdr.length  = 4u;
    sd_mbx_err->mailbox_hdr.type    = ESC_MBX_TYPE__ERR;
    sd_mbx_err->type                = ESC_MBX_ERROR_REPLY__TYPE__MAILBOX_COMMAND;
    sd_mbx_err->detail              = detail;

    esc_err_mailbox_read_upd_req(pesc, length, address, channel, priority, sd_mbx_err);

    pesc->mailbox.coe.state = MAILBOX_STATE_ERR;
}

esc_void_t esc_coe_mailbox_write_ind(esc_t *pesc, esc_coe_service_data_t *sd) {
    assert(pesc != NULL);

    switch (pesc->mailbox.coe.state) {
        case MAILBOX_STATE_IDLE: 
        {
            esc_coe_sdo_hdr_t *sdo_hdr = (esc_coe_sdo_hdr_t *)&sd->data[0];

            switch (ESC_COE_HDR__SERVICE(sd->coe_hdr)) {
                case ESC_COE_HDR__SERVICE__SDO_REQUEST: {
                    // #6 | #7 | #8 | #9 | #10 | #11 | #12 | #13 | #14 | #15 | #16 | #17 | #18
                    esc_coe_sdo_request(pesc, (esc_coe_sdo_request_t *)mbx);
                    break;
                }
                case ESC_COE_HDR__SERVICE__SDO_INFORMATION: {
                    // #19 | #20 | #21 | #22
                    esc_coe_sdo_info_service_ind(pesc, (esc_coe_sdo_information_t *)mbx);
                    break;
                }
                default: {
                    // #5
                    esc_coe_mailbox_error_reply(pesc, ESC_MBXERR_INVALIDHEADER, (esc_mailbox_error_reply_t *)sd);
                }
            }


            break;
        }
    }
}
