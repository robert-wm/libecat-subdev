
#define ESC_COE_HDR__SERVICE__MASK                                  ((osal_uint16_t)0xF000)
#define ESC_COE_HDR__SERVICE__SHIFT                                 ((osal_uint32_t)12)
#define ESC_COE_HDR__SERVICE(coe_hdr)                               (((coe_hdr).hdr & ESC_COE_HDR__SERVICE__MASK) >> ESC_COE_HDR__SERVICE__SHIFT)
#define ESC_COE_HDR__SERVICE_SET(coe_hdr, val)                      ((coe_hdr)->hdr & ~ESC_COE_HDR__SERVICE__MASK) | (val << ESC_COE_HDR__SERVICE__SHIFT)

#define ESC_COE_HDR__SERVICE__EMERGENCY                             ((osal_uint8_t)0x01u)   //!< \brief Emergency
#define ESC_COE_HDR__SERVICE__SDO_REQUEST                           ((osal_uint8_t)0x02u)   //!< \brief SDO Request
#define ESC_COE_HDR__SERVICE__SDO_RESPONSE                          ((osal_uint8_t)0x03u)   //!< \brief SDO Response
#define ESC_COE_HDR__SERVICE__TXPDO                                 ((osal_uint8_t)0x04u)   //!< \brief TxPDO
#define ESC_COE_HDR__SERVICE__RXPDO                                 ((osal_uint8_t)0x05u)   //!< \brief RxPDO
#define ESC_COE_HDR__SERVICE__TXPDO_REMOTE_REQUEST                  ((osal_uint8_t)0x06u)   //!< \brief TxPDO remote request
#define ESC_COE_HDR__SERVICE__RXPDO_REMOTE_REQUEST                  ((osal_uint8_t)0x07u)   //!< \brief RxPDO remote request
#define ESC_COE_HDR__SERVICE__SDO_INFORMATION                       ((osal_uint8_t)0x08u)   //!< \brief SDO Information

#define ESC_COE_SDO_HDR__HDR_SIZE__MASK                             ((osal_uint8_t)0x01u)
#define ESC_COE_SDO_HDR__HDR_SIZE(sdo_hdr)                          ((sdo_hdr).hdr & ESC_COE_SDO_HDR__HDR_SIZE__MASK)
#define ESC_COE_SDO_HDR__HDR_TRANSFER_TYPE__MASK                    ((osal_uint8_t)0x02u)
#define ESC_COE_SDO_HDR__HDR_TRANSFER_TYPE__SHIFT                   ((osal_uint32_t)1u)
#define ESC_COE_SDO_HDR__HDR_TRANSFER_TYPE(sdo_hdr)                 (((sdo_hdr).hdr & ESC_COE_SDO_HDR__HDR_TRANSFER_TYPE__MASK) >> ESC_COE_SDO_HDR__HDR_TRANSFER_TYPE__SHIFT)

#define ESC_COE_SDO_HDR__TRANSFER_TYPE__NORMAL                      ((esc_uint8_t)0u)
#define ESC_COE_SDO_HDR__TRANSFER_TYPE__EXPEDITED                   ((esc_uint8_t)1u)

#define ESC_COE_SDO_HDR__DATA_SET_SIZE__MASK                        ((esc_uint8_t)0x0Cu)
#define ESC_COE_SDO_HDR__DATA_SET_SIZE__SHIFT                       ((esc_uint8_t)2u)
#define ESC_COE_SDO_HDR__DATA_SET_SIZE(sdo_hdr)                     (((sdo_hdr)->hdr & ESC_COE_SDO_HDR__DATA_SET_SIZE__MASK) >> ESC_COE_SDO_HDR__DATA_SET_SIZE__SHIFT)
#define ESC_COE_SDO_HDR__DATA_SET_SIZE_SET(sdo_hdr, val)            ((sdo_hdr)->hdr & ~ESC_COE_SDO_HDR__DATA_SET_SIZE__MASK) | (val << ESC_COE_SDO_HDR__DATA_SET_SIZE__SHIFT)

