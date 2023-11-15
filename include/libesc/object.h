/**
 * \file object.h
 *
 * \author Robert Burger <robert@burger-system.de>
 *
 * \date 08 Nov 2023
 *
 * \brief ESC object header.
 *
 * ESC object header.
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

#ifndef LIBESC_OBJECT_H
#define LIBESC_OBJECT_H

#include <libesc/types.h>

// ===========================
// ===== SDO DATA TYPES ======
// ===========================

#define ESC_SDO__DATA_TYPE__BOOLEAN                             ((esc_uint16_t)    0x0001u) 
#define ESC_SDO__DATA_TYPE__INTEGER8                            ((esc_uint16_t)    0x0002u) 
#define ESC_SDO__DATA_TYPE__INTEGER16                           ((esc_uint16_t)    0x0003u) 
#define ESC_SDO__DATA_TYPE__INTEGER32                           ((esc_uint16_t)    0x0004u) 
#define ESC_SDO__DATA_TYPE__UNSIGNED8                           ((esc_uint16_t)    0x0005u) 
#define ESC_SDO__DATA_TYPE__UNSIGNED16                          ((esc_uint16_t)    0x0006u) 
#define ESC_SDO__DATA_TYPE__UNSIGNED32                          ((esc_uint16_t)    0x0007u) 
#define ESC_SDO__DATA_TYPE__REAL32                              ((esc_uint16_t)    0x0008u) 
#define ESC_SDO__DATA_TYPE__VISIBLE_STRING                      ((esc_uint16_t)    0x0009u) 
#define ESC_SDO__DATA_TYPE__OCTET_STRING                        ((esc_uint16_t)    0x000Au) 
#define ESC_SDO__DATA_TYPE__UNICODE_STRING                      ((esc_uint16_t)    0x000Bu) 
#define ESC_SDO__DATA_TYPE__TIME_OF_DAY                         ((esc_uint16_t)    0x000Cu) 
#define ESC_SDO__DATA_TYPE__TIME_DIFFERENCE                     ((esc_uint16_t)    0x000Du) 
#define ESC_SDO__DATA_TYPE__DOMAIN                              ((esc_uint16_t)    0x000Fu) 
#define ESC_SDO__DATA_TYPE__INTEGER24                           ((esc_uint16_t)    0x0010u) 
#define ESC_SDO__DATA_TYPE__REAL64                              ((esc_uint16_t)    0x0011u) 
#define ESC_SDO__DATA_TYPE__INTEGER40                           ((esc_uint16_t)    0x0012u) 
#define ESC_SDO__DATA_TYPE__INTEGER48                           ((esc_uint16_t)    0x0013u) 
#define ESC_SDO__DATA_TYPE__INTEGER56                           ((esc_uint16_t)    0x0014u) 
#define ESC_SDO__DATA_TYPE__INTEGER64                           ((esc_uint16_t)    0x0015u) 
#define ESC_SDO__DATA_TYPE__UNSIGNED24                          ((esc_uint16_t)    0x0016u) 
#define ESC_SDO__DATA_TYPE__UNSIGNED40                          ((esc_uint16_t)    0x0018u) 
#define ESC_SDO__DATA_TYPE__UNSIGNED48                          ((esc_uint16_t)    0x0019u) 
#define ESC_SDO__DATA_TYPE__UNSIGNED56                          ((esc_uint16_t)    0x001Au) 
#define ESC_SDO__DATA_TYPE__UNSIGNED64                          ((esc_uint16_t)    0x001Bu) 
#define ESC_SDO__DATA_TYPE__GUID                                ((esc_uint16_t)    0x001Du) 
#define ESC_SDO__DATA_TYPE__BYTE                                ((esc_uint16_t)    0x001Eu) 
#define ESC_SDO__DATA_TYPE__BITARR8                             ((esc_uint16_t)    0x002Du) 
#define ESC_SDO__DATA_TYPE__BITARR16                            ((esc_uint16_t)    0x002Eu) 
#define ESC_SDO__DATA_TYPE__BITARR32                            ((esc_uint16_t)    0x002Fu) 
#define ESC_SDO__DATA_TYPE__PDO_MAPPING                         ((esc_uint16_t)    0x0021u)
#define ESC_SDO__DATA_TYPE__IDENTITY                            ((esc_uint16_t)    0x0023u)
#define ESC_SDO__DATA_TYPE__COMMAND_PAR                         ((esc_uint16_t)    0x0025u)
#define ESC_SDO__DATA_TYPE__SYNC_PAR                            ((esc_uint16_t)    0x0029u)
#define ESC_SDO__DATA_TYPE__BIT1                                ((esc_uint16_t)    0x0030u)
#define ESC_SDO__DATA_TYPE__BIT2                                ((esc_uint16_t)    0x0031u)
#define ESC_SDO__DATA_TYPE__BIT3                                ((esc_uint16_t)    0x0032u)
#define ESC_SDO__DATA_TYPE__BIT4                                ((esc_uint16_t)    0x0033u)
#define ESC_SDO__DATA_TYPE__BIT5                                ((esc_uint16_t)    0x0034u)
#define ESC_SDO__DATA_TYPE__BIT6                                ((esc_uint16_t)    0x0035u)
#define ESC_SDO__DATA_TYPE__BIT7                                ((esc_uint16_t)    0x0036u)
#define ESC_SDO__DATA_TYPE__BIT8                                ((esc_uint16_t)    0x0037u)

// ===========================
// ==== SDO OBJECT CODES =====
// ===========================

#define ESC_SDO__OBJECT_CODE__VARIABLE                          ((esc_uint8_t )         7u) //!< \brief Variable
#define ESC_SDO__OBJECT_CODE__ARRAY                             ((esc_uint8_t )         8u) //!< \brief Array
#define ESC_SDO__OBJECT_CODE__RECORD                            ((esc_uint8_t )         9u) //!< \brief Record

// ===========================
// ==== SDO OBJECT ACCESS ====
// ===========================

#define ESC_SDO__OBJECT_ACCESS__READ_PREOP                      ((esc_uint16_t)    0x0001u) //!< \brief Bit 0: read access in Pre-Operational state
#define ESC_SDO__OBJECT_ACCESS__READ_SAFEOP                     ((esc_uint16_t)    0x0002u) //!< \brief Bit 1: read access in Safe-Operational state
#define ESC_SDO__OBJECT_ACCESS__READ_OP                         ((esc_uint16_t)    0x0004u) //!< \brief Bit 2: read access in Operational state
#define ESC_SDO__OBJECT_ACCESS__READ                            ((esc_uint16_t)    0x0007u) //!< \brief Bit 0-2: read access in Pre-Operational-Operational state
#define ESC_SDO__OBJECT_ACCESS__WRITE_PREOP                     ((esc_uint16_t)    0x0008u) //!< \brief Bit 3: write access in Pre-Operational state
#define ESC_SDO__OBJECT_ACCESS__WRITE_SAFEOP                    ((esc_uint16_t)    0x0010u) //!< \brief Bit 4: write access in Safe-Operational state
#define ESC_SDO__OBJECT_ACCESS__WRITE_OP                        ((esc_uint16_t)    0x0020u) //!< \brief Bit 5: write access in Operational state
#define ESC_SDO__OBJECT_ACCESS__WRITE                           ((esc_uint16_t)    0x0038u) //!< \brief Bit 3-5: write access in Pre-Operational-Operational state
#define ESC_SDO__OBJECT_ACCESS__RXPDO_MAPPABLE                  ((esc_uint16_t)    0x0040u) //!< \brief Bit 6: object is mappable in a RxPDO
#define ESC_SDO__OBJECT_ACCESS__TXPDO_MAPPABLE                  ((esc_uint16_t)    0x0080u) //!< \brief Bit 7: object is mappable in a TxPDO
#define ESC_SDO__OBJECT_ACCESS__MAPPABLE                        ((esc_uint16_t)    0x00C0u) //!< \brief Bit 6-7: object is mappable in a TxPDO
#define ESC_SDO__OBJECT_ACCESS__BACKUP                          ((esc_uint16_t)    0x0100u) //!< \brief Bit 8: object can be used for backup
#define ESC_SDO__OBJECT_ACCESS__SETTINGS                        ((esc_uint16_t)    0x0200u) //!< \brief Bit 9: object can be used for settings

// ===========================
// ===== SDO ABORT CODES =====
// ===========================

#define ESC_SDO_ABORT_CODE__TOGGLE_NOT_CHANGED                  ((esc_uint32_t)0x05030000u) //!< \brief Toggle bit not changed
#define ESC_SDO_ABORT_CODE__SDO_TIMEOUT                         ((esc_uint32_t)0x05040000u) //!< \brief SDO protocol timeout
#define ESC_SDO_ABORT_CODE__COMMAND_NOT_VALID                   ((esc_uint32_t)0x05040001u) //!< \brief Client/Server command specifier not valid or unknown
#define ESC_SDO_ABORT_CODE__OUT_OF_MEMORY                       ((esc_uint32_t)0x05040005u) //!< \brief Out of memory
#define ESC_SDO_ABORT_CODE__UNSUPPORTED_ACCESS                  ((esc_uint32_t)0x06010000u) //!< \brief Unsupported access to an object
#define ESC_SDO_ABORT_CODE__ATTEMPT_READ_OF_WRITE_ONLY_OBJECT   ((esc_uint32_t)0x06010001u) //!< \brief Attempt to read to a write only object
#define ESC_SDO_ABORT_CODE__ATTEMPT_WRITE_OF_READ_ONLY_OBJECT   ((esc_uint32_t)0x06010002u) //!< \brief Attempt to write to a read only object
#define ESC_SDO_ABORT_CODE__SUBINDEX_CANNOT_BE_WRITTEN          ((esc_uint32_t)0x06010003u) //!< \brief Subindex cannot be written, SI0 must be 0 for write access
#define ESC_SDO_ABORT_CODE__SDO_COMPLETE_ACCESS_NOT_SUPPORTED   ((esc_uint32_t)0x06010004u) //!< \brief SDO Complete access not supported for objects of variable length 
                                                                                            //          such as ENUM object types
#define ESC_SDO_ABORT_CODE__OBJECT_LENGTH_EXCEED_MAILBOX_SIZE   ((esc_uint32_t)0x06010005u) //!< \brief Object length exceeds mailbox size
#define ESC_SDO_ABORT_CODE__OBJECT_MAPPED_SDO_DOWNLOAD_BLOCKED  ((esc_uint32_t)0x06010006u) //!< \brief Object mapped to RxPDO, SDO Download blocked
#define ESC_SDO_ABORT_CODE__OBJECT_DOES_NOT_EXIST               ((esc_uint32_t)0x06020000u) //!< \brief The object does not exist in the object directory
#define ESC_SDO_ABORT_CODE__OBJECT_CAN_NOT_BE_MAPPED            ((esc_uint32_t)0x06040041u) //!< \brief The object can not be mapped into the PDO
#define ESC_SDO_ABORT_CODE__EXCEED_PDO_LENGTH                   ((esc_uint32_t)0x06040042u) //!< \brief The number and length of the objects to be mapped would exceed the 
                                                                                            //          PDO length
#define ESC_SDO_ABORT_CODE__PARAMETER_INCOMPATIBILITY           ((esc_uint32_t)0x06040043u) //!< \brief General parameter incompatibility reason
#define ESC_SDO_ABORT_CODE__INTERNAL_INCMPATIBILITY             ((esc_uint32_t)0x06040047u) //!< \brief General internal incompatibility in the device
#define ESC_SDO_ABORT_CODE__ACCESS_FAILED_DUE_TO_HARDWARE_ERROR ((esc_uint32_t)0x06060000u) //!< \brief Access failed due to a hardware error
#define ESC_SDO_ABORT_CODE__DATA_TYPE_MISMATCH                  ((esc_uint32_t)0x06070010u) //!< \brief Data type does not match, length of service parameter does not match
#define ESC_SDO_ABORT_CODE__DATA_TYPE_MISMATCH_TOO_HIGH         ((esc_uint32_t)0x06070012u) //!< \brief Data type does not match, length of service parameter too high
#define ESC_SDO_ABORT_CODE__DATA_TYPE_MISMATCH_TOO_LOW          ((esc_uint32_t)0x06070013u) //!< \brief Data type does not match, length of service parameter too low
#define ESC_SDO_ABORT_CODE__SUBINDEX_DOES_NOT_EXIST             ((esc_uint32_t)0x06090011u) //!< \brief Subindex does not exist
#define ESC_SDO_ABORT_CODE__VALUE_RANGE_EXCEEDED                ((esc_uint32_t)0x06090030u) //!< \brief Value range of parameter exceeded (only for write access)
#define ESC_SDO_ABORT_CODE__VALUE_WRITTEN_TO_HIGH               ((esc_uint32_t)0x06090031u) //!< \brief Value of parameter written too high
#define ESC_SDO_ABORT_CODE__VALUE_WRITTEN_TO_LOW                ((esc_uint32_t)0x06090032u) //!< \brief Value of parameter written too low
#define ESC_SDO_ABORT_CODE__MAXIMUM_LESS_THAN_MINIMUM           ((esc_uint32_t)0x06090036u) //!< \brief Maximum value is less than minimum value
#define ESC_SDO_ABORT_CODE__GENERAL_ERROR                       ((esc_uint32_t)0x08000000u) //!< \brief General error
#define ESC_SDO_ABORT_CODE__DATA_CANNOT_BE_STORED               ((esc_uint32_t)0x08000020u) //!< \brief Data cannot be transferred or stored to the application 
                                                                                            //          NOTE: This is the general Abort Code in case no further #
                                                                                            //          detail on the reason can determined. It is recommended to 
                                                                                            //          use one of the more detailed Abort Codes (0x08000021, 0x08000022)
#define ESC_SDO_ABORT_CODE__DATA_CANNOT_BE_STORED_CONTROL       ((esc_uint32_t)0x08000021u) //!< \brief Data cannot be transferred or stored to the application because of 
                                                                                            //          local control 
                                                                                            //          NOTE: “local control” means an application specific reason. It does 
                                                                                            //          not mean the ESM-specific control
#define ESC_SDO_ABORT_CODE__DATA_CANNOT_BE_STORED_DEVICE_STATE  ((esc_uint32_t)0x08000022u) //!< \brief Data cannot be transferred or stored to the application because of 
                                                                                            //          the present device state 
                                                                                            //          NOTE: “device state” means the ESM state
#define ESC_SDO_ABORT_CODE__DYNAMIC_GENEARATION_FAILED          ((esc_uint32_t)0x08000023u) //!< \brief Object dictionary dynamic generation fails or no object dictionary 
                                                                                            //          is present
typedef esc_uint32_t    esc_obj_retval_t;

// ===========================
// ===== SDO STRUCTURES ======
// ===========================

typedef struct esc_object_entry {
    esc_uint16_t        data_type;
    esc_uint16_t        bit_length;
    esc_uint16_t        object_access;  //!< \brief Object access.
    esc_char_t         *name;
    esc_void_t         *data;
} esc_object_entry_t;

typedef struct esc_object {
    esc_uint16_t        index;
    esc_uint16_t        data_type;      //!< \brief Reference to data type list
    esc_uint8_t         max_sub_index;  //!< \brief Maximum number of sub_indexes in the object.
    esc_uint8_t         object_code;    //!< \brief Object code.
    esc_char_t         *name;           //!< \brief Name of the object.
    esc_object_entry_t *entries;
} esc_object_t;

//! \brief Get object from dictionary.
/*!
 * \param[in]   pesc        Pointer to ESC structure.
 * \param[in]   index       Object index for return object.
 *
 * \return CoE object or NULL
 */
