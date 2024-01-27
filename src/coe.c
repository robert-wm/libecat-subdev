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

ec_void_t ecsd_coe_mailbox_error_reply(struct ecsd *pecsd, ec_uint8_t detail, struct ec_mailbox_error_reply *mbx);

ec_retval_t ecsd_coe_abort_sdo_transfer_req(struct ecsd *pecsd, struct ec_coe_abort_sdo *mbx, ec_uint32_t abort_code) {
    
    // mailbox header
    mbx->mailbox_hdr.length = 10u;
    EC_MBX_HDR__TYPE__SET(&mbx->mailbox_hdr, EC_MBX_HDR__TYPE__COE);

    // coe header 
    EC_COE_HDR__SERVICE__SET(&mbx->coe_hdr, EC_COE_HDR__SERVICE__SDO_REQUEST);
    
    // sdo header
    EC_COE_SDO_HDR__COMMAND__SET(mbx->sdo_hdr, EC_COE_SDO_HDR__COMMAND__ABORT_REQUEST);
    mbx->abort_code = abort_code;

    ecsd_mbx_write(pecsd, (struct ec_mailbox *)mbx);

    return 0;
}

// ===================================================================
// Primitives issued by CoESM to Application (see ETG1000.6 Table 109)

ec_retval_t ecsd_coe_sdo_download_expedited_ind(struct ecsd *pecsd, struct ec_coe_sdo_download_expedited_req *mbx) {
    assert(pecsd != NULL);
    ec_retval_t ret = EC_OK;
    ec_uint32_t abort_code = 0u;
    struct ec_object *obj = NULL;

    ec_bool_t   complete_access = EC_COE_SDO_HDR__COMPLETE_ACCESS(&mbx->sdo_hdr);
    ec_uint16_t index           = mbx->sdo_hdr.index;
    ec_uint8_t  sub_index       = mbx->sdo_hdr.sub_index;
    ec_size_t   data_set_size   = 4u - EC_COE_SDO_HDR__DATA_SET_SIZE(mbx->sdo_hdr);
    ec_uint8_t *data            = &mbx->data[0];

    obj = ec_obj_get_object(pecsd->mbx.coe.objects, index);
    if (obj == NULL) {
        // ERROR object index not found
        abort_code = EC_SDO_ABORT_CODE__OBJECT_DOES_NOT_EXIST;
    } else {
        // write downloaded SDO to dictionary
        if (complete_access == EC_TRUE) {
            if (sub_index != 0) {
                // ERROR unsupported access
                abort_code = EC_SDO_ABORT_CODE__UNSUPPORTED_ACCESS;
            } else {
                abort_code = ec_obj_write_object(obj, pecsd->al_state, data, data_set_size);
            }
        } else {
            abort_code = ec_obj_write_object_entry(obj, sub_index, pecsd->al_state, data, data_set_size);
        }
    }
                
    if (abort_code != EC_OK) {
        // #28
        // ERROR return obj_ret error code.
        ecsd_coe_abort_sdo_transfer_req(pecsd, (struct ec_coe_abort_sdo *)mbx, abort_code);
    }

    mbx->coe_hdr.hdr = 0u;
    EC_COE_HDR__SERVICE__SET(&mbx->coe_hdr, EC_COE_HDR__SERVICE__SDO_RESPONSE);
    
    mbx->sdo_hdr.hdr = 0u;
    EC_COE_SDO_HDR__COMMAND__SET(mbx->sdo_hdr, EC_COE_SDO_HDR__COMMAND__DOWNLOAD_RESPONSE);

    return ret;
}

