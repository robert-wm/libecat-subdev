/**
 * \file coe.h
 *
 * \author Robert Burger <robert@burger-system.de>
 *
 * \date 15 Nov 2023
 *
 * \brief ESC CoE header.
 *
 * ESC CoE header.
 */

/*
 * This file is part of libecat-base.
 *
 * libecat-base is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 3 of the License, or (at your option) any later version.
 * 
 * libecat-base is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public License
 * along with libecat-base; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#ifndef LIBECAT_BASE_COE_H
#define LIBECAT_BASE_COE_H

#include <libecat-base/types.h>
#include <libecat-base/mailbox.h>

#define EC_COE_HDR__SERVICE__MASK                               ((ec_uint16_t)0xF000)
#define EC_COE_HDR__SERVICE__SHIFT                              ((ec_uint32_t)12)
#define EC_COE_HDR__SERVICE(coe_hdr)                            (((coe_hdr).hdr & EC_COE_HDR__SERVICE__MASK) >> EC_COE_HDR__SERVICE__SHIFT)
#define EC_COE_HDR__SERVICE__SET(coe_hdr, val)                  (coe_hdr)->hdr = ((coe_hdr)->hdr & ~EC_COE_HDR__SERVICE__MASK) | (val << EC_COE_HDR__SERVICE__SHIFT)

#define EC_COE_HDR__SERVICE__EMERGENCY                          ((ec_uint8_t)0x01u)   //!< \brief Emergency
#define EC_COE_HDR__SERVICE__SDO_REQUEST                        ((ec_uint8_t)0x02u)   //!< \brief SDO Request
#define EC_COE_HDR__SERVICE__SDO_RESPONSE                       ((ec_uint8_t)0x03u)   //!< \brief SDO Response
#define EC_COE_HDR__SERVICE__TXPDO                              ((ec_uint8_t)0x04u)   //!< \brief TxPDO
#define EC_COE_HDR__SERVICE__RXPDO                              ((ec_uint8_t)0x05u)   //!< \brief RxPDO
#define EC_COE_HDR__SERVICE__TXPDO_REMOTE_REQUEST               ((ec_uint8_t)0x06u)   //!< \brief TxPDO remote request
#define EC_COE_HDR__SERVICE__RXPDO_REMOTE_REQUEST               ((ec_uint8_t)0x07u)   //!< \brief RxPDO remote request
#define EC_COE_HDR__SERVICE__SDO_INFORMATION                    ((ec_uint8_t)0x08u)   //!< \brief SDO Information

struct ec_coe_hdr {
    ec_uint16_t        hdr;
} __attribute__((__packed__));

struct ec_coe_service_data {
    struct ec_mailbox_hdr mailbox_hdr;
    struct ec_coe_hdr coe_hdr;
    ec_data_t data[0];
} __attribute__((__packed__));

#define EC_COE_SDO_HDR__HDR_SIZE__MASK                          ((ec_uint8_t)0x01u)
#define EC_COE_SDO_HDR__HDR_SIZE(sdo_hdr)                       ((sdo_hdr)->hdr & EC_COE_SDO_HDR__HDR_SIZE__MASK)
#define EC_COE_SDO_HDR__HDR_TRANSFER_TYPE__MASK                 ((ec_uint8_t)0x02u)
#define EC_COE_SDO_HDR__HDR_TRANSFER_TYPE__SHIFT                ((ec_uint32_t)1u)
#define EC_COE_SDO_HDR__HDR_TRANSFER_TYPE(sdo_hdr)              (((sdo_hdr)->hdr & EC_COE_SDO_HDR__HDR_TRANSFER_TYPE__MASK) >> EC_COE_SDO_HDR__HDR_TRANSFER_TYPE__SHIFT)

#define EC_COE_SDO_HDR__TRANSFER_TYPE__NORMAL                   ((ec_uint8_t)0u)
#define EC_COE_SDO_HDR__TRANSFER_TYPE__EXPEDITED                ((ec_uint8_t)1u)

#define EC_COE_SDO_HDR__SIZE_INDICATOR__MASK                    ((ec_uint8_t)0x01u)
#define EC_COE_SDO_HDR__SIZE_INDICATOR(sdo_hdr)                 (((sdo_hdr)->hdr & EC_COE_SDO_HDR__SIZE_INDICATOR__MASK))
#define EC_COE_SDO_HDR__SIZE_INDICATOR__SET(sdo_hdr, val)       (sdo_hdr)->hdr = ((sdo_hdr)->hdr & ~EC_COE_SDO_HDR__SIZE_INDICATOR__MASK) | (val)

#define EC_COE_SDO_HDR__TRANSFER_TYPE__MASK                     ((ec_uint8_t)0x02u)
#define EC_COE_SDO_HDR__TRANSFER_TYPE__SHIFT                    ((ec_uint8_t)1u)
#define EC_COE_SDO_HDR__TRANSFER_TYPE(sdo_hdr)                  (((sdo_hdr)->hdr & EC_COE_SDO_HDR__TRANSFER_TYPE__MASK) >> EC_COE_SDO_HDR__TRANSFER_TYPE__SHIFT)
#define EC_COE_SDO_HDR__TRANSFER_TYPE__SET(sdo_hdr, val)        (sdo_hdr)->hdr = ((sdo_hdr)->hdr & ~EC_COE_SDO_HDR__TRANSFER_TYPE__MASK) | ((val << EC_COE_SDO_HDR__TRANSFER_TYPE__SHIFT) & EC_COE_SDO_HDR__TRANSFER_TYPE__MASK)

#define EC_COE_SDO_HDR__DATA_SET_SIZE__MASK                     ((ec_uint8_t)0x0Cu)
#define EC_COE_SDO_HDR__DATA_SET_SIZE__SHIFT                    ((ec_uint8_t)2u)
#define EC_COE_SDO_HDR__DATA_SET_SIZE(sdo_hdr)                  (((sdo_hdr)->hdr & EC_COE_SDO_HDR__DATA_SET_SIZE__MASK) >> EC_COE_SDO_HDR__DATA_SET_SIZE__SHIFT)
#define EC_COE_SDO_HDR__DATA_SET_SIZE__SET(sdo_hdr, val)        (sdo_hdr)->hdr = ((sdo_hdr)->hdr & ~EC_COE_SDO_HDR__DATA_SET_SIZE__MASK) | ((val << EC_COE_SDO_HDR__DATA_SET_SIZE__SHIFT) & EC_COE_SDO_HDR__DATA_SET_SIZE__MASK)

#define EC_COE_SDO_HDR__COMPLETE_ACCESS__MASK                   ((ec_uint8_t)0x10u)
#define EC_COE_SDO_HDR__COMPLETE_ACCESS__SHIFT                  ((ec_uint8_t)4u)
#define EC_COE_SDO_HDR__COMPLETE_ACCESS(sdo_hdr)                (((sdo_hdr)->hdr & EC_COE_SDO_HDR__COMPLETE_ACCESS__MASK) >> EC_COE_SDO_HDR__COMPLETE_ACCESS__SHIFT)
#define EC_COE_SDO_HDR__COMPLETE_ACCESS__SET(sdo_hdr, val)      (sdo_hdr)->hdr = ((sdo_hdr)->hdr & ~EC_COE_SDO_HDR__COMPLETE_ACCESS__MASK) | ((val << EC_COE_SDO_HDR__COMPLETE_ACCESS__SHIFT) & EC_COE_SDO_HDR__COMPLETE_ACCESS__MASK)

#define EC_COE_SDO_HDR__COMMAND__MASK                           ((ec_uint8_t)0xE0u)
#define EC_COE_SDO_HDR__COMMAND__SHIFT                          ((ec_uint8_t)5u)
#define EC_COE_SDO_HDR__COMMAND(sdo_hdr)                        (((sdo_hdr)->hdr & EC_COE_SDO_HDR__COMMAND__MASK) >> EC_COE_SDO_HDR__COMMAND__SHIFT)
#define EC_COE_SDO_HDR__COMMAND__SET(sdo_hdr, val)              (sdo_hdr)->hdr = ((sdo_hdr)->hdr & ~EC_COE_SDO_HDR__COMMAND__MASK) | ((val << EC_COE_SDO_HDR__COMMAND__SHIFT) & EC_COE_SDO_HDR__COMMAND__MASK)

#define EC_COE_SDO_HDR__COMMAND__DOWNLOAD_SEGMENT_REQUEST       ((ec_uint8_t)0x00u)    //!< \brief Download Segment Request
#define EC_COE_SDO_HDR__COMMAND__DOWNLOAD_REQUEST               ((ec_uint8_t)0x01u)    //!< \brief Download Request
#define EC_COE_SDO_HDR__COMMAND__UPLOAD_REQUEST                 ((ec_uint8_t)0x02u)    //!< \brief Upload Request
#define EC_COE_SDO_HDR__COMMAND__UPLOAD_SEGMENT_REQUEST         ((ec_uint8_t)0x03u)    //!< \brief Upload Segment Request
#define EC_COE_SDO_HDR__COMMAND__ABORT_REQUEST                  ((ec_uint8_t)0x04u)    //!< \brief Abort Transfer Request

#define EC_COE_SDO_HDR__COMMAND__DOWNLOAD_SEGMENT_RESPONSE      ((ec_uint8_t)0x01u)    //!< \brief Download Segment Response
#define EC_COE_SDO_HDR__COMMAND__DOWNLOAD_RESPONSE              ((ec_uint8_t)0x03u)    //!< \brief Download Response
#define EC_COE_SDO_HDR__COMMAND__UPLOAD_RESPONSE                ((ec_uint8_t)0x02u)    //!< \brief Upload Response
#define EC_COE_SDO_HDR__COMMAND__UPLOAD_SEGMENT_RESPONSE        ((ec_uint8_t)0x00u)    //!< \brief Upload Segment Response

struct ec_coe_sdo_hdr {
    ec_uint8_t hdr;
    ec_uint16_t index;
    ec_uint8_t sub_index;
} __attribute__((__packed__));

#define EC_COE_SDO_SEG_HDR__MORE_FOLLOWS_TYPE__MASK                 ((ec_uint8_t)0x01u)
#define EC_COE_SDO_SEG_HDR__MORE_FOLLOWS_TYPE__SHIFT                ((ec_uint8_t)0u)
#define EC_COE_SDO_SEG_HDR__MORE_FOLLOWS_TYPE(sdo_hdr)              (((sdo_hdr)->hdr & EC_COE_SDO_HDR__MORE_FOLLOWS_TYPE__MASK) >> EC_COE_SDO_HDR__MORE_FOLLOWS_TYPE__SHIFT)
#define EC_COE_SDO_SEG_HDR__MORE_FOLLOWS_TYPE__SET(sdo_hdr, val)    (sdo_hdr)->hdr = ((sdo_hdr)->hdr & ~EC_COE_SDO_HDR__MORE_FOLLOWS_TYPE__MASK) | ((val << EC_COE_SDO_HDR__MORE_FOLLOWS_TYPE__SHIFT) & EC_COE_SDO_HDR__MORE_FOLLOWS_TYPE__MASK)

#define EC_COE_SDO_SEG_HDR__SEG_DATA_SIZE_TYPE__MASK                ((ec_uint8_t)0x0Eu)
#define EC_COE_SDO_SEG_HDR__SEG_DATA_SIZE_TYPE__SHIFT               ((ec_uint8_t)1u)
#define EC_COE_SDO_SEG_HDR__SEG_DATA_SIZE_TYPE(sdo_hdr)             (((sdo_hdr)->hdr & EC_COE_SDO_HDR__SEG_DATA_SIZE_TYPE__MASK) >> EC_COE_SDO_HDR__SEG_DATA_SIZE_TYPE__SHIFT)
#define EC_COE_SDO_SEG_HDR__SEG_DATA_SIZE_TYPE__SET(sdo_hdr, val)   (sdo_hdr)->hdr = ((sdo_hdr)->hdr & ~EC_COE_SDO_HDR__SEG_DATA_SIZE_TYPE__MASK) | ((val << EC_COE_SDO_HDR__SEG_DATA_SIZE_TYPE__SHIFT) & EC_COE_SDO_HDR__SEG_DATA_SIZE_TYPE__MASK)

#define EC_COE_SDO_SEG_HDR__TOGGLE_TYPE__MASK                       ((ec_uint8_t)0x10u)
#define EC_COE_SDO_SEG_HDR__TOGGLE_TYPE__SHIFT                      ((ec_uint8_t)4u)
#define EC_COE_SDO_SEG_HDR__TOGGLE_TYPE(sdo_hdr)                    (((sdo_hdr)->hdr & EC_COE_SDO_HDR__TOGGLE_TYPE__MASK) >> EC_COE_SDO_HDR__TOGGLE_TYPE__SHIFT)
#define EC_COE_SDO_SEG_HDR__TOGGLE_TYPE__SET(sdo_hdr, val)          (sdo_hdr)->hdr = ((sdo_hdr)->hdr & ~EC_COE_SDO_HDR__TOGGLE_TYPE__MASK) | ((val << EC_COE_SDO_HDR__TOGGLE_TYPE__SHIFT) & EC_COE_SDO_HDR__TOGGLE_TYPE__MASK)

#define EC_COE_SDO_SEG_HDR__COMMAND_TYPE__MASK                      ((ec_uint8_t)0xE0u)
#define EC_COE_SDO_SEG_HDR__COMMAND_TYPE__SHIFT                     ((ec_uint8_t)5u)
#define EC_COE_SDO_SEG_HDR__COMMAND_TYPE(sdo_hdr)                   (((sdo_hdr)->hdr & EC_COE_SDO_HDR__COMMAND_TYPE__MASK) >> EC_COE_SDO_HDR__COMMAND_TYPE__SHIFT)
#define EC_COE_SDO_SEG_HDR__COMMAND_TYPE__SET(sdo_hdr, val)         (sdo_hdr)->hdr = ((sdo_hdr)->hdr & ~EC_COE_SDO_HDR__COMMAND_TYPE__MASK) | ((val << EC_COE_SDO_HDR__COMMAND_TYPE__SHIFT) & EC_COE_SDO_HDR__COMMAND_TYPE__MASK)

struct ec_coe_sdo_seg_hdr {
    ec_uint8_t hdr;
} __attribute__((__packed__));

struct ec_coe_sdo_request {
    struct ec_mailbox_hdr mailbox_hdr;
    struct ec_coe_hdr coe_hdr;
    struct ec_coe_sdo_hdr sdo_hdr;
} __attribute__((__packed__));

//! expedited download request/upload response
struct ec_coe_sdo_download_expedited {
    struct ec_mailbox_hdr mailbox_hdr;
    struct ec_coe_hdr coe_hdr;
    struct ec_coe_sdo_hdr sdo_hdr;
    ec_data_t data[0];
} __attribute__((__packed__));

struct ec_coe_sdo_download_normal {
    struct ec_mailbox_hdr mailbox_hdr;
    struct ec_coe_hdr coe_hdr;
    struct ec_coe_sdo_hdr sdo_hdr;
    ec_uint32_t complete_size;
    ec_data_t data[0];
} __attribute__((__packed__));

struct ec_coe_abort_sdo {
    struct ec_mailbox_hdr mailbox_hdr;
    struct ec_coe_hdr coe_hdr;
    struct ec_coe_sdo_hdr sdo_hdr;
    ec_uint32_t abort_code;
} __attribute__((__packed__));

#define EC_COE_SDO_INFO_HDR__GET_OD_LIST_REQUEST                ((ec_uint8_t)0x01u)    //!< \brief Get OD List Request
#define EC_COE_SDO_INFO_HDR__GET_OD_LIST_RESPONSE               ((ec_uint8_t)0x02u)    //!< \brief Get OD List Response
#define EC_COE_SDO_INFO_HDR__GET_OBJECT_DESCRIPTION_REQUEST     ((ec_uint8_t)0x03u)    //!< \brief Get Object Description Request
#define EC_COE_SDO_INFO_HDR__GET_OBJECT_DESCRIPTION_RESPONSE    ((ec_uint8_t)0x04u)    //!< \brief Get Object Description Response
#define EC_COE_SDO_INFO_HDR__GET_ENTRY_DESCRIPTION_REQUEST      ((ec_uint8_t)0x05u)    //!< \brief Get Entry Description Request
#define EC_COE_SDO_INFO_HDR__GET_ENTRY_DESCRIPTION_RESPONSE     ((ec_uint8_t)0x06u)    //!< \brief Get Entry Description Response
#define EC_COE_SDO_INFO_HDR__SDO_INFO_ERROR_REQUEST             ((ec_uint8_t)0x07u)    //!< \brief SDO Info Error Request

#define EC_COE_SDO_INFO_HDR__OPCODE__MASK                       ((ec_uint8_t)0x7Fu)
#define EC_COE_SDO_INFO_HDR__OPCODE__SHIFT                      ((ec_uint8_t)0u)
#define EC_COE_SDO_INFO_HDR__OPCODE(sdo_hdr)                    (((sdo_hdr)->opcode_incomplete & EC_COE_SDO_INFO_HDR__OPCODE__MASK) >> EC_COE_SDO_INFO_HDR__OPCODE__SHIFT)
#define EC_COE_SDO_INFO_HDR__OPCODE__SET(sdo_hdr, val)          (sdo_hdr)->opcode_incomplete = ((sdo_hdr)->opcode_incomplete & ~EC_COE_SDO_INFO_HDR__OPCODE__MASK) | ((val << EC_COE_SDO_INFO_HDR__OPCODE__SHIFT) & EC_COE_SDO_INFO_HDR__OPCODE__MASK)

struct ec_coe_sdo_info_hdr {
    ec_uint16_t opcode_incomplete;
    ec_uint16_t fragment_left;
} __attribute__((__packed__));

struct ec_coe_sdo_information {
    struct ec_mailbox_hdr mailbox_hdr;
    struct ec_coe_hdr coe_hdr;
    struct ec_coe_sdo_info_hdr sdo_hdr;
} __attribute__((__packed__));


#endif // LIBECAT_BASE_COE_H