#define ESC_COE_SDO_HDR__COMMAND_SPECIFIER__MASK                    ((esc_uint8_t)0xE0u)
#define ESC_COE_SDO_HDR__COMMAND_SPECIFIER__SHIFT                   ((esc_uint8_t)5u)
#define ESC_COE_SDO_HDR__COMMAND_SPECIFIER(sdo_hdr)                 (((sdo_hdr)->hdr & ESC_COE_SDO_HDR__COMMAND_SPECIFIER__MASK) >> ESC_COE_SDO_HDR__COMMAND_SPECIFIER__SHIFT)
#define ESC_COE_SDO_HDR__COMMAND_SPECIFIER_SET(sdo_hdr, val)        ((sdo_hdr)->hdr & ~ESC_COE_SDO_HDR__COMMAND_SPECIFIER__MASK) | (val << ESC_COE_SDO_HDR__COMMAND_SPECIFIER__SHIFT)

#define ESC_COE_SDO_HDR__COMMAND_SPECIFIER__DOWNLOAD_SEGMENT_REQUEST    ((esc_uint8_t)0x00u)    //!< \brief Download Segment Request
#define ESC_COE_SDO_HDR__COMMAND_SPECIFIER__DOWNLOAD_REQUEST            ((esc_uint8_t)0x01u)    //!< \brief Download Request
#define ESC_COE_SDO_HDR__COMMAND_SPECIFIER__UPLOAD_REQUEST              ((esc_uint8_t)0x02u)    //!< \brief Upload Request
#define ESC_COE_SDO_HDR__COMMAND_SPECIFIER__UPLOAD_SEGMENT_REQUEST      ((esc_uint8_t)0x03u)    //!< \brief Upload Segment Request
#define ESC_COE_SDO_HDR__COMMAND_SPECIFIER__ABORT_REQUEST               ((esc_uint8_t)0x04u)    //!< \brief Abort Transfer Request

#define ESC_COE_SDO_HDR__COMMAND_SPECIFIER__DOWNLOAD_SEGMENT_RESPONSE   ((esc_uint8_t)0x01u)    //!< \brief Download Segment Response
#define ESC_COE_SDO_HDR__COMMAND_SPECIFIER__DOWNLOAD_RESPONSE           ((esc_uint8_t)0x03u)    //!< \brief Download Response
#define ESC_COE_SDO_HDR__COMMAND_SPECIFIER__UPLOAD_RESPONSE             ((esc_uint8_t)0x02u)    //!< \brief Upload Response
#define ESC_COE_SDO_HDR__COMMAND_SPECIFIER__UPLOAD_SEGMENT_RESPONSE     ((esc_uint8_t)0x00u)    //!< \brief Upload Segment Response

#define ESC_COE_SDO_INFO_HDR__GET_OD_LIST_REQUEST                   ((esc_uint8_t)0x01u)    //!< \brief Get OD List Request
#define ESC_COE_SDO_INFO_HDR__GET_OD_LIST_RESPONSE                  ((esc_uint8_t)0x02u)    //!< \brief Get OD List Response
#define ESC_COE_SDO_INFO_HDR__GET_OBJECT_DESCRIPTION_REQUEST        ((esc_uint8_t)0x03u)    //!< \brief Get Object Description Request
#define ESC_COE_SDO_INFO_HDR__GET_OBJECT_DESCRIPTION_RESPONSE       ((esc_uint8_t)0x04u)    //!< \brief Get Object Description Response
#define ESC_COE_SDO_INFO_HDR__GET_ENTRY_DESCRIPTION_REQUEST         ((esc_uint8_t)0x05u)    //!< \brief Get Entry Description Request
#define ESC_COE_SDO_INFO_HDR__GET_ENTRY_DESCRIPTION_RESPONSE        ((esc_uint8_t)0x06u)    //!< \brief Get Entry Description Response
#define ESC_COE_SDO_INFO_HDR__SDO_INFO_ERROR_REQUEST                ((esc_uint8_t)0x07u)    //!< \brief SDO Info Error Request


