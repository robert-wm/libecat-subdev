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

#ifndef LIBECAT_SUBDEV_OBJECT_H
#define LIBECAT_SUBDEV_OBJECT_H

#include <libecat-subdev/types.h>

// ===========================
// ===== SDO DATA TYPES ======
// ===========================

#define ECSD_SDO__DATA_TYPE__BOOLEAN                             ((ecsd_uint16_t)    0x0001u) 
#define ECSD_SDO__DATA_TYPE__INTEGER8                            ((ecsd_uint16_t)    0x0002u) 
#define ECSD_SDO__DATA_TYPE__INTEGER16                           ((ecsd_uint16_t)    0x0003u) 
#define ECSD_SDO__DATA_TYPE__INTEGER32                           ((ecsd_uint16_t)    0x0004u) 
#define ECSD_SDO__DATA_TYPE__UNSIGNED8                           ((ecsd_uint16_t)    0x0005u) 
#define ECSD_SDO__DATA_TYPE__UNSIGNED16                          ((ecsd_uint16_t)    0x0006u) 
#define ECSD_SDO__DATA_TYPE__UNSIGNED32                          ((ecsd_uint16_t)    0x0007u) 
#define ECSD_SDO__DATA_TYPE__REAL32                              ((ecsd_uint16_t)    0x0008u) 
#define ECSD_SDO__DATA_TYPE__VISIBLE_STRING                      ((ecsd_uint16_t)    0x0009u) 
#define ECSD_SDO__DATA_TYPE__OCTET_STRING                        ((ecsd_uint16_t)    0x000Au) 
#define ECSD_SDO__DATA_TYPE__UNICODE_STRING                      ((ecsd_uint16_t)    0x000Bu) 
#define ECSD_SDO__DATA_TYPE__TIME_OF_DAY                         ((ecsd_uint16_t)    0x000Cu) 
#define ECSD_SDO__DATA_TYPE__TIME_DIFFERENCE                     ((ecsd_uint16_t)    0x000Du) 
#define ECSD_SDO__DATA_TYPE__DOMAIN                              ((ecsd_uint16_t)    0x000Fu) 
#define ECSD_SDO__DATA_TYPE__INTEGER24                           ((ecsd_uint16_t)    0x0010u) 
#define ECSD_SDO__DATA_TYPE__REAL64                              ((ecsd_uint16_t)    0x0011u) 
#define ECSD_SDO__DATA_TYPE__INTEGER40                           ((ecsd_uint16_t)    0x0012u) 
#define ECSD_SDO__DATA_TYPE__INTEGER48                           ((ecsd_uint16_t)    0x0013u) 
#define ECSD_SDO__DATA_TYPE__INTEGER56                           ((ecsd_uint16_t)    0x0014u) 
#define ECSD_SDO__DATA_TYPE__INTEGER64                           ((ecsd_uint16_t)    0x0015u) 
#define ECSD_SDO__DATA_TYPE__UNSIGNED24                          ((ecsd_uint16_t)    0x0016u) 
#define ECSD_SDO__DATA_TYPE__UNSIGNED40                          ((ecsd_uint16_t)    0x0018u) 
#define ECSD_SDO__DATA_TYPE__UNSIGNED48                          ((ecsd_uint16_t)    0x0019u) 
#define ECSD_SDO__DATA_TYPE__UNSIGNED56                          ((ecsd_uint16_t)    0x001Au) 
#define ECSD_SDO__DATA_TYPE__UNSIGNED64                          ((ecsd_uint16_t)    0x001Bu) 
#define ECSD_SDO__DATA_TYPE__GUID                                ((ecsd_uint16_t)    0x001Du) 
#define ECSD_SDO__DATA_TYPE__BYTE                                ((ecsd_uint16_t)    0x001Eu) 
#define ECSD_SDO__DATA_TYPE__BITARR8                             ((ecsd_uint16_t)    0x002Du) 
#define ECSD_SDO__DATA_TYPE__BITARR16                            ((ecsd_uint16_t)    0x002Eu) 
#define ECSD_SDO__DATA_TYPE__BITARR32                            ((ecsd_uint16_t)    0x002Fu) 
#define ECSD_SDO__DATA_TYPE__PDO_MAPPING                         ((ecsd_uint16_t)    0x0021u)
#define ECSD_SDO__DATA_TYPE__IDENTITY                            ((ecsd_uint16_t)    0x0023u)
#define ECSD_SDO__DATA_TYPE__COMMAND_PAR                         ((ecsd_uint16_t)    0x0025u)
#define ECSD_SDO__DATA_TYPE__SYNC_PAR                            ((ecsd_uint16_t)    0x0029u)
#define ECSD_SDO__DATA_TYPE__BIT1                                ((ecsd_uint16_t)    0x0030u)
#define ECSD_SDO__DATA_TYPE__BIT2                                ((ecsd_uint16_t)    0x0031u)
#define ECSD_SDO__DATA_TYPE__BIT3                                ((ecsd_uint16_t)    0x0032u)
#define ECSD_SDO__DATA_TYPE__BIT4                                ((ecsd_uint16_t)    0x0033u)
#define ECSD_SDO__DATA_TYPE__BIT5                                ((ecsd_uint16_t)    0x0034u)
#define ECSD_SDO__DATA_TYPE__BIT6                                ((ecsd_uint16_t)    0x0035u)
#define ECSD_SDO__DATA_TYPE__BIT7                                ((ecsd_uint16_t)    0x0036u)
#define ECSD_SDO__DATA_TYPE__BIT8                                ((ecsd_uint16_t)    0x0037u)

