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

typedef struct mailbox {
    osal_bool_t supported;
    
    ec_slave_sm_t sm_receive;
    ec_slave_sm_t sm_send;
} mailbox_t;

typedef struct esc {
    mailbox_t mbx;
} esc_t;

#endif // LIBESC_ESC_H

