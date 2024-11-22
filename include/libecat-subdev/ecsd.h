/**
 * \file ecsd.h
 *
 * \author Robert Burger <robert@burger-system.de>
 *
 * \date 25 Oct 2023
 *
 * \brief ECSD main header.
 *
 * ECSD main header.
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

#ifndef LIBECAT_SUBDEV_ECSD_H
#define LIBECAT_SUBDEV_ECSD_H

#include <stdint.h>
#include <libecat-base/types.h>
#include <libecat-base/regs.h>
#include <libecat-subdev/hw.h>
#include <libecat-subdev/foe.h>

struct ec_object;

struct mailbox {
    struct ec_slave_sm sm_receive;
    struct ec_slave_sm sm_send;

    struct {
        ec_uint8_t state;
        uint32_t seg;
        struct ec_object *objects;
    } coe;

    struct ecsd_foe foe;
};

struct ecsd {
    ec_uint8_t al_state;
    ec_bool_t al_error_flag;
    ec_uint16_t al_status_code;
    ec_bool_t id_flag;
    ec_uint8_t al_control_state;

    ec_bool_t boot_supported;                   //!< \brief Bootstrap mode is supported by slave
    ec_bool_t dc_not_accepted;                  //!< \brief The device does not support DCs and does not accept DC settings,
                                                // either. If DC settings are made the device will go into Error state. This
                                                // means Sync Control register 0x0981 shall be 0.
    ec_bool_t dc_required;                      //!< \brief Device requires DC settings (Bits 0x0981:00 to 0x0981:02 shall be set
                                                // accordingly). Other modes such as “Freerun” and “Synchronous with
                                                // SM event” are not supported
    ec_bool_t dc_running;                       //!< \brief DCs are running and used for synchronization between master and
                                                // slave application.
    ec_bool_t dc_supported;                     //!< \brief Device supports at least one OpMode which uses Distributed Clocks

    ec_uint16_t local_error_code;               //!< \brief Original reason of a previous local error which resulted in disabeling
                                                // SyncManager channels
    ec_bool_t local_error_flag;                 //!< \brief Flag of local application indicating a local error causing that
                                                // SyncManagers were disabled. Used in case of an error resulting in a
                                                // state change from Op to SafeOp 
    ec_bool_t pll_running;                      //!< \brief Local application is synchronized with DC event and master application.
    ec_uint32_t safeop_2_op_timeout_timer;      //!< \brief Timeout for state change from SafeOp to Op
    ec_bool_t dc_event_received;                //!< \brief Slave has received the Sync0/Sync1 event at least once
    ec_bool_t wait_for_pll_running;             //!< \brief Wait until local PLL has locked with master cycle (wait until 
                                                // pll_running = TRUE). This time shall be smaller than the safeop_2_op_timeout.
    ec_bool_t wd_enabled;                       //!< \brief The watchdog behavior is disabled if at least the watchdog time (R400,
                                                // R420) is equal zero, additionally the watchdog behavior can be
                                                // disabled if the watchdog bit of the SyncManager is disabled
                                                // (SyncManager register +0x04.06=0) (when using the watchdog
                                                // functionality of the ECSD, this watchdog is only enabled if the watchdog
                                                // bit in the Sync Manager is set)
                                                // TRUE: watchdog behaviour is enabled
                                                // FALSE: watchdog behaviour is disabled
                                                // If slave has only inputs wdEnabled may always be FALSE
    ec_bool_t ready_for_op;                     //!< \brief Internal variable which is set when the device is ready to be switched
                                                // to OP in Non-DC-Mode, this should happen when the outputs have
                                                // been received within the watchdog time if enabled or during SafeOp
                                                // state when the watchdog is disabled. However there can be another
                                                // behavior due to legacy reasons.
    ec_bool_t sm_event_received_internal;       //!< \brief Variable which indicates that the SM-event was received
                                            
    ec_bool_t mbx_supported;                    //!< \brief Flag indicating if mailbox is supported by slave.
    ec_bool_t mbx_running;                      //!< \brief Flag indicating mailbox is currently running.
                                                //
    struct mailbox mbx;
    struct hw hw;
};

//! \brief Opens and initializes EtherCAT SubDev
/*!
 * \param[in]   pecsd           Pointer to ECSD struture.
 * \param[in]   hw_hwl          Handle to hardware.
 * \param[in]   read_func       Pointer to hw read function.
 * \param[in]   write_func      Pointer to hw write function.
 * \param[in]   enable_int_func Pointer to hw enable interrupt function.
 * \param[in]   pdi_type        Type of PDI waiting for ECSD to become ready.
 *
 * \retval  OK on success.
 */
ec_void_t ecsd_open(struct ecsd *pecsd, void *hw_hdl, ecsd_hw_read read_func,
        ecsd_hw_write write_func, ecsd_hw_enable_int enable_int_func, ec_uint16_t pdi_type);
            
//! \brief AL control indication
/*!
 * \param[in]   pecsd               Pointer to EtherCAT SubDevice structure.
 * \param[in]   al_control_state    Requested AL control state.
 * \param[in]   ack_flag            Requested error ack flag.
 * \param[in]   id_request          Request identification.
 *
 * \retval EC_OK    On success.
 */
ec_retval_t ecsd_al_control_ind(struct ecsd *pecsd, ec_uint8_t al_control_state, 
        ec_bool_t ack_flag, ec_bool_t id_request);

//! \brief AL Control response function
/*!
 * \param[in]   pecsd               Pointer to EtherCAT SubDevice structure.
 * \param[in]   al_state            AL state to be set.
 * \param[in]   al_status_code      AL status code in case of error.
 * \param[in]   al_error_flag       Error indication flag.
 * \param[in]   id_flag             ID response flag.
 */
ec_void_t ecsd_al_control_rsp(struct ecsd *pecsd, ec_uint8_t al_state,
        ec_uint16_t al_status_code, ec_bool_t al_error_flag, ec_bool_t id_flag);

//! \brief Sync manager change event occured
/*!
 * \param[in]   pecsd        Pointer to ECSD struture.
 */
ec_void_t ecsd_sm_chg(struct ecsd *pecsd);

ec_void_t ecsd_sync0(struct ecsd *pecsd);
ec_void_t ecsd_sync1(struct ecsd *pecsd);
ec_void_t ecsd_pdo(struct ecsd *pecsd);
ec_void_t ecsd_pdi(struct ecsd *pecsd);

#endif // LIBECAT_SUBDEV_ECSD_H

