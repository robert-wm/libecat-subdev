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

#include <libecat-subdev/coe.h>

#include <assert.h>
#include <stddef.h>
#include <string.h>

#ifdef ECSD_MAILBOX_COE_SUPPORT

/* include generated object dictionary */
#include "object.h"

// forward declarations

//! \brief CoE Mailbox error reply.
/*!
 * \param[in]   pecsd       Pointer to EtherCAT SubDevice structure.
 * \param[in]   detail      Error code to reply.
 * \param[in]   mbx         Mailbox message to process.
 */
static ec_void_t ecsd_coe_mailbox_error_reply(struct ecsd *pecsd, ec_uint8_t detail, struct ec_mailbox_error_reply *mbx);

//! \brief CoE Abort SDO transfer.
/*!
 * \param[in]   pecsd       Pointer to EtherCAT SubDevice structure.
 * \param[in]   mbx         Mailbox message to process.
 * \param[in]   abort_code  Abort code to reply.
 *
 * \return EC_OK on success.
 */
static ec_retval_t ecsd_coe_abort_sdo_transfer_req(struct ecsd *pecsd, struct ec_coe_abort_sdo *mbx, ec_uint32_t abort_code);

//! \brief CoE SDO download expedited indication.
/*!
 * \param[in]   pecsd       Pointer to EtherCAT SubDevice structure.
 * \param[in]   mbx         Mailbox message to process.
 *
 * \return EC_OK on success.
 */
static ec_retval_t ecsd_coe_sdo_download_expedited_ind(struct ecsd *pecsd, struct ec_coe_sdo_download_expedited_req *mbx);

//! \brief CoE SDO download normal indication.
/*!
 * \param[in]   pecsd       Pointer to EtherCAT SubDevice structure.
 * \param[in]   mbx         Mailbox message to process.
 *
 * \return EC_OK on success.
 */
static ec_retval_t ecsd_coe_sdo_download_normal_ind(struct ecsd *pecsd, struct ec_coe_sdo_download_normal_req *mbx);

//! \brief CoE SDO download segment indication.
/*!
 * \param[in]   pecsd       Pointer to EtherCAT SubDevice structure.
 * \param[in]   mbx         Mailbox message to process.
 *
 * \return EC_OK on success.
 */
static ec_retval_t ecsd_coe_download_sdo_segment_ind(struct ecsd *pecsd, struct ec_coe_sdo_download_segmented_req *mbx);

//! \brief CoE SDO upload expedited indication.
/*!
 * \param[in]   pecsd       Pointer to EtherCAT SubDevice structure.
 * \param[in]   mbx         Mailbox message to process.
 *
 * \return EC_OK on success.
 */
static ec_retval_t ecsd_coe_sdo_upload_expedited_ind(struct ecsd *pecsd, struct ec_coe_sdo_upload_expedited_req *mbx);

//! \brief CoE SDO Object Dictionary list indication.
/*!
 * \param[in]   pecsd           Pointer to EtherCAT SubDevice structure.
 * \param[in]   mbx_odlist_req  Mailbox message to process.
 *
 * \return EC_OK on success.
 */
static ec_retval_t ecsd_coe_sdo_info_service_odlist_ind(struct ecsd *pecsd, struct ec_coe_sdo_information_odlist_req *mbx_odlist_req);

//! \brief CoE SDO Object Description indication.
/*!
 * \param[in]   pecsd           Pointer to EtherCAT SubDevice structure.
 * \param[in]   mbx_desc_req    Mailbox message to process.
 *
 * \return EC_OK on success.
 */
static ec_retval_t ecsd_coe_sdo_info_service_object_desc_ind(struct ecsd *pecsd, struct ec_coe_sdo_information_desc_req *mbx_desc_req);

// Function definitions