// ===========================
// ==== SDO OBJECT CODES =====
// ===========================

#define ECSD_SDO__OBJECT_CODE__VARIABLE                          ((ecsd_uint8_t )         7u) //!< \brief Variable
#define ECSD_SDO__OBJECT_CODE__ARRAY                             ((ecsd_uint8_t )         8u) //!< \brief Array
#define ECSD_SDO__OBJECT_CODE__RECORD                            ((ecsd_uint8_t )         9u) //!< \brief Record

// ===========================
// ==== SDO OBJECT ACCESS ====
// ===========================

#define ECSD_SDO__OBJECT_ACCESS__READ_PREOP                      ((ecsd_uint16_t)    0x0001u) //!< \brief Bit 0: read access in Pre-Operational state
#define ECSD_SDO__OBJECT_ACCESS__READ_SAFEOP                     ((ecsd_uint16_t)    0x0002u) //!< \brief Bit 1: read access in Safe-Operational state
#define ECSD_SDO__OBJECT_ACCESS__READ_OP                         ((ecsd_uint16_t)    0x0004u) //!< \brief Bit 2: read access in Operational state
#define ECSD_SDO__OBJECT_ACCESS__READ                            ((ecsd_uint16_t)    0x0007u) //!< \brief Bit 0-2: read access in Pre-Operational-Operational state
#define ECSD_SDO__OBJECT_ACCESS__WRITE_PREOP                     ((ecsd_uint16_t)    0x0008u) //!< \brief Bit 3: write access in Pre-Operational state
#define ECSD_SDO__OBJECT_ACCESS__WRITE_SAFEOP                    ((ecsd_uint16_t)    0x0010u) //!< \brief Bit 4: write access in Safe-Operational state
#define ECSD_SDO__OBJECT_ACCESS__WRITE_OP                        ((ecsd_uint16_t)    0x0020u) //!< \brief Bit 5: write access in Operational state
#define ECSD_SDO__OBJECT_ACCESS__WRITE                           ((ecsd_uint16_t)    0x0038u) //!< \brief Bit 3-5: write access in Pre-Operational-Operational state
#define ECSD_SDO__OBJECT_ACCESS__RXPDO_MAPPABLE                  ((ecsd_uint16_t)    0x0040u) //!< \brief Bit 6: object is mappable in a RxPDO
#define ECSD_SDO__OBJECT_ACCESS__TXPDO_MAPPABLE                  ((ecsd_uint16_t)    0x0080u) //!< \brief Bit 7: object is mappable in a TxPDO
#define ECSD_SDO__OBJECT_ACCESS__MAPPABLE                        ((ecsd_uint16_t)    0x00C0u) //!< \brief Bit 6-7: object is mappable in a TxPDO
#define ECSD_SDO__OBJECT_ACCESS__BACKUP                          ((ecsd_uint16_t)    0x0100u) //!< \brief Bit 8: object can be used for backup
#define ECSD_SDO__OBJECT_ACCESS__SETTINGS                        ((ecsd_uint16_t)    0x0200u) //!< \brief Bit 9: object can be used for settings

// ===========================
// ===== SDO ABORT CODES =====
// ===========================

