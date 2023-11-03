#include <assert.h>
#include <stddef.h>

#include <libesc/settings.h>
#include <libesc/esc.h>
#include <libesc/hw.h>

#define GEN_TRANSITION(from, to)            (ec_state_transition_t)((((osal_uint16_t)(from) << 8) & 0xFF00) | (((osal_uint16_t)to) & 0x00FF))

esc_retval_t esc_al_state_change_req(esc_t *pesc, osal_uint8_t al_state, osal_uint16_t al_status_code);

osal_bool_t esc_sm_settings_0_and_1_match(esc_t *pesc) {
    osal_bool_t ret = OSAL_TRUE;

    if (pesc->mbx_supported) {
        ec_slave_sm_t sm_mbx;

        // Check first Sync manager for write mailbox config.
        hw_read(pesc, ESC_REG_SYNC_MANAGER + (0 * ESC_REG_SYNC_MANAGER__BASE_OFFSET), (osal_uint8_t *)&sm_mbx, sizeof(sm_mbx));

        if ((sm_mbx.control & ESC_REG_SYNC_MANAGER__CONTROL__OP_MODE_MASK) != 
                ESC_REG_SYNC_MANAGER__CONTROL__OP_MODE_MAILBOX) {
            ret = OSAL_FALSE; // ERROR: Operation mode is not set to MAILBOX
        } else if ((sm_mbx.control & ESC_REG_SYNC_MANAGER__CONTROL__DIRECTION_MASK) != 
                ESC_REG_SYNC_MANAGER__CONTROL__DIRECTION_WRITE) {
            ret = OSAL_FALSE; // ERROR: Direction is not set to WRITE (ESC write <-> PDI read)
        } else if (!(sm_mbx.activate & ESC_REG_SYNC_MANAGER__ACTIVATE__ENABLE_SM)) {
            ret = OSAL_FALSE; // ERROR: Sync manager is not activated.
        } else if ((sm_mbx.address < ESC_MAILBOX_WRITE__MIN_ADDRESS) ||
                ((sm_mbx.address + sm_mbx.length) >= ESC_MAILBOX_WRITE__MAX_ADDRESS)) {
            ret = OSAL_FALSE; // ERROR: Sync manager address out of range
        } else if ((sm_mbx.length < ESC_MAILBOX_WRITE__MIN_LENGTH) ||
                (sm_mbx.length > ESC_MAILBOX_WRITE__MAX_LENGTH)) {
            ret = OSAL_FALSE; // ERROR: Sync manager to long 
        } else if ((sm_mbx.address & 0x1) || (sm_mbx.length & 0x1)) {
            ret = OSAL_FALSE; // ERROR: Sync manager address or length not at 16-bit boundaries!
        }

        if (ret == OSAL_TRUE) {
            // Check second Sync manager for read mailbox config.
            hw_read(pesc, ESC_REG_SYNC_MANAGER + (1 * ESC_REG_SYNC_MANAGER__BASE_OFFSET), (osal_uint8_t *)&sm_mbx, sizeof(sm_mbx));

            if ((sm_mbx.control & ESC_REG_SYNC_MANAGER__CONTROL__OP_MODE_MASK) != 
                    ESC_REG_SYNC_MANAGER__CONTROL__OP_MODE_MAILBOX) {
                ret = OSAL_FALSE; // ERROR: Operation mode is not set to MAILBOX
            } else if ((sm_mbx.control & ESC_REG_SYNC_MANAGER__CONTROL__DIRECTION_MASK) != 
                    ESC_REG_SYNC_MANAGER__CONTROL__DIRECTION_READ) {
                ret = OSAL_FALSE; // ERROR: Direction is not set to READ (ESC read <-> PDI write)
            } else if (!(sm_mbx.activate & ESC_REG_SYNC_MANAGER__ACTIVATE__ENABLE_SM)) {
                ret = OSAL_FALSE; // ERROR: Sync manager is not activated.
            } else if ((sm_mbx.address < ESC_MAILBOX_READ__MIN_ADDRESS) ||
                    ((sm_mbx.address + sm_mbx.length) >= ESC_MAILBOX_READ__MAX_ADDRESS)) {
                ret = OSAL_FALSE; // ERROR: Sync manager address out of range
            } else if ((sm_mbx.length < ESC_MAILBOX_READ__MIN_LENGTH) ||
                    (sm_mbx.length > ESC_MAILBOX_READ__MAX_LENGTH)) {
                ret = OSAL_FALSE; // ERROR: Sync manager to long 
            } else if ((sm_mbx.address & 0x1) || (sm_mbx.length & 0x1)) {
                ret = OSAL_FALSE; // ERROR: Sync manager address or length not at 16-bit boundaries!
            }
        }
    }

    return ret;
}

osal_bool_t esc_sm_settings_2_to_n_match(esc_t *pesc) {
    return OSAL_TRUE;
}

//! \brief Enable sync manager channel
/*!
 * \param[in]   pesc        Pointer to ESC struture.
 * \param[in]   ch          Sync manager channel number.
 */
esc_void_t esc_enable_sm_channel(esc_t *pesc, esc_uint8_t ch) {
    assert(pesc != NULL);

    esc_uint16_t sm_control_reg_address = ESC_REG_SYNC_MANAGER_N(ch) + ESC_REG_SYNC_MANAGER__PDI_CONTROL_OFFSET;

    esc_uint8_t ctrl = 0u;
    hw_write(pesc, sm_control_reg_address, &ctrl, sizeof(ctrl));

    do {
        hw_read(pesc, sm_control_reg_address, &ctrl, sizeof(ctrl));
    } while ((ctrl & ESC_REG_SYNC_MANAGER__PDI_CONTROL__DEACTIVATE) != 0u);
}

