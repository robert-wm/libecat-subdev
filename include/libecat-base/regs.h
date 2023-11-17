/**
 * \file regs.h
 *
 * \author Robert Burger <robert@burger-system.de>
 *
 * \date 25 Oct 2023
 *
 * \brief ESC registers header.
 *
 * ESC registers header.
 */

/*
 * This file is part of libecat-base.
 *
 * libecat-base is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 3 of the License, or (at your option) any later version.
 * 
 * libecat-base is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public License
 * along with libecat-base; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#ifndef LIBECAT_BASE_EC_H
#define LIBECAT_BASE_EC_H

#include <stdint.h>
#include <libecat-base/types.h>

#define EC_REG_AL_STATUS                                            ((ec_uint16_t)0x0130u)
#define EC_REG_AL_STATUS_CODE                                       ((ec_uint16_t)0x0134u)
#define EC_REG_PDI_CONTROL                                          ((ec_uint16_t)0x0140u)

#define EC_REG_AL_EVENT_REQUEST                                     ((ec_uint16_t)0x0220u)
#define EC_REG_AL_EVENT_REQUEST__SIZE                               ((ec_uint8_t)4u)

#define EC_REG_AL_EVENT_REQUEST__AL_CONTROL_EVENT__MASK             ((ec_uint32_t)0x00000001u)
#define EC_REG_AL_EVENT_REQUEST__AL_CONTROL_EVENT__SHIFT            ((ec_uint8_t)0x00000000u)
#define EC_REG_AL_EVENT_REQUEST__AL_CONTROL_EVENT(reg)              (((reg) & EC_REG_AL_EVENT_REQUEST__AL_CONTROL_EVENT__MASK) >> EC_REG_AL_EVENT_REQUEST__AL_CONTROL_EVENT__SHIFT)
#define EC_REG_AL_EVENT_REQUEST__AL_CONTROL_EVENT__SET(reg, val)    (((reg) = (reg) & ~EC_REG_AL_EVENT_REQUEST__AL_CONTROL_EVENT__MASK | \
                                                                        ((val) << EC_REG_AL_EVENT_REQUEST__AL_CONTROL_EVENT__SHIFT) & EC_REG_AL_EVENT_REQUEST__AL_CONTROL_EVENT__MASK)

#define EC_REG_AL_EVENT_REQUEST__DC_LATCH_EVENT__MASK               ((ec_uint32_t)0x00000002u)
#define EC_REG_AL_EVENT_REQUEST__DC_LATCH_EVENT__SHIFT              ((ec_uint8_t)0x00000001u)
#define EC_REG_AL_EVENT_REQUEST__DC_LATCH_EVENT(reg)                (((reg) & EC_REG_AL_EVENT_REQUEST__DC_LATCH_EVENT__MASK) >> EC_REG_AL_EVENT_REQUEST__DC_LATCH_EVENT__SHIFT)
#define EC_REG_AL_EVENT_REQUEST__DC_LATCH_EVENT__SET(reg, val)      (((reg) = (reg) & ~EC_REG_AL_EVENT_REQUEST__DC_LATCH_EVENT__MASK | \
                                                                        ((val) << EC_REG_AL_EVENT_REQUEST__DC_LATCH_EVENT__SHIFT) & EC_REG_AL_EVENT_REQUEST__DC_LATCH_EVENT__MASK)

#define EC_REG_AL_EVENT_REQUEST__DC_SYNC0__MASK                     ((ec_uint32_t)0x00000004u)
#define EC_REG_AL_EVENT_REQUEST__DC_SYNC0__SHIFT                    ((ec_uint8_t)0x00000002u)
#define EC_REG_AL_EVENT_REQUEST__DC_SYNC0(reg)                      (((reg) & EC_REG_AL_EVENT_REQUEST__DC_SYNC0__MASK) >> EC_REG_AL_EVENT_REQUEST__DC_SYNC0__SHIFT)
#define EC_REG_AL_EVENT_REQUEST__DC_SYNC0__SET(reg, val)            (((reg) = (reg) & ~EC_REG_AL_EVENT_REQUEST__DC_SYNC0__MASK | \
                                                                        ((val) << EC_REG_AL_EVENT_REQUEST__DC_SYNC0__SHIFT) & EC_REG_AL_EVENT_REQUEST__DC_SYNC0__MASK)

#define EC_REG_AL_EVENT_REQUEST__DC_SYNC1__MASK                     ((ec_uint32_t)0x00000008u)
#define EC_REG_AL_EVENT_REQUEST__DC_SYNC1__SHIFT                    ((ec_uint8_t)0x00000003u)
#define EC_REG_AL_EVENT_REQUEST__DC_SYNC1(reg)                      (((reg) & EC_REG_AL_EVENT_REQUEST__DC_SYNC1__MASK) >> EC_REG_AL_EVENT_REQUEST__DC_SYNC1__SHIFT)
#define EC_REG_AL_EVENT_REQUEST__DC_SYNC1__SET(reg, val)            (((reg) = (reg) & ~EC_REG_AL_EVENT_REQUEST__DC_SYNC1__MASK | \
                                                                        ((val) << EC_REG_AL_EVENT_REQUEST__DC_SYNC1__SHIFT) & EC_REG_AL_EVENT_REQUEST__DC_SYNC1__MASK)

#define EC_REG_AL_EVENT_REQUEST__SM_ACTIVATION__MASK                ((ec_uint32_t)0x00000010u)
#define EC_REG_AL_EVENT_REQUEST__SM_ACTIVATION__SHIFT               ((ec_uint8_t)0x00000004u)
#define EC_REG_AL_EVENT_REQUEST__SM_ACTIVATION(reg)                 (((reg) & EC_REG_AL_EVENT_REQUEST__SM_ACTIVATION__MASK) >> EC_REG_AL_EVENT_REQUEST__SM_ACTIVATION__SHIFT)
#define EC_REG_AL_EVENT_REQUEST__SM_ACTIVATION__SET(reg, val)       (((reg) = (reg) & ~EC_REG_AL_EVENT_REQUEST__SM_ACTIVATION__MASK | \
                                                                        ((val) << EC_REG_AL_EVENT_REQUEST__SM_ACTIVATION__SHIFT) & EC_REG_AL_EVENT_REQUEST__SM_ACTIVATION__MASK)

#define EC_REG_AL_EVENT_REQUEST__EEPROM_EMULATION__MASK             ((ec_uint32_t)0x00000020u)
#define EC_REG_AL_EVENT_REQUEST__EEPROM_EMULATION__SHIFT            ((ec_uint8_t)0x00000005u)
#define EC_REG_AL_EVENT_REQUEST__EEPROM_EMULATION(reg)              (((reg) & EC_REG_AL_EVENT_REQUEST__EEPROM_EMULATION__MASK) >> EC_REG_AL_EVENT_REQUEST__EEPROM_EMULATION__SHIFT)
#define EC_REG_AL_EVENT_REQUEST__EEPROM_EMULATION__SET(reg, val)    (((reg) = (reg) & ~EC_REG_AL_EVENT_REQUEST__EEPROM_EMULATION__MASK | \
                                                                        ((val) << EC_REG_AL_EVENT_REQUEST__EEPROM_EMULATION__SHIFT) & EC_REG_AL_EVENT_REQUEST__EEPROM_EMULATION__MASK)

#define EC_REG_AL_EVENT_REQUEST__WATCHDOG_PD__MASK                  ((ec_uint32_t)0x00000040u)
#define EC_REG_AL_EVENT_REQUEST__WATCHDOG_PD__SHIFT                 ((ec_uint8_t)0x00000006u)
#define EC_REG_AL_EVENT_REQUEST__WATCHDOG_PD(reg)                   (((reg) & EC_REG_AL_EVENT_REQUEST__WATCHDOG_PD__MASK) >> EC_REG_AL_EVENT_REQUEST__WATCHDOG_PD__SHIFT)
#define EC_REG_AL_EVENT_REQUEST__WATCHDOG_PD__SET(reg, val)         (((reg) = (reg) & ~EC_REG_AL_EVENT_REQUEST__WATCHDOG_PD__MASK | \
                                                                        ((val) << EC_REG_AL_EVENT_REQUEST__WATCHDOG_PD__SHIFT) & EC_REG_AL_EVENT_REQUEST__WATCHDOG_PD__MASK)

#define EC_REG_AL_EVENT_REQUEST__SM_INTERRUPT__MASK                 ((ec_uint32_t)0x00FFFF00u)
#define EC_REG_AL_EVENT_REQUEST__SM_INTERRUPT__SHIFT                ((ec_uint8_t)0x00000008u)
#define EC_REG_AL_EVENT_REQUEST__SM_INTERRUPT(reg)                  (((reg) & EC_REG_AL_EVENT_REQUEST__SM_INTERRUPT__MASK) >> EC_REG_AL_EVENT_REQUEST__SM_INTERRUPT__SHIFT)
#define EC_REG_AL_EVENT_REQUEST__SM_INTERRUPT__SET(reg, val)        (((reg) = (reg) & ~EC_REG_AL_EVENT_REQUEST__SM_INTERRUPT__MASK | \
                                                                        ((val) << EC_REG_AL_EVENT_REQUEST__SM_INTERRUPT__SHIFT) & EC_REG_AL_EVENT_REQUEST__SM_INTERRUPT__MASK)

#define EC_REG_AL_EVENT_REQUEST__SM_INTERRUPT__SM(nr)               ((nr) + 1u)
//-------------------------------------
// SII registers.
//

#define EC_REG_SII_EEPROM                                  ((ec_uint16_t)0x0500u)

#define EC_REG_SII_EEPROM__CONFIGURATION                   ((ec_uint16_t)0x0500u)
#define EC_REG_SII_EEPROM__CONFIGURATION__PDI_CONTROL      ((ec_uint8_t)0x01u)
#define EC_REG_SII_EEPROM__CONFIGURATION__RES0             ((ec_uint8_t)0xFEu)

#define EC_REG_SII_EEPROM__PDI_ACCESS                      ((ec_uint16_t)0x0501u)
#define EC_REG_SII_EEPROM__PDI_ACCESS__PDI_ACCESS          ((ec_uint8_t)0x01u)
#define EC_REG_SII_EEPROM__PDI_ACCESS__RES0                ((ec_uint8_t)0xFEu)

#define EC_REG_SII_EEPROM__CTRLSTAT                        ((ec_uint16_t)0x0502u)
#define EC_REG_SII_EEPROM__CTRLSTAT__ECAT_WRITE_ENABLE     ((ec_uint16_t)0x0001u)
#define EC_REG_SII_EEPROM__CTRLSTAT__RES0                  ((ec_uint16_t)0x001Eu)
#define EC_REG_SII_EEPROM__CTRLSTAT__PDI_EMULATION         ((ec_uint16_t)0x0020u)
#define EC_REG_SII_EEPROM__CTRLSTAT                        ((ec_uint16_t)0x0502u)
#define EC_REG_SII_EEPROM__CTRLSTAT                        ((ec_uint16_t)0x0502u)


//-------------------------------------
// FMMU registers.
//

#define EC_REG_FMMU__LOG_START_ADDRESS                     ((ec_uint16_t)0x0600u)
#define EC_REG_FMMU__LENGTH                                ((ec_uint16_t)0x0604u)
#define EC_REG_FMMU__LOG_START_BIT                         ((ec_uint16_t)0x0606u)
#define EC_REG_FMMU__LOG_STOP_BIT                          ((ec_uint16_t)0x0607u)
#define EC_REG_FMMU__PHYS_START_ADDRESS                    ((ec_uint16_t)0x0608u)
#define EC_REG_FMMU__PHYS_START_BIT                        ((ec_uint16_t)0x060Au)
#define EC_REG_FMMU__TYPE                                  ((ec_uint16_t)0x060Bu)
#define EC_REG_FMMU__ACTIVATE                              ((ec_uint16_t)0x060Cu)
#define EC_REG_FMMU__RESERVERD                             ((ec_uint16_t)0x060Du)

#define EC_REG_FMMU__BASE_OFFSET                           ((ec_uint8_t)0x10u)

//-------------------------------------
// Sync Manager registers.
//

#define EC_REG_SYNC_MANAGER                                ((ec_uint16_t)0x0800u)
#define EC_REG_SYNC_MANAGER__BASE_OFFSET                   ((ec_uint8_t)0x08u)
#define EC_REG_SYNC_MANAGER_N(ch)                          (EC_REG_SYNC_MANAGER + ((ch) * EC_REG_SYNC_MANAGER__BASE_OFFSET))
#define EC_REG_SYNC_MANAGER_0                              (EC_REG_SYNC_MANAGER_N(0))
#define EC_REG_SYNC_MANAGER_1                              (EC_REG_SYNC_MANAGER_N(1))
#define EC_REG_SYNC_MANAGER_2                              (EC_REG_SYNC_MANAGER_N(2))
#define EC_REG_SYNC_MANAGER_3                              (EC_REG_SYNC_MANAGER_N(3))
#define EC_REG_SYNC_MANAGER_4                              (EC_REG_SYNC_MANAGER_N(4))
#define EC_REG_SYNC_MANAGER_5                              (EC_REG_SYNC_MANAGER_N(5))
#define EC_REG_SYNC_MANAGER_6                              (EC_REG_SYNC_MANAGER_N(6))
#define EC_REG_SYNC_MANAGER_7                              (EC_REG_SYNC_MANAGER_N(7))

#define EC_REG_SYNC_MANAGER__CONTROL__OP_MODE_MASK         ((ec_uint8_t)0x03u)
#define EC_REG_SYNC_MANAGER__CONTROL__OP_MODE_BUFFERED     ((ec_uint8_t)0x00u)
#define EC_REG_SYNC_MANAGER__CONTROL__OP_MODE_RES0         ((ec_uint8_t)0x01u)
#define EC_REG_SYNC_MANAGER__CONTROL__OP_MODE_MAILBOX      ((ec_uint8_t)0x02u)
#define EC_REG_SYNC_MANAGER__CONTROL__OP_MODE_RES1         ((ec_uint8_t)0x03u)

#define EC_REG_SYNC_MANAGER__CONTROL__DIRECTION_MASK       ((ec_uint8_t)0x0Cu)
#define EC_REG_SYNC_MANAGER__CONTROL__DIRECTION_READ       ((ec_uint8_t)0x00u)
#define EC_REG_SYNC_MANAGER__CONTROL__DIRECTION_WRITE      ((ec_uint8_t)0x04u)
#define EC_REG_SYNC_MANAGER__CONTROL__DIRECTION_RES0       ((ec_uint8_t)0x08u)
#define EC_REG_SYNC_MANAGER__CONTROL__DIRECTION_RES1       ((ec_uint8_t)0x0Cu)

#define EC_REG_SYNC_MANAGER__CONTROL__IRQ_ECAT_EVENT       ((ec_uint8_t)0x10u)
#define EC_REG_SYNC_MANAGER__CONTROL__IRQ_AL_EVENT         ((ec_uint8_t)0x20u)
#define EC_REG_SYNC_MANAGER__CONTROL__WATCHDOG_TRIGGER     ((ec_uint8_t)0x40u)
#define EC_REG_SYNC_MANAGER__CONTROL__RES0                 ((ec_uint8_t)0x80u)

#define EC_REG_SYNC_MANAGER__STATUS__IRQ_WRITE             ((ec_uint8_t)0x01u)
#define EC_REG_SYNC_MANAGER__STATUS__IRQ_READ              ((ec_uint8_t)0x02u)
#define EC_REG_SYNC_MANAGER__STATUS__IRQ_RES0              ((ec_uint8_t)0x04u)
#define EC_REG_SYNC_MANAGER__STATUS__MAILBOX_FULL          ((ec_uint8_t)0x08u)

#define EC_REG_SYNC_MANAGER__STATUS__BUFFER_MASK           ((ec_uint8_t)0x30u)
#define EC_REG_SYNC_MANAGER__STATUS__BUFFER_1              ((ec_uint8_t)0x00u)
#define EC_REG_SYNC_MANAGER__STATUS__BUFFER_2              ((ec_uint8_t)0x10u)
#define EC_REG_SYNC_MANAGER__STATUS__BUFFER_3              ((ec_uint8_t)0x20u)
#define EC_REG_SYNC_MANAGER__STATUS__BUFFER_NONE           ((ec_uint8_t)0x30u)

#define EC_REG_SYNC_MANAGER__STATUS__READ_BUFFER_OPEN      ((ec_uint8_t)0x40u)
#define EC_REG_SYNC_MANAGER__STATUS__WRITE_BUFFER_OPEN     ((ec_uint8_t)0x80u)

#define EC_REG_SYNC_MANAGER__ACTIVATE__ENABLE_SM           ((ec_uint8_t)0x01u)
#define EC_REG_SYNC_MANAGER__ACTIVATE__REPEAT_REQUEST      ((ec_uint8_t)0x02u)
#define EC_REG_SYNC_MANAGER__ACTIVATE__RES0                ((ec_uint8_t)0x3Cu)
#define EC_REG_SYNC_MANAGER__ACTIVATE__LATCH_ECAT          ((ec_uint8_t)0x40u)
#define EC_REG_SYNC_MANAGER__ACTIVATE__LATCH_PDI           ((ec_uint8_t)0x80u)

#define EC_REG_SYNC_MANAGER__PDI_CONTROL_OFFSET            ((ec_uint8_t)0x07u)
#define EC_REG_SYNC_MANAGER__PDI_CONTROL__DEACTIVATE       ((ec_uint8_t)0x01u)
#define EC_REG_SYNC_MAANGER__PDI_CONTROL__REPEAT_ACK       ((ec_uint8_t)0x02u)
#define EC_REG_SYNC_MANAGER__PDI_CONTROL__RES0             ((ec_uint8_t)0xFCu)

//! \brief Sync Manager Register structure.
struct ec_slave_sm {
    ec_uint16_t   address;        //!< \brief Sync Manager start address.
                                    /*!<
                                     * Specifies ESC memory address where the
                                     * Sync Manager data starts.
                                     */
    ec_uint16_t   length;         //!< \brief Sync Manager length.
                                    /*!<
                                     * Specifies the length of the ESC
                                     * memory mapped by Sync Manager.
                                     */

    ec_uint8_t    control;        //!< \brief Control register.
    ec_uint8_t    status;         //!< \brief Status register.
    ec_uint8_t    activate;       //!< \brief Activate register.
    ec_uint8_t    pdi_control;    //!< \brief PDI Control register.
} __attribute__((__packed__));

