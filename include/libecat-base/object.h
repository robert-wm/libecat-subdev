/**
 * vim: set nowrap:
 *
 * \file object.h
 *
 * \author Robert Burger <robert@burger-system.de>
 *
 * \date 08 Nov 2023
 *
 * \brief ESC object header.
 *
 * ESC object header.
 * 
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

#ifndef LIBECAT_BASE_OBJECT_H
#define LIBECAT_BASE_OBJECT_H

#include <libecat-base/types.h>

// ===========================
// ===== SDO DATA TYPES ======
// ===========================

#define EC_SDO__DATA_TYPE__BOOLEAN                             ((ec_uint16_t)    0x0001u) 
#define EC_SDO__DATA_TYPE__INTEGER8                            ((ec_uint16_t)    0x0002u) 
#define EC_SDO__DATA_TYPE__INTEGER16                           ((ec_uint16_t)    0x0003u) 
#define EC_SDO__DATA_TYPE__INTEGER32                           ((ec_uint16_t)    0x0004u) 
#define EC_SDO__DATA_TYPE__UNSIGNED8                           ((ec_uint16_t)    0x0005u) 
#define EC_SDO__DATA_TYPE__UNSIGNED16                          ((ec_uint16_t)    0x0006u) 
#define EC_SDO__DATA_TYPE__UNSIGNED32                          ((ec_uint16_t)    0x0007u) 
#define EC_SDO__DATA_TYPE__REAL32                              ((ec_uint16_t)    0x0008u) 
#define EC_SDO__DATA_TYPE__VISIBLE_STRING                      ((ec_uint16_t)    0x0009u) 
#define EC_SDO__DATA_TYPE__OCTET_STRING                        ((ec_uint16_t)    0x000Au) 
#define EC_SDO__DATA_TYPE__UNICODE_STRING                      ((ec_uint16_t)    0x000Bu) 
#define EC_SDO__DATA_TYPE__TIME_OF_DAY                         ((ec_uint16_t)    0x000Cu) 
#define EC_SDO__DATA_TYPE__TIME_DIFFERENCE                     ((ec_uint16_t)    0x000Du) 
#define EC_SDO__DATA_TYPE__DOMAIN                              ((ec_uint16_t)    0x000Fu) 
#define EC_SDO__DATA_TYPE__INTEGER24                           ((ec_uint16_t)    0x0010u) 
#define EC_SDO__DATA_TYPE__REAL64                              ((ec_uint16_t)    0x0011u) 
#define EC_SDO__DATA_TYPE__INTEGER40                           ((ec_uint16_t)    0x0012u) 
#define EC_SDO__DATA_TYPE__INTEGER48                           ((ec_uint16_t)    0x0013u) 
#define EC_SDO__DATA_TYPE__INTEGER56                           ((ec_uint16_t)    0x0014u) 
#define EC_SDO__DATA_TYPE__INTEGER64                           ((ec_uint16_t)    0x0015u) 
#define EC_SDO__DATA_TYPE__UNSIGNED24                          ((ec_uint16_t)    0x0016u) 
#define EC_SDO__DATA_TYPE__UNSIGNED40                          ((ec_uint16_t)    0x0018u) 
#define EC_SDO__DATA_TYPE__UNSIGNED48                          ((ec_uint16_t)    0x0019u) 
#define EC_SDO__DATA_TYPE__UNSIGNED56                          ((ec_uint16_t)    0x001Au) 
#define EC_SDO__DATA_TYPE__UNSIGNED64                          ((ec_uint16_t)    0x001Bu) 
#define EC_SDO__DATA_TYPE__GUID                                ((ec_uint16_t)    0x001Du) 
#define EC_SDO__DATA_TYPE__BYTE                                ((ec_uint16_t)    0x001Eu) 
#define EC_SDO__DATA_TYPE__BITARR8                             ((ec_uint16_t)    0x002Du) 
#define EC_SDO__DATA_TYPE__BITARR16                            ((ec_uint16_t)    0x002Eu) 
#define EC_SDO__DATA_TYPE__BITARR32                            ((ec_uint16_t)    0x002Fu) 
#define EC_SDO__DATA_TYPE__PDO_MAPPING                         ((ec_uint16_t)    0x0021u)
#define EC_SDO__DATA_TYPE__IDENTITY                            ((ec_uint16_t)    0x0023u)
#define EC_SDO__DATA_TYPE__COMMAND_PAR                         ((ec_uint16_t)    0x0025u)
#define EC_SDO__DATA_TYPE__SYNC_PAR                            ((ec_uint16_t)    0x0029u)
#define EC_SDO__DATA_TYPE__BIT1                                ((ec_uint16_t)    0x0030u)
#define EC_SDO__DATA_TYPE__BIT2                                ((ec_uint16_t)    0x0031u)
#define EC_SDO__DATA_TYPE__BIT3                                ((ec_uint16_t)    0x0032u)
#define EC_SDO__DATA_TYPE__BIT4                                ((ec_uint16_t)    0x0033u)
#define EC_SDO__DATA_TYPE__BIT5                                ((ec_uint16_t)    0x0034u)
#define EC_SDO__DATA_TYPE__BIT6                                ((ec_uint16_t)    0x0035u)
#define EC_SDO__DATA_TYPE__BIT7                                ((ec_uint16_t)    0x0036u)
#define EC_SDO__DATA_TYPE__BIT8                                ((ec_uint16_t)    0x0037u)

// ===========================
// ==== SDO OBJECT CODES =====
// ===========================

#define EC_SDO__OBJECT_CODE__VARIABLE                          ((ec_uint8_t )         7u) //!< \brief Variable
#define EC_SDO__OBJECT_CODE__ARRAY                             ((ec_uint8_t )         8u) //!< \brief Array
#define EC_SDO__OBJECT_CODE__RECORD                            ((ec_uint8_t )         9u) //!< \brief Record

// ===========================
// ==== SDO OBJECT ACCESS ====
// ===========================

#define EC_SDO__OBJECT_ACCESS__READ_PREOP                      ((ec_uint16_t)    0x0001u) //!< \brief Bit 0: read access in Pre-Operational state
#define EC_SDO__OBJECT_ACCESS__READ_SAFEOP                     ((ec_uint16_t)    0x0002u) //!< \brief Bit 1: read access in Safe-Operational state
#define EC_SDO__OBJECT_ACCESS__READ_OP                         ((ec_uint16_t)    0x0004u) //!< \brief Bit 2: read access in Operational state
#define EC_SDO__OBJECT_ACCESS__READ                            ((ec_uint16_t)    0x0007u) //!< \brief Bit 0-2: read access in Pre-Operational-Operational state
#define EC_SDO__OBJECT_ACCESS__WRITE_PREOP                     ((ec_uint16_t)    0x0008u) //!< \brief Bit 3: write access in Pre-Operational state
#define EC_SDO__OBJECT_ACCESS__WRITE_SAFEOP                    ((ec_uint16_t)    0x0010u) //!< \brief Bit 4: write access in Safe-Operational state
#define EC_SDO__OBJECT_ACCESS__WRITE_OP                        ((ec_uint16_t)    0x0020u) //!< \brief Bit 5: write access in Operational state
#define EC_SDO__OBJECT_ACCESS__WRITE                           ((ec_uint16_t)    0x0038u) //!< \brief Bit 3-5: write access in Pre-Operational-Operational state
#define EC_SDO__OBJECT_ACCESS__RXPDO_MAPPABLE                  ((ec_uint16_t)    0x0040u) //!< \brief Bit 6: object is mappable in a RxPDO
#define EC_SDO__OBJECT_ACCESS__TXPDO_MAPPABLE                  ((ec_uint16_t)    0x0080u) //!< \brief Bit 7: object is mappable in a TxPDO
#define EC_SDO__OBJECT_ACCESS__MAPPABLE                        ((ec_uint16_t)    0x00C0u) //!< \brief Bit 6-7: object is mappable in a TxPDO
#define EC_SDO__OBJECT_ACCESS__BACKUP                          ((ec_uint16_t)    0x0100u) //!< \brief Bit 8: object can be used for backup
#define EC_SDO__OBJECT_ACCESS__SETTINGS                        ((ec_uint16_t)    0x0200u) //!< \brief Bit 9: object can be used for settings

// ===========================
// ===== SDO ABORT CODES =====
// ===========================

#define EC_SDO_ABORT_CODE__TOGGLE_NOT_CHANGED                  ((ec_uint32_t)0x05030000u) //!< \brief Toggle bit not changed
#define EC_SDO_ABORT_CODE__SDO_TIMEOUT                         ((ec_uint32_t)0x05040000u) //!< \brief SDO protocol timeout
#define EC_SDO_ABORT_CODE__COMMAND_NOT_VALID                   ((ec_uint32_t)0x05040001u) //!< \brief Client/Server command specifier not valid or unknown
#define EC_SDO_ABORT_CODE__OUT_OF_MEMORY                       ((ec_uint32_t)0x05040005u) //!< \brief Out of memory
#define EC_SDO_ABORT_CODE__UNSUPPORTED_ACCESS                  ((ec_uint32_t)0x06010000u) //!< \brief Unsupported access to an object
#define EC_SDO_ABORT_CODE__ATTEMPT_READ_OF_WRITE_ONLY_OBJECT   ((ec_uint32_t)0x06010001u) //!< \brief Attempt to read to a write only object
#define EC_SDO_ABORT_CODE__ATTEMPT_WRITE_OF_READ_ONLY_OBJECT   ((ec_uint32_t)0x06010002u) //!< \brief Attempt to write to a read only object
#define EC_SDO_ABORT_CODE__SUBINDEX_CANNOT_BE_WRITTEN          ((ec_uint32_t)0x06010003u) //!< \brief Subindex cannot be written, SI0 must be 0 for write access
#define EC_SDO_ABORT_CODE__SDO_COMPLETE_ACCESS_NOT_SUPPORTED   ((ec_uint32_t)0x06010004u) //!< \brief SDO Complete access not supported for objects of variable length 
                                                                                            //          such as ENUM object types
#define EC_SDO_ABORT_CODE__OBJECT_LENGTH_EXCEED_MAILBOX_SIZE   ((ec_uint32_t)0x06010005u) //!< \brief Object length exceeds mailbox size
#define EC_SDO_ABORT_CODE__OBJECT_MAPPED_SDO_DOWNLOAD_BLOCKED  ((ec_uint32_t)0x06010006u) //!< \brief Object mapped to RxPDO, SDO Download blocked
#define EC_SDO_ABORT_CODE__OBJECT_DOES_NOT_EXIST               ((ec_uint32_t)0x06020000u) //!< \brief The object does not exist in the object directory
#define EC_SDO_ABORT_CODE__OBJECT_CAN_NOT_BE_MAPPED            ((ec_uint32_t)0x06040041u) //!< \brief The object can not be mapped into the PDO
#define EC_SDO_ABORT_CODE__EXCEED_PDO_LENGTH                   ((ec_uint32_t)0x06040042u) //!< \brief The number and length of the objects to be mapped would exceed the 
                                                                                            //          PDO length
#define EC_SDO_ABORT_CODE__PARAMETER_INCOMPATIBILITY           ((ec_uint32_t)0x06040043u) //!< \brief General parameter incompatibility reason
#define EC_SDO_ABORT_CODE__INTERNAL_INCMPATIBILITY             ((ec_uint32_t)0x06040047u) //!< \brief General internal incompatibility in the device
#define EC_SDO_ABORT_CODE__ACCESS_FAILED_DUE_TO_HARDWARE_ERROR ((ec_uint32_t)0x06060000u) //!< \brief Access failed due to a hardware error
#define EC_SDO_ABORT_CODE__DATA_TYPE_MISMATCH                  ((ec_uint32_t)0x06070010u) //!< \brief Data type does not match, length of service parameter does not match
#define EC_SDO_ABORT_CODE__DATA_TYPE_MISMATCH_TOO_HIGH         ((ec_uint32_t)0x06070012u) //!< \brief Data type does not match, length of service parameter too high
#define EC_SDO_ABORT_CODE__DATA_TYPE_MISMATCH_TOO_LOW          ((ec_uint32_t)0x06070013u) //!< \brief Data type does not match, length of service parameter too low
#define EC_SDO_ABORT_CODE__SUBINDEX_DOES_NOT_EXIST             ((ec_uint32_t)0x06090011u) //!< \brief Subindex does not exist
#define EC_SDO_ABORT_CODE__VALUE_RANGE_EXCEEDED                ((ec_uint32_t)0x06090030u) //!< \brief Value range of parameter exceeded (only for write access)
#define EC_SDO_ABORT_CODE__VALUE_WRITTEN_TO_HIGH               ((ec_uint32_t)0x06090031u) //!< \brief Value of parameter written too high
#define EC_SDO_ABORT_CODE__VALUE_WRITTEN_TO_LOW                ((ec_uint32_t)0x06090032u) //!< \brief Value of parameter written too low
#define EC_SDO_ABORT_CODE__MAXIMUM_LESS_THAN_MINIMUM           ((ec_uint32_t)0x06090036u) //!< \brief Maximum value is less than minimum value
#define EC_SDO_ABORT_CODE__GENERAL_ERROR                       ((ec_uint32_t)0x08000000u) //!< \brief General error
#define EC_SDO_ABORT_CODE__DATA_CANNOT_BE_STORED               ((ec_uint32_t)0x08000020u) //!< \brief Data cannot be transferred or stored to the application 
                                                                                            //          NOTE: This is the general Abort Code in case no further #
                                                                                            //          detail on the reason can determined. It is recommended to 
                                                                                            //          use one of the more detailed Abort Codes (0x08000021, 0x08000022)
#define EC_SDO_ABORT_CODE__DATA_CANNOT_BE_STORED_CONTROL       ((ec_uint32_t)0x08000021u) //!< \brief Data cannot be transferred or stored to the application because of 
                                                                                            //          local control 
                                                                                            //          NOTE: “local control” means an application specific reason. It does 
                                                                                            //          not mean the ESM-specific control
#define EC_SDO_ABORT_CODE__DATA_CANNOT_BE_STORED_DEVICE_STATE  ((ec_uint32_t)0x08000022u) //!< \brief Data cannot be transferred or stored to the application because of 
                                                                                            //          the present device state 
                                                                                            //          NOTE: “device state” means the ESM state
#define EC_SDO_ABORT_CODE__DYNAMIC_GENEARATION_FAILED          ((ec_uint32_t)0x08000023u) //!< \brief Object dictionary dynamic generation fails or no object dictionary 
                                                                                            //          is present

// ===========================
// ===== SDO STRUCTURES ======
// ===========================

struct ec_object_entry {
    ec_uint16_t data_type;
    ec_uint16_t bit_length;
    ec_uint16_t object_access;  //!< \brief Object access.
    ec_char_t *name;
    ec_void_t *data;
};

struct ec_object {
    ec_uint16_t index;
    ec_uint16_t data_type;              //!< \brief Reference to data type list
    ec_uint8_t max_sub_index;           //!< \brief Maximum number of sub_indexes in the object.
    ec_uint8_t object_code;             //!< \brief Object code.
    ec_char_t *name;                    //!< \brief Name of the object.
    struct ec_object_entry *entries;
};

//! \brief Get object from dictionary.
/*!
 * \param[in]   pdict       Pointer to start of dictionary.
 * \param[in]   index       Object index for return object.
 *
 * \return CoE object or NULL
 */
