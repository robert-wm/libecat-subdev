/**
 * \file ecsd.c
 *
 * \author Robert Burger <robert@burger-system.de>
 *
 * \date 25 Oct 2023
 *
 * \brief ECSD main source.
 *
 * ECSD main source.
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

#include <assert.h>
#include <stddef.h>

#include <libecat-subdev/settings.h>
#include <libecat-subdev/ecsd.h>
#include <libecat-subdev/hw.h>

//! \brief Opens and initializes EtherCAT SubDev
/*!
 * \param[in]   pesc        Pointer to ECSD struture.
 * \param[in]   ecsd_base   Base address of ECSD memory.
 * \param[in]   pdi_type    Type of PDI waiting for ECSD to become ready.
 *
 * \retval  OK on success.
 */
ec_void_t ecsd_open(struct ecsd *pecsd, void *ecsd_base, ec_uint16_t pdi_type) {
    hw_open(pecsd, ecsd_base, pdi_type);

    pecsd->al_state                    = EC_STATE_INIT;
    pecsd->al_error_flag               = EC_FALSE;
    pecsd->al_status_code              = 0u;
    pecsd->id_flag                     = EC_FALSE;;
    pecsd->al_control_state            = 0u;;

    pecsd->boot_supported              = EC_FALSE;
    pecsd->dc_not_accepted             = EC_FALSE;
    pecsd->dc_required                 = EC_FALSE;
    pecsd->dc_running                  = EC_FALSE;
    pecsd->dc_supported                = EC_FALSE;
    pecsd->local_error_code            = 0u;
    pecsd->local_error_flag            = EC_FALSE;
    pecsd->pll_running                 = EC_FALSE;
    pecsd->safeop_2_op_timeout_timer   = 0u;
    pecsd->dc_event_received           = EC_FALSE;
    pecsd->wait_for_pll_running        = EC_FALSE;
    pecsd->wd_enabled                  = EC_FALSE;
    pecsd->ready_for_op                = EC_FALSE;
    pecsd->sm_event_received_internal  = EC_FALSE;
    pecsd->mbx_supported               = EC_FALSE;
    pecsd->mbx_running                 = EC_FALSE;
}

ec_retval_t ecsd_al_state_change_req(struct ecsd *pecsd, ec_uint8_t al_state, ec_uint16_t al_status_code);

ec_bool_t ecsd_sm_settings_0_and_1_match(struct ecsd *pecsd) {
    ec_bool_t ret = EC_TRUE;

    if (pecsd->mbx_supported) {
        struct ec_slave_sm sm_mbx;

        // Check first Sync manager for write mailbox config.
        hw_read(pecsd, EC_REG_SYNC_MANAGER + (0 * EC_REG_SYNC_MANAGER__BASE_OFFSET), (ec_uint8_t *)&sm_mbx, sizeof(sm_mbx));

        if ((sm_mbx.control & EC_REG_SYNC_MANAGER__CONTROL__OP_MODE_MASK) != 
                EC_REG_SYNC_MANAGER__CONTROL__OP_MODE_MAILBOX) {
            ret = EC_FALSE; // ERROR: Operation mode is not set to MAILBOX
        } else if ((sm_mbx.control & EC_REG_SYNC_MANAGER__CONTROL__DIRECTION_MASK) != 
                EC_REG_SYNC_MANAGER__CONTROL__DIRECTION_WRITE) {
            ret = EC_FALSE; // ERROR: Direction is not set to WRITE (ESC write <-> PDI read)
        } else if (!(sm_mbx.activate & EC_REG_SYNC_MANAGER__ACTIVATE__ENABLE_SM)) {
            ret = EC_FALSE; // ERROR: Sync manager is not activated.
        } else if ((sm_mbx.address < ECSD_MAILBOX_WRITE__MIN_ADDRESS) ||
                ((sm_mbx.address + sm_mbx.length) >= ECSD_MAILBOX_WRITE__MAX_ADDRESS)) {
            ret = EC_FALSE; // ERROR: Sync manager address out of range
        } else if ((sm_mbx.length < ECSD_MAILBOX_WRITE__MIN_LENGTH) ||
                (sm_mbx.length > ECSD_MAILBOX_WRITE__MAX_LENGTH)) {
            ret = EC_FALSE; // ERROR: Sync manager to long 
        } else if ((sm_mbx.address & 0x1) || (sm_mbx.length & 0x1)) {
            ret = EC_FALSE; // ERROR: Sync manager address or length not at 16-bit boundaries!
        }

        if (ret == EC_TRUE) {
            // Check second Sync manager for read mailbox config.
            hw_read(pecsd, EC_REG_SYNC_MANAGER + (1 * EC_REG_SYNC_MANAGER__BASE_OFFSET), (ec_uint8_t *)&sm_mbx, sizeof(sm_mbx));

            if ((sm_mbx.control & EC_REG_SYNC_MANAGER__CONTROL__OP_MODE_MASK) != 
                    EC_REG_SYNC_MANAGER__CONTROL__OP_MODE_MAILBOX) {
                ret = EC_FALSE; // ERROR: Operation mode is not set to MAILBOX
            } else if ((sm_mbx.control & EC_REG_SYNC_MANAGER__CONTROL__DIRECTION_MASK) != 
                    EC_REG_SYNC_MANAGER__CONTROL__DIRECTION_READ) {
                ret = EC_FALSE; // ERROR: Direction is not set to READ (ESC read <-> PDI write)
            } else if (!(sm_mbx.activate & EC_REG_SYNC_MANAGER__ACTIVATE__ENABLE_SM)) {
                ret = EC_FALSE; // ERROR: Sync manager is not activated.
            } else if ((sm_mbx.address < ECSD_MAILBOX_READ__MIN_ADDRESS) ||
                    ((sm_mbx.address + sm_mbx.length) >= ECSD_MAILBOX_READ__MAX_ADDRESS)) {
                ret = EC_FALSE; // ERROR: Sync manager address out of range
            } else if ((sm_mbx.length < ECSD_MAILBOX_READ__MIN_LENGTH) ||
                    (sm_mbx.length > ECSD_MAILBOX_READ__MAX_LENGTH)) {
                ret = EC_FALSE; // ERROR: Sync manager to long 
            } else if ((sm_mbx.address & 0x1) || (sm_mbx.length & 0x1)) {
                ret = EC_FALSE; // ERROR: Sync manager address or length not at 16-bit boundaries!
            }
        }
    }

    return ret;
}

ec_bool_t ecsd_sm_settings_2_to_n_match(struct ecsd *pecsd) {
    return EC_TRUE;
}