//! \brief Disable sync manager channel
/*!
 * \param[in]   pesc        Pointer to ESC struture.
 * \param[in]   ch          Sync manager channel number.
 */
esc_void_t esc_disable_sm_channel(esc_t *pesc, esc_uint8_t ch) {
    assert(pesc != NULL);

    esc_uint16_t sm_control_reg_address = ESC_REG_SYNC_MANAGER_N(ch) + ESC_REG_SYNC_MANAGER__PDI_CONTROL_OFFSET;

    esc_uint8_t ctrl = ESC_REG_SYNC_MANAGER__PDI_CONTROL__DEACTIVATE;
    hw_write(pesc, sm_control_reg_address, &ctrl, sizeof(ctrl));

    do {
        hw_read(pesc, sm_control_reg_address, &ctrl, sizeof(ctrl));
    } while ((ctrl & ESC_REG_SYNC_MANAGER__PDI_CONTROL__DEACTIVATE) == 0u);
}

//! \brief Start mailbox handling.
/*!
 * \param[in]   pesc        Pointer to ESC struture.
 */
esc_bool_t esc_start_mbx_handler(esc_t *pesc) {
    assert(pesc != NULL);
    esc_bool_t ret = ESC_TRUE;

    if (pesc->mbx_supported == ESC_TRUE) {
        // check SyncManager 0/1 for mailbox configuration.
        hw_read(pesc, ESC_REG_SYNC_MANAGER_0, (osal_uint8_t *)&pesc->mbx.sm_receive, sizeof(pesc->mbx.sm_receive));
        hw_read(pesc, ESC_REG_SYNC_MANAGER_1, (osal_uint8_t *)&pesc->mbx.sm_send, sizeof(pesc->mbx.sm_send));

        if ((pesc->mbx.sm_receive.address < 0x1000) || (pesc->mbx.sm_send.address < 0x1000)) {
            // ERROR: programmed address to small
            ret = ESC_FALSE;
        } else if ( 
                (   (pesc->mbx.sm_receive.address < pesc->mbx.sm_send.address) && 
                    ((pesc->mbx.sm_receive.address + pesc->mbx.sm_receive.length) > (pesc->mbx.sm_send.address + pesc->mbx.sm_send.length)) ) ||
                (   (pesc->mbx.sm_receive.address > pesc->mbx.sm_send.address) && 
                    ((pesc->mbx.sm_receive.address + pesc->mbx.sm_receive.length) < (pesc->mbx.sm_send.address + pesc->mbx.sm_send.length))) ) 
        {
            // ERROR: address ranges do overlap
            ret = ESC_FALSE;
        }

        if (ret == ESC_TRUE) {
            // All OK -> Enable receive and send sync manager.
            esc_enable_sm_channel(pesc, 0);
            esc_enable_sm_channel(pesc, 1);

            pesc->mbx_running = ESC_TRUE;
        }
    }

    return ret;
}

//! \brief Start mailbox handling.
/*!
 * \param[in]   pesc        Pointer to ESC struture.
 */
esc_bool_t esc_stop_mbx_handler(esc_t *pesc) {
    assert(pesc != NULL);
    esc_bool_t ret = ESC_TRUE;

    if (pesc->mbx_supported == ESC_TRUE) {
        esc_disable_sm_channel(pesc, 0);
        esc_disable_sm_channel(pesc, 1);
            
        pesc->mbx_running = ESC_FALSE;
    }

    return ret;
}

typedef enum esc_sm_direction {
    esc_sm_direction_read,      //!< \brief Read configuration  => ESC-read  <-> PDI-write
    esc_sm_direction_write      //!< \brief Write configuration => ESC-write <-> PDI-read
} esc_sm_direction_t;

typedef enum esc_sm_mode {
    esc_sm_mode_enable,         //!< \brief Enable Sync Manager.
    esc_sm_mode_disable         //!< \brief Disable Sync Manager.
} esc_sm_mode_t;

//! \brief Enable sync manager channels for PD In- or Outputs
/*!
 * \param[in]   pesc        Pointer to ESC struture.
 * \param[in]   sm_dir      Flag identifying sync managers direction.
 * \param[in]   sm_mode     Disable instead of enable.
 */
