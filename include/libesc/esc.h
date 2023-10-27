/**
 * \file esc.h
 *
 * \author Robert Burger <robert@burger-system.de>
 *
 * \date 25 Oct 2023
 *
 * \brief ESC main header.
 *
 * ESC main header.
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

#ifndef LIBESC_ESC_H
#define LIBESC_ESC_H

#include <stdint.h>
#include <libesc/types.h>

//-------------------------------------
// SII registers.
//

#define ESC_REG_SII_EEPROM                                  ((osal_uint16_t)0x0500u)

#define ESC_REG_SII_EEPROM__CONFIGURATION                   ((osal_uint16_t)0x0500u)
#define ESC_REG_SII_EEPROM__CONFIGURATION__PDI_CONTROL      ((osal_uint8_t)0x01u)
#define ESC_REG_SII_EEPROM__CONFIGURATION__RES0             ((osal_uint8_t)0xFEu)

#define ESC_REG_SII_EEPROM__PDI_ACCESS                      ((osal_uint16_t)0x0501u)
#define ESC_REG_SII_EEPROM__PDI_ACCESS__PDI_ACCESS          ((osal_uint8_t)0x01u)
#define ESC_REG_SII_EEPROM__PDI_ACCESS__RES0                ((osal_uint8_t)0xFEu)

#define ESC_REG_SII_EEPROM__CTRLSTAT                        ((osal_uint16_t)0x0502u)
#define ESC_REG_SII_EEPROM__CTRLSTAT__ECAT_WRITE_ENABLE     ((osal_uint16_t)0x0001u)
#define ESC_REG_SII_EEPROM__CTRLSTAT__RES0                  ((osal_uint16_t)0x001Eu)
#define ESC_REG_SII_EEPROM__CTRLSTAT__PDI_EMULATION         ((osal_uint16_t)0x0020u)
#define ESC_REG_SII_EEPROM__CTRLSTAT                        ((osal_uint16_t)0x0502u)
#define ESC_REG_SII_EEPROM__CTRLSTAT                        ((osal_uint16_t)0x0502u)


//-------------------------------------
// FMMU registers.
//

#define ESC_REG_FMMU__LOG_START_ADDRESS                     ((osal_uint16_t)0x0600u)
#define ESC_REG_FMMU__LENGTH                                ((osal_uint16_t)0x0604u)
#define ESC_REG_FMMU__LOG_START_BIT                         ((osal_uint16_t)0x0606u)
#define ESC_REG_FMMU__LOG_STOP_BIT                          ((osal_uint16_t)0x0607u)
#define ESC_REG_FMMU__PHYS_START_ADDRESS                    ((osal_uint16_t)0x0608u)
#define ESC_REG_FMMU__PHYS_START_BIT                        ((osal_uint16_t)0x060Au)
#define ESC_REG_FMMU__TYPE                                  ((osal_uint16_t)0x060Bu)
#define ESC_REG_FMMU__ACTIVATE                              ((osal_uint16_t)0x060Cu)
#define ESC_REG_FMMU__RESERVERD                             ((osal_uint16_t)0x060Du)

#define ESC_REG_FMMU__BASE_OFFSET                           ((osal_uint8_t)0x10u)

//-------------------------------------
// Sync Manager registers.
//

#define ESC_REG_SYNC_MANAGER                                ((osal_uint16_t)0x0800u)
#define ESC_REG_SYNC_MANAGER__BASE_OFFSET                   ((osal_uint8_t)0x08u)
#define ESC_REG_SYNC_MANAGER_N(ch)                          (ESC_REG_SYNC_MANAGER + ((ch) * ESC_REG_SYNC_MANAGER__BASE_OFFSET))
#define ESC_REG_SYNC_MANAGER_0                              (ESC_REG_SYNC_MANAGER_N(0))
#define ESC_REG_SYNC_MANAGER_1                              (ESC_REG_SYNC_MANAGER_N(1))
#define ESC_REG_SYNC_MANAGER_2                              (ESC_REG_SYNC_MANAGER_N(2))
#define ESC_REG_SYNC_MANAGER_3                              (ESC_REG_SYNC_MANAGER_N(3))
#define ESC_REG_SYNC_MANAGER_4                              (ESC_REG_SYNC_MANAGER_N(4))
#define ESC_REG_SYNC_MANAGER_5                              (ESC_REG_SYNC_MANAGER_N(5))
#define ESC_REG_SYNC_MANAGER_6                              (ESC_REG_SYNC_MANAGER_N(6))
#define ESC_REG_SYNC_MANAGER_7                              (ESC_REG_SYNC_MANAGER_N(7))

#define ESC_REG_SYNC_MANAGER__CONTROL__OP_MODE_MASK         ((osal_uint8_t)0x03u)
#define ESC_REG_SYNC_MANAGER__CONTROL__OP_MODE_BUFFERED     ((osal_uint8_t)0x00u)
#define ESC_REG_SYNC_MANAGER__CONTROL__OP_MODE_RES0         ((osal_uint8_t)0x01u)
#define ESC_REG_SYNC_MANAGER__CONTROL__OP_MODE_MAILBOX      ((osal_uint8_t)0x02u)
#define ESC_REG_SYNC_MANAGER__CONTROL__OP_MODE_RES1         ((osal_uint8_t)0x03u)

#define ESC_REG_SYNC_MANAGER__CONTROL__DIRECTION_MASK       ((osal_uint8_t)0x0Cu)
#define ESC_REG_SYNC_MANAGER__CONTROL__DIRECTION_READ       ((osal_uint8_t)0x00u)
#define ESC_REG_SYNC_MANAGER__CONTROL__DIRECTION_WRITE      ((osal_uint8_t)0x04u)
#define ESC_REG_SYNC_MANAGER__CONTROL__DIRECTION_RES0       ((osal_uint8_t)0x08u)
#define ESC_REG_SYNC_MANAGER__CONTROL__DIRECTION_RES1       ((osal_uint8_t)0x0Cu)

#define ESC_REG_SYNC_MANAGER__CONTROL__IRQ_ECAT_EVENT       ((osal_uint8_t)0x10u)
#define ESC_REG_SYNC_MANAGER__CONTROL__IRQ_AL_EVENT         ((osal_uint8_t)0x20u)
#define ESC_REG_SYNC_MANAGER__CONTROL__WATCHDOG_TRIGGER     ((osal_uint8_t)0x40u)
#define ESC_REG_SYNC_MANAGER__CONTROL__RES0                 ((osal_uint8_t)0x80u)

#define ESC_REG_SYNC_MANAGER__STATUS__IRQ_WRITE             ((osal_uint8_t)0x01u)
#define ESC_REG_SYNC_MANAGER__STATUS__IRQ_READ              ((osal_uint8_t)0x02u)
#define ESC_REG_SYNC_MANAGER__STATUS__IRQ_RES0              ((osal_uint8_t)0x04u)
#define ESC_REG_SYNC_MANAGER__STATUS__MAILBOX_FULL          ((osal_uint8_t)0x08u)

#define ESC_REG_SYNC_MANAGER__STATUS__BUFFER_MASK           ((osal_uint8_t)0x30u)
#define ESC_REG_SYNC_MANAGER__STATUS__BUFFER_1              ((osal_uint8_t)0x00u)
#define ESC_REG_SYNC_MANAGER__STATUS__BUFFER_2              ((osal_uint8_t)0x10u)
#define ESC_REG_SYNC_MANAGER__STATUS__BUFFER_3              ((osal_uint8_t)0x20u)
#define ESC_REG_SYNC_MANAGER__STATUS__BUFFER_NONE           ((osal_uint8_t)0x30u)

#define ESC_REG_SYNC_MANAGER__STATUS__READ_BUFFER_OPEN      ((osal_uint8_t)0x40u)
#define ESC_REG_SYNC_MANAGER__STATUS__WRITE_BUFFER_OPEN     ((osal_uint8_t)0x80u)

#define ESC_REG_SYNC_MANAGER__ACTIVATE__ENABLE_SM           ((osal_uint8_t)0x01u)
#define ESC_REG_SYNC_MANAGER__ACTIVATE__REPEAT_REQUEST      ((osal_uint8_t)0x02u)
#define ESC_REG_SYNC_MANAGER__ACTIVATE__RES0                ((osal_uint8_t)0x3Cu)
#define ESC_REG_SYNC_MANAGER__ACTIVATE__LATCH_ECAT          ((osal_uint8_t)0x40u)
#define ESC_REG_SYNC_MANAGER__ACTIVATE__LATCH_PDI           ((osal_uint8_t)0x80u)

#define ESC_REG_SYNC_MANAGER__PDI_CONTROL_OFFSET            ((osal_uint8_t)0x07u)
#define ESC_REG_SYNC_MANAGER__PDI_CONTROL__DEACTIVATE       ((osal_uint8_t)0x01u)
#define ESC_REG_SYNC_MAANGER__PDI_CONTROL__REPEAT_ACK       ((osal_uint8_t)0x02u)
#define ESC_REG_SYNC_MANAGER__PDI_CONTROL__RES0             ((osal_uint8_t)0xFCu)

//! \brief Sync Manager Register structure.
typedef struct PACKED ec_slave_sm {
    osal_uint16_t   address;        //!< \brief Sync Manager start address.
                                    /*!<
                                     * Specifies ESC memory address where the
                                     * Sync Manager data starts.
                                     */
    osal_uint16_t   length;         //!< \brief Sync Manager length.
                                    /*!<
                                     * Specifies the length of the ESC
                                     * memory mapped by Sync Manager.
                                     */

    osal_uint8_t    control;        //!< \brief Control register.
    osal_uint8_t    status;         //!< \brief Status register.
    osal_uint8_t    activate;       //!< \brief Activate register.
    osal_uint8_t    pdi_control;    //!< \brief PDI Control register.
} ec_slave_sm_t;