//! \brief Enable sync manager channel
/*!
 * \param[in]   pecsd        Pointer to ESC struture.
 * \param[in]   ch          Sync manager channel number.
 */
ec_void_t ecsd_enable_sm_channel(struct ecsd *pecsd, ec_uint8_t ch) {
    assert(pecsd != NULL);

    ec_uint16_t sm_control_reg_address = EC_REG_SYNC_MANAGER_N(ch) + EC_REG_SYNC_MANAGER__PDI_CONTROL_OFFSET;

    ec_uint8_t ctrl = 0u;
    hw_write(pecsd, sm_control_reg_address, &ctrl, sizeof(ctrl));

    do {
        hw_read(pecsd, sm_control_reg_address, &ctrl, sizeof(ctrl));
    } while ((ctrl & EC_REG_SYNC_MANAGER__PDI_CONTROL__DEACTIVATE) != 0u);
}

//! \brief Disable sync manager channel
/*!
 * \param[in]   pecsd        Pointer to ESC struture.
 * \param[in]   ch          Sync manager channel number.
 */
ec_void_t ecsd_disable_sm_channel(struct ecsd *pecsd, ec_uint8_t ch) {
    assert(pecsd != NULL);

    ec_uint16_t sm_control_reg_address = EC_REG_SYNC_MANAGER_N(ch) + EC_REG_SYNC_MANAGER__PDI_CONTROL_OFFSET;

    ec_uint8_t ctrl = EC_REG_SYNC_MANAGER__PDI_CONTROL__DEACTIVATE;
    hw_write(pecsd, sm_control_reg_address, &ctrl, sizeof(ctrl));

    do {
        hw_read(pecsd, sm_control_reg_address, &ctrl, sizeof(ctrl));
    } while ((ctrl & EC_REG_SYNC_MANAGER__PDI_CONTROL__DEACTIVATE) == 0u);
}

//! \brief Start mailbox handling.
/*!
 * \param[in]   pecsd        Pointer to ESC struture.
 */
ec_bool_t ecsd_start_mbx_handler(struct ecsd *pecsd) {
    assert(pecsd != NULL);
    ec_bool_t ret = EC_TRUE;

    if (pecsd->mbx_supported == EC_TRUE) {
        // check SyncManager 0/1 for mailbox configuration.
        hw_read(pecsd, EC_REG_SYNC_MANAGER_0, (ec_uint8_t *)&pecsd->mbx.sm_receive, sizeof(pecsd->mbx.sm_receive));
        hw_read(pecsd, EC_REG_SYNC_MANAGER_1, (ec_uint8_t *)&pecsd->mbx.sm_send, sizeof(pecsd->mbx.sm_send));

        if ((pecsd->mbx.sm_receive.address < 0x1000) || (pecsd->mbx.sm_send.address < 0x1000)) {
            // ERROR: programmed address to small
            ret = EC_FALSE;
        } else if ( 
                (   (pecsd->mbx.sm_receive.address < pecsd->mbx.sm_send.address) && 
                    ((pecsd->mbx.sm_receive.address + pecsd->mbx.sm_receive.length) > (pecsd->mbx.sm_send.address + pecsd->mbx.sm_send.length)) ) ||
                (   (pecsd->mbx.sm_receive.address > pecsd->mbx.sm_send.address) && 
                    ((pecsd->mbx.sm_receive.address + pecsd->mbx.sm_receive.length) < (pecsd->mbx.sm_send.address + pecsd->mbx.sm_send.length))) ) 
        {
            // ERROR: address ranges do overlap
            ret = EC_FALSE;
        }

        if (ret == EC_TRUE) {
            // All OK -> Enable receive and send sync manager.
            ecsd_enable_sm_channel(pecsd, 0);
            ecsd_enable_sm_channel(pecsd, 1);

            pecsd->mbx_running = EC_TRUE;
        }
    }

    return ret;
}

//! \brief Start mailbox handling.
/*!
 * \param[in]   pecsd        Pointer to ESC struture.
 */
ec_bool_t ecsd_stop_mbx_handler(struct ecsd *pecsd) {
    assert(pecsd != NULL);
    ec_bool_t ret = EC_TRUE;

    if (pecsd->mbx_supported == EC_TRUE) {
        ecsd_disable_sm_channel(pecsd, 0);
        ecsd_disable_sm_channel(pecsd, 1);
            
        pecsd->mbx_running = EC_FALSE;
    }

    return ret;
}

typedef enum ec_sm_direction {
    ecsd_sm_direction_read,      //!< \brief Read configuration  => ESC-read  <-> PDI-write
    ecsd_sm_direction_write      //!< \brief Write configuration => ESC-write <-> PDI-read
} ecsd_sm_direction_t;

typedef enum ecsd_sm_mode {
    ecsd_sm_mode_enable,         //!< \brief Enable Sync Manager.
    ecsd_sm_mode_disable         //!< \brief Disable Sync Manager.
} ecsd_sm_mode_t;

//! \brief Enable sync manager channels for PD In- or Outputs
/*!
 * \param[in]   pecsd        Pointer to ESC struture.
 * \param[in]   sm_dir      Flag identifying sync managers direction.
 * \param[in]   sm_mode     Disable instead of enable.
 */
ec_void_t ecsd_configure_sm_channels_pd(struct ecsd *pecsd, ecsd_sm_direction_t sm_dir, ecsd_sm_mode_t sm_mode) {
    assert(pecsd != NULL);
    ec_uint8_t ch_start = 0u, ch;
    ec_uint8_t direction = sm_dir == ecsd_sm_direction_read ? 
        EC_REG_SYNC_MANAGER__CONTROL__DIRECTION_READ :
        EC_REG_SYNC_MANAGER__CONTROL__DIRECTION_WRITE;

    if (pecsd->mbx_supported) {
        // skip mailbox sync managers
        ch_start = 2u;
    }

    for (ch = ch_start; ch < ECSD_MAX_SYNC_MANAGER; ++ch) {
        struct ec_slave_sm sm_mbx;
        hw_read(pecsd, EC_REG_SYNC_MANAGER_N(ch), (ec_uint8_t *)&sm_mbx, sizeof(sm_mbx));
            
        if ((sm_mbx.control & EC_REG_SYNC_MANAGER__CONTROL__DIRECTION_MASK) == direction) {
            if (sm_mode == ecsd_sm_mode_enable) {            
                ecsd_enable_sm_channel(pecsd, ch);
            } else {
                ecsd_disable_sm_channel(pecsd, ch);
            }
        }
    }
}

