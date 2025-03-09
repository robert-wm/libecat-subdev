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

#include <libecat-subdev/foe.h>
#include <libecat-subdev/ecsd.h>
#include <libecat-subdev/mailbox.h>

#include <assert.h>
#include <stddef.h>
#include <string.h>

// forward declarations

ec_retval_t ecsd_foe_mailbox_read_upd_req(struct ecsd *pecsd, struct ec_mailbox *mbx);

ec_retval_t __attribute__((weak)) foe_read_ind(struct ecsd *pecsd, ec_uint32_t password, ec_char_t *filename) {
    ec_retval_t ret = EC_OK;
    return ret;
}

ec_retval_t __attribute__((weak)) foe_write_ind(struct ecsd *pecsd, ec_uint32_t password, ec_char_t *filename) {
    ec_retval_t ret = EC_OK;
    return ret;
}

//! \brief 
/*!
 * Refer to Service Definition ETG.1000.5
 */
ec_retval_t __attribute__((weak)) ecsd_foe_mailbox_write_ind(struct ecsd *pecsd, struct ec_foe_service_data *mbx) {
    ec_retval_t ret = EC_OK;

    switch (pecsd->mbx.foe.state) {
        case FOE_STATE_IDLE: {
            // #5 | #6 | #7 | #8 | #9 | #10
            if ((mbx->foe_hdr.opcode == 0) || (mbx->foe_hdr.opcode > 4)) {
                // #5
                struct ec_mailbox_error_reply *mbx_err = (struct ec_mailbox_error_reply *)mbx;
                mbx_err->mailbox_hdr.length  = 4u;
                EC_MBX_HDR__TYPE__SET(&mbx_err->mailbox_hdr, EC_MBX_HDR__TYPE__ERR);
                mbx_err->type                = EC_MBX_ERROR_REPLY__TYPE__MAILBOX_COMMAND;
                mbx_err->detail              = EC_MBXERR_INVALIDHEADER;

                ecsd_mailbox_read_upd_req(pecsd, (struct ec_mailbox *)mbx_err);

                pecsd->mbx.foe.state = FOE_STATE_ERR;
            } else if ((mbx->mailbox_hdr.length <= 6) && (mbx->foe_hdr.opcode == FOE_OPCODE__WRITE_REQUEST)) {
                // #6
                struct ec_mailbox_error_reply *mbx_err = (struct ec_mailbox_error_reply *)mbx;
                mbx_err->mailbox_hdr.length  = 4u;
                EC_MBX_HDR__TYPE__SET(&mbx_err->mailbox_hdr, EC_MBX_HDR__TYPE__ERR);
                mbx_err->type                = EC_MBX_ERROR_REPLY__TYPE__MAILBOX_COMMAND;
                mbx_err->detail              = EC_MBXERR_INVALIDSIZE;

                ecsd_mailbox_read_upd_req(pecsd, (struct ec_mailbox *)mbx_err);

                pecsd->mbx.foe.state = FOE_STATE_ERR;
            } else if ((mbx->mailbox_hdr.length > 6) && (mbx->foe_hdr.opcode == FOE_OPCODE__WRITE_REQUEST)) {
                // #7
                struct ec_foe_write_request *mbx_wr = (struct ec_foe_write_request *)mbx;
                pecsd->mbx.foe.state = FOE_STATE_DWLS;

                // reuse repeat mbx buffer for temporary filename storage
                memcpy(&pecsd->mbx.foe.repeat_mbx_buffer[0], mbx_wr->filename, mbx_wr->mailbox_hdr.length - 6);
                pecsd->mbx.foe.repeat_mbx_buffer[mbx_wr->mailbox_hdr.length - 6] = '\0';

                ret = foe_write_ind(pecsd, mbx_wr->password, (ec_char_t *)&pecsd->mbx.foe.repeat_mbx_buffer[0]);
            } else if ((mbx->mailbox_hdr.length <= 6) && (mbx->foe_hdr.opcode == FOE_OPCODE__READ_REQUEST)) {
                // #8
                struct ec_mailbox_error_reply *mbx_err = (struct ec_mailbox_error_reply *)mbx;
                mbx_err->mailbox_hdr.length  = 4u;
                EC_MBX_HDR__TYPE__SET(&mbx_err->mailbox_hdr, EC_MBX_HDR__TYPE__ERR);
                mbx_err->type                = EC_MBX_ERROR_REPLY__TYPE__MAILBOX_COMMAND;
                mbx_err->detail              = EC_MBXERR_INVALIDSIZE;

                ecsd_mailbox_read_upd_req(pecsd, (struct ec_mailbox *)mbx_err);

                pecsd->mbx.foe.state = FOE_STATE_ERR;
            } else if ((mbx->mailbox_hdr.length > 6) && (mbx->foe_hdr.opcode == FOE_OPCODE__READ_REQUEST)) {
                // #9
                struct ec_foe_read_request *mbx_rr = (struct ec_foe_read_request *)mbx;
                pecsd->mbx.foe.state = FOE_STATE_UPLS;

                // reuse repeat mbx buffer for temporary filename storage
                memcpy(&pecsd->mbx.foe.repeat_mbx_buffer[0], mbx_rr->filename, mbx_rr->mailbox_hdr.length - 6);
                pecsd->mbx.foe.repeat_mbx_buffer[mbx_rr->mailbox_hdr.length - 6] = '\0';

                ret = foe_read_ind(pecsd, mbx_rr->password, (ec_char_t *)&pecsd->mbx.foe.repeat_mbx_buffer[0]);
            } else if ((mbx->mailbox_hdr.length == 3) && (
                        (mbx->foe_hdr.opcode == FOE_OPCODE__DATA_REQUEST) || (mbx->foe_hdr.opcode == FOE_OPCODE__ACK_REQUEST))) 
            {
                // #10
                // TODO
            }
            break;
        }
    }

    return ret;
}