ec_retval_t ecsd_coe_sdo_download_normal_ind(struct ecsd *pecsd, struct ec_coe_sdo_download_normal_req *mbx) {
    assert(pecsd != NULL);
    ec_retval_t ret = EC_OK;
    ec_uint32_t abort_code = 0u;
    struct ec_object *obj = NULL;

    ec_bool_t   complete_access = EC_COE_SDO_HDR__COMPLETE_ACCESS(&mbx->sdo_hdr);
    ec_uint16_t index           = mbx->sdo_hdr.index;
    ec_uint8_t  sub_index       = mbx->sdo_hdr.sub_index;
    ec_size_t   complete_size   = mbx->complete_size;
    ec_uint8_t *data            = &mbx->data[4];

    obj = ec_obj_get_object(pecsd->mbx.coe.objects, index);
    if (obj == NULL) {
        // ERROR object index not found
        abort_code = EC_SDO_ABORT_CODE__OBJECT_DOES_NOT_EXIST;
    } else {
        // write downloaded SDO to dictionary
        if (complete_access == EC_TRUE) {
            if (sub_index != 0) {
                // ERROR unsupported access
                abort_code = EC_SDO_ABORT_CODE__UNSUPPORTED_ACCESS;
            } else {
                abort_code = ec_obj_write_object(obj, pecsd->al_state, data, complete_size);
            }
        } else {
            abort_code = ec_obj_write_object_entry(obj, sub_index, pecsd->al_state, data, complete_size);
        }
    }
                
    if (abort_code != EC_OK) {
        // #28
        // ERROR return obj_ret error code.
        ecsd_coe_abort_sdo_transfer_req(pecsd, (struct ec_coe_abort_sdo *)mbx, abort_code);
    }

    mbx->coe_hdr.hdr = 0u;
    EC_COE_HDR__SERVICE__SET(&mbx->coe_hdr, EC_COE_HDR__SERVICE__SDO_RESPONSE);
    
    mbx->sdo_hdr.hdr = 0u;
    EC_COE_SDO_HDR__COMMAND__SET(mbx->sdo_hdr, EC_COE_SDO_HDR__COMMAND__DOWNLOAD_RESPONSE);

    return ret;
}

ec_retval_t ecsd_coe_download_sdo_segment_ind(struct ecsd *pecsd, struct ec_coe_sdo_download_segmented_req *mbx) {
    assert(pecsd != NULL);
    ec_retval_t ret = EC_OK;
    return ret;
}

ec_retval_t ecsd_coe_sdo_upload_expedited_ind(struct ecsd *pecsd, struct ec_coe_sdo_upload_expedited_req *mbx) {
    assert(pecsd != NULL);
    ec_retval_t ret = EC_OK;
    return ret;
}

ec_retval_t ecsd_coe_sdo_upload_normal_ind(struct ecsd *pecsd, struct ec_coe_sdo_upload_normal_req *mbx) {
    assert(pecsd != NULL);
    ec_retval_t ret = EC_OK;
    return ret;
}

// ===================================================================