//! \brief Start input update.
/*!
 * PREOP->SAFEOP: 
 * Start Input Update to the ESC by enabling SyncManager
 * Start Output Update from the ESC by enabling SyncManager
 *
 * \param[in]   pecsd        Pointer to ESC struture.
 */
ec_retval_t ecsd_start_input_handler(struct ecsd *pecsd) {
    // dc checks
    uint8_t dc_control[4];
    hw_read(pecsd, EC_REG_DC_CYCLIC_UNIT_CONTROL, &dc_control[0], 4 * sizeof(uint8_t));

    if (dc_control[1] & EC_REG_DC_ACTIVATION__SYNC_OUT_UNIT_ACTIVATION) {
        // Sync out unit activation configured by maindevice.
        ec_bool_t dc_sync0_generation = (dc_control[1] & EC_REG_DC_ACTIVATION__SYNC0_GENERATION) ? 1 : 0;
        ec_bool_t dc_sync1_generation = (dc_control[1] & EC_REG_DC_ACTIVATION__SYNC1_GENERATION) ? 1 : 0;
    }

    return EC_OK;
}

ec_retval_t ecsd_stop_input_handler(struct ecsd *pecsd) {
    return EC_OK;
}

ec_retval_t ecsd_start_local_output_handler(struct ecsd *pecsd) {
    return EC_OK;
}

ec_retval_t ecsd_stop_local_output_handler(struct ecsd *pecsd) {
    return EC_OK;
}

//! \brief AL Control response function
/*!
 * \param[in]   pecsd               Pointer to EtherCAT SubDevice structure.
 * \param[in]   al_state            AL state to be set.
 * \param[in]   al_status_code      AL status code in case of error.
 * \param[in]   al_error_flag       Error indication flag.
 * \param[in]   id_flag             ID response flag.
 */
static ec_void_t ecsd_al_control_rsp(struct ecsd *pecsd, ec_uint8_t al_state, 
        ec_uint16_t al_status_code, ec_bool_t al_error_flag, ec_bool_t id_flag) 
{
    ec_uint16_t al_status = 0u;
    EC_REG_AL_STATUS__STATE__SET(al_status, al_state);
    EC_REG_AL_STATUS__ERROR_IND__SET(al_status, al_error_flag);
    EC_REG_AL_STATUS__ID__SET(al_status, id_flag);

    if (al_error_flag == EC_TRUE) {
        hw_write(pecsd, EC_REG_AL_STATUS_CODE, (ec_uint8_t *)&al_status_code, sizeof(al_status_code));
    }

    hw_write(pecsd, EC_REG_AL_STATUS, (ec_uint8_t *)&al_status, sizeof(al_status));
}

ec_void_t ecsd_id_info(struct ecsd *pecsd, ec_bool_t id_request) {
    // TODO
}

ec_bool_t ecsd_dc_activated(struct ecsd *pecsd) {
    return EC_FALSE;
}
                            
ec_void_t ecsd_start_safeop_2_op_timeout_timer(struct ecsd *pecsd) {
}
                
ec_void_t ecsd_ack_sm_chg(struct ecsd *pecsd) {
}

//! \brief AL control indication
/*!
 * \param[in]   pecsd               Pointer to EtherCAT SubDevice structure.
 * \param[in]   al_control_state    Requested AL control state.
 * \param[in]   ack_flag            Requested error ack flag.
 * \param[in]   id_request          Request identification.
 *
 * \retval EC_OK    On success.
 */