esc_object_t *esc_obj_get_object(esc_t *pesc, esc_uint16_t index);

//! \brief Write entry of object.
/*!
 * \param[in]   obj         Ponter to CANOpen object.
 * \param[in]   sub_index   Sub index to write.
 * \param[in]   al_state    Current slave AL status.
 * \param[in]   data        Pointer to data to write to object entry.
 * \param[in]   data_len    Length of data to write.
 *
 * \retval  ESC_OK                                          Writing object entry was successfull.
 * \retval  ESC_SDO_ABORT_CODE__SUBINDEX_DOES_NOT_EXIST     Sub index does not exist in object.
 * \retval  ESC_SDO_ABORT_CODE__UNSUPPORTED_ACCESS          Writing object not supported (in current al_state).
 * \retval  ESC_SDO_ABORT_CODE__DATA_TYPE_MISMATCH          Data length mismatch.
 */
esc_obj_retval_t esc_obj_write_object_entry(esc_object_t *obj, esc_uint8_t sub_index,
        esc_uint8_t al_state, esc_void_t *data, esc_size_t data_len);

//! \brief Write complete object (complete access)
/*!
 * \param[in]   obj         Ponter to CANOpen object.
 * \param[in]   al_state    Current slave AL status.
 * \param[in]   data        Pointer to data to write to object.
 * \param[in]   data_len    Length of data to write.
 *
 * \retval  ESC_OK      Writing object was successfull.
 */
esc_obj_retval_t esc_obj_write_object(esc_object_t *obj, esc_uint8_t al_state, 
        esc_void_t *data, esc_size_t data_len);

#endif // LIBESC_OBJECT_H