esc_void_t esc_coe_sdo_download_expedited_ind(esc_t *pesc, esc_uint16_t address, esc_uint16_t index, esc_uint8_t sub_index, esc_size_t size, esc_uint8_t *data);
esc_void_t esc_coe_sdo_download_normal_ind(esc_t *pesc, esc_uint16_t address, esc_uint16_t index, esc_uint8_t sub_index, esc_size_t complete_size, esc_size_t size, esc_uint8_t *data);
esc_void_t esc_coe_download_sdo_segment_ind(esc_t *pesc, esc_uint16_t address, esc_bool_t toggle, esc_bool_t more_follows, esc_size_t_size, esc_uint8_t *data);
esc_void_t esc_coe_sdo_upload_expedited_ind(esc_t *pesc, esc_uint16_t address, esc_uint16_t index, esc_uint8_t sub_index);

typedef struct esc_coe_hdr {
    esc_uint16_t        hdr;
} esc_coe_hdr_t;

typedef struct esc_coe_service_data {
    esc_mailbox_hdr_t   mailbox_hdr;
    esc_coe_hdr_t       coe_hdr;
    esc_data_t          data[0];
} esc_coe_service_data_t;

typedef struct esc_coe_sdo_hdr {
    esc_uint8_t         hdr;
    esc_uint16_t        index;
    esc_uint8_t         sub_index;
} esc_coe_sdo_hdr_t;

typedef struct esc_coe_sdo_download_expedited {
    esc_mailbox_hdr_t   mailbox_hdr;
    esc_coe_hdr_t       coe_hdr;
    esc_coe_sdo_hdr_t   sdo_hdr;
    esc_data_t          data[0];
} esc_coe_sdo_download_expedited_t;

esc_retval_t esc_coe_sdo_download_expedited_ind(esc_t *pesc, esc_coe_sdo_download_expedited_t *mbx) {
    assert(pesc != NULL);
    esc_retval_t ret = ESC_OK;

    esc_bool_t   complete_access = mbx->sdo_hdr.complete_access;
    esc_uint16_t index           = mbx->sdo_hdr.index;
    esc_uint8_t  sub_index       = mbx->sdo_hdr.sub_index;
    esc_size_t   data_set_size   = ESC_COE_SDO_HDR__DATA_SET_SIZE(&mbx->sdo_hdr);
    esc_uint8_t *data            = &mbx->data[0];
    esc_size_t   data_length     = mbx->mailbox_hdr.length - 10;

    // write downloaded SDO to dictionary
    esc_obj_write_object(esc, index, sub_index, complete_access, *data, data_set_size);

    mbx->coe_hdr.hdr = 0u;
    ESC_COE_HDR__SERVICE_SET(&mbx->coe_hdr, ESC_COE_HDR__SERVICE__SDO_RESPONSE);
    
    mbx->sdo_hdr.hdr = 0u;
    ESC_COE_SDO_HDR__COMMAND_SPECIFIER_SET(&mbx->sdo_hdr, ESC_COE_SDO_HDR__COMMAND_SPECIFIER__DOWNLOAD_RESPONSE);

    return ret;
}

