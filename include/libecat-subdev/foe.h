/**
 * \file foe.h
 *
 * \author Robert Burger <robert@burger-system.de>
 *
 * \date 15 Nov 2023
 *
 * \brief ECSD FoE header.
 *
 * ECSD FoE header.
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

#ifndef LIBECAT_SUBDEV_FOE_H
#define LIBECAT_SUBDEV_FOE_H

#include <stdint.h>
#include <libecat-base/types.h>
#include <libecat-base/mailbox.h>
#include <libecat-subdev/settings.h>
//#include <libecat-base/foe.h>

#define FOE_STATE_OFF       ((uint8_t)  0u)
#define FOE_STATE_IDLE      ((uint8_t)  1u)
#define FOE_STATE_DWLS      ((uint8_t)  2u)
#define FOE_STATE_UPLS      ((uint8_t)  3u)
#define FOE_STATE_UWUPD     ((uint8_t)  4u)
#define FOE_STATE_WUPD      ((uint8_t)  5u)
#define FOE_STATE_DWUPD     ((uint8_t)  6u)
#define FOE_STATE_DWSEG     ((uint8_t)  7u)
#define FOE_STATE_UWSEG     ((uint8_t)  8u)
#define FOE_STATE_ERR       ((uint8_t)255u)

struct ecsd_foe {
    ec_uint8_t state;

    uint16_t seg;
    uint8_t repeat_mbx_buffer[ECSD_MAILBOX_READ__MAX_LENGTH];
};

#define FOE_OPCODE__READ_REQUEST    ((ec_uint8_t)0x01u)
#define FOE_OPCODE__WRITE_REQUEST   ((ec_uint8_t)0x02u)
#define FOE_OPCODE__DATA_REQUEST    ((ec_uint8_t)0x03u)
#define FOE_OPCODE__ACK_REQUEST     ((ec_uint8_t)0x04u)
#define FOE_OPCODE__ERROR_REQUEST   ((ec_uint8_t)0x05u)

struct ec_foe_hdr {
    ec_uint8_t opcode;
    ec_uint8_t reserved;
} __attribute__((__packed__));

struct ec_foe_service_data {
    struct ec_mailbox_hdr mailbox_hdr;
    struct ec_foe_hdr foe_hdr;
};

struct ec_foe_read_request {
    struct ec_mailbox_hdr mailbox_hdr;
    struct ec_foe_hdr foe_hdr;
    ec_uint32_t password;
    ec_char_t filename[0];
} __attribute__((__packed__));

struct ec_foe_write_request {
    struct ec_mailbox_hdr mailbox_hdr;
    struct ec_foe_hdr foe_hdr;
    ec_uint32_t password;
    ec_char_t filename[0];
} __attribute__((__packed__));

struct ec_foe_data_request {
    struct ec_mailbox_hdr mailbox_hdr;
    struct ec_foe_hdr foe_hdr;
    ec_uint32_t packet_no;
    ec_uint8_t data[0];
} __attribute__((__packed__));

struct ec_foe_error_request {
    struct ec_mailbox_hdr mailbox_hdr;
    struct ec_foe_hdr foe_hdr;
    ec_uint32_t error_code;
    ec_char_t error_text[0];
} __attribute__((__packed__));

struct ec_foe_ack_request {
    struct ec_mailbox_hdr mailbox_hdr;
    struct ec_foe_hdr foe_hdr;
    ec_uint32_t packet_no;
} __attribute__((__packed__));

struct ec_foe_busy_request {
    struct ec_mailbox_hdr mailbox_hdr;
    struct ec_foe_hdr foe_hdr;
    ec_uint16_t done;           /*!< @brief if 'entire' is 0: progress in percent
                                            else            : size already transfered */
    ec_uint16_t entire;         /*!< @brief file size or 0 (see 'done') */
    ec_char_t busy_text[0];     /*!< @brief optional busy text */
} __attribute__((__packed__));

// forward declarations
struct ecsd;

//! \brief 
/*!
 * Refer to Service Definition ETG.1000.5
 */
ec_retval_t ecsd_foe_mailbox_write_ind(struct ecsd *pecsd, struct ec_foe_service_data *mbx);

//! \brief 
/*!
 * Refer to Service Definition ETG.1000.5
 */
ec_retval_t ecsd_foe_mailbox_read_ind(struct ecsd *pecsd, struct ec_foe_service_data *mbx);

//! \brief 
/*!
 * Refer to Service Definition ETG.1000.5
 */
ec_retval_t ecsd_foe_mailbox_data_ind(struct ecsd *pecsd, struct ec_foe_service_data *mbx);

//! \brief 
/*!
 * Refer to Service Definition ETG.1000.5
 */
ec_retval_t ecsd_foe_mailbox_ack_ind(struct ecsd *pecsd, struct ec_foe_service_data *mbx);

//! \brief 
/*!
 * Refer to Service Definition ETG.1000.5
 */
ec_retval_t ecsd_foe_mailbox_error_ind(struct ecsd *pecsd, struct ec_foe_service_data *mbx);

//! \brief
/*!
 */
ec_retval_t ecsd_foe_mailbox_start(struct ecsd *pecsd);

//! \brief
/*!
 */
ec_retval_t ecsd_foe_mailbox_stop(struct ecsd *pecsd);


ec_retval_t foe_read_ind(struct ecsd *pecsd, ec_uint32_t password, ec_char_t *filename);
ec_retval_t foe_write_ind(struct ecsd *pecsd, ec_uint32_t password, ec_char_t *filename);

#endif /* LIBECAT_SUBDEV_FOE_H */