ec_retval_t ecsd_al_control_ind(struct ecsd *pecsd, ec_uint8_t al_control_state, ec_bool_t ack_flag, ec_bool_t id_request) 
{
    ec_retval_t retval = EC_OK;

    pecsd->al_control_state = al_control_state;

    ec_log(10, __func__, "al_state %X, al_control %X\n", pecsd->al_state, al_control_state);
    sleep(1);

    switch (pecsd->al_state) 
    {
        case EC_STATE_INIT: 
            switch (al_control_state) 
            {
                case EC_STATE_INIT:
                    // #1.1 | #2
                    if ((pecsd->al_error_flag == EC_TRUE) && (ack_flag == EC_FALSE)) {
                        // #1.1
                        ec_log(100, __func__, "Transition #1.1, reset al_status_code.\n");

                        pecsd->al_status_code = 0u;
                        pecsd->al_state = EC_STATE_INIT;
                    } else { // al_error_flag == EC_FALSE || ack_flag == EC_TRUE
                        // #2
                        ec_log(100, __func__, "Transition #2, stay.\n");

                        pecsd->al_error_flag = EC_FALSE;
                    }

                    ecsd_id_info(pecsd, id_request);
                    ecsd_al_control_rsp(pecsd, pecsd->al_state, pecsd->al_status_code, pecsd->al_error_flag, pecsd->id_flag);
                    break;
                case EC_STATE_PREOP:
                    if ((pecsd->al_error_flag == EC_FALSE) || (ack_flag == EC_TRUE)) {
                        // #3 | #4
                        if (ecsd_sm_settings_0_and_1_match(pecsd) == EC_TRUE) {
                            // #3
                            ec_log(100, __func__, "Transition #3, switching to PREOP.\n");

                            pecsd->al_error_flag = EC_FALSE;
                            pecsd->al_status_code = 0;
                            pecsd->al_state = EC_STATE_PREOP;

                            (void)ecsd_start_mbx_handler(pecsd);
                    
                            ecsd_id_info(pecsd, id_request);
                        } else {
                            // #4
                            ec_log(100, __func__, "Transition #4, wrong mbx sm settings, staying in INIT.\n");

                            pecsd->id_flag = EC_FALSE;
                            pecsd->al_state = EC_STATE_INIT;
                            pecsd->al_status_code = EC_AL_STATUS_CODE__INVALID_MAILBOX_CONFIGURATION__PREOP;
                            pecsd->al_error_flag = EC_TRUE;
                        }
                    } else {
                        // #1.2
                        ec_log(100, __func__, "Transition #1.2, stay in INIT.\n");
                    }

                    ecsd_al_control_rsp(pecsd, pecsd->al_state, pecsd->al_status_code, pecsd->al_error_flag, pecsd->id_flag);
                    break;
                case EC_STATE_BOOT:
                    // #5 | #6 | #7 | #1.2
                    if ((pecsd->al_error_flag == EC_FALSE) || (ack_flag == EC_TRUE)) {
                        // #5 | #6 | #7
                        if (pecsd->boot_supported == EC_TRUE) {
                            // #5 | #6
                            if (ecsd_sm_settings_0_and_1_match(pecsd) == EC_TRUE) {
                                // #5
                                ec_log(100, __func__, "Transition #5, switching to BOOT.\n");

                                pecsd->al_error_flag = EC_FALSE;
                                pecsd->al_state = EC_STATE_BOOT;
                                pecsd->al_status_code = 0;

                                (void)ecsd_start_mbx_handler(pecsd);
                            } else {
                                // #6
                                ec_log(100, __func__, "Transition #6, wrong mbx sm settings, staying in INIT.\n");

                                pecsd->id_flag = EC_FALSE;
                                pecsd->al_state = EC_STATE_INIT;
                                pecsd->al_status_code = EC_AL_STATUS_CODE__INVALID_MAILBOX_CONFIGURATION__BOOT;
                                pecsd->al_error_flag = EC_TRUE;
                            }
                        } else {
                            // #7
                            ec_log(100, __func__, "Transition #7, BOOT not supported, staying in INIT.\n");

                            pecsd->id_flag = EC_FALSE;
                            pecsd->al_state = EC_STATE_INIT;
                            pecsd->al_status_code = EC_AL_STATUS_CODE__BOOTSTRAP_NOT_SUPPORTED;
                            pecsd->al_error_flag = EC_TRUE;
                        }
                    } else {
                        // #1.2
                        ec_log(100, __func__, "Transition #1.2, stay in INIT.\n");

                    }

                    ecsd_al_control_rsp(pecsd, pecsd->al_state, pecsd->al_status_code, pecsd->al_error_flag, pecsd->id_flag);
                    break;
                case EC_STATE_SAFEOP:
                case EC_STATE_OP:
                    if ((pecsd->al_error_flag == EC_FALSE) || (ack_flag == EC_TRUE)) {
                        // #8
                        ec_log(100, __func__, "Transition #8, invalid state change requested, stay in INIT.\n");

                        pecsd->id_flag = EC_FALSE;
                        pecsd->al_state = EC_STATE_INIT;
                        pecsd->al_status_code = EC_AL_STATUS_CODE__INVALID_REQUESTED_STATE_CHANGE;
                        pecsd->al_error_flag = EC_TRUE;
                    } else {
                        // #1.2
                        ec_log(100, __func__, "Transition #1.2, stay in INIT.\n");
                    }
                        
                    ecsd_al_control_rsp(pecsd, pecsd->al_state, pecsd->al_status_code, pecsd->al_error_flag, pecsd->id_flag);
                    break;
                default: 
                    // #9
                    ec_log(100, __func__, "Transition #9, unknown requested state, stay in INIT.\n");

                    pecsd->id_flag = EC_FALSE;
                    pecsd->al_state = EC_STATE_INIT;
                    pecsd->al_status_code = EC_AL_STATUS_CODE__UNKNOWN_REQUESTED_STATE;
                    pecsd->al_error_flag = EC_TRUE;

                    ecsd_al_control_rsp(pecsd, pecsd->al_state, pecsd->al_status_code, pecsd->al_error_flag, pecsd->id_flag);
                    break;
            }
            break; // EC_STATE_INIT
        case EC_STATE_PREOP:
            switch (al_control_state) {
                case EC_STATE_INIT:
                    // #11.1 | #12
                    if ((pecsd->al_error_flag == EC_TRUE) && (ack_flag == EC_FALSE)) {
                        // #11.1
                        ec_log(100, __func__, "Transition #11.1, reset ERROR, switching to INIT.\n");

                        pecsd->al_error_flag = EC_FALSE;
                        pecsd->al_status_code = 0;
                        pecsd->al_state = EC_STATE_INIT;

                        (void)ecsd_stop_mbx_handler(pecsd);

                        ecsd_id_info(pecsd, id_request);
                    } else {
                        // #12
                        ec_log(100, __func__, "Transition #12, switching to INIT.\n");

                        pecsd->al_error_flag = EC_FALSE;
                        pecsd->al_status_code = 0;
                        pecsd->al_state = EC_STATE_INIT;

                        (void)ecsd_stop_mbx_handler(pecsd);
                        
                        ecsd_id_info(pecsd, id_request);
                    }

                    ecsd_al_control_rsp(pecsd, pecsd->al_state, pecsd->al_status_code, pecsd->al_error_flag, pecsd->id_flag);
                    break;
                case EC_STATE_PREOP:
                    if ((pecsd->al_error_flag == EC_FALSE) || (ack_flag == EC_TRUE)) {
                        // #13
                        ec_log(100, __func__, "Transition #13, reset ERROR, stay in PREOP.\n");

                        pecsd->al_error_flag = EC_FALSE;
                        pecsd->al_status_code = 0;

                        ecsd_id_info(pecsd, id_request);
                    } else {
                        // #11.2
                        ec_log(100, __func__, "Transition #11.2, stay in PREOP.\n");

                    }
                        
                    ecsd_al_control_rsp(pecsd, pecsd->al_state, pecsd->al_status_code, pecsd->al_error_flag, pecsd->id_flag);
                    break;
                case EC_STATE_SAFEOP:
                    if ((pecsd->al_error_flag == EC_FALSE) || (ack_flag == EC_TRUE)) {
                        // #14.1 | #14.2 | #14.3 | #17
                        if (ecsd_sm_settings_2_to_n_match(pecsd) == EC_TRUE) {
                            // #14.1 | #14.2 | #14.3
                            if (    ((ecsd_dc_activated(pecsd) == EC_FALSE) && (pecsd->dc_required == EC_FALSE)) ||
                                    ((ecsd_dc_activated(pecsd) == EC_TRUE) && (pecsd->dc_not_accepted == EC_FALSE) && (pecsd->dc_supported == EC_FALSE)) ) {
                                // #14.1
                                ec_log(100, __func__, "Transition #14.1, switching to SAFEOP.\n");

                                pecsd->dc_running = EC_FALSE;
                                pecsd->al_error_flag = EC_FALSE;
                                pecsd->al_status_code = 0;
                                pecsd->al_state = EC_STATE_SAFEOP;

                                (void)ecsd_start_input_handler(pecsd);
                                ecsd_id_info(pecsd, id_request);
                            } else if ((ecsd_dc_activated(pecsd) == EC_TRUE) && (pecsd->dc_not_accepted == EC_FALSE) && (pecsd->dc_supported == EC_TRUE)) {
                                // #14.2
                                ec_log(100, __func__, "Transition #14.2, switching to SAFEOP.\n");

                                pecsd->dc_running = EC_TRUE;
                                pecsd->al_error_flag = EC_FALSE;
                                pecsd->al_status_code = 0;
                                pecsd->al_state = EC_STATE_SAFEOP;

                                (void)ecsd_start_input_handler(pecsd);
                                ecsd_id_info(pecsd, id_request);
                            } else if ( ((ecsd_dc_activated(pecsd) == EC_TRUE) && (pecsd->dc_not_accepted == EC_TRUE)) ||
                                    ((ecsd_dc_activated(pecsd) == EC_FALSE) && (pecsd->dc_required == EC_TRUE)) ) {
                                // #14.3
                                ec_log(100, __func__, "Transition #14.3, switching to SAFEOP.\n");

                                pecsd->id_flag = EC_FALSE;
                                pecsd->al_status_code = EC_AL_STATUS_CODE__INVALID_DC_SYNC_CONFIGURATION;
                                pecsd->al_error_flag = EC_TRUE;
                                pecsd->al_state = EC_STATE_PREOP;
                            }
                        } else {
                            // #17
                            ec_log(100, __func__, "Transition #17, stay in PREOP.\n");

                            pecsd->id_flag = EC_FALSE;
                            pecsd->al_state = EC_STATE_PREOP;
                            pecsd->al_status_code = EC_AL_STATUS_CODE__INVALID_SYNC_MANAGER_CONFIGURATION;
                            pecsd->al_error_flag = EC_TRUE;
                        }
                    } else {
                        // #11.2
                    }

                    ecsd_al_control_rsp(pecsd, pecsd->al_state, pecsd->al_status_code, pecsd->al_error_flag, pecsd->id_flag);
                    break;
                case EC_STATE_OP:
                case EC_STATE_BOOT:
                    // #18
                    ec_log(100, __func__, "Transition #18, invalid state change requested, stay in PREOP.\n");

                    pecsd->id_flag = EC_FALSE;
                    pecsd->al_state = EC_STATE_PREOP;
                    pecsd->al_status_code = EC_AL_STATUS_CODE__INVALID_REQUESTED_STATE_CHANGE;
                    pecsd->al_error_flag = EC_TRUE;

                    ecsd_al_control_rsp(pecsd, pecsd->al_state, pecsd->al_status_code, pecsd->al_error_flag, pecsd->id_flag);
                    break;
                default:
                    // #19
                    ec_log(100, __func__, "Transition #19, unknown state change requested, stay in PREOP.\n");

                    pecsd->id_flag = EC_FALSE;
                    pecsd->al_state = EC_STATE_PREOP;
                    pecsd->al_status_code = EC_AL_STATUS_CODE__UNKNOWN_REQUESTED_STATE;
                    pecsd->al_error_flag = EC_TRUE;

                    ecsd_al_control_rsp(pecsd, pecsd->al_state, pecsd->al_status_code, pecsd->al_error_flag, pecsd->id_flag);
                    break;
            }
            break; // EC_STATE_PREOP
        case EC_STATE_SAFEOP:
            switch (al_control_state) {
                case EC_STATE_INIT:
                    // #22.1 | #23
                    if ((pecsd->al_error_flag == EC_TRUE) && (ack_flag == EC_FALSE)) {
                        // #22.1
                        ec_log(100, __func__, "Transition #22.1, reset ERROR, switching to INIT.\n");

                        pecsd->al_error_flag = EC_FALSE;
                        pecsd->al_status_code = 0u;
                        pecsd->al_state = EC_STATE_INIT;

                        ecsd_stop_mbx_handler(pecsd);
                        ecsd_stop_input_handler(pecsd);
                        ecsd_stop_local_output_handler(pecsd);

                        ecsd_id_info(pecsd, id_request);
                    } else {
                        // #23
                        ec_log(100, __func__, "Transition #23, switching to INIT.\n");

                        pecsd->al_error_flag = EC_FALSE;
                        pecsd->al_state = EC_STATE_INIT;

                        ecsd_stop_mbx_handler(pecsd);
                        ecsd_stop_input_handler(pecsd);

                        ecsd_id_info(pecsd, id_request);
                    }

                    ecsd_al_control_rsp(pecsd, pecsd->al_state, pecsd->al_status_code, pecsd->al_error_flag, pecsd->id_flag);
                    break;
                case EC_STATE_PREOP:
                    // #24
                    ec_log(100, __func__, "Transition #23, switching to PREOP.\n");

                    if ((pecsd->al_error_flag == EC_FALSE) || (ack_flag == EC_TRUE)) {
                        pecsd->al_error_flag = EC_FALSE;
                        pecsd->al_state = EC_STATE_PREOP;
                    
                        ecsd_stop_input_handler(pecsd);

                        ecsd_id_info(pecsd, id_request);
                        ecsd_al_control_rsp(pecsd, pecsd->al_state, pecsd->al_status_code, pecsd->al_error_flag, pecsd->id_flag);
                    }
                    break;
                case EC_STATE_SAFEOP:
                    // #25.1 | #25.2 
                    if ((pecsd->al_error_flag == EC_TRUE) && ((pecsd->local_error_flag == EC_FALSE) && (ack_flag == EC_TRUE))) {
                        // #25.1
                        ec_log(100, __func__, "Transition #25.1, reset ERROR, stay in SAFEOP.\n");

                        pecsd->al_error_flag = EC_FALSE;
                        pecsd->al_status_code = 0u;

                        ecsd_configure_sm_channels_pd(pecsd, ecsd_sm_direction_read, ecsd_sm_mode_enable);

                        ecsd_id_info(pecsd, id_request);
                    } else if ((pecsd->al_error_flag == EC_FALSE) || ((pecsd->local_error_flag == EC_TRUE) && (ack_flag == EC_TRUE))) {
                        // #25.2
                        ec_log(100, __func__, "Transition #25.2, reset ERROR, stay in SAFEOP.\n");

                        pecsd->al_error_flag = EC_FALSE;
                        pecsd->al_status_code = 0u;

                        ecsd_id_info(pecsd, id_request);
                    }

                    ecsd_al_control_rsp(pecsd, pecsd->al_state, pecsd->al_status_code, pecsd->al_error_flag, pecsd->id_flag);
                    break;
                case EC_STATE_OP:
                    // # 26.2 | #26.4 | #27.1 | #27.2 | #22.2
                    if ((pecsd->al_error_flag == EC_FALSE) || (ack_flag == EC_TRUE)) { 
                        if (    (pecsd->ready_for_op == EC_TRUE) && 
                                ((pecsd->dc_running == EC_FALSE) || (pecsd->pll_running == EC_TRUE)) && 
                                (pecsd->local_error_flag == EC_FALSE)) 
                        {
                            // #26.2
                            ec_log(100, __func__, "Transition #26.2, switching to OP.\n");

                            pecsd->al_error_flag = EC_FALSE;
                            pecsd->al_status_code = 0u;
                            pecsd->al_state = EC_STATE_OP;

                            ecsd_configure_sm_channels_pd(pecsd, ecsd_sm_direction_read, ecsd_sm_mode_enable);
                            ecsd_configure_sm_channels_pd(pecsd, ecsd_sm_direction_write, ecsd_sm_mode_enable);
                            ecsd_start_local_output_handler(pecsd);

                            ecsd_id_info(pecsd, id_request);
                        } else if ((pecsd->dc_running == EC_TRUE) && (pecsd->pll_running == EC_FALSE) && (pecsd->local_error_flag == EC_FALSE)) {
                            // #26.4
                            ec_log(100, __func__, "Transition #26.4, switching to OP.\n");

                            ecsd_configure_sm_channels_pd(pecsd, ecsd_sm_direction_write, ecsd_sm_mode_enable);

                            pecsd->wait_for_pll_running = EC_TRUE;

                            ecsd_start_safeop_2_op_timeout_timer(pecsd);

                            ecsd_id_info(pecsd, id_request);
                        } else if ( (pecsd->ready_for_op == EC_FALSE) && 
                                    ((pecsd->dc_running == EC_FALSE) || (pecsd->pll_running == EC_TRUE)) &&
                                    (pecsd->local_error_flag == EC_FALSE) ) {
                            // #27.1
                            ec_log(100, __func__, "Transition #27.1, sync manager watchdog expired, stay in SAFEOP.\n");

                            pecsd->id_flag = EC_FALSE;
                            pecsd->al_state = EC_STATE_SAFEOP;
                            pecsd->al_status_code = EC_AL_STATUS_CODE__SYNC_MANAGER_WATCHDOG;
                            pecsd->al_error_flag = EC_TRUE;
                        } else if (pecsd->local_error_flag == EC_TRUE) {
                            // #27.2
                            ec_log(100, __func__, "Transition #27.2, stay in SAFEOP.\n");

                            pecsd->id_flag = EC_FALSE;
                            pecsd->al_state = EC_STATE_SAFEOP;
                            pecsd->al_status_code = pecsd->local_error_code;
                            pecsd->al_error_flag = EC_TRUE;
                        }
                    } else {
                        // #22.2
                    }
                        
                    ecsd_al_control_rsp(pecsd, pecsd->al_state, pecsd->al_status_code, pecsd->al_error_flag, pecsd->id_flag);
                    break;
                case EC_STATE_BOOT:
                    // #29 | #22.2
                    if ((pecsd->al_error_flag == EC_FALSE) || (ack_flag == EC_TRUE)) { 
                        // #29
                        ec_log(100, __func__, "Transition #29, invalid state change requested, stay in SAFEOP.\n");

                        pecsd->id_flag = EC_FALSE;
                        pecsd->al_state = EC_STATE_SAFEOP;
                        pecsd->al_status_code = EC_AL_STATUS_CODE__INVALID_REQUESTED_STATE_CHANGE;
                        pecsd->al_error_flag = EC_TRUE;
                    } else {
                        // #22.2
                        ec_log(100, __func__, "Transition #22.2, stay in SAFEOP.\n");

                    }
                        
                    ecsd_al_control_rsp(pecsd, pecsd->al_state, pecsd->al_status_code, pecsd->al_error_flag, pecsd->id_flag);
                    break;
                default:
                    // #30
                    ec_log(100, __func__, "Transition #30, unknown state change requested, stay in SAFEOP.\n");

                    pecsd->id_flag = EC_FALSE;
                    pecsd->al_state = EC_STATE_SAFEOP;
                    pecsd->al_status_code = EC_AL_STATUS_CODE__UNKNOWN_REQUESTED_STATE;
                    pecsd->al_error_flag = EC_TRUE;

                    ecsd_al_control_rsp(pecsd, pecsd->al_state, pecsd->al_status_code, pecsd->al_error_flag, pecsd->id_flag);
                    break;
            }
            break;
        case EC_STATE_OP:
            switch (al_control_state) {
                case EC_STATE_INIT: 
                    // #37
                    ec_log(100, __func__, "Transition #37, switch to INIT.\n");

                    pecsd->al_status_code = 0u;
                    pecsd->al_error_flag = EC_FALSE;
                    pecsd->al_state = EC_STATE_INIT;

                    ecsd_stop_mbx_handler(pecsd);
                    ecsd_stop_input_handler(pecsd);
                    ecsd_stop_local_output_handler(pecsd);

                    ecsd_id_info(pecsd, id_request);
                    break;
                case EC_STATE_PREOP:
                    // #38
                    ec_log(100, __func__, "Transition #38, switch to PREOP.\n");

                    pecsd->al_error_flag = EC_FALSE;
                    pecsd->al_status_code = 0u;
                    pecsd->al_state = EC_STATE_PREOP;

                    ecsd_stop_input_handler(pecsd);
                    ecsd_stop_local_output_handler(pecsd);

                    ecsd_id_info(pecsd, id_request);
                    break;
                case EC_STATE_SAFEOP:
                    // #39
                    ec_log(100, __func__, "Transition #39, switch to SAFEOP.\n");

                    pecsd->al_error_flag = EC_FALSE;
                    pecsd->al_status_code = 0u;
                    pecsd->al_state = EC_STATE_SAFEOP;

                    ecsd_stop_local_output_handler(pecsd);

                    ecsd_id_info(pecsd, id_request);
                    break;
                case EC_STATE_OP:
                    // #40
                    ec_log(100, __func__, "Transition #40, stay in OP.\n");

                    ecsd_id_info(pecsd, id_request);
                    break;
                case EC_STATE_BOOT:
                    // #42
                    ec_log(100, __func__, "Transition #42, invaild state change requested, switch to SAFEOP.\n");

                    pecsd->id_flag = EC_FALSE;
                    pecsd->al_state = EC_STATE_SAFEOP;
                    pecsd->al_status_code = EC_AL_STATUS_CODE__INVALID_REQUESTED_STATE_CHANGE;
                    pecsd->al_error_flag = EC_TRUE;

                    ecsd_configure_sm_channels_pd(pecsd, ecsd_sm_direction_read, ecsd_sm_mode_disable);
                    ecsd_configure_sm_channels_pd(pecsd, ecsd_sm_direction_write, ecsd_sm_mode_disable);
                    ecsd_stop_local_output_handler(pecsd);
                    break;
                default:
                    // #43
                    ec_log(100, __func__, "Transition #43, unknown state change requested, switch to SAFEOP.\n");

                    pecsd->id_flag = EC_FALSE;
                    pecsd->al_state = EC_STATE_SAFEOP;
                    pecsd->al_status_code = EC_AL_STATUS_CODE__UNKNOWN_REQUESTED_STATE;
                    pecsd->al_error_flag = EC_TRUE;

                    ecsd_configure_sm_channels_pd(pecsd, ecsd_sm_direction_write, ecsd_sm_mode_disable);
                    ecsd_stop_local_output_handler(pecsd);
                    break;
            }
                    
            ecsd_al_control_rsp(pecsd, pecsd->al_state, pecsd->al_status_code, pecsd->al_error_flag, pecsd->id_flag);
            break;
        case EC_STATE_BOOT:
            switch (al_control_state) {
                case EC_STATE_INIT:
                    // #51
                    ec_log(100, __func__, "Transition #51, switch to INIT.\n");

                    pecsd->al_error_flag = EC_FALSE;
                    pecsd->al_state = EC_STATE_INIT;

                    ecsd_stop_mbx_handler(pecsd);
                    break;
                case EC_STATE_BOOT:
                    // #52
                    ec_log(100, __func__, "Transition #52, stay in BOOT.\n");

                    pecsd->al_error_flag = EC_FALSE;
                case EC_STATE_PREOP:
                case EC_STATE_SAFEOP:
                case EC_STATE_OP:
                    break;
                default:
                    break;
            }
            ecsd_al_control_rsp(pecsd, pecsd->al_state, pecsd->al_status_code, pecsd->al_error_flag, pecsd->id_flag);
            break;
        default: {
            pecsd->al_state = EC_STATE_INIT;
            break;
        }
    }

    return retval;
}