//-------------------------------------
// Distributed Clocks registers.
//

#define EC_REG_DC_RECEIVE_TIME_0                           ((ec_uint16_t)0x0900u)
#define EC_REG_DC_RECEIVE_TIME_1                           ((ec_uint16_t)0x0904u)
#define EC_REG_DC_RECEIVE_TIME_2                           ((ec_uint16_t)0x0908u)
#define EC_REG_DC_RECEIVE_TIME_3                           ((ec_uint16_t)0x090Cu)
#define EC_REG_DC_SYSTEM_TIME                              ((ec_uint16_t)0x0910u)
#define EC_REG_DC_RECEIVE_TIME_ECAT_PU                     ((ec_uint16_t)0x0918u)
#define EC_REG_DC_SYSTEM_TIME_OFFSET                       ((ec_uint16_t)0x0920u)
#define EC_REG_DC_SYSTEM_TIME_DELAY                        ((ec_uint16_t)0x0928u)
#define EC_REG_DC_SYSTEM_TIME_DIFFERENCE                   ((ec_uint16_t)0x092Cu)
#define EC_REG_DC_SPEED_COUNTER_START                      ((ec_uint16_t)0x0930u)
#define EC_REG_DC_SPEED_COUNTER_DIFF                       ((ec_uint16_t)0x0932u)
#define EC_REG_DC_SYSTEM_TIME_DIFFERENCE_FILTER_DEPTH      ((ec_uint16_t)0x0934u)
#define EC_REG_DC_SPEED_COUNTER_FILTER_DEPTH               ((ec_uint16_t)0x0935u)
#define EC_REG_DC_RECEIVE_TIME_LATCH_MODE                  ((ec_uint16_t)0x0936u)

