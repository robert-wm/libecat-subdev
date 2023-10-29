
#define ESC_COE_HDR__SERVICE__MASK                                  ((osal_uint16_t)0xF000)
#define ESC_COE_HDR__SERVICE__SHIFT                                 ((osal_uint32_t)12)
#define ESC_COE_HDR__SERVICE(coe_hdr)                               (((coe_hdr).hdr & ESC_COE_HDR__SERVICE__MASK) >> ESC_COE_HDR__SERVICE__SHIFT)

#define ESC_COE_HDR__SERVICE__SDO_REQUEST                           ((osal_uint8_t)0x02)
#define ESC_COE_HDR__SERVICE__SDO_INFORMATION                       ((osal_uint8_t)0x08)


#define ESC_COE_SDO_HDR__HDR_SIZE__MASK                             ((osal_uint8_t)0x01u)
#define ESC_COE_SDO_HDR__HDR_SIZE(sdo_hdr)                          ((sdo_hdr).hdr & ESC_COE_SDO_HDR__HDR_SIZE__MASK)
#define ESC_COE_SDO_HDR__HDR_TRANSFER_TYPE__MASK                    ((osal_uint8_t)0x02u)
#define ESC_COE_SDO_HDR__HDR_TRANSFER_TYPE__SHIFT                   ((osal_uint32_t)1u)
#define ESC_COE_SDO_HDR__HDR_TRANSFER_TYPE(sdo_hdr)                 (((sdo_hdr).hdr & ESC_COE_SDO_HDR__HDR_TRANSFER_TYPE__MASK) >> ESC_COE_SDO_HDR__HDR_TRANSFER_TYPE__SHIFT)

#define ESC_COE_SDO_HDR__TRANSFER_TYPE__NORMAL                      ((esc_uint8_t)0u)
#define ESC_COE_SDO_HDR__TRANSFER_TYPE__EXPEDITED                   ((esc_uint8_t)1u)

#define ESC_COE_SDO_HDR__COMMAND_SPECIFIER__DOWNLOAD_REQUEST        ((esc_uint8_t)0x01u)
#define ESC_COE_SDO_HDR__COMMAND_SPECIFIER__DOWNLOAD_RESPONSE       ((esc_uint8_t)0x03u)

esc_void_t esc_coe_sdo_download_expedited_ind(esc_t *pesc, esc_uint16_t address, esc_uint16_t index, esc_uint8_t sub_index, esc_size_t size, esc_uint8_t *data);
esc_void_t esc_coe_sdo_download_normal_ind(esc_t *pesc, esc_uint16_t address, esc_uint16_t index, esc_uint8_t sub_index, esc_size_t complete_size, esc_size_t size, esc_uint8_t *data);
esc_void_t esc_coe_download_sdo_segment_ind(esc_t *pesc, esc_uint16_t address, esc_bool_t toggle, esc_bool_t more_follows, esc_size_t_size, esc_uint8_t *data);
esc_void_t esc_coe_sdo_upload_expedited_ind(esc_t *pesc, esc_uint16_t address, esc_uint16_t index, esc_uint8_t sub_index);

typedef struct esc_mailbox_hdr {
    esc_uint16_t        length;
    esc_uint16_t        address;
    esc_uint8_t         channel_priority;
    esc_uint8_t         type_cnt;
} esc_mailbox_hdr_t;

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
    esc_uint32_t        complete_size;
} esc_coe_sdo_hdr_t;

esc_void_t esc_coe_mailbox_write_ind(esc_t *pesc, esc_size_t length, esc_uint16_t address, 
        esc_uint16_t channel, esc_uint8_t priority, esc_coe_service_data_t *sd) 
{

    switch (pesc->mailbox.coe.state) {
        case MAILBOX_STATE_IDLE: 
        {
            esc_coe_sdo_hdr_t *sdo_hdr = (esc_coe_sdo_hdr_t *)&sd->data[0];

            if (
                    (ESC_COE_HDR__SERVICE(sd->coe_hdr) != ESC_COE_SERVICE_SDO_REQUEST) && 
                    (ESC_COE_HDR__SERVICE(sd->coe_hdr) != ESC_COE_SERVICE_SDO_INFORMATION)   ) 
            {
                // #5
                length = 4u;
                sd == 1, MBXERR_INVALIDHEADER;

                esc_err_mailbox_read_upd_req(pesc, length, address, channel, priority, sd);
            } else if (
                    (ESC_COE_HDR__SERVICE(sd->coe_hdr) == ESC_COE_SERVICE_SDO_REQUEST) &&
                    (ESC_COE_SDO_HDR__COMMAND_SPECIFIER(*sdo_hdr) > ESC_COE_SDO_HDR__COMMAND_SPECIFIER__DOWNLOAD_RESPONSE)  ) {
                // #6
                length = 4u;
                sd == 1, MBXERR_INVALIDHEADER;

                esc_err_mailbox_read_upd_req(pesc, length, address, channel, priority, sd);
            } else if ( 
                    (sd->mailbox.hdr.length != 10) && 
                    (ESC_COE_HDR__SERVICE(sd->coe_hdr) == ESC_COE_SERVICE_SDO_REQUEST) && 
                    (ESC_COE_SDO_HDR__COMMAND_SPECIFIER(*sdo_hdr) == ESC_COE_SDO_HDR__COMMAND_SPECIFIER__DOWNLOAD_REQUEST) && 
                    (ESC_COE_SDO_HDR__HDR_TRANSFER_TYPE(*sdo_hdr) == ESC_COE_SDO_HDR__TRANSFER_TYPE__EXPEDITED)     ) 
            {
                // #7
                length = 4u;
                sd == 1, MBXERR_INVALIDSIZE;

                esc_err_mailbox_read_upd_req(pesc, length, address, channel, priority, sd);
            } else if (
                    (sd->mailbox_hdr.length == 10) &&
                    (ESC_COE_HDR__SERVICE(sd->coe_hdr) == ESC_COE_SERVICE_SDO_REQUEST) &&
                    (ESC_COE_SDO_HDR__COMMAND_SPECIFIER(*sdo_hdr) == ESC_COE_SDO_HDR__COMMAND_SPECIFIER__DOWNLOAD_REQUEST) &&
                    (ESC_COE_SDO_HDR__HDR_TRANSFER_TYPE(*sdo_hdr) == ESC_COE_SDO_HDR__TRANSFER_TYPE__EXPEDITED) ) {
                // #8
            }


            break;
        }
    }
}