ec_retval_t ecsd_coe_sdo_request(struct ecsd *pecsd, struct ec_coe_sdo_request *mbx) {
    assert(pecsd != NULL);
    ec_retval_t ret = EC_OK;

    switch (EC_COE_SDO_HDR__COMMAND(mbx->sdo_hdr)) {
        case EC_COE_SDO_HDR__COMMAND__DOWNLOAD_SEGMENT_REQUEST: {
            // #11 | #12
            if (mbx->mailbox_hdr.length < 10) {
                // #11
                ecsd_coe_mailbox_error_reply(pecsd, EC_MBXERR_INVALIDSIZE, (struct ec_mailbox_error_reply *)mbx);
                pecsd->mbx.coe.state = COE_STATE_ERR;
            } else {
                // #12
            }
            break;
        }
        case EC_COE_SDO_HDR__COMMAND__DOWNLOAD_REQUEST: {
            // #7 | #8 | #9 | #10 
            if (EC_COE_SDO_HDR__HDR_TRANSFER_TYPE(&mbx->sdo_hdr) == EC_COE_SDO_HDR__TRANSFER_TYPE__EXPEDITED) {
                // #7 | #8
                if (mbx->mailbox_hdr.length != 10) {
                    // #7
                    ecsd_coe_mailbox_error_reply(pecsd, EC_MBXERR_INVALIDSIZE, (struct ec_mailbox_error_reply *)mbx);
                } else {
                    // #8
                    ecsd_coe_sdo_download_expedited_ind(pecsd, (struct ec_coe_sdo_download_expedited_req *)mbx);
                }
            } else {
                // #9 | #10
                if (mbx->mailbox_hdr.length <= 10) {
                    // #9
                    ecsd_coe_mailbox_error_reply(pecsd, EC_MBXERR_INVALIDSIZE, (struct ec_mailbox_error_reply *)mbx);
                    pecsd->mbx.coe.state = COE_STATE_ERR;
                } else {
                    // #10
                    ecsd_coe_sdo_download_normal_ind(pecsd, (struct ec_coe_sdo_download_normal_req *)mbx);
                }
            }
            break;
        }
        case EC_COE_SDO_HDR__COMMAND__UPLOAD_REQUEST: {
            // #13 | #14 | #15 | #16
            break;
        }
        case EC_COE_SDO_HDR__COMMAND__UPLOAD_SEGMENT_REQUEST: {
            // #17 | #18
            break;
        }
        case EC_COE_SDO_HDR__COMMAND__ABORT_REQUEST:
        default: {
            // #6
            ecsd_coe_mailbox_error_reply(pecsd, EC_MBXERR_INVALIDHEADER, (struct ec_mailbox_error_reply *)mbx);
            pecsd->mbx.coe.state = MAILBOX_STATE_ERR;
            break;
        }
    }

    return ret;
}

ec_retval_t ecsd_coe_sdo_info_service_ind(struct ecsd *pecsd, struct ec_coe_sdo_information *mbx) {
    assert(pecsd != NULL);
    ec_retval_t ret = EC_OK;

    switch (EC_COE_SDO_INFO_HDR__OPCODE(&mbx->sdo_hdr)) {
        case EC_COE_SDO_INFO_HDR__GET_OD_LIST_REQUEST:
        case EC_COE_SDO_INFO_HDR__GET_OBJECT_DESCRIPTION_REQUEST:
            // #19 | #20
            if (mbx->mailbox_hdr.length != 8) {
                // #19
                ecsd_coe_mailbox_error_reply(pecsd, EC_MBXERR_INVALIDSIZE, (struct ec_mailbox_error_reply *)mbx);
            } else {
                // #20
                pecsd->mbx.coe.state = MAILBOX_STATE_INF;
            }
            break;
        case EC_COE_SDO_INFO_HDR__GET_ENTRY_DESCRIPTION_REQUEST:
            // #21 | #22
            if (mbx->mailbox_hdr.length != 10) {
                // #21
                ecsd_coe_mailbox_error_reply(pecsd, EC_MBXERR_INVALIDSIZE, (struct ec_mailbox_error_reply *)mbx);
            } else {
                // #22
                pecsd->mbx.coe.state = MAILBOX_STATE_INF;
            }
            break;
        default:
            break;
    }

    return ret;
}

ec_void_t ecsd_coe_mailbox_error_reply(struct ecsd *pecsd, ec_uint8_t detail, struct ec_mailbox_error_reply *mbx) 
{
    mbx->mailbox_hdr.length  = 4u;
    EC_MBX_HDR__TYPE__SET(&mbx->mailbox_hdr, EC_MBX_HDR__TYPE__ERR);
    mbx->type                = EC_MBX_ERROR_REPLY__TYPE__MAILBOX_COMMAND;
    mbx->detail              = detail;

    //TODO ecsd_err_mailbox_read_upd_req(pecsd, length, address, channel, priority, mbx);

    pecsd->mbx.coe.state = MAILBOX_STATE_ERR;
}

//! \brief 
/*!
 * Refer to Mailbox Read Upd Service Definition ETG.1000.3
 */