//-------------------------------------
// Distributed Clocks registers.
//

#define ESC_REG_DC_RECEIVE_TIME_0                           ((osal_uint16_t)0x0900u)
#define ESC_REG_DC_RECEIVE_TIME_1                           ((osal_uint16_t)0x0904u)
#define ESC_REG_DC_RECEIVE_TIME_2                           ((osal_uint16_t)0x0908u)
#define ESC_REG_DC_RECEIVE_TIME_3                           ((osal_uint16_t)0x090Cu)
#define ESC_REG_DC_SYSTEM_TIME                              ((osal_uint16_t)0x0910u)
#define ESC_REG_DC_RECEIVE_TIME_ECAT_PU                     ((osal_uint16_t)0x0918u)
#define ESC_REG_DC_SYSTEM_TIME_OFFSET                       ((osal_uint16_t)0x0920u)
#define ESC_REG_DC_SYSTEM_TIME_DELAY                        ((osal_uint16_t)0x0928u)
#define ESC_REG_DC_SYSTEM_TIME_DIFFERENCE                   ((osal_uint16_t)0x092Cu)
#define ESC_REG_DC_SPEED_COUNTER_START                      ((osal_uint16_t)0x0930u)
#define ESC_REG_DC_SPEED_COUNTER_DIFF                       ((osal_uint16_t)0x0932u)
#define ESC_REG_DC_SYSTEM_TIME_DIFFERENCE_FILTER_DEPTH      ((osal_uint16_t)0x0934u)
#define ESC_REG_DC_SPEED_COUNTER_FILTER_DEPTH               ((osal_uint16_t)0x0935u)
#define ESC_REG_DC_RECEIVE_TIME_LATCH_MODE                  ((osal_uint16_t)0x0936u)