esc_retval_t esc_coe_sdo_download_normal_ind(esc_t *pesc, esc_coe_sdo_download_expedited_t *mbx) {
    assert(pesc != NULL);
    esc_retval_t ret = ESC_OK;

    esc_bool_t   complete_access = mbx->sdo_hdr.complete_access;
    esc_uint16_t index           = mbx->sdo_hdr.index;
    esc_uint8_t  sub_index       = mbx->sdo_hdr.sub_index;
    esc_size_t   complete_size   = *(esc_uint32_t *)&mbx->data[0];
    esc_uint8_t *data            = &mbx->data[4];
    esc_size_t   data_length     = mbx->mailbox_hdr.length - 10;

    // write downloaded SDO to dictionary
    esc_uint32_t abort_code = esc_obj_write_object(esc, index, sub_index, complete_access, *data, data_length);

    if (abort_code != 0u) {
        mbx->coe_hdr.hdr = 0u;
        ESC_COE_HDR__SERVICE_SET(&mbx->coe_hdr, ESC_COE_HDR__SERVICE__SDO_REQUEST);

        mbx->sdo_hdr.hdr = 0u;
        ESC_COE_SDO_HDR__COMMAND_SPECIFIER_SET(&mbx->sdo_hdr, ESC_COE_SDO_HDR__COMMAND_SPECIFIER__ABORT_REQUEST);

        mbx->sdo_hdr.index = index;
        mbx->sdo_hdr.sub_index = sub_index;
        *(esc_uint32_t *)&mbx->data[0] = abort_code;
    } else {
        mbx->coe_hdr.hdr = 0u;
        ESC_COE_HDR__SERVICE_SET(&mbx->coe_hdr, ESC_COE_HDR__SERVICE__SDO_RESPONSE);

        mbx->sdo_hdr.hdr = 0u;
        ESC_COE_SDO_HDR__COMMAND_SPECIFIER_SET(&mbx->sdo_hdr, ESC_COE_SDO_HDR__COMMAND_SPECIFIER__DOWNLOAD_RESPONSE);
    }

    return ret;
}


esc_retval_t esc_coe_sdo_request(esc_t *pesc, esc_coe_sdo_request_t *mbx) {
    assert(pesc != NULL);
    esc_retval_t ret = ESC_OK;

    switch (ESC_COE_SDO_HDR__COMMAND_SPECIFIER(*mbx)) {
        case ESC_COE_SDO_HDR__COMMAND_SPECIFIER__DOWNLOAD_SEGMENT_REQUEST:
            // #11 | #12
            if (mbx->mailbox_hdr.length < 10) {
                // #11
                esc_coe_mailbox_error_reply(pesc, (esc_mailbox_error_reply_t *)mbx, ESC_MBXERR_INVALIDSIZE);
            } else {
                // #12
            }
            break;
        case ESC_COE_SDO_HDR__COMMAND_SPECIFIER__DOWNLOAD_REQUEST:
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
        case ESC_COE_SDO_HDR__COMMAND_SPECIFIER__UPLOAD_REQUEST:
            // #13 | #14 | #15 | #16
            break;
        case ESC_COE_SDO_HDR__COMMAND_SPECIFIER__UPLOAD_SEGMENT_REQUEST:
            break;
            // #17 | #18
        default:
            // #6
            esc_coe_mailbox_error_reply(pesc, ESC_MBXERR_INVALIDHEADER, (esc_mailbox_error_reply_t *)sd);
            break;
    }

    if ( 
            (sd->mailbox_hdr.length != 10) && 
            (ESC_COE_SDO_HDR__COMMAND_SPECIFIER(*sdo_hdr) == ESC_COE_SDO_HDR__COMMAND_SPECIFIER__DOWNLOAD_REQUEST) && 
            (ESC_COE_SDO_HDR__HDR_TRANSFER_TYPE(*sdo_hdr) == ESC_COE_SDO_HDR__TRANSFER_TYPE__EXPEDITED)     ) 
    {
        // #7
        esc_coe_mailbox_error_reply(pesc, ESC_MBXERR_INVALIDSIZE, (esc_mailbox_error_reply_t *)sd);
    } else if (
            (sd->mailbox_hdr.length == 10) &&
            (ESC_COE_SDO_HDR__COMMAND_SPECIFIER(*sdo_hdr) == ESC_COE_SDO_HDR__COMMAND_SPECIFIER__DOWNLOAD_REQUEST) &&
            (ESC_COE_SDO_HDR__HDR_TRANSFER_TYPE(*sdo_hdr) == ESC_COE_SDO_HDR__TRANSFER_TYPE__EXPEDITED) ) {
        // #8
        // TODO
    } else if (
            (sd->mailbox_hdr.length >= 10) &&
            (ESC_COE_SDO_HDR__COMMAND_SPECIFIER(*sdo_hdr) == ESC_COE_SDO_HDR__COMMAND_SPECIFIER__DOWNLOAD_REQUEST) &&
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