struct ec_object *ec_obj_get_object(struct ec_object *pdict, ec_uint16_t index);

//! \brief Write entry of object.
/*!
 * \param[in]   obj         Ponter to CANOpen object.
 * \param[in]   sub_index   Sub index to write.
 * \param[in]   al_state    Current slave AL status.
 * \param[in]   data        Pointer to data to write to object entry.
 * \param[in]   data_len    Length of data to write.
 *
 * \retval  EC_OK                                          Writing object entry was successfull.
 * \retval  EC_SDO_ABORT_CODE__SUBINDEX_DOES_NOT_EXIST     Sub index does not exist in object.
 * \retval  EC_SDO_ABORT_CODE__UNSUPPORTED_ACCESS          Writing object not supported (in current al_state).
 * \retval  EC_SDO_ABORT_CODE__DATA_TYPE_MISMATCH          Data length mismatch.
 */
ec_abort_code ec_obj_write_object_entry(struct ec_object *obj, ec_uint8_t sub_index,
        ec_uint8_t al_state, ec_void_t *data, ec_size_t data_len);

//! \brief Write complete object (complete access)
/*!
 * \param[in]   obj         Ponter to CANOpen object.
 * \param[in]   al_state    Current slave AL status.
 * \param[in]   data        Pointer to data to write to object.
 * \param[in]   data_len    Length of data to write.
 *
 * \retval  EC_OK      Writing object was successfull.
 */