#define ESC_REG_DC_CYCLIC_UNIT_CONTROL                      ((osal_uint16_t)0x0980u)
#define ESC_REG_DC_CYCLIC_UNIT_CONTROL__UNIT_CONTROL_ECAT   ((osal_uint8_t)0x00u)
#define ESC_REG_DC_CYCLIC_UNIT_CONTROL__UNIT_CONTROL_PDI    ((osal_uint8_t)0x01u)
#define ESC_REG_DC_CYCLIC_UNIT_CONTROL__RES0                ((osal_uint8_t)0x0Eu)
#define ESC_REG_DC_CYCLIC_UNIT_CONTROL__LATCH_IN_UNIT_0_PDI ((osal_uint8_t)0x10u)
#define ESC_REG_DC_CYCLIC_UNIT_CONTROL__LATCH_IN_UNIT_1_PDI ((osal_uint8_t)0x20u)
#define ESC_REG_DC_CYCLIC_UNIT_CONTROL__RES1                ((osal_uint8_t)0xC0u)

#define ESC_STATE_INIT          ((osal_uint8_t)0x1)
#define ESC_STATE_PREOP         ((osal_uint8_t)0x2)
#define ESC_STATE_SAFEOP        ((osal_uint8_t)0x4)
#define ESC_STATE_OP            ((osal_uint8_t)0x8)
#define ESC_STATE_BOOT          ((osal_uint8_t)0x3)