esc_void_t esc_configure_sm_channels_pd(esc_t *pesc, esc_sm_direction_t sm_dir, esc_sm_mode_t sm_mode) {
    assert(pesc != NULL);
    esc_uint8_t ch_start = 0u, ch;
    esc_uint8_t direction = sm_dir == esc_sm_direction_read ? 
        ESC_REG_SYNC_MANAGER__CONTROL__DIRECTION_READ :
        ESC_REG_SYNC_MANAGER__CONTROL__DIRECTION_WRITE;

    if (pesc->mbx_supported) {
        // skip mailbox sync managers
        ch_start = 2u;
    }

    for (ch = ch_start; ch < ESC_MAX_SYNC_MANAGER; ++ch) {
        ec_slave_sm_t sm_mbx;
        hw_read(pesc, ESC_REG_SYNC_MANAGER_N(ch), (osal_uint8_t *)&sm_mbx, sizeof(sm_mbx));
            
        if ((sm_mbx.control & ESC_REG_SYNC_MANAGER__CONTROL__DIRECTION_MASK) == direction) {
            if (sm_mode == esc_sm_mode_enable) {            
                esc_enable_sm_channel(pesc, ch);
            } else {
                esc_disable_sm_channel(pesc, ch);
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
 * \param[in]   pesc        Pointer to ESC struture.
 */
esc_retval_t esc_start_input_handler(esc_t *pesc) {
    return ESC_OK;
}

esc_retval_t esc_stop_input_handler(esc_t *pesc) {
    return ESC_OK;
}

esc_retval_t esc_start_local_output_handler(esc_t *pesc) {
    return ESC_OK;
}

esc_retval_t esc_stop_local_output_handler(esc_t *pesc) {
    return ESC_OK;
}

esc_retval_t esc_al_control_rsp(esc_t *pesc, osal_uint8_t al_state, osal_uint16_t al_status_code, osal_bool_t al_error_flag, osal_bool_t id_flag) {
    return ESC_OK;
}

osal_void_t esc_id_info(esc_t *pesc, osal_bool_t id_request) {
    // TODO
}

osal_bool_t esc_dc_activated(esc_t *pesc) {
    return OSAL_FALSE;
}

esc_retval_t esc_al_control_ind(esc_t *pesc, osal_uint8_t al_control_state, osal_bool_t ack_flag, osal_bool_t id_request) 
{
    esc_retval_t retval = ESC_OK;
    ec_state_transition_t transition = GEN_TRANSITION(pesc->al_state, al_control_state);        

    pesc->al_control_state = al_control_state;

    switch (pesc->al_state) 
    {
        case ESC_STATE_INIT: 
            switch (al_control_state) 
            {
                case ESC_STATE_INIT:
                    // #1.1 | #2
                    if ((pesc->al_error_flag == OSAL_TRUE) && (ack_flag == OSAL_FALSE)) {
                        // #1.1
                        pesc->al_status_code = 0u;
                        pesc->al_state = ESC_STATE_INIT;
                    } else { // al_error_flag == ESC_FALSE || ack_flag == OSAL_TRUE
                        // #2
                        pesc->al_error_flag = OSAL_FALSE;
                    }

                    esc_id_info(pesc, id_request);
                    esc_al_control_rsp(pesc, pesc->al_state, pesc->al_status_code, pesc->al_error_flag, pesc->id_flag);
                    break;
                case ESC_STATE_PREOP:
                    if ((pesc->al_error_flag == OSAL_FALSE) || (ack_flag == OSAL_TRUE)) {
                        // #3 | #4
                        if (esc_sm_settings_0_and_1_match(pesc) == OSAL_TRUE) {
                            // #3
                            pesc->al_error_flag = OSAL_FALSE;
                            pesc->al_status_code = 0;
                            pesc->al_state = ESC_STATE_PREOP;

                            (void)esc_start_mbx_handler(pesc);
                    
                            esc_id_info(pesc, id_request);
                        } else {
                            // #4
                            pesc->id_flag = OSAL_FALSE;
                            pesc->al_state = ESC_STATE_INIT;
                            pesc->al_status_code = ESC_AL_STATUS_CODE__INVALID_MAILBOX_CONFIGURATION__PREOP;
                            pesc->al_error_flag = OSAL_TRUE;
                        }
                    } else {
                        // #1.2
                    }

                    esc_al_control_rsp(pesc, pesc->al_state, pesc->al_status_code, pesc->al_error_flag, pesc->id_flag);
                    break;
                case ESC_STATE_BOOT:
                    // #5 | #6 | #7 | #1.2
                    if ((pesc->al_error_flag == OSAL_FALSE) || (ack_flag == OSAL_TRUE)) {
                        // #5 | #6 | #7
                        if (pesc->boot_supported == OSAL_TRUE) {
                            // #5 | #6
                            if (esc_sm_settings_0_and_1_match(pesc) == OSAL_TRUE) {
                                // #5
                                pesc->al_error_flag = OSAL_FALSE;
                                pesc->al_state = ESC_STATE_BOOT;
                                pesc->al_status_code = 0;

                                (void)esc_start_mbx_handler(pesc);
                            } else {
                                // #6
                                pesc->id_flag = OSAL_FALSE;
                                pesc->al_state = ESC_STATE_INIT;
                                pesc->al_status_code = ESC_AL_STATUS_CODE__INVALID_MAILBOX_CONFIGURATION__BOOT;
                                pesc->al_error_flag = OSAL_TRUE;
                            }
                        } else {
                            // #7
                            pesc->id_flag = OSAL_FALSE;
                            pesc->al_state = ESC_STATE_INIT;
                            pesc->al_status_code = ESC_AL_STATUS_CODE__BOOTSTRAP_NOT_SUPPORTED;
                            pesc->al_error_flag = OSAL_TRUE;
                        }
                    } else {
                        // #1.2
                    }

                    esc_al_control_rsp(pesc, pesc->al_state, pesc->al_status_code, pesc->al_error_flag, pesc->id_flag);
                    break;
                case ESC_STATE_SAFEOP:
                case ESC_STATE_OP:
                    if ((pesc->al_error_flag == OSAL_FALSE) || (ack_flag == OSAL_TRUE)) {
                        // #8
                        pesc->id_flag = OSAL_FALSE;
                        pesc->al_state = ESC_STATE_INIT;
                        pesc->al_status_code = ESC_AL_STATUS_CODE__INVALID_REQUESTED_STATE_CHANGE;
                        pesc->al_error_flag = OSAL_TRUE;
                    } else {
                        // #1.2
                    }
                        
                    esc_al_control_rsp(pesc, pesc->al_state, pesc->al_status_code, pesc->al_error_flag, pesc->id_flag);
                    break;
                default: 
                    // #9
                    pesc->id_flag = OSAL_FALSE;
                    pesc->al_state = ESC_STATE_INIT;
                    pesc->al_status_code = ESC_AL_STATUS_CODE__UNKNOWN_REQUESTED_STATE;
                    pesc->al_error_flag = OSAL_TRUE;

                    esc_al_control_rsp(pesc, pesc->al_state, pesc->al_status_code, pesc->al_error_flag, pesc->id_flag);
                    break;
            }
            break; // ESC_STATE_INIT
        case ESC_STATE_PREOP:
            switch (al_control_state) {
                case ESC_STATE_INIT:
                    // #11.1 | #12
                    if ((pesc->al_error_flag == OSAL_TRUE) && (ack_flag == OSAL_FALSE)) {
                        // #11.1
                        pesc->al_error_flag = OSAL_FALSE;
                        pesc->al_status_code = 0;
                        pesc->al_state = ESC_STATE_INIT;

                        (void)esc_stop_mbx_handler(pesc);

                        esc_id_info(pesc, id_request);
                    } else {
                        // #12
                        pesc->al_error_flag = OSAL_FALSE;
                        pesc->al_status_code = 0;
                        pesc->al_state = ESC_STATE_INIT;

                        (void)esc_stop_mbx_handler(pesc);
                        
                        esc_id_info(pesc, id_request);
                    }

                    esc_al_control_rsp(pesc, pesc->al_state, pesc->al_status_code, pesc->al_error_flag, pesc->id_flag);
                    break;
                case ESC_STATE_PREOP:
                    if ((pesc->al_error_flag == OSAL_FALSE) || (ack_flag == OSAL_TRUE)) {
                        // #13
                        pesc->al_error_flag = OSAL_FALSE;
                        pesc->al_status_code = 0;

                        esc_id_info(pesc, id_request);
                    } else {
                        // #11.2
                    }
                        
                    esc_al_control_rsp(pesc, pesc->al_state, pesc->al_status_code, pesc->al_error_flag, pesc->id_flag);
                    break;
                case ESC_STATE_SAFEOP:
                    if ((pesc->al_error_flag == OSAL_FALSE) || (ack_flag == OSAL_TRUE)) {
                        // #14.1 | #14.2 | #14.3 | #17
                        if (esc_sm_settings_2_to_n_match(pesc) == OSAL_TRUE) {
                            // #14.1 | #14.2 | #14.3
                            if (    ((esc_dc_activated(pesc) == OSAL_FALSE) && (pesc->dc_required == OSAL_FALSE)) ||
                                    ((esc_dc_activated(pesc) == OSAL_TRUE) && (pesc->dc_not_accepted == OSAL_FALSE) && (pesc->dc_supported == OSAL_FALSE)) ) {
                                // #14.1
                                pesc->dc_running = OSAL_FALSE;
                                pesc->al_error_flag = OSAL_FALSE;
                                pesc->al_status_code = 0;
                                pesc->al_state = ESC_STATE_SAFEOP;

                                (void)esc_start_input_handler(pesc);
                                esc_id_info(pesc, id_request);
                            } else if ((esc_dc_activated(pesc) == OSAL_TRUE) && (pesc->dc_not_accepted == OSAL_FALSE) && (pesc->dc_supported == OSAL_TRUE)) {
                                // #14.2
                                pesc->dc_running = OSAL_TRUE;
                                pesc->al_error_flag = OSAL_FALSE;
                                pesc->al_status_code = 0;
                                pesc->al_state = ESC_STATE_SAFEOP;

                                (void)esc_start_input_handler(pesc);
                                esc_id_info(pesc, id_request);
                            } else if ( ((esc_dc_activated(pesc) == OSAL_TRUE) && (pesc->dc_not_accepted == OSAL_TRUE)) ||
                                    ((esc_dc_activated(pesc) == OSAL_FALSE) && (pesc->dc_required == OSAL_TRUE)) ) {
                                // #14.3
                                pesc->id_flag = OSAL_FALSE;
                                pesc->al_status_code = ESC_AL_STATUS_CODE__INVALID_DC_SYNC_CONFIGURATION;
                                pesc->al_error_flag = OSAL_TRUE;
                                pesc->al_state = ESC_STATE_PREOP;
                            }
                        } else {
                            // #17
                            pesc->id_flag = OSAL_FALSE;
                            pesc->al_state = ESC_STATE_PREOP;
                            pesc->al_status_code = ESC_AL_STATUS_CODE__INVALID_SYNC_MANAGER_CONFIGURATION;
                            pesc->al_error_flag = OSAL_TRUE;
                        }
                    } else {
                        // #11.2
                    }

                    esc_al_control_rsp(pesc, pesc->al_state, pesc->al_status_code, pesc->al_error_flag, pesc->id_flag);
                    break;
                case ESC_STATE_OP:
                case ESC_STATE_BOOT:
                    // #18
                    pesc->id_flag = OSAL_FALSE;
                    pesc->al_state = ESC_STATE_PREOP;
                    pesc->al_status_code = ESC_AL_STATUS_CODE__INVALID_REQUESTED_STATE_CHANGE;
                    pesc->al_error_flag = OSAL_TRUE;

                    esc_al_control_rsp(pesc, pesc->al_state, pesc->al_status_code, pesc->al_error_flag, pesc->id_flag);
                    break;
                default:
                    // #19
                    pesc->id_flag = OSAL_FALSE;
                    pesc->al_state = ESC_STATE_PREOP;
                    pesc->al_status_code = ESC_AL_STATUS_CODE__UNKNOWN_REQUESTED_STATE;
                    pesc->al_error_flag = OSAL_TRUE;

                    esc_al_control_rsp(pesc, pesc->al_state, pesc->al_status_code, pesc->al_error_flag, pesc->id_flag);
                    break;
            }
            break; // ESC_STATE_PREOP
        case ESC_STATE_SAFEOP:
            switch (al_control_state) {
                case ESC_STATE_INIT:
                    // #22.1 | #23
                    if ((pesc->al_error_flag == OSAL_TRUE) && (ack_flag == OSAL_FALSE)) {
                        // #22.1
                        pesc->al_error_flag = OSAL_FALSE;
                        pesc->al_status_code = 0u;
                        pesc->al_state = ESC_STATE_INIT;

                        esc_stop_mbx_handler(pesc);
                        esc_stop_input_handler(pesc);
                        esc_stop_local_output_handler(pesc);

                        esc_id_info(pesc, id_request);
                    } else {
                        // #23
                        pesc->al_error_flag = OSAL_FALSE;
                        pesc->al_state = ESC_STATE_INIT;

                        esc_stop_mbx_handler(pesc);
                        esc_stop_input_handler(pesc);

                        esc_id_info(pesc, id_request);
                    }

                    esc_al_control_rsp(pesc, pesc->al_state, pesc->al_status_code, pesc->al_error_flag, pesc->id_flag);
                    break;
                case ESC_STATE_PREOP:
                    // #24
                    if ((pesc->al_error_flag == OSAL_FALSE) || (ack_flag == OSAL_TRUE)) {
                        pesc->al_error_flag = OSAL_FALSE;
                        pesc->al_state = ESC_STATE_PREOP;
                    
                        esc_stop_input_handler(pesc);

                        esc_id_info(pesc, id_request);
                        esc_al_control_rsp(pesc, pesc->al_state, pesc->al_status_code, pesc->al_error_flag, pesc->id_flag);
                    }
                    break;
                case ESC_STATE_SAFEOP:
                    // #25.1 | #25.2 
                    if ((pesc->al_error_flag == OSAL_TRUE) && ((pesc->local_error_flag == OSAL_FALSE) && (ack_flag == OSAL_TRUE))) {
                        // #25.1
                        pesc->al_error_flag = OSAL_FALSE;
                        pesc->al_status_code = 0u;

                        esc_configure_sm_channels_pd(pesc, esc_sm_direction_read, esc_sm_mode_enable);

                        esc_id_info(pesc, id_request);
                    } else if ((pesc->al_error_flag == OSAL_FALSE) || ((pesc->local_error_flag == OSAL_TRUE) && (ack_flag == OSAL_TRUE))) {
                        // #25.2
                        pesc->al_error_flag = OSAL_FALSE;
                        pesc->al_status_code = 0u;

                        esc_id_info(pesc, id_request);
                    }

                    esc_al_control_rsp(pesc, pesc->al_state, pesc->al_status_code, pesc->al_error_flag, pesc->id_flag);
                    break;
                case ESC_STATE_OP:
                    // # 26.2 | #26.4 | #27.1 | #27.2 | #22.2
                    if ((pesc->al_error_flag == OSAL_FALSE) || (ack_flag == OSAL_TRUE)) { 
                        if (    (pesc->ready_for_op == OSAL_TRUE) && 
                                ((pesc->dc_running == OSAL_FALSE) || (pesc->pll_running == OSAL_TRUE)) && 
                                (pesc->local_error_flag == OSAL_FALSE)) 
                        {
                            // #26.2
                            pesc->al_error_flag = OSAL_FALSE;
                            pesc->al_status_code = 0u;
                            pesc->al_state = ESC_STATE_OP;

                            esc_configure_sm_channels_pd(pesc, esc_sm_direction_read, esc_sm_mode_enable);
                            esc_configure_sm_channels_pd(pesc, esc_sm_direction_write, esc_sm_mode_enable);
                            esc_start_local_output_handler(pesc);

                            esc_id_info(pesc, id_request);
                        } else if ((pesc->dc_running == OSAL_TRUE) && (pesc->pll_running == OSAL_FALSE) && (pesc->local_error_flag == OSAL_FALSE)) {
                            // #26.4
                            esc_configure_sm_channels_pd(pesc, esc_sm_direction_write, esc_sm_mode_enable);

                            pesc->wait_for_pll_running = OSAL_TRUE;

                            esc_start_safeop_2_op_timeout_timer(pesc);

                            esc_id_info(pesc, id_request);
                        } else if ( (pesc->ready_for_op == OSAL_FALSE) && 
                                    ((pesc->dc_running == OSAL_FALSE) || (pesc->pll_running == OSAL_TRUE)) &&
                                    (pesc->local_error_flag == OSAL_FALSE) ) {
                            // #27.1
                            pesc->id_flag = OSAL_FALSE;
                            pesc->al_state = ESC_STATE_SAFEOP;
                            pesc->al_status_code = ESC_AL_STATUS_CODE__SYNC_MANAGER_WATCHDOG;
                            pesc->al_error_flag = OSAL_TRUE;
                        } else if (pesc->local_error_flag == OSAL_TRUE) {
                            // #27.2
                            pesc->id_flag = OSAL_FALSE;
                            pesc->al_state = ESC_STATE_SAFEOP;
                            pesc->al_status_code = pesc->local_error_code;
                            pesc->al_error_flag = OSAL_TRUE;
                        }
                    } else {
                        // #22.2
                    }
                        
                    esc_al_control_rsp(pesc, pesc->al_state, pesc->al_status_code, pesc->al_error_flag, pesc->id_flag);
                    break;
                case ESC_STATE_BOOT:
                    // #29 | #22.2
                    if ((pesc->al_error_flag == OSAL_FALSE) || (ack_flag == OSAL_TRUE)) { 
                        // #29
                        pesc->id_flag = OSAL_FALSE;
                        pesc->al_state = ESC_STATE_SAFEOP;
                        pesc->al_status_code = ESC_AL_STATUS_CODE__INVALID_REQUESTED_STATE_CHANGE;
                        pesc->al_error_flag = OSAL_TRUE;
                    } else {
                        // #22.2
                    }
                        
                    esc_al_control_rsp(pesc, pesc->al_state, pesc->al_status_code, pesc->al_error_flag, pesc->id_flag);
                    break;
                default:
                    // #30
                    pesc->id_flag = OSAL_FALSE;
                    pesc->al_state = ESC_STATE_SAFEOP;
                    pesc->al_status_code = ESC_AL_STATUS_CODE__UNKNOWN_REQUESTED_STATE;
                    pesc->al_error_flag = OSAL_TRUE;

                    esc_al_control_rsp(pesc, pesc->al_state, pesc->al_status_code, pesc->al_error_flag, pesc->id_flag);
                    break;
            }
            break;
        case ESC_STATE_OP:
            switch (al_control_state) {
                case ESC_STATE_INIT: 
                    // #37
                    pesc->al_status_code = 0u;
                    pesc->al_error_flag = OSAL_FALSE;
                    pesc->al_state = ESC_STATE_INIT;

                    esc_stop_mbx_handler(pesc);
                    esc_stop_input_handler(pesc);
                    esc_stop_local_output_handler(pesc);

                    esc_id_info(pesc, id_request);
                    break;
                case ESC_STATE_PREOP:
                    // #38
                    pesc->al_error_flag = OSAL_FALSE;
                    pesc->al_status_code = 0u;
                    pesc->al_state = ESC_STATE_PREOP;

                    esc_stop_input_handler(pesc);
                    esc_stop_local_output_handler(pesc);

                    esc_id_info(pesc, id_request);
                    break;
                case ESC_STATE_SAFEOP:
                    // #39
                    pesc->al_error_flag = OSAL_FALSE;
                    pesc->al_status_code = 0u;
                    pesc->al_state = ESC_STATE_SAFEOP;

                    esc_stop_local_output_handler(pesc);

                    esc_id_info(pesc, id_request);
                    break;
                case ESC_STATE_OP:
                    // #40
                    esc_id_info(pesc, id_request);
                    break;
                case ESC_STATE_BOOT:
                    // #42
                    pesc->id_flag = OSAL_FALSE;
                    pesc->al_state = ESC_STATE_SAFEOP;
                    pesc->al_status_code = ESC_AL_STATUS_CODE__INVALID_REQUESTED_STATE_CHANGE;
                    pesc->al_error_flag = OSAL_TRUE;

                    esc_configure_sm_channels_pd(pesc, esc_sm_direction_read, esc_sm_mode_disable);
                    esc_configure_sm_channels_pd(pesc, esc_sm_direction_write, esc_sm_mode_disable);
                    esc_stop_local_output_handler(pesc);
                    break;
                default:
                    // #43
                    pesc->id_flag = OSAL_FALSE;
                    pesc->al_state = ESC_STATE_SAFEOP;
                    pesc->al_status_code = ESC_AL_STATUS_CODE__UNKNOWN_REQUESTED_STATE;
                    pesc->al_error_flag = OSAL_TRUE;

                    esc_configure_sm_channels_pd(pesc, esc_sm_direction_write, esc_sm_mode_disable);
                    esc_stop_local_output_handler(pesc);
                    break;
            }
                    
            esc_al_control_rsp(pesc, pesc->al_state, pesc->al_status_code, pesc->al_error_flag, pesc->id_flag);
            break;
        case ESC_STATE_BOOT:
            switch (al_control_state) {
                case ESC_STATE_INIT:
                    // #51
                    pesc->al_error_flag = OSAL_FALSE;
                    pesc->al_state = ESC_STATE_INIT;

                    esc_stop_mbx_handler(pesc);
                    break;
                case ESC_STATE_BOOT:
                    // #52
                    pesc->al_error_flag = OSAL_FALSE;
                case ESC_STATE_PREOP:
                case ESC_STATE_SAFEOP:
                case ESC_STATE_OP:
                    break;
                default:
                    break;
            }
            esc_al_control_rsp(pesc, pesc->al_state, pesc->al_status_code, pesc->al_error_flag, pesc->id_flag);
            break;
    }

    return retval;
}

//! \brief Sync manager change event occured
/*!
 * \param[in]   pesc        Pointer to ESC struture.
 */
esc_void_t esc_sm_chg(esc_t *pesc) {
    switch (pesc->al_state) {
        case ESC_STATE_INIT:
            // #10
            break;
        case ESC_STATE_PREOP:
            // #20.1 | #20.2 | #21
            if ((pesc->al_error_flag == OSAL_FALSE) && (esc_sm_settings_0_and_1_match(pesc) == OSAL_TRUE)) {
                // #20.1
                esc_ack_sm_chg(pesc);
            } else if (pesc->al_error_flag == OSAL_TRUE) {
                // #20.2
            } else {
                // #21
                pesc->id_flag = OSAL_FALSE;
                pesc->al_status_code = ESC_AL_STATUS_CODE__INVALID_MAILBOX_CONFIGURATION__PREOP;
                pesc->al_state = ESC_STATE_INIT;

                esc_stop_mbx_handler(pesc);
                esc_ack_sm_chg(pesc);
                esc_al_state_change_req(pesc, pesc->al_state, pesc->al_status_code);
            }
            break;
        case ESC_STATE_SAFEOP:
            // #31.1 | #31.2 | #32 | #33
            if (pesc->al_error_flag == OSAL_TRUE) {
                // #31.2
            } else {
                // #31.1 | #32 | #33
                if ((esc_sm_settings_0_and_1_match(pesc) == OSAL_TRUE) && (esc_sm_settings_2_to_n_match(pesc) == OSAL_TRUE)) {
                    // #31.1
                    esc_ack_sm_chg(pesc);
                } else if ((esc_sm_settings_0_and_1_match(pesc) == OSAL_TRUE) && (esc_sm_settings_2_to_n_match(pesc) == OSAL_FALSE)) {
                    // #32
                    pesc->id_flag = OSAL_FALSE;
                    pesc->al_status_code = ESC_AL_STATUS_CODE__INVALID_SYNC_MANAGER_CONFIGURATION;
                    pesc->al_error_flag = OSAL_TRUE;
                    pesc->al_state = ESC_STATE_PREOP;

                    esc_stop_input_handler(pesc);
                    esc_ack_sm_chg(pesc);
                    esc_al_state_change_req(pesc, pesc->al_state, pesc->al_status_code);
                } else if (esc_sm_settings_0_and_1_match(pesc) == OSAL_FALSE) {
                    // #33
                    pesc->id_flag = OSAL_FALSE;
                    pesc->al_status_code = ESC_AL_STATUS_CODE__INVALID_MAILBOX_CONFIGURATION__PREOP;
                    pesc->al_error_flag = OSAL_TRUE;
                    pesc->al_state = ESC_STATE_INIT;

                    esc_stop_input_handler(pesc);
                    esc_stop_mbx_handler(pesc);
                    esc_ack_sm_chg(pesc);
                    esc_al_state_change_req(pesc, pesc->al_state, pesc->al_status_code);
                }
            }
            break;
        case ESC_STATE_OP:
            // #44 | #45 | #46 
            if (pesc->al_error_flag == OSAL_FALSE) {
                osal_bool_t sm_0_and_1_match = esc_sm_settings_0_and_1_match(pesc);
                osal_bool_t sm_2_to_n_match = esc_sm_settings_2_to_n_match(pesc);

                if ((sm_0_and_1_match == OSAL_TRUE) && (sm_2_to_n_match == OSAL_TRUE)) {
                    // #44
                    esc_ack_sm_chg(pesc);
                } else if ((sm_0_and_1_match == OSAL_TRUE) && (sm_2_to_n_match == OSAL_FALSE)) {
                    // #45
                    pesc->id_flag = OSAL_FALSE;
                    pesc->al_status_code = ESC_AL_STATUS_CODE__INVALID_SYNC_MANAGER_CONFIGURATION;
                    pesc->al_error_flag = OSAL_TRUE;
                    pesc->al_state = ESC_STATE_PREOP;

                    esc_stop_local_output_handler(pesc);
                    esc_stop_input_handler(pesc);
                    esc_ack_sm_chg(pesc);
                    esc_al_state_change_req(pesc, pesc->al_state, pesc->al_status_code);
                } else {
                    // #46
                    pesc->id_flag = OSAL_FALSE;
                    pesc->al_status_code = ESC_AL_STATUS_CODE__INVALID_MAILBOX_CONFIGURATION__PREOP;
                    pesc->al_error_flag = OSAL_TRUE;
                    pesc->al_state = ESC_STATE_INIT;

                    esc_stop_local_output_handler(pesc);
                    esc_stop_input_handler(pesc);
                    esc_stop_mbx_handler(pesc);
                    esc_ack_sm_chg(pesc);
                    esc_al_state_change_req(pesc, pesc->al_state, pesc->al_status_code);
                }
            }
            break;
        case ESC_STATE_BOOT:
            // #55 | #56.1 | #56.2
            if (esc_sm_settings_0_and_1_match(pesc) == OSAL_TRUE) {
                // #55
            } else {
                // #56.1 | #56.2
                pesc->al_status_code = ESC_AL_STATUS_CODE__INVALID_MAILBOX_CONFIGURATION__PREOP;
                pesc->al_error_flag = OSAL_TRUE;
                pesc->al_state = ESC_STATE_INIT;
                
                esc_stop_mbx_handler(pesc);
                esc_al_state_change_req(pesc, pesc->al_state, pesc->al_status_code);
            }
            break;
        default:
            break;
    }
}

esc_retval_t esc_al_state_change_req(esc_t *pesc, osal_uint8_t al_state, osal_uint16_t al_status_code) {
    switch (pesc->al_state) {
        case ESC_STATE_SAFEOP:
            // #34.1 | #34.2 | #34.3 | #34.4 | #34.5
            if ((al_state == ESC_STATE_SAFEOP) && (pesc->al_error_flag == OSAL_TRUE)) {
                // #34.1
                pesc->id_flag = OSAL_FALSE;
                esc_configure_sm_channels_pd(pesc, esc_sm_direction_write, esc_sm_mode_disable);
                pesc->al_state = ESC_STATE_SAFEOP;
                pesc->al_error_flag = OSAL_TRUE;
                pesc->local_error_flag = OSAL_TRUE;
                pesc->local_error_code = al_status_code;

                // TODO this is an endless recursion:
                //esc_al_state_change_req(pesc, pesc->al_state, pesc->al_status_code);
            } else if ((al_state == ESC_STATE_SAFEOP) && (pesc->al_error_flag == OSAL_FALSE)) {
                // #34.2
                esc_configure_sm_channels_pd(pesc, esc_sm_direction_read, esc_sm_mode_enable);
                pesc->local_error_flag = OSAL_FALSE;
                pesc->local_error_code = 0u;
            } else if ((al_state == ESC_STATE_PREOP) && (pesc->al_error_flag == OSAL_TRUE)) {
                // #34.3
                pesc->id_flag = OSAL_FALSE;
                esc_stop_input_handler(pesc);
                pesc->al_state = ESC_STATE_PREOP;
                pesc->al_error_flag = OSAL_TRUE;

                esc_al_state_change_req(pesc, pesc->al_state, pesc->al_status_code);
            } else if ((al_state == ESC_STATE_INIT) && (pesc->al_error_flag == OSAL_TRUE)) {
                // #34.4
                pesc->id_flag = OSAL_FALSE;
                esc_stop_mbx_handler(pesc);
                esc_stop_input_handler(pesc);
                pesc->al_state = ESC_STATE_INIT;
                pesc->al_error_flag = OSAL_TRUE;

                esc_al_state_change_req(pesc, pesc->al_state, pesc->al_status_code);
            } else if (((al_state == ESC_STATE_INIT) || (al_state == ESC_STATE_PREOP) && (pesc->al_error_flag == OSAL_FALSE)) ||
                    (   (pesc->al_control_state != ESC_STATE_INIT) && 
                        (pesc->al_control_state != ESC_STATE_PREOP) && 
                        (pesc->al_control_state != ESC_STATE_SAFEOP) ))
            {
                // #34.5
            }
            break;
        case ESC_STATE_OP:
            // #47.1 | #47.2 | #47.3 | #47.4
            if ((al_state == ESC_STATE_SAFEOP) && (pesc->al_error_flag == OSAL_TRUE)) {
                // #47.1
                esc_stop_local_output_handler(pesc);
                esc_configure_sm_channels_pd(pesc, esc_sm_direction_write, esc_sm_mode_disable);
                pesc->al_state = ESC_STATE_SAFEOP;
                pesc->local_error_flag = OSAL_TRUE;
                pesc->local_error_code = al_status_code;

                esc_al_state_change_req(pesc, pesc->al_state, pesc->al_status_code);
            } else if ((al_state == ESC_STATE_PREOP) && (pesc->al_error_flag == OSAL_TRUE)) {
                // #47.2
                pesc->id_flag = OSAL_FALSE;
                esc_stop_input_handler(pesc);
                esc_stop_local_output_handler(pesc);
                pesc->al_state = ESC_STATE_PREOP;
                pesc->local_error_flag = OSAL_TRUE;
                pesc->local_error_code = al_status_code;

                esc_al_state_change_req(pesc, pesc->al_state, pesc->al_status_code);
            } else if ((al_state == ESC_STATE_INIT) && (pesc->al_error_flag == OSAL_TRUE)) {
                // #47.3
                pesc->id_flag = OSAL_FALSE;
                esc_stop_mbx_handler(pesc);
                esc_stop_input_handler(pesc);
                esc_stop_local_output_handler(pesc);
                pesc->al_state = ESC_STATE_INIT;
                pesc->local_error_flag = OSAL_TRUE;
                pesc->local_error_code = al_status_code;

                esc_al_state_change_req(pesc, pesc->al_state, pesc->al_status_code);
            } else if ((((al_state == ESC_STATE_SAFEOP) || (al_state == ESC_STATE_PREOP) || (al_state == ESC_STATE_INIT)) && (pesc->al_error_flag == OSAL_FALSE)) ||
                    (   (pesc->al_control_state != ESC_STATE_INIT) && 
                        (pesc->al_control_state != ESC_STATE_PREOP) && 
                        (pesc->al_control_state != ESC_STATE_SAFEOP) && 
                        (pesc->al_control_state != ESC_STATE_OP))) 
            {
                // #47.4
            }
            break;
        case ESC_STATE_BOOT:
            // #57 | #58
            if ((al_state == ESC_STATE_INIT) && (pesc->al_error_flag == OSAL_TRUE)) {
                // #57
                esc_stop_mbx_handler(pesc);
                pesc->al_state = ESC_STATE_INIT;
                pesc->al_error_flag = OSAL_TRUE;

                esc_al_state_change_req(pesc, pesc->al_state, pesc->al_status_code);
            } else if (pesc->al_error_flag == OSAL_FALSE) {
                // #58
            }
            break;
        default:
            break;
    }

    return ESC_OK;
}

