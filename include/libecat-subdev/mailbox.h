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

#ifndef LIBECAT_SUBDEV_MAILBOX_H
#define LIBECAT_SUBDEV_MAILBOX_H

#include <libecat-subdev/esc.h>
#include <libecat-base/mailbox.h>

#define MAILBOX_STATE_IDLE      1
#define MAILBOX_STATE_INF       2
#define MAILBOX_STATE_ERR       3

ec_retval_t ecsd_mbx_write(struct ecsd *pesc, struct ec_mailbox *mbx);

#endif // LIBECAT_SUBDEV_MAILBOX_H