#define OP_2_OP         0x0808
#define OP_2_SAFEOP     0x0804
#define OP_2_PREOP      0x0802
#define OP_2_INIT       0x0801
#define INIT_2_BOOT     0x0103
#define INIT_2_INIT     0x0101
#define INIT_2_PREOP    0x0102
#define INIT_2_SAFEOP   0x0104
#define INIT_2_OP       0x0108

#define ESC_AL_STATUS_CODE__NO_ERROR                                        ((osal_uint16_t)0x0000u) //!< \brief No error
#define ESC_AL_STATUS_CODE__UNSPECIFIED_ERROR                               ((osal_uint16_t)0x0001u) //!< \brief Unspecified error
#define ESC_AL_STATUS_CODE__NO_MEMORY                                       ((osal_uint16_t)0x0002u) //!< \brief No Memory
#define ESC_AL_STATUS_CODE__INVALID_DEVICE_SETUP                            ((osal_uint16_t)0x0003u) //!< \brief Invalid Device Setup
#define ESC_AL_STATUS_CODE__RESERVED_DUE_TO_COMPATIBILITY_REASONS           ((osal_uint16_t)0x0005u) //!< \brief Reserved due to compatibility reasons
#define ESC_AL_STATUS_CODE__INVALID_REQUESTED_STATE_CHANGE                  ((osal_uint16_t)0x0011u) //!< \brief Invalid requested state change
#define ESC_AL_STATUS_CODE__UNKNOWN_REQUESTED_STATE                         ((osal_uint16_t)0x0012u) //!< \brief Unknown requested state
#define ESC_AL_STATUS_CODE__BOOTSTRAP_NOT_SUPPORTED                         ((osal_uint16_t)0x0013u) //!< \brief Bootstrap not supported
#define ESC_AL_STATUS_CODE__NO_VALID_FIRMWARE                               ((osal_uint16_t)0x0014u) //!< \brief No valid firmware
#define ESC_AL_STATUS_CODE__INVALID_MAILBOX_CONFIGURATION__BOOT             ((osal_uint16_t)0x0015u) //!< \brief Invalid mailbox configuration
#define ESC_AL_STATUS_CODE__INVALID_MAILBOX_CONFIGURATION__PREOP            ((osal_uint16_t)0x0016u) //!< \brief Invalid mailbox configuration
#define ESC_AL_STATUS_CODE__INVALID_SYNC_MANAGER_CONFIGURATION              ((osal_uint16_t)0x0017u) //!< \brief Invalid sync manager configuration
#define ESC_AL_STATUS_CODE__NO_VALID_INPUTS_AVAILABLE                       ((osal_uint16_t)0x0018u) //!< \brief No valid inputs available
#define ESC_AL_STATUS_CODE__NO_VALID_OUTPUTS                                ((osal_uint16_t)0x0019u) //!< \brief No valid outputs
#define ESC_AL_STATUS_CODE__SYNCHRONIZATION_ERROR                           ((osal_uint16_t)0x001Au) //!< \brief Synchronization error
#define ESC_AL_STATUS_CODE__SYNC_MANAGER_WATCHDOG                           ((osal_uint16_t)0x001Bu) //!< \brief Sync manager watchdog
#define ESC_AL_STATUS_CODE__INVALID_SYNC_MANAGER_TYPES                      ((osal_uint16_t)0x001Cu) //!< \brief Invalid Sync Manager Types
#define ESC_AL_STATUS_CODE__INVALID_OUTPUT_CONFIGURATION                    ((osal_uint16_t)0x001Du) //!< \brief Invalid Output Configuration
#define ESC_AL_STATUS_CODE__INVALID_INPUT_CONFIGURATION                     ((osal_uint16_t)0x001Eu) //!< \brief Invalid Input Configuration
#define ESC_AL_STATUS_CODE__INVALID_WATCHDOG_CONFIGURATION                  ((osal_uint16_t)0x001Fu) //!< \brief Invalid Watchdog Configuration
#define ESC_AL_STATUS_CODE__SLAVE_NEEDS_COLD_START                          ((osal_uint16_t)0x0020u) //!< \brief Slave needs cold start
#define ESC_AL_STATUS_CODE__SLAVE_NEEDS_INIT                                ((osal_uint16_t)0x0021u) //!< \brief Slave needs INIT
#define ESC_AL_STATUS_CODE__SLAVE_NEEDS_PREOP                               ((osal_uint16_t)0x0022u) //!< \brief Slave needs PREOP
#define ESC_AL_STATUS_CODE__SLAVE_NEEDS_SAFEOP                              ((osal_uint16_t)0x0023u) //!< \brief Slave needs SAFEOP
#define ESC_AL_STATUS_CODE__INVALID_INPUT_MAPPING                           ((osal_uint16_t)0x0024u) //!< \brief Invalid Input Mapping
#define ESC_AL_STATUS_CODE__INVALID_OUTPUT_MAPPING                          ((osal_uint16_t)0x0025u) //!< \brief Invalid Output Mapping
#define ESC_AL_STATUS_CODE__INCONSISTENT_SETTINGS                           ((osal_uint16_t)0x0026u) //!< \brief Inconsistent Settings
#define ESC_AL_STATUS_CODE__FREERUN_NOT_SUPPORTED                           ((osal_uint16_t)0x0027u) //!< \brief FreeRun not supported
#define ESC_AL_STATUS_CODE__SYNCMODE_NOT_SUPPORTED                          ((osal_uint16_t)0x0028u) //!< \brief SyncMode not supported
#define ESC_AL_STATUS_CODE__FREERUN_NEEDS_3BUFFER_MODE                      ((osal_uint16_t)0x0029u) //!< \brief FreeRun needs 3Buffer Mode
#define ESC_AL_STATUS_CODE__BACKGROUND_WATCHDOG                             ((osal_uint16_t)0x002Au) //!< \brief Background Watchdog
#define ESC_AL_STATUS_CODE__NO_VALID_INPUTS_AND_OUTPUTS                     ((osal_uint16_t)0x002Bu) //!< \brief No Valid Inputs and Outputs
#define ESC_AL_STATUS_CODE__FATAL_SYNC_ERROR                                ((osal_uint16_t)0x002Cu) //!< \brief Fatal Sync Error
#define ESC_AL_STATUS_CODE__NO_SYNC_ERROR                                   ((osal_uint16_t)0x002Du) //!< \brief No Sync Error
#define ESC_AL_STATUS_CODE__INVALID_DC_SYNC_CONFIGURATION                   ((osal_uint16_t)0x0030u) //!< \brief Invalid DC SYNC Configuration
#define ESC_AL_STATUS_CODE__INVALID_DC_LATCH_CONFIGURATION                  ((osal_uint16_t)0x0031u) //!< \brief Invalid DC Latch Configuration
#define ESC_AL_STATUS_CODE__PLL_ERROR                                       ((osal_uint16_t)0x0032u) //!< \brief PLL Error
#define ESC_AL_STATUS_CODE__DC_SYNC_IO_ERROR                                ((osal_uint16_t)0x0033u) //!< \brief DC Sync IO Error
#define ESC_AL_STATUS_CODE__DC_SYNC_TIMEOUT_ERROR                           ((osal_uint16_t)0x0034u) //!< \brief DC Sync Timeout Error
#define ESC_AL_STATUS_CODE__DC_INVALID_SYNC_CYCLE_TIME                      ((osal_uint16_t)0x0035u) //!< \brief DC Invalid Sync Cycle Time
#define ESC_AL_STATUS_CODE__DC_SYNC0_CYCLE_TIME                             ((osal_uint16_t)0x0036u) //!< \brief DC Sync0 Cycle Time
#define ESC_AL_STATUS_CODE__DC_SYNC1_CYCLE_TIME                             ((osal_uint16_t)0x0037u) //!< \brief DC Sync1 Cycle Time
#define ESC_AL_STATUS_CODE__MBX_AOE                                         ((osal_uint16_t)0x0041u) //!< \brief MBX_AOE
#define ESC_AL_STATUS_CODE__MBX_EOE                                         ((osal_uint16_t)0x0042u) //!< \brief MBX_EOE
#define ESC_AL_STATUS_CODE__MBX_COE                                         ((osal_uint16_t)0x0043u) //!< \brief MBX_COE
#define ESC_AL_STATUS_CODE__MBX_FOE                                         ((osal_uint16_t)0x0044u) //!< \brief MBX_FOE
#define ESC_AL_STATUS_CODE__MBX_SOE                                         ((osal_uint16_t)0x0045u) //!< \brief MBX_SOE
#define ESC_AL_STATUS_CODE__MBX_VOE                                         ((osal_uint16_t)0x004Fu) //!< \brief MBX_VOE
#define ESC_AL_STATUS_CODE__EEPROM_NO_ACCESS                                ((osal_uint16_t)0x0050u) //!< \brief EEPROM no access
#define ESC_AL_STATUS_CODE__EEPROM_ERROR                                    ((osal_uint16_t)0x0051u) //!< \brief EEPROM Error
#define ESC_AL_STATUS_CODE__SLAVE_RESTARTED_LOCALLY                         ((osal_uint16_t)0x0060u) //!< \brief Slave restarted locally
#define ESC_AL_STATUS_CODE__DEVICE_IDENTIFICATION_VALUE_UPDATED             ((osal_uint16_t)0x0061u) //!< \brief Device Identification value updated
#define ESC_AL_STATUS_CODE__APPLICATION_CONTROLLER_AVAILABLE                ((osal_uint16_t)0x00F0u) //!< \brief Application controller available

