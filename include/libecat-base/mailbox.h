/**
 * \file mailbox.h
 *
 * \author Robert Burger <robert@burger-system.de>
 *
 * \date 25 Oct 2023
 *
 * \brief ESC mailbox header.
 *
 * ESC mailbox header.
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
 * along with libecat-subdev; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#ifndef LIBECAT_BASE_MAILBOX_H
#define LIBECAT_BASE_MAILBOX_H

#include <libecat-base/types.h>

#define EC_MBX_ERROR_REPLY__TYPE__MAILBOX_COMMAND  ((ec_uint16_t)0x01u)

#define EC_MBXERR_SYNTAX                           ((ec_uint16_t)0x01u)    //!< \brief Syntax of 6 octet Mailbox Header is wrong
#define EC_MBXERR_UNSUPPORTEDPROTOCOL              ((ec_uint16_t)0x02u)    //!< \brief The Mailbox protocol is not supported
#define EC_MBXERR_INVALIDCHANNEL                   ((ec_uint16_t)0x03u)    //!< \brief Channel Field contains wrong value (a slave can ignore the channel field)
#define EC_MBXERR_SERVICENOTSUPPORTED              ((ec_uint16_t)0x04u)    //!< \brief The service in the Mailbox protocol is not supported
#define EC_MBXERR_INVALIDHEADER                    ((ec_uint16_t)0x05u)    //!< \brief The mailbox protocol header of the mailbox protocol is wrong (without the 6 octet mailbox header)
#define EC_MBXERR_SIZETOOSHORT                     ((ec_uint16_t)0x06u)    //!< \brief Length of received mailbox data is too short 
#define EC_MBXERR_NOMOREMEMORY                     ((ec_uint16_t)0x07u)    //!< \brief Mailbox protocol cannot be processed because of limited ressources
#define EC_MBXERR_INVALIDSIZE                      ((ec_uint16_t)0x08u)    //!< \brief The length of data is inconsistent 
#define EC_MBXERR_SERVICEINWORK                    ((ec_uint16_t)0x09u)    //!< \brief Mailbox service already in use

#define EC_MBX_HDR__TYPE__MASK                     ((ec_uint8_t)0x0Fu)
#define EC_MBX_HDR__TYPE__SHIFT                    ((ec_uint8_t)0u)
#define EC_MBX_HDR__TYPE(mbx_hdr)                  (((mbx_hdr)->type_cnt & EC_MBX_HDR__TYPE__MASK) >> EC_MBX_HDR__TYPE__SHIFT)
#define EC_MBX_HDR__TYPE__SET(mbx_hdr, val)        (mbx_hdr)->type_cnt = ((mbx_hdr)->type_cnt & ~EC_MBX_HDR__TYPE__MASK) | ((val << EC_MBX_HDR__TYPE__SHIFT) & EC_MBX_HDR__TYPE__MASK)
#define EC_MBX_HDR__TYPE__ERR                      ((ec_uint8_t)0x00u);
#define EC_MBX_HDR__TYPE__ADS                      ((ec_uint8_t)0x01u)    //!< \brief ADS over EtherCAT (AoE)
#define EC_MBX_HDR__TYPE__EOE                      ((ec_uint8_t)0x02u)    //!< \brief Ethernet over EtherCAT (EoE)
#define EC_MBX_HDR__TYPE__COE                      ((ec_uint8_t)0x03u)    //!< \brief CAN application protocol over EtherCAT (CoE)
#define EC_MBX_HDR__TYPE__FOE                      ((ec_uint8_t)0x04u)    //!< \brief File Access over EtherCAT (FoE)
#define EC_MBX_HDR__TYPE__SOE                      ((ec_uint8_t)0x05u)    //!< \brief Servo profile over EtherCAT (SoE)
#define EC_MBX_HDR__TYPE__VEN                      ((ec_uint8_t)0x0fu)    //!< \brief vendor specific

#define EC_MBX_HDR__CNT__MASK                      ((ec_uint8_t)0x70u)
#define EC_MBX_HDR__CNT__SHIFT                     ((ec_uint8_t)4u)
#define EC_MBX_HDR__CNT(mbx_hdr)                   (((mbx_hdr)->type_cnt & EC_MBX_HDR__CNT__MASK) >> EC_MBX_HDR__CNT__SHIFT)
#define EC_MBX_HDR__CNT__SET(mbx_hdr, val)         (mbx_hdr)->type_cnt = ((mbx_hdr)->type_cnt & ~EC_MBX_HDR__CNT__MASK) | ((val << EC_MBX_HDR__CNT__SHIFT) & EC_MBX_HDR__CNT__MASK)

//! \brief Mailbox message header.
typedef struct __attribute__((__packed__)) ec_mailbox_hdr {
    ec_uint16_t        length;           //!< \brief Actual length of mailbox message without mailbox hdr.
    ec_uint16_t        address;          //!< \brief Mailbox message address field.  
    ec_uint8_t         channel_priority; //!< \brief Mailbox channel and pririoty.
    ec_uint8_t         type_cnt;         //!< \brief Mailbox message type and count for answer assignment.
} ec_mailbox_hdr_t;

typedef struct __attribute__((__packed__)) ec_mailbox_error_reply {
    ec_mailbox_hdr_t   mailbox_hdr;     //!< \brief Mailbox header.
    ec_uint16_t        type;            //!< \brief Mailbox content type.
    ec_uint16_t        detail;          //!< \brief Mailbox content details.
} ec_mailbox_error_reply_t;

typedef struct __attribute__((__packed__)) ec_mailbox {
    ec_mailbox_hdr_t   mailbox_hdr;     //!< \brief Mailbox header.
    ec_data_t          data[0];         //!< \brief Mailbox content payload.
} ec_mailbox_t;
    
#endif // LIBECAT_BASE_MAILBOX_H

