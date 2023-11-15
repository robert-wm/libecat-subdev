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

#ifndef LIBESC_MAILBOX_H
#define LIBESC_MAILBOX_H

#include <libesc/esc.h>

#define ESC_MBX_TYPE__ERR                           ((esc_uint8_t)0x00u);
#define ESC_MBX_TYPE__ADS                           ((esc_uint8_t)0x01u)    //!< \brief ADS over EtherCAT (AoE)
#define ESC_MBX_TYPE__EOE                           ((esc_uint8_t)0x02u)    //!< \brief Ethernet over EtherCAT (EoE)
#define ESC_MBX_TYPE__COE                           ((esc_uint8_t)0x03u)    //!< \brief CAN application protocol over EtherCAT (CoE)
#define ESC_MBX_TYPE__FOE                           ((esc_uint8_t)0x04u)    //!< \brief File Access over EtherCAT (FoE)
#define ESC_MBX_TYPE__SOE                           ((esc_uint8_t)0x05u)    //!< \brief Servo profile over EtherCAT (SoE)
#define ESC_MBX_TYPE__VEN                           ((esc_uint8_t)0x0fu)    //!< \brief vendor specific

#define ESC_MBX_ERROR_REPLY__TYPE__MAILBOX_COMMAND  ((esc_uint16_t)0x01u)

#define ESC_MBXERR_SYNTAX                           ((esc_uint16_t)0x01u)    //!< \brief Syntax of 6 octet Mailbox Header is wrong
#define ESC_MBXERR_UNSUPPORTEDPROTOCOL              ((esc_uint16_t)0x02u)    //!< \brief The Mailbox protocol is not supported
#define ESC_MBXERR_INVALIDCHANNEL                   ((esc_uint16_t)0x03u)    //!< \brief Channel Field contains wrong value (a slave can ignore the channel field)
#define ESC_MBXERR_SERVICENOTSUPPORTED              ((esc_uint16_t)0x04u)    //!< \brief The service in the Mailbox protocol is not supported
#define ESC_MBXERR_INVALIDHEADER                    ((esc_uint16_t)0x05u)    //!< \brief The mailbox protocol header of the mailbox protocol is wrong (without the 6 octet mailbox header)
#define ESC_MBXERR_SIZETOOSHORT                     ((esc_uint16_t)0x06u)    //!< \brief Length of received mailbox data is too short 
#define ESC_MBXERR_NOMOREMEMORY                     ((esc_uint16_t)0x07u)    //!< \brief Mailbox protocol cannot be processed because of limited ressources
#define ESC_MBXERR_INVALIDSIZE                      ((esc_uint16_t)0x08u)    //!< \brief The length of data is inconsistent 
#define ESC_MBXERR_SERVICEINWORK                    ((esc_uint16_t)0x09u)    //!< \brief Mailbox service already in use

#define ESC_MBX_HDR__TYPE__MASK                     ((esc_uint8_t)0x0Fu)
#define ESC_MBX_HDR__TYPE__SHIFT                    ((esc_uint8_t)0u)
#define ESC_MBX_HDR__TYPE(mbx_hdr)                  (((mbx_hdr)->type_cnt & ESC_MBX_HDR__TYPE__MASK) >> ESC_MBX_HDR__TYPE__SHIFT)
#define ESC_MBX_HDR__TYPE_SET(mbx_hdr, val)         (mbx_hdr)->type_cnt = ((mbx_hdr)->type_cnt & ~ESC_MBX_HDR__TYPE__MASK) | ((val << ESC_MBX_HDR__TYPE__SHIFT) & ESC_MBX_HDR__TYPE__MASK)

#define ESC_MBX_HDR__CNT__MASK                      ((esc_uint8_t)0x70u)
#define ESC_MBX_HDR__CNT__SHIFT                     ((esc_uint8_t)4u)
#define ESC_MBX_HDR__CNT(mbx_hdr)                   (((mbx_hdr)->type_cnt & ESC_MBX_HDR__CNT__MASK) >> ESC_MBX_HDR__CNT__SHIFT)
#define ESC_MBX_HDR__CNT_SET(mbx_hdr, val)          (mbx_hdr)->type_cnt = ((mbx_hdr)->type_cnt & ~ESC_MBX_HDR__CNT__MASK) | ((val << ESC_MBX_HDR__CNT__SHIFT) & ESC_MBX_HDR__CNT__MASK)

//! \brief Mailbox message header.
typedef struct esc_mailbox_hdr {
    esc_uint16_t        length;           //!< \brief Actual length of mailbox message without mailbox hdr.
    esc_uint16_t        address;          //!< \brief Mailbox message address field.  
    esc_uint8_t         channel_priority; //!< \brief Mailbox channel and pririoty.
    esc_uint8_t         type_cnt;         //!< \brief Mailbox message type and count for answer assignment.
} esc_mailbox_hdr_t;

typedef struct esc_mailbox_error_reply {
    esc_mailbox_hdr_t   mailbox_hdr;
    esc_uint16_t        type;
    esc_uint16_t        detail;
} esc_mailbox_error_reply_t;

typedef struct esc_mailbox {
    esc_mailbox_hdr_t   mailbox_hdr;
    esc_data_t          data[0];
} esc_mailbox_t;
    
esc_retval_t esc_mbx_write(esc_t *pesc, esc_mailbox_t *mbx);

#endif // LIBESC_MAILBOX_H


