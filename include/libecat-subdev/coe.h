/**
 * \file coe.h
 *
 * \author Robert Burger <robert@burger-system.de>
 *
 * \date 15 Nov 2023
 *
 * \brief ECSD CoE header.
 *
 * ECSD CoE header.
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

#ifndef LIBECAT_SUBDEV_COE_H
#define LIBECAT_SUBDEV_COE_H

#include <stdint.h>
#include <libecat-subdev/mailbox.h>
#include <libecat-subdev/ecsd.h>
#include <libecat-base/types.h>
#include <libecat-base/coe.h>

//! \brief 
/*!
 * Refer to Mailbox Read Upd Service Definition ETG.1000.3
 */
ec_retval_t ecsd_coe_mailbox_read_upd(struct ecsd *pecsd, struct ec_mailbox *mbx);

//! \brief 
/*!
 * Refer to Service Definition ETG.1000.3
 */
ec_retval_t ecsd_coe_mailbox_write_ind(struct ecsd *pecsd, struct ec_coe_service_data *mbx);

//! \brief 
/*!
 * Refer to Service Definition ETG.1000.3
 */
ec_retval_t ecsd_coe_mailbox_read_upd_cnf(struct ecsd *pecsd, ec_bool_t success);

//! \brief CoE SDO download expedited indication.
/*!
 * \param[in]   pecsd        Pointer to EtherCAT SubDevice structure.
 * \param[in]   mbx         Mailbox message to process.
 */
ec_retval_t ecsd_coe_sdo_download_expedited_ind(struct ecsd *pecsd, struct ec_coe_sdo_download_expedited_req *mbx);

//! \brief CoE SDO download normal indication.
/*!
 * \param[in]   pecsd        Pointer to EtherCAT SubDevice structure.
 * \param[in]   mbx         Mailbox message to process.
 */
ec_retval_t ecsd_coe_sdo_download_normal_ind(struct ecsd *pecsd, struct ec_coe_sdo_download_normal_req *mbx);

//! \brief CoE SDO download segment indication.
/*!
 * \param[in]   pecsd        Pointer to EtherCAT SubDevice structure.
 * \param[in]   mbx         Mailbox message to process.
 */
ec_retval_t ecsd_coe_download_sdo_segment_ind(struct ecsd *pecsd, struct ec_coe_sdo_download_segmented_req *mbx);

//! \brief CoE SDO upload expedited indication.
/*!
 * \param[in]   pecsd        Pointer to EtherCAT SubDevice structure.
 * \param[in]   mbx         Mailbox message to process.
 */
ec_retval_t ecsd_coe_sdo_upload_expedited_ind(struct ecsd *pecsd, struct ec_coe_sdo_upload_expedited_req *mbx);

#endif // LIBECAT_SUBDEV_COE_H