#define ECSD_SDO_ABORT_CODE__TOGGLE_NOT_CHANGED                  ((ecsd_uint32_t)0x05030000u) //!< \brief Toggle bit not changed
#define ECSD_SDO_ABORT_CODE__SDO_TIMEOUT                         ((ecsd_uint32_t)0x05040000u) //!< \brief SDO protocol timeout
#define ECSD_SDO_ABORT_CODE__COMMAND_NOT_VALID                   ((ecsd_uint32_t)0x05040001u) //!< \brief Client/Server command specifier not valid or unknown
#define ECSD_SDO_ABORT_CODE__OUT_OF_MEMORY                       ((ecsd_uint32_t)0x05040005u) //!< \brief Out of memory
#define ECSD_SDO_ABORT_CODE__UNSUPPORTED_ACCESS                  ((ecsd_uint32_t)0x06010000u) //!< \brief Unsupported access to an object
#define ECSD_SDO_ABORT_CODE__ATTEMPT_READ_OF_WRITE_ONLY_OBJECT   ((ecsd_uint32_t)0x06010001u) //!< \brief Attempt to read to a write only object
#define ECSD_SDO_ABORT_CODE__ATTEMPT_WRITE_OF_READ_ONLY_OBJECT   ((ecsd_uint32_t)0x06010002u) //!< \brief Attempt to write to a read only object
#define ECSD_SDO_ABORT_CODE__SUBINDEX_CANNOT_BE_WRITTEN          ((ecsd_uint32_t)0x06010003u) //!< \brief Subindex cannot be written, SI0 must be 0 for write access
#define ECSD_SDO_ABORT_CODE__SDO_COMPLETE_ACCESS_NOT_SUPPORTED   ((ecsd_uint32_t)0x06010004u) //!< \brief SDO Complete access not supported for objects of variable length 
                                                                                            //          such as ENUM object types
#define ECSD_SDO_ABORT_CODE__OBJECT_LENGTH_EXCEED_MAILBOX_SIZE   ((ecsd_uint32_t)0x06010005u) //!< \brief Object length exceeds mailbox size
#define ECSD_SDO_ABORT_CODE__OBJECT_MAPPED_SDO_DOWNLOAD_BLOCKED  ((ecsd_uint32_t)0x06010006u) //!< \brief Object mapped to RxPDO, SDO Download blocked
#define ECSD_SDO_ABORT_CODE__OBJECT_DOES_NOT_EXIST               ((ecsd_uint32_t)0x06020000u) //!< \brief The object does not exist in the object directory
#define ECSD_SDO_ABORT_CODE__OBJECT_CAN_NOT_BE_MAPPED            ((ecsd_uint32_t)0x06040041u) //!< \brief The object can not be mapped into the PDO
#define ECSD_SDO_ABORT_CODE__EXCEED_PDO_LENGTH                   ((ecsd_uint32_t)0x06040042u) //!< \brief The number and length of the objects to be mapped would exceed the 
                                                                                            //          PDO length
#define ECSD_SDO_ABORT_CODE__PARAMETER_INCOMPATIBILITY           ((ecsd_uint32_t)0x06040043u) //!< \brief General parameter incompatibility reason
#define ECSD_SDO_ABORT_CODE__INTERNAL_INCMPATIBILITY             ((ecsd_uint32_t)0x06040047u) //!< \brief General internal incompatibility in the device
#define ECSD_SDO_ABORT_CODE__ACCESS_FAILED_DUE_TO_HARDWARE_ERROR ((ecsd_uint32_t)0x06060000u) //!< \brief Access failed due to a hardware error
#define ECSD_SDO_ABORT_CODE__DATA_TYPE_MISMATCH                  ((ecsd_uint32_t)0x06070010u) //!< \brief Data type does not match, length of service parameter does not match
#define ECSD_SDO_ABORT_CODE__DATA_TYPE_MISMATCH_TOO_HIGH         ((ecsd_uint32_t)0x06070012u) //!< \brief Data type does not match, length of service parameter too high
#define ECSD_SDO_ABORT_CODE__DATA_TYPE_MISMATCH_TOO_LOW          ((ecsd_uint32_t)0x06070013u) //!< \brief Data type does not match, length of service parameter too low
#define ECSD_SDO_ABORT_CODE__SUBINDEX_DOES_NOT_EXIST             ((ecsd_uint32_t)0x06090011u) //!< \brief Subindex does not exist
#define ECSD_SDO_ABORT_CODE__VALUE_RANGE_EXCEEDED                ((ecsd_uint32_t)0x06090030u) //!< \brief Value range of parameter exceeded (only for write access)
#define ECSD_SDO_ABORT_CODE__VALUE_WRITTEN_TO_HIGH               ((ecsd_uint32_t)0x06090031u) //!< \brief Value of parameter written too high
#define ECSD_SDO_ABORT_CODE__VALUE_WRITTEN_TO_LOW                ((ecsd_uint32_t)0x06090032u) //!< \brief Value of parameter written too low
#define ECSD_SDO_ABORT_CODE__MAXIMUM_LESS_THAN_MINIMUM           ((ecsd_uint32_t)0x06090036u) //!< \brief Maximum value is less than minimum value
#define ECSD_SDO_ABORT_CODE__GENERAL_ERROR                       ((ecsd_uint32_t)0x08000000u) //!< \brief General error
#define ECSD_SDO_ABORT_CODE__DATA_CANNOT_BE_STORED               ((ecsd_uint32_t)0x08000020u) //!< \brief Data cannot be transferred or stored to the application 
                                                                                            //          NOTE: This is the general Abort Code in case no further #
                                                                                            //          detail on the reason can determined. It is recommended to 
                                                                                            //          use one of the more detailed Abort Codes (0x08000021, 0x08000022)
