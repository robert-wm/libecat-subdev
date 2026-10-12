/**
 * \file settings.h
 *
 * \author Robert Burger <robert@burger-system.de>
 *
 * \date 25 Nov 2024
 *
 * \brief ECSD settings header.
 *
 * ECSD settings header.
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

#ifndef LIBECAT_SUBDEV_SETTINGS_H
#define LIBECAT_SUBDEV_SETTINGS_H

#define ECSD_MAILBOX_SUPPORTED                   EC_TRUE
#define ECSD_MAILBOX_COE_SUPPORT                 1
#define ECSD_BOOT_SUPPORTED                      OSAL_TRUE
#define ECSD_DC_SUPPORTED                        OSAL_TRUE

#define ECSD_MAX_SYNC_MANAGER                    4u

/*
 * A sync manager in buffered (triple buffered) operation mode needs
 * three consecutive process data windows, each of the sync manager's
 * length, starting at its start address. The ESC rotates through the
 * three buffers, so the whole 3 x length region must be contiguous
 * and must not overlap any other sync manager's region.
 */
#define ECSD_SM_BUFFERED__REGION_MULTIPLIER      3u

#define ECSD_MAILBOX_WRITE__MIN_ADDRESS          0x1000
#define ECSD_MAILBOX_WRITE__MAX_ADDRESS          0x2000
#define ECSD_MAILBOX_WRITE__MIN_LENGTH           0x80
#define ECSD_MAILBOX_WRITE__MAX_LENGTH           0x400

#define ECSD_MAILBOX_READ__MIN_ADDRESS           0x1000
#define ECSD_MAILBOX_READ__MAX_ADDRESS           0x2000
#define ECSD_MAILBOX_READ__MIN_LENGTH            0x80
#define ECSD_MAILBOX_READ__MAX_LENGTH            0x400

struct ecsd_sm_settings {
    ec_uint16_t min_address;
    ec_uint16_t max_address;
    ec_uint16_t min_length;
    ec_uint16_t max_length;
    ec_uint8_t op_mode;
    ec_uint8_t direction;    
};

#endif /* LIBECAT_SUBDEV_SETTINGS_H */