#define EC_REG_DC_CYCLIC_UNIT_CONTROL                      ((ec_uint16_t)0x0980u)
#define EC_REG_DC_CYCLIC_UNIT_CONTROL__UNIT_CONTROL_ECAT   ((ec_uint8_t)0x00u)
#define EC_REG_DC_CYCLIC_UNIT_CONTROL__UNIT_CONTROL_PDI    ((ec_uint8_t)0x01u)
#define EC_REG_DC_CYCLIC_UNIT_CONTROL__RES0                ((ec_uint8_t)0x0Eu)
#define EC_REG_DC_CYCLIC_UNIT_CONTROL__LATCH_IN_UNIT_0_PDI ((ec_uint8_t)0x10u)
#define EC_REG_DC_CYCLIC_UNIT_CONTROL__LATCH_IN_UNIT_1_PDI ((ec_uint8_t)0x20u)
#define EC_REG_DC_CYCLIC_UNIT_CONTROL__RES1                ((ec_uint8_t)0xC0u)

#define EC_STATE_INIT          ((ec_uint8_t)0x1)
#define EC_STATE_PREOP         ((ec_uint8_t)0x2)
#define EC_STATE_SAFEOP        ((ec_uint8_t)0x4)
#define EC_STATE_OP            ((ec_uint8_t)0x8)
#define EC_STATE_BOOT          ((ec_uint8_t)0x3)