//! \brief Sync manager change event occured
/*!
 * \param[in]   pecsd        Pointer to ESC struture.
 */
ec_void_t ecsd_sm_chg(struct ecsd *pecsd) {
    switch (pecsd->al_state) {
        case EC_STATE_INIT:
            // #10
            break;
        case EC_STATE_PREOP:
            // #20.1 | #20.2 | #21
            if ((pecsd->al_error_flag == EC_FALSE) && (ecsd_sm_settings_0_and_1_match(pecsd) == EC_TRUE)) {
                // #20.1
                ecsd_ack_sm_chg(pecsd);
            } else if (pecsd->al_error_flag == EC_TRUE) {
                // #20.2
            } else {
                // #21
                pecsd->id_flag = EC_FALSE;
                pecsd->al_status_code = EC_AL_STATUS_CODE__INVALID_MAILBOX_CONFIGURATION__PREOP;
                pecsd->al_state = EC_STATE_INIT;

                ecsd_stop_mbx_handler(pecsd);
                ecsd_ack_sm_chg(pecsd);
                ecsd_al_state_change_req(pecsd, pecsd->al_state, pecsd->al_status_code);
            }
            break;
        case EC_STATE_SAFEOP:
            // #31.1 | #31.2 | #32 | #33
            if (pecsd->al_error_flag == EC_TRUE) {
                // #31.2
            } else {
                // #31.1 | #32 | #33
                if ((ecsd_sm_settings_0_and_1_match(pecsd) == EC_TRUE) && (ecsd_sm_settings_2_to_n_match(pecsd) == EC_TRUE)) {
                    // #31.1
                    ecsd_ack_sm_chg(pecsd);
                } else if ((ecsd_sm_settings_0_and_1_match(pecsd) == EC_TRUE) && (ecsd_sm_settings_2_to_n_match(pecsd) == EC_FALSE)) {
                    // #32
                    pecsd->id_flag = EC_FALSE;
                    pecsd->al_status_code = EC_AL_STATUS_CODE__INVALID_SYNC_MANAGER_CONFIGURATION;
                    pecsd->al_error_flag = EC_TRUE;
                    pecsd->al_state = EC_STATE_PREOP;

                    ecsd_stop_input_handler(pecsd);
                    ecsd_ack_sm_chg(pecsd);
                    ecsd_al_state_change_req(pecsd, pecsd->al_state, pecsd->al_status_code);
                } else if (ecsd_sm_settings_0_and_1_match(pecsd) == EC_FALSE) {
                    // #33
                    pecsd->id_flag = EC_FALSE;
                    pecsd->al_status_code = EC_AL_STATUS_CODE__INVALID_MAILBOX_CONFIGURATION__PREOP;
                    pecsd->al_error_flag = EC_TRUE;
                    pecsd->al_state = EC_STATE_INIT;

                    ecsd_stop_input_handler(pecsd);
                    ecsd_stop_mbx_handler(pecsd);
                    ecsd_ack_sm_chg(pecsd);
                    ecsd_al_state_change_req(pecsd, pecsd->al_state, pecsd->al_status_code);
                }
            }
            break;
        case EC_STATE_OP:
            // #44 | #45 | #46 
            if (pecsd->al_error_flag == EC_FALSE) {
                ec_bool_t sm_0_and_1_match = ecsd_sm_settings_0_and_1_match(pecsd);
                ec_bool_t sm_2_to_n_match = ecsd_sm_settings_2_to_n_match(pecsd);

                if ((sm_0_and_1_match == EC_TRUE) && (sm_2_to_n_match == EC_TRUE)) {
                    // #44
                    ecsd_ack_sm_chg(pecsd);
                } else if ((sm_0_and_1_match == EC_TRUE) && (sm_2_to_n_match == EC_FALSE)) {
                    // #45
                    pecsd->id_flag = EC_FALSE;
                    pecsd->al_status_code = EC_AL_STATUS_CODE__INVALID_SYNC_MANAGER_CONFIGURATION;
                    pecsd->al_error_flag = EC_TRUE;
                    pecsd->al_state = EC_STATE_PREOP;

                    ecsd_stop_local_output_handler(pecsd);
                    ecsd_stop_input_handler(pecsd);
                    ecsd_ack_sm_chg(pecsd);
                    ecsd_al_state_change_req(pecsd, pecsd->al_state, pecsd->al_status_code);
                } else {
                    // #46
                    pecsd->id_flag = EC_FALSE;
                    pecsd->al_status_code = EC_AL_STATUS_CODE__INVALID_MAILBOX_CONFIGURATION__PREOP;
                    pecsd->al_error_flag = EC_TRUE;
                    pecsd->al_state = EC_STATE_INIT;

                    ecsd_stop_local_output_handler(pecsd);
                    ecsd_stop_input_handler(pecsd);
                    ecsd_stop_mbx_handler(pecsd);
                    ecsd_ack_sm_chg(pecsd);
                    ecsd_al_state_change_req(pecsd, pecsd->al_state, pecsd->al_status_code);
                }
            }
            break;
        case EC_STATE_BOOT:
            // #55 | #56.1 | #56.2
            if (ecsd_sm_settings_0_and_1_match(pecsd) == EC_TRUE) {
                // #55
            } else {
                // #56.1 | #56.2
                pecsd->al_status_code = EC_AL_STATUS_CODE__INVALID_MAILBOX_CONFIGURATION__PREOP;
                pecsd->al_error_flag = EC_TRUE;
                pecsd->al_state = EC_STATE_INIT;
                
                ecsd_stop_mbx_handler(pecsd);
                ecsd_al_state_change_req(pecsd, pecsd->al_state, pecsd->al_status_code);
            }
            break;
        default:
            break;
    }
}