typedef struct mailbox {
    ec_slave_sm_t sm_receive;
    ec_slave_sm_t sm_send;
} mailbox_t;

typedef struct esc {
    osal_uint8_t al_state;
    osal_bool_t al_error_flag;
    osal_uint16_t al_status_code;
    osal_bool_t id_flag;
    osal_uint8_t al_control_state;

    osal_bool_t boot_supported;                 //!< \brief Bootstrap mode is supported by slave
    osal_bool_t dc_not_accepted;                //!< \brief The device does not support DCs and does not accept DC settings,
                                                // either. If DC settings are made the device will go into Error state. This
                                                // means Sync Control register 0x0981 shall be 0.
    osal_bool_t dc_required;                    //!< \brief Device requires DC settings (Bits 0x0981:00 to 0x0981:02 shall be set
                                                // accordingly). Other modes such as “Freerun” and “Synchronous with
                                                // SM event” are not supported
    osal_bool_t dc_running;                     //!< \brief DCs are running and used for synchronization between master and
                                                // slave application.
    osal_bool_t dc_supported;                   //!< \brief Device supports at least one OpMode which uses Distributed Clocks

    osal_uint16_t local_error_code;             //!< \brief Original reason of a previous local error which resulted in disabeling
                                                // SyncManager channels
    osal_bool_t local_error_flag;               //!< \brief Flag of local application indicating a local error causing that
                                                // SyncManagers were disabled. Used in case of an error resulting in a
                                                // state change from Op to SafeOp 
    osal_bool_t pll_running;                    //!< \brief Local application is synchronized with DC event and master application.
    osal_uint32_t safeop_2_op_timeout_timer;    //!< \brief Timeout for state change from SafeOp to Op
    osal_bool_t dc_event_received;              //!< \brief Slave has received the Sync0/Sync1 event at least once
    osal_bool_t wait_for_pll_running;           //!< \brief Wait until local PLL has locked with master cycle (wait until 
                                                // pll_running = TRUE). This time shall be smaller than the safeop_2_op_timeout.
    osal_bool_t wd_enabled;                     //!< \brief The watchdog behavior is disabled if at least the watchdog time (R400,
                                                // R420) is equal zero, additionally the watchdog behavior can be
                                                // disabled if the watchdog bit of the SyncManager is disabled
                                                // (SyncManager register +0x04.06=0) (when using the watchdog
                                                // functionality of the ESC, this watchdog is only enabled if the watchdog
                                                // bit in the Sync Manager is set)
                                                // TRUE: watchdog behaviour is enabled
                                                // FALSE: watchdog behaviour is disabled
                                                // If slave has only inputs wdEnabled may always be FALSE
    osal_bool_t ready_for_op;                   //!< \brief Internal variable which is set when the device is ready to be switched
                                                // to OP in Non-DC-Mode, this should happen when the outputs have
                                                // been received within the watchdog time if enabled or during SafeOp
                                                // state when the watchdog is disabled. However there can be another
                                                // behavior due to legacy reasons.
    osal_bool_t sm_event_received_internal;     //!< \brief Variable which indicates that the SM-event was received
                                            
    esc_bool_t mbx_supported;                   //!< \brief Flag indicating if mailbox is supported by slave.
    esc_bool_t mbx_running;                     //!< \brief Flag indicating mailbox is currently running.
                                                //
    mailbox_t mbx;
} esc_t;

#endif // LIBESC_ESC_H