ec_retval_t ecsd_foe_read_upd_cnf(struct ecsd *pecsd, ec_bool_t success) {
    ec_retval_t ret = EC_OK;

    switch (pecsd->mbx.foe.state) {
        case FOE_STATE_IDLE: {
            // #11
            // ignore
            break;
        }
        case FOE_STATE_UWUPD: {
            // #26
            pecsd->mbx.foe.state = FOE_STATE_UWSEG;
            break;
        }
        case FOE_STATE_DWUPD: {
            // #34
            pecsd->mbx.foe.state = FOE_STATE_DWSEG;
            break;
        }
        default: {
            // ignore
            break;
        }
    }

    return ret;
}

ec_retval_t ecsd_foe_error_request(struct ecsd *pecsd, struct ec_mailbox *mbx, ec_uint32_t error_code, ec_char_t *error_text) {
    ec_retval_t ret = EC_OK;
    struct ec_foe_error_request *mbx_err = (struct ec_foe_error_request *)mbx;

    switch (pecsd->mbx.foe.state) {
        case FOE_STATE_UPLS:
        case FOE_STATE_DWLS:
            // #15 | #20
            pecsd->mbx.foe.seg = 0u;
            size_t error_len = min(pecsd->mbx.sm_send.length - 6 - 6, strlen(error_text));
            mbx_err->mailbox_hdr.length = 6 + error_len;
            mbx_err->foe_hdr.opcode = FOE_OPCODE__ERROR_REQUEST;
            mbx_err->error_code = error_code;
            memcpy(&mbx_err->error_text[0], error_text, error_len);

            ret = ecsd_foe_mailbox_read_upd_req(pecsd, mbx);

            pecsd->mbx.foe.state = FOE_STATE_WUPD;
            break;
        default:
            break;
    }

    return ret;
}

ec_retval_t ecsd_foe_ack_request(struct ecsd *pecsd, struct ec_mailbox *mbx, ec_uint32_t packet_no) {
    ec_retval_t ret = EC_OK;
    struct ec_foe_ack_request *mbx_ack = (struct ec_foe_ack_request *)mbx;

    switch (pecsd->mbx.foe.state) {
        case FOE_STATE_DWLS:
            // #16
            mbx_ack->mailbox_hdr.length = 6;
            mbx_ack->foe_hdr.opcode = FOE_OPCODE__ACK_REQUEST;
            mbx_ack->packet_no = packet_no;

            ret = ecsd_foe_mailbox_read_upd_req(pecsd, mbx);

            pecsd->mbx.foe.state = FOE_STATE_DWUPD;
            break;
        default:
            break;
    }

    return ret;
}