ec_retval_t ecsd_coe_mailbox_read_upd(struct ecsd *pecsd, struct ec_mailbox *mbx) {
    assert (pecsd != NULL);
    ec_retval_t ret = 0u;

    switch (pecsd->mbx.coe.state) {
        case COE_STATE_ERR: {
            // #71
            pecsd->mbx.coe.state = COE_STATE_IDLE;
            break;
        }
        case COE_STATE_WUPD: {
            // #63 | #64 | #65 | #66 | #67 
            // TODO
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
 * Refer to Service Definition ETG.1000.3
 */
ec_retval_t ecsd_coe_mailbox_write_ind(struct ecsd *pecsd, struct ec_coe_service_data *mbx) {
    assert(pecsd != NULL);

    ec_retval_t ret = 0;

    switch (pecsd->mbx.coe.state) {
        case COE_STATE_IDLE: 
        {
            switch (EC_COE_HDR__SERVICE(mbx->coe_hdr)) {
                case EC_COE_HDR__SERVICE__SDO_REQUEST: {
                    // #6 | #7 | #8 | #9 | #10 | #11 | #12 | #13 | #14 | #15 | #16 | #17 | #18
                    ecsd_coe_sdo_request(pecsd, (struct ec_coe_sdo_request *)mbx);
                    break;
                }
                case EC_COE_HDR__SERVICE__SDO_INFORMATION: {
                    // #19 | #20 | #21 | #22
                    ecsd_coe_sdo_info_service_ind(pecsd, (struct ec_coe_sdo_information *)mbx);
                    break;
                }
                default: {
                    // #5
                    ecsd_coe_mailbox_error_reply(pecsd, EC_MBXERR_INVALIDHEADER, (struct ec_mailbox_error_reply *)mbx);
                }
            }


            break;
        }
    }

    return 0;
}

//! \brief 
/*!
 * Refer to Service Definition ETG.1000.3
 */
ec_retval_t ecsd_coe_mailbox_read_upd_cnf(struct ecsd *pecsd, ec_bool_t success) {
    assert (pecsd != NULL);
    ec_retval_t ret = 0u;

    return ret;
}

//! \brief 
/*!
 * Refer to Service Definition ETG.1000.3
 */
ec_retval_t ecsd_coe_mailbox_start(struct ecsd *pecsd) {
    switch (pecsd->mbx.coe.state) {
        case COE_STATE_OFF: {
            pecsd->mbx.coe.seg   = 0u;
            pecsd->mbx.coe.state = COE_STATE_IDLE;
            break;
        }
        case COE_STATE_DWLE:
        case COE_STATE_DWLN:
        case COE_STATE_DWLS:
        case COE_STATE_UPLE:
        case COE_STATE_UPLN:
        case COE_STATE_UPLS:
        case COE_STATE_INF: {
            // TODO 
            // CANCEL SERVICE
            break;
        }
        case COE_STATE_IDLE:
        case COE_STATE_WSEG: {
            break;
        }
        case COE_STATE_ERR:
        case COE_STATE_WUPD: {
            // TODO
            // RESET MAILBOX
            break;
        }

    }
}

//! \brief 
/*!
 * Refer to Service Definition ETG.1000.3
 */
ec_retval_t ecsd_coe_mailbox_stop(struct ecsd *pecsd) {
    switch (pecsd->mbx.coe.state) {
        case COE_STATE_DWLE:
        case COE_STATE_DWLN:
        case COE_STATE_DWLS:
        case COE_STATE_UPLE:
        case COE_STATE_UPLN:
        case COE_STATE_UPLS:
        case COE_STATE_INF: {
            // TODO 
            // CANCEL SERVICE
            break;
        }
        case COE_STATE_OFF:
        case COE_STATE_IDLE:
        case COE_STATE_WSEG: {
            break;
        }
        case COE_STATE_ERR:
        case COE_STATE_WUPD: {
            // TODO
            // RESET MAILBOX
            break;
        }

    }

    pecsd->mbx.coe.state = COE_STATE_OFF;
}