#define ECSD_SDO_ABORT_CODE__DATA_CANNOT_BE_STORED_CONTROL       ((ecsd_uint32_t)0x08000021u) //!< \brief Data cannot be transferred or stored to the application because of 
                                                                                            //          local control 
                                                                                            //          NOTE: “local control” means an application specific reason. It does 
                                                                                            //          not mean the ESM-specific control
#define ECSD_SDO_ABORT_CODE__DATA_CANNOT_BE_STORED_DEVICE_STATE  ((ecsd_uint32_t)0x08000022u) //!< \brief Data cannot be transferred or stored to the application because of 
                                                                                            //          the present device state 
                                                                                            //          NOTE: “device state” means the ESM state
#define ECSD_SDO_ABORT_CODE__DYNAMIC_GENEARATION_FAILED          ((ecsd_uint32_t)0x08000023u) //!< \brief Object dictionary dynamic generation fails or no object dictionary 
                                                                                            //          is present
typedef ecsd_uint32_t    ecsd_obj_retval_t;

// ===========================
// ===== SDO STRUCTURES ======
// ===========================

typedef struct ecsd_object_entry {
    ecsd_uint16_t        data_type;
    ecsd_uint16_t        bit_length;
    ecsd_uint16_t        object_access;  //!< \brief Object access.
    ecsd_char_t         *name;
    ecsd_void_t         *data;
} ecsd_object_entry_t;

typedef struct ecsd_object {
    ecsd_uint16_t        index;
    ecsd_uint16_t        data_type;      //!< \brief Reference to data type list
    ecsd_uint8_t         max_sub_index;  //!< \brief Maximum number of sub_indexes in the object.
    ecsd_uint8_t         object_code;    //!< \brief Object code.
    ecsd_char_t         *name;           //!< \brief Name of the object.
    ecsd_object_entry_t *entries;
} ecsd_object_t;

//! \brief Get object from dictionary.
/*!
 * \param[in]   pesc        Pointer to ESC structure.
 * \param[in]   index       Object index for return object.
 *
 * \return CoE object or NULL
 */
ecsd_object_t *ecsd_obj_get_object(ecsd_t *pesc, ecsd_uint16_t index);

//! \brief Write entry of object.
/*!
 * \param[in]   obj         Ponter to CANOpen object.
 * \param[in]   sub_index   Sub index to write.
 * \param[in]   al_state    Current slave AL status.
 * \param[in]   data        Pointer to data to write to object entry.
 * \param[in]   data_len    Length of data to write.
 *
 * \retval  ECSD_OK                                          Writing object entry was successfull.
 * \retval  ECSD_SDO_ABORT_CODE__SUBINDEX_DOES_NOT_EXIST     Sub index does not exist in object.
 * \retval  ECSD_SDO_ABORT_CODE__UNSUPPORTED_ACCESS          Writing object not supported (in current al_state).
 * \retval  ECSD_SDO_ABORT_CODE__DATA_TYPE_MISMATCH          Data length mismatch.
 */
ecsd_obj_retval_t ecsd_obj_write_object_entry(ecsd_object_t *obj, ecsd_uint8_t sub_index,
        ecsd_uint8_t al_state, ecsd_void_t *data, ecsd_size_t data_len);

//! \brief Write complete object (complete access)
/*!
 * \param[in]   obj         Ponter to CANOpen object.
 * \param[in]   al_state    Current slave AL status.
 * \param[in]   data        Pointer to data to write to object.
 * \param[in]   data_len    Length of data to write.
 *
 * \retval  ECSD_OK      Writing object was successfull.
 */
ecsd_obj_retval_t ecsd_obj_write_object(ecsd_object_t *obj, ecsd_uint8_t al_state, 
        ecsd_void_t *data, ecsd_size_t data_len);

#endif // LIBECAT_SUBDEV_OBJECT_H