ec_retval_t ecsd_foe_data_req(struct ecsd *pecsd, struct ec_mailbox *mbx, ec_uint32_t packet_no, ec_size_t size, uint8_t *data) {
    ec_retval_t ret = EC_OK;

    struct ec_foe_data_request *mbx_data = (struct ec_foe_data_request *)mbx;

    switch (pecsd->mbx.foe.state) {
        case FOE_STATE_UPLS: {
            // #21 | #22
            size_t copy_len = min(pecsd->mbx.sm_send.length - 6 - 6, size);

            if (copy_len == size) {
                // #21
                mbx_data->mailbox_hdr.length = 6u + copy_len;
                mbx_data->foe_hdr.opcode = FOE_OPCODE__DATA_REQUEST;
                mbx_data->packet_no = packet_no;
                memcpy(&mbx_data->data[0], &data[0], copy_len);
                pecsd->mbx.foe.seg = packet_no + 1;

                ret = ecsd_foe_mailbox_read_upd_req(pecsd, mbx);
                pecsd->mbx.foe.state = FOE_STATE_UWUPD;
            } else if (copy_len < size) {
                // #22
                mbx_data->mailbox_hdr.length = 6u + copy_len;
                mbx_data->foe_hdr.opcode = FOE_OPCODE__DATA_REQUEST;
                mbx_data->packet_no = packet_no;
                memcpy(&mbx_data->data[0], &data[0], copy_len);
                pecsd->mbx.foe.seg = packet_no + 1;

                ret = ecsd_foe_mailbox_read_upd_req(pecsd, mbx);
                pecsd->mbx.foe.state = FOE_STATE_WUPD;
            }
            break;
        }
        default:
            break;
    }

    return ret;
}

//! \brief 
/*!
 * Refer to Mailbox Read Upd Service Definition ETG.1000.3
 */
ec_retval_t ecsd_foe_mailbox_read_upd_req(struct ecsd *pecsd, struct ec_mailbox *mbx) {
    assert (pecsd != NULL);
    ec_retval_t ret = 0u;

    // Send update request to MainDevice.
    ecsd_mailbox_read_upd_req(pecsd, mbx);

    // Store for repeat or continuation.
    memcpy(&pecsd->mbx.foe.repeat_mbx_buffer[0], (void *)mbx, ECSD_MAILBOX_READ__MAX_LENGTH);

    return ret;
}

//! \brief 
/*!
 * Refer to Service Definition ETG.1000.5
 */
ec_retval_t __attribute__((weak)) ecsd_foe_mailbox_read_ind(struct ecsd *pecsd, struct ec_foe_service_data *mbx) {
    ec_retval_t ret = EC_OK;

    return ret;
}

//! \brief 
/*!
 * Refer to Service Definition ETG.1000.5
 */
ec_retval_t __attribute__((weak)) ecsd_foe_mailbox_data_ind(struct ecsd *pecsd, struct ec_foe_service_data *mbx) {
    ec_retval_t ret = EC_OK;

    return ret;
}

//! \brief 
/*!
 * Refer to Service Definition ETG.1000.5
 */
ec_retval_t __attribute__((weak)) ecsd_foe_mailbox_ack_ind(struct ecsd *pecsd, struct ec_foe_service_data *mbx) {
    ec_retval_t ret = EC_OK;    

    return ret;
}

//! \brief 
/*!
 * Refer to Service Definition ETG.1000.5
 */
ec_retval_t __attribute__((weak)) ecsd_foe_mailbox_error_ind(struct ecsd *pecsd, struct ec_foe_service_data *mbx) {
    ec_retval_t ret = EC_OK;

    return ret;
}

//! \brief
/*!
 */
ec_retval_t ecsd_foe_mailbox_start(struct ecsd *pecsd) {
    ec_retval_t ret = EC_OK;

    pecsd->mbx.foe.seg = 0u;

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