#define OP_2_OP         0x0808
#define OP_2_SAFEOP     0x0804
#define OP_2_PREOP      0x0802
#define OP_2_INIT       0x0801
#define INIT_2_BOOT     0x0103
#define INIT_2_INIT     0x0101
#define INIT_2_PREOP    0x0102
#define INIT_2_SAFEOP   0x0104
#define INIT_2_OP       0x0108

#define EC_AL_STATUS_CODE__NO_ERROR                                        ((ec_uint16_t)0x0000u) //!< \brief No error
#define EC_AL_STATUS_CODE__UNSPECIFIED_ERROR                               ((ec_uint16_t)0x0001u) //!< \brief Unspecified error
#define EC_AL_STATUS_CODE__NO_MEMORY                                       ((ec_uint16_t)0x0002u) //!< \brief No Memory
#define EC_AL_STATUS_CODE__INVALID_DEVICE_SETUP                            ((ec_uint16_t)0x0003u) //!< \brief Invalid Device Setup
#define EC_AL_STATUS_CODE__RESERVED_DUE_TO_COMPATIBILITY_REASONS           ((ec_uint16_t)0x0005u) //!< \brief Reserved due to compatibility reasons
#define EC_AL_STATUS_CODE__INVALID_REQUESTED_STATE_CHANGE                  ((ec_uint16_t)0x0011u) //!< \brief Invalid requested state change
#define EC_AL_STATUS_CODE__UNKNOWN_REQUESTED_STATE                         ((ec_uint16_t)0x0012u) //!< \brief Unknown requested state
#define EC_AL_STATUS_CODE__BOOTSTRAP_NOT_SUPPORTED                         ((ec_uint16_t)0x0013u) //!< \brief Bootstrap not supported
#define EC_AL_STATUS_CODE__NO_VALID_FIRMWARE                               ((ec_uint16_t)0x0014u) //!< \brief No valid firmware
#define EC_AL_STATUS_CODE__INVALID_MAILBOX_CONFIGURATION__BOOT             ((ec_uint16_t)0x0015u) //!< \brief Invalid mailbox configuration
#define EC_AL_STATUS_CODE__INVALID_MAILBOX_CONFIGURATION__PREOP            ((ec_uint16_t)0x0016u) //!< \brief Invalid mailbox configuration
#define EC_AL_STATUS_CODE__INVALID_SYNC_MANAGER_CONFIGURATION              ((ec_uint16_t)0x0017u) //!< \brief Invalid sync manager configuration
#define EC_AL_STATUS_CODE__NO_VALID_INPUTS_AVAILABLE                       ((ec_uint16_t)0x0018u) //!< \brief No valid inputs available
#define EC_AL_STATUS_CODE__NO_VALID_OUTPUTS                                ((ec_uint16_t)0x0019u) //!< \brief No valid outputs
#define EC_AL_STATUS_CODE__SYNCHRONIZATION_ERROR                           ((ec_uint16_t)0x001Au) //!< \brief Synchronization error
#define EC_AL_STATUS_CODE__SYNC_MANAGER_WATCHDOG                           ((ec_uint16_t)0x001Bu) //!< \brief Sync manager watchdog
#define EC_AL_STATUS_CODE__INVALID_SYNC_MANAGER_TYPES                      ((ec_uint16_t)0x001Cu) //!< \brief Invalid Sync Manager Types
#define EC_AL_STATUS_CODE__INVALID_OUTPUT_CONFIGURATION                    ((ec_uint16_t)0x001Du) //!< \brief Invalid Output Configuration
#define EC_AL_STATUS_CODE__INVALID_INPUT_CONFIGURATION                     ((ec_uint16_t)0x001Eu) //!< \brief Invalid Input Configuration
#define EC_AL_STATUS_CODE__INVALID_WATCHDOG_CONFIGURATION                  ((ec_uint16_t)0x001Fu) //!< \brief Invalid Watchdog Configuration
#define EC_AL_STATUS_CODE__SLAVE_NEEDS_COLD_START                          ((ec_uint16_t)0x0020u) //!< \brief Slave needs cold start
#define EC_AL_STATUS_CODE__SLAVE_NEEDS_INIT                                ((ec_uint16_t)0x0021u) //!< \brief Slave needs INIT
#define EC_AL_STATUS_CODE__SLAVE_NEEDS_PREOP                               ((ec_uint16_t)0x0022u) //!< \brief Slave needs PREOP
#define EC_AL_STATUS_CODE__SLAVE_NEEDS_SAFEOP                              ((ec_uint16_t)0x0023u) //!< \brief Slave needs SAFEOP
#define EC_AL_STATUS_CODE__INVALID_INPUT_MAPPING                           ((ec_uint16_t)0x0024u) //!< \brief Invalid Input Mapping
#define EC_AL_STATUS_CODE__INVALID_OUTPUT_MAPPING                          ((ec_uint16_t)0x0025u) //!< \brief Invalid Output Mapping
#define EC_AL_STATUS_CODE__INCONSISTENT_SETTINGS                           ((ec_uint16_t)0x0026u) //!< \brief Inconsistent Settings
#define EC_AL_STATUS_CODE__FREERUN_NOT_SUPPORTED                           ((ec_uint16_t)0x0027u) //!< \brief FreeRun not supported
#define EC_AL_STATUS_CODE__SYNCMODE_NOT_SUPPORTED                          ((ec_uint16_t)0x0028u) //!< \brief SyncMode not supported
#define EC_AL_STATUS_CODE__FREERUN_NEEDS_3BUFFER_MODE                      ((ec_uint16_t)0x0029u) //!< \brief FreeRun needs 3Buffer Mode
#define EC_AL_STATUS_CODE__BACKGROUND_WATCHDOG                             ((ec_uint16_t)0x002Au) //!< \brief Background Watchdog
#define EC_AL_STATUS_CODE__NO_VALID_INPUTS_AND_OUTPUTS                     ((ec_uint16_t)0x002Bu) //!< \brief No Valid Inputs and Outputs
#define EC_AL_STATUS_CODE__FATAL_SYNC_ERROR                                ((ec_uint16_t)0x002Cu) //!< \brief Fatal Sync Error
#define EC_AL_STATUS_CODE__NO_SYNC_ERROR                                   ((ec_uint16_t)0x002Du) //!< \brief No Sync Error
#define EC_AL_STATUS_CODE__INVALID_DC_SYNC_CONFIGURATION                   ((ec_uint16_t)0x0030u) //!< \brief Invalid DC SYNC Configuration
#define EC_AL_STATUS_CODE__INVALID_DC_LATCH_CONFIGURATION                  ((ec_uint16_t)0x0031u) //!< \brief Invalid DC Latch Configuration
#define EC_AL_STATUS_CODE__PLL_ERROR                                       ((ec_uint16_t)0x0032u) //!< \brief PLL Error
#define EC_AL_STATUS_CODE__DC_SYNC_IO_ERROR                                ((ec_uint16_t)0x0033u) //!< \brief DC Sync IO Error
#define EC_AL_STATUS_CODE__DC_SYNC_TIMEOUT_ERROR                           ((ec_uint16_t)0x0034u) //!< \brief DC Sync Timeout Error
#define EC_AL_STATUS_CODE__DC_INVALID_SYNC_CYCLE_TIME                      ((ec_uint16_t)0x0035u) //!< \brief DC Invalid Sync Cycle Time
#define EC_AL_STATUS_CODE__DC_SYNC0_CYCLE_TIME                             ((ec_uint16_t)0x0036u) //!< \brief DC Sync0 Cycle Time
#define EC_AL_STATUS_CODE__DC_SYNC1_CYCLE_TIME                             ((ec_uint16_t)0x0037u) //!< \brief DC Sync1 Cycle Time
#define EC_AL_STATUS_CODE__MBX_AOE                                         ((ec_uint16_t)0x0041u) //!< \brief MBX_AOE
#define EC_AL_STATUS_CODE__MBX_EOE                                         ((ec_uint16_t)0x0042u) //!< \brief MBX_EOE
#define EC_AL_STATUS_CODE__MBX_COE                                         ((ec_uint16_t)0x0043u) //!< \brief MBX_COE
#define EC_AL_STATUS_CODE__MBX_FOE                                         ((ec_uint16_t)0x0044u) //!< \brief MBX_FOE
#define EC_AL_STATUS_CODE__MBX_SOE                                         ((ec_uint16_t)0x0045u) //!< \brief MBX_SOE
#define EC_AL_STATUS_CODE__MBX_VOE                                         ((ec_uint16_t)0x004Fu) //!< \brief MBX_VOE
#define EC_AL_STATUS_CODE__EEPROM_NO_ACCESS                                ((ec_uint16_t)0x0050u) //!< \brief EEPROM no access
#define EC_AL_STATUS_CODE__EEPROM_ERROR                                    ((ec_uint16_t)0x0051u) //!< \brief EEPROM Error
#define EC_AL_STATUS_CODE__SLAVE_RESTARTED_LOCALLY                         ((ec_uint16_t)0x0060u) //!< \brief Slave restarted locally
#define EC_AL_STATUS_CODE__DEVICE_IDENTIFICATION_VALUE_UPDATED             ((ec_uint16_t)0x0061u) //!< \brief Device Identification value updated
#define EC_AL_STATUS_CODE__APPLICATION_CONTROLLER_AVAILABLE                ((ec_uint16_t)0x00F0u) //!< \brief Application controller available

#endif // LIBECAT_BASE_EC_H