ec_retval_t ecsd_al_state_change_req(struct ecsd *pecsd, ec_uint8_t al_state, ec_uint16_t al_status_code) {
    switch (pecsd->al_state) {
        case EC_STATE_SAFEOP:
            // #34.1 | #34.2 | #34.3 | #34.4 | #34.5
            if ((al_state == EC_STATE_SAFEOP) && (pecsd->al_error_flag == EC_TRUE)) {
                // #34.1
                pecsd->id_flag = EC_FALSE;
                ecsd_configure_sm_channels_pd(pecsd, ecsd_sm_direction_write, ecsd_sm_mode_disable);
                pecsd->al_state = EC_STATE_SAFEOP;
                pecsd->al_error_flag = EC_TRUE;
                pecsd->local_error_flag = EC_TRUE;
                pecsd->local_error_code = al_status_code;

                // TODO this is an endless recursion:
                //ecsd_al_state_change_req(pecsd, pecsd->al_state, pecsd->al_status_code);
            } else if ((al_state == EC_STATE_SAFEOP) && (pecsd->al_error_flag == EC_FALSE)) {
                // #34.2
                ecsd_configure_sm_channels_pd(pecsd, ecsd_sm_direction_read, ecsd_sm_mode_enable);
                pecsd->local_error_flag = EC_FALSE;
                pecsd->local_error_code = 0u;
            } else if ((al_state == EC_STATE_PREOP) && (pecsd->al_error_flag == EC_TRUE)) {
                // #34.3
                pecsd->id_flag = EC_FALSE;
                ecsd_stop_input_handler(pecsd);
                pecsd->al_state = EC_STATE_PREOP;
                pecsd->al_error_flag = EC_TRUE;

                ecsd_al_state_change_req(pecsd, pecsd->al_state, pecsd->al_status_code);
            } else if ((al_state == EC_STATE_INIT) && (pecsd->al_error_flag == EC_TRUE)) {
                // #34.4
                pecsd->id_flag = EC_FALSE;
                ecsd_stop_mbx_handler(pecsd);
                ecsd_stop_input_handler(pecsd);
                pecsd->al_state = EC_STATE_INIT;
                pecsd->al_error_flag = EC_TRUE;

                ecsd_al_state_change_req(pecsd, pecsd->al_state, pecsd->al_status_code);
            } else if ((((al_state == EC_STATE_INIT) || (al_state == EC_STATE_PREOP)) && (pecsd->al_error_flag == EC_FALSE)) ||
                    (   (pecsd->al_control_state != EC_STATE_INIT) && 
                        (pecsd->al_control_state != EC_STATE_PREOP) && 
                        (pecsd->al_control_state != EC_STATE_SAFEOP) ))
            {
                // #34.5
            }
            break;
        case EC_STATE_OP:
            // #47.1 | #47.2 | #47.3 | #47.4
            if ((al_state == EC_STATE_SAFEOP) && (pecsd->al_error_flag == EC_TRUE)) {
                // #47.1
                ecsd_stop_local_output_handler(pecsd);
                ecsd_configure_sm_channels_pd(pecsd, ecsd_sm_direction_write, ecsd_sm_mode_disable);
                pecsd->al_state = EC_STATE_SAFEOP;
                pecsd->local_error_flag = EC_TRUE;
                pecsd->local_error_code = al_status_code;

                ecsd_al_state_change_req(pecsd, pecsd->al_state, pecsd->al_status_code);
            } else if ((al_state == EC_STATE_PREOP) && (pecsd->al_error_flag == EC_TRUE)) {
                // #47.2
                pecsd->id_flag = EC_FALSE;
                ecsd_stop_input_handler(pecsd);
                ecsd_stop_local_output_handler(pecsd);
                pecsd->al_state = EC_STATE_PREOP;
                pecsd->local_error_flag = EC_TRUE;
                pecsd->local_error_code = al_status_code;

                ecsd_al_state_change_req(pecsd, pecsd->al_state, pecsd->al_status_code);
            } else if ((al_state == EC_STATE_INIT) && (pecsd->al_error_flag == EC_TRUE)) {
                // #47.3
                pecsd->id_flag = EC_FALSE;
                ecsd_stop_mbx_handler(pecsd);
                ecsd_stop_input_handler(pecsd);
                ecsd_stop_local_output_handler(pecsd);
                pecsd->al_state = EC_STATE_INIT;
                pecsd->local_error_flag = EC_TRUE;
                pecsd->local_error_code = al_status_code;

                ecsd_al_state_change_req(pecsd, pecsd->al_state, pecsd->al_status_code);
            } else if ((((al_state == EC_STATE_SAFEOP) || (al_state == EC_STATE_PREOP) || (al_state == EC_STATE_INIT)) && (pecsd->al_error_flag == EC_FALSE)) ||
                    (   (pecsd->al_control_state != EC_STATE_INIT) && 
                        (pecsd->al_control_state != EC_STATE_PREOP) && 
                        (pecsd->al_control_state != EC_STATE_SAFEOP) && 
                        (pecsd->al_control_state != EC_STATE_OP))) 
            {
                // #47.4
            }
            break;
        case EC_STATE_BOOT:
            // #57 | #58
            if ((al_state == EC_STATE_INIT) && (pecsd->al_error_flag == EC_TRUE)) {
                // #57
                ecsd_stop_mbx_handler(pecsd);
                pecsd->al_state = EC_STATE_INIT;
                pecsd->al_error_flag = EC_TRUE;

                ecsd_al_state_change_req(pecsd, pecsd->al_state, pecsd->al_status_code);
            } else if (pecsd->al_error_flag == EC_FALSE) {
                // #58
            }
            break;
        default:
            break;
    }

    return EC_OK;
}

ec_void_t ecsd_sync0(struct ecsd *pecsd) {
}

ec_void_t ecsd_sync1(struct ecsd *pecsd) {
}

ec_void_t ecsd_pdo(struct ecsd *pecsd) {
}

ec_void_t ecsd_pdi(struct ecsd *pecsd) {
    pecsd->ready_for_op                = EC_TRUE;
}