ec_retval_t ecsd_coe_abort_sdo_transfer_req(struct ecsd *pecsd, struct ec_coe_abort_sdo *mbx, ec_uint32_t abort_code) {
    
    // mailbox header
    mbx->mailbox_hdr.length = 10u;
    EC_MBX_HDR__TYPE__SET(&mbx->mailbox_hdr, EC_MBX_HDR__TYPE__COE);

    // coe header 
    EC_COE_HDR__SERVICE__SET(&mbx->coe_hdr, EC_COE_HDR__SERVICE__SDO_REQUEST);
    
    // sdo header
    EC_COE_SDO_HDR__COMMAND__SET(&mbx->sdo_hdr, EC_COE_SDO_HDR__COMMAND__ABORT_REQUEST);
    mbx->abort_code = abort_code;

    ecsd_coe_mailbox_read_upd_req(pecsd, (struct ec_mailbox *)mbx);

    return 0;
}

// ===================================================================
// Primitives issued by CoESM to Application (see ETG1000.6 Table 109)

ec_retval_t ecsd_coe_sdo_download_expedited_ind(struct ecsd *pecsd, struct ec_coe_sdo_download_expedited_req *mbx) {
    assert(pecsd != NULL);
    ec_retval_t ret = EC_OK;
    ec_uint32_t abort_code = 0u;
    struct object *obj = NULL;

    ec_bool_t   complete_access = EC_COE_SDO_HDR__COMPLETE_ACCESS(&mbx->sdo_hdr);
    ec_uint16_t index           = mbx->sdo_hdr.index;
    ec_uint8_t  sub_index       = mbx->sdo_hdr.sub_index;
    ec_size_t   data_set_size   = EC_COE_SDO_HDR__DATA_SET_SIZE(&mbx->sdo_hdr);
    ec_uint8_t *data            = &mbx->data[0];

    if (object_get(index, &obj) != OBJECT_OK) {
        // ERROR object index not found
        abort_code = OBJECT_ERROR__OBJECT_DOES_NOT_EXIST;
    } else {
        // write downloaded SDO to dictionary
        if (complete_access == EC_TRUE) {
            if (sub_index != 0) {
                // ERROR unsupported access
                abort_code = OBJECT_ERROR__UNSUPPORTED_ACCESS;
            } else {
                abort_code = object_write_object(obj, pecsd->al_state, data, data_set_size);
            }
        } else {
            abort_code = object_write_object_entry(obj, sub_index, pecsd->al_state, data, data_set_size);
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
    EC_COE_SDO_HDR__COMMAND__SET(&mbx->sdo_hdr, EC_COE_SDO_HDR__COMMAND__DOWNLOAD_RESPONSE);

    pecsd->mbx.coe.state = COE_STATE_DWLE;

    ecsd_coe_mailbox_read_upd_req(pecsd, (struct ec_mailbox *)mbx);
    pecsd->mbx.coe.state = COE_STATE_WUPD;

    return ret;
}

ec_retval_t ecsd_coe_sdo_download_normal_ind(struct ecsd *pecsd, struct ec_coe_sdo_download_normal_req *mbx) {
    assert(pecsd != NULL);
    ec_retval_t ret = EC_OK;
    ec_uint32_t abort_code = 0u;
    struct object *obj = NULL;

    ec_bool_t   complete_access = EC_COE_SDO_HDR__COMPLETE_ACCESS(&mbx->sdo_hdr);
    ec_uint16_t index           = mbx->sdo_hdr.index;
    ec_uint8_t  sub_index       = mbx->sdo_hdr.sub_index;
    ec_size_t   complete_size   = mbx->complete_size;
    ec_uint8_t *data            = &mbx->data[0];

    if (object_get(index, &obj) != OBJECT_OK) {
        // ERROR object index not found
        abort_code = OBJECT_ERROR__OBJECT_DOES_NOT_EXIST;
    } else {
        // write downloaded SDO to dictionary
        if (complete_access == EC_TRUE) {
            if (sub_index != 0) {
                // ERROR unsupported access
                abort_code = OBJECT_ERROR__UNSUPPORTED_ACCESS;
            } else {
                abort_code = object_write_object(obj, pecsd->al_state, data, complete_size);
            }
        } else {
            abort_code = object_write_object_entry(obj, sub_index, pecsd->al_state, data, complete_size);
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
    EC_COE_SDO_HDR__COMMAND__SET(&mbx->sdo_hdr, EC_COE_SDO_HDR__COMMAND__DOWNLOAD_RESPONSE);
    
    mbx->mailbox_hdr.length = 10;

    pecsd->mbx.coe.state = COE_STATE_DWLN;

    ecsd_coe_mailbox_read_upd_req(pecsd, (struct ec_mailbox *)mbx);
    pecsd->mbx.coe.state = COE_STATE_WUPD;

    return ret;
}


ec_retval_t ecsd_coe_download_sdo_segment_ind(struct ecsd *pecsd, struct ec_coe_sdo_download_segmented_req *mbx) {
    assert(pecsd != NULL);
    ec_retval_t ret = EC_OK;
    (void)mbx;
    return ret;
}

ec_retval_t ecsd_coe_sdo_upload_expedited_ind(struct ecsd *pecsd, struct ec_coe_sdo_upload_expedited_req *mbx) {
    assert(pecsd != NULL);
    ec_retval_t ret = EC_OK;
    ec_uint32_t abort_code = 0u;
    ec_size_t complete_size;
    struct object *obj = NULL;

    ec_bool_t   complete_access = EC_COE_SDO_HDR__COMPLETE_ACCESS(&mbx->sdo_hdr);
    ec_uint16_t index           = mbx->sdo_hdr.index;
    ec_uint8_t  sub_index       = mbx->sdo_hdr.sub_index;

    struct ec_coe_sdo_upload_expedited_resp *mbx_resp = (struct ec_coe_sdo_upload_expedited_resp *)mbx;

    if (object_get(index, &obj) != OBJECT_OK) {
        // ERROR object index not found
        abort_code = OBJECT_ERROR__OBJECT_DOES_NOT_EXIST;
    } else {
        // write downloaded SDO to dictionary
        if (complete_access == EC_TRUE) {
            if (sub_index != 0) {
                // ERROR unsupported access
                abort_code = OBJECT_ERROR__UNSUPPORTED_ACCESS;
            } else {
                size_t obj_len;
                object_get_length(obj, &obj_len);

                complete_size = obj_len;
                abort_code = object_read_object(obj, pecsd->al_state, mbx_resp->data, complete_size);
            }
        } else {
            size_t obj_len;
            object_get_length_object_entry(obj, sub_index, &obj_len);

            complete_size = obj_len;
            abort_code = object_read_object_entry(obj, sub_index, pecsd->al_state, mbx_resp->data, complete_size);
        }
    }

    if (abort_code != EC_OK) {
        // #28
        // ERROR return obj_ret error code.
        ecsd_coe_abort_sdo_transfer_req(pecsd, (struct ec_coe_abort_sdo *)mbx, abort_code);
    }

    mbx_resp->coe_hdr.hdr = 0u;
    EC_COE_HDR__SERVICE__SET(&mbx_resp->coe_hdr, EC_COE_HDR__SERVICE__SDO_RESPONSE);

    mbx_resp->sdo_hdr.hdr = 0u;
    EC_COE_SDO_HDR__COMMAND__SET(&mbx_resp->sdo_hdr, EC_COE_SDO_HDR__COMMAND__UPLOAD_RESPONSE);

    EC_COE_SDO_HDR__TRANSFER_TYPE__SET(&mbx_resp->sdo_hdr, 1u);
    EC_COE_SDO_HDR__DATA_SET_SIZE__SET(&mbx_resp->sdo_hdr, complete_size);

    ecsd_coe_mailbox_read_upd_req(pecsd, (struct ec_mailbox *)mbx_resp);

    return ret;
}

ec_retval_t ecsd_coe_sdo_upload_normal_ind(struct ecsd *pecsd, struct ec_coe_sdo_upload_normal_req *mbx) {
    assert(pecsd != NULL);
    ec_retval_t ret = EC_OK;
    ec_uint32_t abort_code = 0u;
    ec_size_t complete_size;
    struct object *obj = NULL;

    ec_bool_t   complete_access = EC_COE_SDO_HDR__COMPLETE_ACCESS(&mbx->sdo_hdr);
    ec_uint16_t index           = mbx->sdo_hdr.index;
    ec_uint8_t  sub_index       = mbx->sdo_hdr.sub_index;

    struct ec_coe_sdo_upload_normal_resp *mbx_resp = (struct ec_coe_sdo_upload_normal_resp *)mbx;

    if (object_get(index, &obj) != OBJECT_OK) {
        // ERROR object index not found
        abort_code = OBJECT_ERROR__OBJECT_DOES_NOT_EXIST;
    } else {
        // write downloaded SDO to dictionary
        if (complete_access == EC_TRUE) {
            if (sub_index != 0) {
                // ERROR unsupported access
                abort_code = OBJECT_ERROR__UNSUPPORTED_ACCESS;
            } else {
                size_t obj_len;
                object_get_length(obj, &obj_len);

                complete_size = obj_len;
                abort_code = object_read_object(obj, pecsd->al_state, mbx_resp->data, complete_size);
            }
        } else {
            size_t obj_len;
            object_get_length_object_entry(obj, sub_index, &obj_len);

            complete_size = obj_len;
            abort_code = object_read_object_entry(obj, sub_index, pecsd->al_state, mbx_resp->data, complete_size);
        }
    }

    if (abort_code != EC_OK) {
        // #28
        // ERROR return obj_ret error code.
        ecsd_coe_abort_sdo_transfer_req(pecsd, (struct ec_coe_abort_sdo *)mbx, abort_code);
    }

    mbx_resp->coe_hdr.hdr = 0u;
    EC_COE_HDR__SERVICE__SET(&mbx_resp->coe_hdr, EC_COE_HDR__SERVICE__SDO_RESPONSE);

    mbx_resp->sdo_hdr.hdr = 0u;
    EC_COE_SDO_HDR__COMMAND__SET(&mbx_resp->sdo_hdr, EC_COE_SDO_HDR__COMMAND__UPLOAD_RESPONSE);

    mbx_resp->complete_size = complete_size;
    mbx_resp->mailbox_hdr.length = 10 + 4 + complete_size;

    ecsd_coe_mailbox_read_upd_req(pecsd, (struct ec_mailbox *)mbx_resp);

    return ret;
}

// ===================================================================

ec_retval_t ecsd_coe_sdo_request(struct ecsd *pecsd, struct ec_coe_sdo_request *mbx) {
    assert(pecsd != NULL);
    ec_retval_t ret = EC_OK;

    switch (EC_COE_SDO_HDR__COMMAND(&mbx->sdo_hdr)) {
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
                    pecsd->mbx.coe.state = COE_STATE_ERR;
                } else {
                    // #8
                    ecsd_coe_sdo_download_expedited_ind(pecsd, (struct ec_coe_sdo_download_expedited_req *)mbx);
                    pecsd->mbx.coe.state = COE_STATE_WUPD;
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
                    pecsd->mbx.coe.state = COE_STATE_WUPD;
                }
            }
            break;
        }
        case EC_COE_SDO_HDR__COMMAND__UPLOAD_REQUEST: {
            // #13 | #14 | #15 | #16
            if (mbx->mailbox_hdr.length != 10) {
                // #13 | #15
                ecsd_coe_mailbox_error_reply(pecsd, EC_MBXERR_INVALIDSIZE, (struct ec_mailbox_error_reply *)mbx);
                pecsd->mbx.coe.state = COE_STATE_ERR;
            } else {
                // #14 | #16
                struct object *obj;

                if (object_get(mbx->sdo_hdr.index, &obj) == OBJECT_OK) {
                    size_t obj_len;

                    if ((mbx->sdo_hdr.hdr & EC_COE_SDO_HDR__COMPLETE_ACCESS__MASK) != 0u) {
                        object_get_length(obj, &obj_len);
                    } else {
                        object_get_length_object_entry(obj, mbx->sdo_hdr.sub_index, &obj_len);
                    }

                    if (obj_len <= 4u) {
                        // #14
                        ecsd_coe_sdo_upload_expedited_ind(pecsd, (struct ec_coe_sdo_upload_expedited_req *)mbx);
                    } else {
                        // #16
                        ecsd_coe_sdo_upload_normal_ind(pecsd, (struct ec_coe_sdo_upload_normal_req *)mbx);
                    }
                } else {
                    // #43 | #48
                    // ERROR return obj_ret error code.
                    ecsd_coe_abort_sdo_transfer_req(pecsd, (struct ec_coe_abort_sdo *)mbx, OBJECT_ERROR__OBJECT_DOES_NOT_EXIST);
                    pecsd->mbx.coe.state = COE_STATE_WUPD;
                }
            }
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

// CoE SDO Object Description indication.
ec_retval_t ecsd_coe_sdo_info_service_object_desc_ind(struct ecsd *pecsd, struct ec_coe_sdo_information_desc_req *mbx_desc_req) {
    assert(pecsd != NULL);
    ec_retval_t ret = EC_OK;
    ec_uint32_t abort_code = 0u;

    ec_uint16_t index = mbx_desc_req->index;
    struct object *obj = NULL;

    if (object_get(index, &obj) != OBJECT_OK) {
        // ERROR object index not found
        abort_code = OBJECT_ERROR__OBJECT_DOES_NOT_EXIST;
    } else {
        struct ec_coe_sdo_information_desc_resp *mbx_desc_resp = (struct ec_coe_sdo_information_desc_resp *)mbx_desc_req;
        EC_COE_SDO_INFO_HDR__OPCODE__SET(&mbx_desc_resp->sdo_hdr, EC_COE_SDO_INFO_HDR__GET_OBJECT_DESCRIPTION_RESPONSE);

        pecsd->mbx.coe.state = COE_STATE_INF;

        mbx_desc_resp->data_type = obj->data_type;
        mbx_desc_resp->max_subindex = obj->max_subindex;
        mbx_desc_resp->obj_code = obj->code;

        ec_size_t name_len = strlen(obj->name);
        mbx_desc_resp->mailbox_hdr.length = 12 + name_len;
        memcpy(&mbx_desc_resp->name[0], obj->name, name_len);

        // #59
        pecsd->mbx.coe.state = COE_STATE_WUPD;
        ecsd_coe_mailbox_read_upd_req(pecsd, (struct ec_mailbox *)mbx_desc_resp);
    }

    if (abort_code != EC_OK) {
        // ERROR return obj_ret error code.
        ecsd_coe_abort_sdo_transfer_req(pecsd, (struct ec_coe_abort_sdo *)mbx_desc_req, abort_code);
    }

    return ret;
}

// CoE SDO Object Dictionary list indication.
ec_retval_t ecsd_coe_sdo_info_service_odlist_ind(struct ecsd *pecsd, struct ec_coe_sdo_information_odlist_req *mbx_odlist_req) {
    assert(pecsd != NULL);
    ec_retval_t ret = EC_OK;

    // for now only listtype 1

    struct ec_coe_sdo_information_odlist_resp *mbx_odlist_resp = (struct ec_coe_sdo_information_odlist_resp *)mbx_odlist_req;

    if ((pecsd->mbx.coe.fragments_left > 0) && (pecsd->mbx.coe.seg == 0x80000000)) {
        // send next fragment
        size_t data_size = pecsd->mbx.sm_send.length - 6 - 8;
        data_size = object_get_list(1, &pecsd->mbx.coe.odlist_start_after, (uint16_t *)&mbx_odlist_resp->data[0], data_size);

        mbx_odlist_resp->mailbox_hdr.length = 6 + data_size;
        pecsd->mbx.coe.fragments_left--;

        if (pecsd->mbx.coe.fragments_left == 0) {
            pecsd->mbx.coe.seg = 0;
        }
    } else {
        // start sending odlist
        unsigned int obj_cnt = object_cnt(1);
        size_t data_size = pecsd->mbx.sm_send.length - 6 - 6 - 2;

        if (obj_cnt > (data_size / 2)) {
            // need fragments;
            pecsd->mbx.coe.fragments_left = (obj_cnt - 1) / (data_size / 2);
            pecsd->mbx.coe.seg = 0x80000000;
        }

        pecsd->mbx.coe.odlist_start_after = 0x0FFF; // start with 0x1000
        data_size = object_get_list(1, &pecsd->mbx.coe.odlist_start_after, (uint16_t *)&mbx_odlist_resp->data[2], data_size);

        mbx_odlist_resp->mailbox_hdr.length = 6 + 2 + data_size;
    }


    mbx_odlist_resp->sdo_hdr.fragment_left = pecsd->mbx.coe.fragments_left;

    ecsd_coe_mailbox_read_upd_req(pecsd, (struct ec_mailbox *)mbx_odlist_resp);

    pecsd->mbx.coe.state = COE_STATE_WUPD;

    return ret;
}

ec_retval_t ecsd_coe_sdo_info_service_ind(struct ecsd *pecsd, struct ec_coe_sdo_information *mbx) {
    assert(pecsd != NULL);
    ec_retval_t ret = EC_OK;

    switch (EC_COE_SDO_INFO_HDR__OPCODE(&mbx->sdo_hdr)) {
        case EC_COE_SDO_INFO_HDR__GET_OD_LIST_REQUEST:
        case EC_COE_SDO_INFO_HDR__GET_OBJECT_DESCRIPTION_REQUEST: {
            // #19 | #20
            if (mbx->mailbox_hdr.length != 8) {
                // #19
                ecsd_coe_mailbox_error_reply(pecsd, EC_MBXERR_INVALIDSIZE, (struct ec_mailbox_error_reply *)mbx);
            } else {
                // #20
                pecsd->mbx.coe.state = COE_STATE_INF;

                // #60
                if (EC_COE_SDO_INFO_HDR__OPCODE(&mbx->sdo_hdr) == EC_COE_SDO_INFO_HDR__GET_OBJECT_DESCRIPTION_REQUEST) {
                    ecsd_coe_sdo_info_service_object_desc_ind(pecsd, (struct ec_coe_sdo_information_desc_req *)mbx);
                } else { // if (EC_COE_SDO_INFO_HDR__OPCODE(&mbx->sdo_hdr) == EC_COE_SDO_INFO_HDR__GET_OD_LIST_REQUEST)
                    ecsd_coe_sdo_info_service_odlist_ind(pecsd, (struct ec_coe_sdo_information_odlist_req *)mbx);
                }
            }
            break;
        }
        case EC_COE_SDO_INFO_HDR__GET_ENTRY_DESCRIPTION_REQUEST: {
            // #21 | #22
            if (mbx->mailbox_hdr.length != 10) {
                // #21
                ecsd_coe_mailbox_error_reply(pecsd, EC_MBXERR_INVALIDSIZE, (struct ec_mailbox_error_reply *)mbx);
            } else {
                // #22
                struct ec_coe_sdo_information_entry_desc_req *mbx_entry_desc_req = (struct ec_coe_sdo_information_entry_desc_req *)mbx;
                ec_uint32_t abort_code = 0u;
                ec_uint16_t index = mbx_entry_desc_req->index;
                struct object *obj = NULL;

                if (object_get(index, &obj) != OBJECT_OK) {
                    // ERROR object index not found
                    abort_code = OBJECT_ERROR__OBJECT_DOES_NOT_EXIST;
                } else {
                    if (obj->max_subindex < mbx_entry_desc_req->sub_index) {
                        abort_code = OBJECT_ERROR__SUBINDEX_DOES_NOT_EXIST;
                    } else {
                        struct object_entry *entry = &obj->entries[mbx_entry_desc_req->sub_index];
                        struct ec_coe_sdo_information_entry_desc_resp *mbx_entry_desc_resp = (struct ec_coe_sdo_information_entry_desc_resp *)mbx;
                        EC_COE_SDO_INFO_HDR__OPCODE__SET(&mbx_entry_desc_resp->sdo_hdr, EC_COE_SDO_INFO_HDR__GET_ENTRY_DESCRIPTION_RESPONSE);

                        pecsd->mbx.coe.state = COE_STATE_INF;

                        mbx_entry_desc_resp->data_type = entry->data_type;
                        mbx_entry_desc_resp->bit_length = entry->bit_length;
                        mbx_entry_desc_resp->obj_access = entry->access;

//                        ec_size_t entry_byte_size = (entry->bit_length + 7u) / 8u;
                        ec_size_t data_len = 0u;
      
                        mbx_entry_desc_resp->value_info = 0u;
#if 0
                        if (mbx_entry_desc_req->value_info & EC_COE_SDO_INFO_ENTRY_DESC__VALUE_INFO__UNIT) {
                            // TODO insert unit at &mbx_desc_resp->data[data_len]
                            data_len += 4u;
                        }

                        if (mbx_entry_desc_req->value_info & EC_COE_SDO_INFO_ENTRY_DESC__VALUE_INFO__DEFAULT_VALUE) {
                            // TODO insert default value at &mbx_desc_resp->data[data_len]
                            data_len += entry_byte_size;
                        }

                        if (mbx_entry_desc_req->value_info & EC_COE_SDO_INFO_ENTRY_DESC__VALUE_INFO__MIN_VALUE) {
                            // TODO insert min value at &mbx_desc_resp->data[data_len]
                            data_len += entry_byte_size;
                        }

                        if (mbx_entry_desc_req->value_info & EC_COE_SDO_INFO_ENTRY_DESC__VALUE_INFO__MAX_VALUE) {
                            // TODO insert max value at &mbx_desc_resp->data[data_len]
                            data_len += entry_byte_size;
                        }
#endif

                        ec_size_t name_len = strlen(entry->name);
                        mbx->mailbox_hdr.length = 16 + data_len + name_len;
                        memcpy(&mbx_entry_desc_resp->data[data_len], entry->name, name_len);

                        // #59
                        pecsd->mbx.coe.state = COE_STATE_WUPD;
                        ecsd_coe_mailbox_read_upd_req(pecsd, (struct ec_mailbox *)mbx);
                    }
                }

                if (abort_code != EC_OK) {
                    // ERROR return obj_ret error code.
                    ecsd_coe_abort_sdo_transfer_req(pecsd, (struct ec_coe_abort_sdo *)mbx, abort_code);
                }
            }
            break;
        }
        default: {
            break;
        }
    }

    return ret;
}

ec_void_t ecsd_coe_mailbox_error_reply(struct ecsd *pecsd, ec_uint8_t detail, struct ec_mailbox_error_reply *mbx) 
{
    mbx->mailbox_hdr.length  = 4u;
    EC_MBX_HDR__TYPE__SET(&mbx->mailbox_hdr, EC_MBX_HDR__TYPE__ERR);
    mbx->type                = EC_MBX_ERROR_REPLY__TYPE__MAILBOX_COMMAND;
    mbx->detail              = detail;

    ecsd_coe_mailbox_read_upd_req(pecsd, (struct ec_mailbox *)mbx);

    pecsd->mbx.coe.state = COE_STATE_ERR;
}

//! \brief 
/*!
 * Refer to Mailbox Read Upd Service Definition ETG.1000.3
 */
ec_retval_t ecsd_coe_mailbox_read_upd_req(struct ecsd *pecsd, struct ec_mailbox *mbx) {
    assert (pecsd != NULL);
    ec_retval_t ret = 0u;

    // Send update request to MainDevice.
    ecsd_mailbox_read_upd_req(pecsd, mbx);

    // Store for repeat or continuation.
    memcpy(&pecsd->mbx.coe.repeat_mbx_buffer[0], (void *)mbx, ECSD_MAILBOX_READ__MAX_LENGTH);

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
ec_retval_t ecsd_coe_mailbox_read_upd_cnf(struct ecsd *pecsd, ec_bool_t success) {
    assert (pecsd != NULL);
    ec_retval_t ret = 0u;

    if (success == EC_TRUE) {
        switch (pecsd->mbx.coe.state) {
            case COE_STATE_WUPD:
                // #63 | #64 | #65 | #66 | #67
                if (pecsd->mbx.coe.seg == 0) {
                    // #63
                    pecsd->mbx.coe.state = COE_STATE_IDLE;
                } else if ((pecsd->mbx.coe.seg == 0x80000000) && (pecsd->mbx.coe.fragments_left == 0)) {
                    // #64
                    pecsd->mbx.coe.seg = 0u;
                    pecsd->mbx.coe.state = COE_STATE_IDLE;
                } else if ((pecsd->mbx.coe.seg == 0x80000000) && (pecsd->mbx.coe.fragments_left > 0)) {
                    // #65
                    pecsd->mbx.coe.state = COE_STATE_INF;
                    struct ec_coe_sdo_information *mbx = (struct ec_coe_sdo_information *)&pecsd->mbx.coe.repeat_mbx_buffer[0];

                    switch (EC_COE_SDO_INFO_HDR__OPCODE(&mbx->sdo_hdr)) {
                        case EC_COE_SDO_INFO_HDR__GET_OD_LIST_REQUEST:
                        case EC_COE_SDO_INFO_HDR__GET_OBJECT_DESCRIPTION_REQUEST: {
                            // #59
                            if (EC_COE_SDO_INFO_HDR__OPCODE(&mbx->sdo_hdr) == EC_COE_SDO_INFO_HDR__GET_OBJECT_DESCRIPTION_REQUEST) {
                                ecsd_coe_sdo_info_service_object_desc_ind(pecsd, (struct ec_coe_sdo_information_desc_req *)mbx);
                            } else { // if (EC_COE_SDO_INFO_HDR__OPCODE(&mbx->sdo_hdr) == EC_COE_SDO_INFO_HDR__GET_OD_LIST_REQUEST)
                                ecsd_coe_sdo_info_service_odlist_ind(pecsd, (struct ec_coe_sdo_information_odlist_req *)mbx);
                            }
                            break;
                        }
                        default:
                            break;
                    }

                } else if (pecsd->mbx.coe.seg < 0x80000000) {
                    // #66
                    pecsd->mbx.coe.state = COE_STATE_WSEG;
                } else if (pecsd->mbx.coe.seg > 0x80000000) {
                    // #66
                    pecsd->mbx.coe.state = COE_STATE_WSEG;
                }
                break;
            case COE_STATE_INF:
                break;
            default:
                break;
        }
    } else {
        // send again
        ecsd_coe_mailbox_read_upd_req(pecsd, (struct ec_mailbox *)&pecsd->mbx.coe.repeat_mbx_buffer[0]);
    }

    return ret;
}

//! \brief 
/*!
 * Refer to Service Definition ETG.1000.3
 */
ec_retval_t ecsd_coe_mailbox_start(struct ecsd *pecsd) {
    pecsd->mbx.coe.fragments_left = 0;
    pecsd->mbx.coe.seg = 0x0u;

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
            // Ignore
            break;
        }
        case COE_STATE_IDLE:
        case COE_STATE_WSEG: {
            break;
        }
        case COE_STATE_ERR:
        case COE_STATE_WUPD: {
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

    return 0;
}

#endif /* ECSD_MAILBOX_COE_SUPPORT */