ec_abort_code ec_obj_write_object(struct ec_object *obj, ec_uint8_t al_state, 
        ec_void_t *data, ec_size_t data_len);

//! \brief Read entry of object.
/*!
 * \param[in]   obj         Ponter to CANOpen object.
 * \param[in]   sub_index   Sub index to read.
 * \param[in]   al_state    Current slave AL status.
 * \param[out   data        Pointer to data to read to object entry.
 * \param[in]   data_len    Length of data to read.
 *
 * \retval  EC_OK                                          Reading object entry was successfull.
 * \retval  EC_SDO_ABORT_CODE__SUBINDEX_DOES_NOT_EXIST     Sub index does not exist in object.
 * \retval  EC_SDO_ABORT_CODE__UNSUPPORTED_ACCESS          Reading object not supported (in current al_state).
 * \retval  EC_SDO_ABORT_CODE__DATA_TYPE_MISMATCH          Data length mismatch.
 */
ec_abort_code ec_obj_read_object_entry(struct ec_object *obj, ec_uint8_t sub_index,
        ec_uint8_t al_state, ec_void_t *data, ec_size_t data_len);

//! \brief Write complete object (complete access)
/*!
 * \param[in]   obj         Ponter to CANOpen object.
 * \param[in]   al_state    Current slave AL status.
 * \param[out]  data        Pointer to data to read to object.
 * \param[in]   data_len    Length of data to read.
 *
 * \retval  EC_OK      Writing object was successfull.
 */
ec_abort_code ec_obj_read_object(struct ec_object *obj, ec_uint8_t al_state, 
        ec_void_t *data, ec_size_t data_len);

#endif // LIBECAT_BASE_OBJECT_H

