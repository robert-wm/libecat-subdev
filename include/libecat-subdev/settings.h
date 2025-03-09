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

#define ECSD_MAILBOX_SUPPORTED                   OSAL_TRUE
#define ECSD_BOOT_SUPPORTED                      OSAL_TRUE
#define ECSD_DC_SUPPORTED                        OSAL_TRUE

#define ECSD_MAX_SYNC_MANAGER                    8u

#define ECSD_MAILBOX_WRITE__MIN_ADDRESS          0x1000
#define ECSD_MAILBOX_WRITE__MAX_ADDRESS          0x2000
#define ECSD_MAILBOX_WRITE__MIN_LENGTH           0x80
#define ECSD_MAILBOX_WRITE__MAX_LENGTH           0x400

#define ECSD_MAILBOX_READ__MIN_ADDRESS           0x1000
#define ECSD_MAILBOX_READ__MAX_ADDRESS           0x2000
#define ECSD_MAILBOX_READ__MIN_LENGTH            0x80
#define ECSD_MAILBOX_READ__MAX_LENGTH            0x400

#endif /* LIBECAT_SUBDEV_SETTINGS_H */

