/**
 * \file object.c
 *
 * \author Robert Burger <robert@burger-system.de>
 *
 * \date 15 Nov 2023
 *
 * \brief Object dictionary access source.
 *
 * Object dictionary access source.
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

#include <libecat-base/types.h>
#include <libecat-base/object.h>

#include <stdio.h>
#include <stddef.h>
#include <assert.h>
#include <string.h>

//! \brief Get object from dictionary.
/*!
 * \param[in]   pdict       Pointer to start of dictionary.
 * \param[in]   index       Object index for return object.
 *
 * \return CoE object or NULL
 */
struct ec_object *ec_obj_get_object(struct ec_object *pdict, ec_uint16_t index) {
    assert (pdict != NULL);
    struct ec_object *ret_obj = &pdict[0];

    while (1) {
        if (ret_obj->index == 0xFFFF) {
            ret_obj = NULL;
            break;
        }

        if (ret_obj->index == index) {
            break; 
        }

        ret_obj++;
    }
    
    return ret_obj;
}

//! \brief Get object entry length in bytes
/*!
 * \param[in]   obj         Ponter to CANOpen object.
 * \param[in]   sub_index   Sub index to calculate length.
 * \param[out]  len         Return calculated byte length.
 *
 * \retval  EC_OK                                          Writing object entry was successfull.
 * \retval  EC_SDO_ABORT_CODE__SUBINDEX_DOES_NOT_EXIST     Sub index does not exist in object.
 */
ec_abort_code ec_obj_get_length_object_entry(struct ec_object *obj, 
        ec_uint8_t sub_index, ec_size_t *len) 
{
    assert (obj != NULL);
    assert (len != NULL);

    ec_abort_code ret = EC_OK;

    if (sub_index > obj->max_sub_index) {
        ret = EC_SDO_ABORT_CODE__SUBINDEX_DOES_NOT_EXIST;
        *len = 0u;
    } else {
        if (    (obj->object_code == EC_SDO__OBJECT_CODE__RECORD) &&
                (sub_index == 0u) ) 
        {
            *len = 2u;
        } else {
            *len = (obj->entries[sub_index].bit_length + 7u) / 8u;
        }
    }

    return ret;
}

//! \brief Get complete object length in bytes
/*!
 * \param[in]   obj         Ponter to CANOpen object.
 * \param[in]   sub_index   Sub index to calculate length.
 * \param[out]  len         Return calculated byte length.
 *
 * \retval  EC_OK                                          Writing object entry was successfull.
 */
ec_abort_code ec_obj_get_length(struct ec_object *obj, ec_size_t *len) {
    assert (obj != NULL);
    assert (len != NULL);

    ec_abort_code ret = EC_OK;

    if (obj->object_code == EC_SDO__OBJECT_CODE__VARIABLE) {
        *len = obj->entries[0].bit_length;
    } else if (obj->object_code == EC_SDO__OBJECT_CODE__ARRAY) {
        *len = obj->entries[0].bit_length * obj->max_sub_index;
    } else { // EC_SDO__OBJECT_CODE__RECORD
        *len = 16; // sub_index 0 is always stored as 16-bit

        for (unsigned i = 1; i < obj->max_sub_index; ++i) {
            *len += obj->entries[i].bit_length;
        }
    }

    return ret;
}

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
        ec_uint8_t al_state, ec_void_t *data, ec_size_t data_len) 
{
    assert (obj != NULL);
    ec_abort_code ret = EC_OK;

    if (sub_index > obj->max_sub_index) {
        ret = EC_SDO_ABORT_CODE__SUBINDEX_DOES_NOT_EXIST;
    }

    if (ret == EC_OK) {                
        ec_uint16_t state_write_access = (obj->entries[sub_index].object_access & EC_SDO__OBJECT_ACCESS__WRITE) >> 2u;
        if ((state_write_access & al_state) == 0u) {
            ret = EC_SDO_ABORT_CODE__UNSUPPORTED_ACCESS;
        } 
    }

    if (ret == EC_OK) {
        if (data_len < ((obj->entries[sub_index].bit_length + 7u) / 8u)) {
            ret = EC_SDO_ABORT_CODE__DATA_TYPE_MISMATCH;
        }
    }

    if (ret == EC_OK) {
        memcpy(obj->entries[sub_index].data, data, ((obj->entries[sub_index].bit_length + 7u) / 8u));
    }

    return ret;
}

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
        ec_void_t *data, ec_size_t data_len) 
{
    assert (obj != NULL);
    ec_abort_code ret = EC_OK;

    ec_size_t obj_len = 0u;
    ret = ec_obj_get_length(obj, &obj_len);
    if (ret == EC_OK) {
        if (obj_len != data_len) {
            ret = EC_SDO_ABORT_CODE__DATA_TYPE_MISMATCH;
        }
    }

    if (ret == EC_OK) {
        ec_void_t *tmp_data = data;
        ec_size_t  act_bit_offset = 0u;

        for (unsigned sub = 0; sub < obj->max_sub_index; ++sub) {
            ec_size_t sub_byte_len = 0u;
            ec_size_t sub_bit_len = obj->entries[sub].bit_length;
            ec_obj_get_length_object_entry(obj, sub, &sub_byte_len);

            if (sub_byte_len != (sub_bit_len / 8u)) {
                // we are writing single bits
                ec_uint8_t tmp_bit_data = 0u;
                ec_uint8_t bitmask = (1 << sub_bit_len) -1;
                tmp_bit_data = (*(ec_uint8_t *)tmp_data >> act_bit_offset) & bitmask;

                ec_obj_write_object_entry(obj, sub, al_state, &tmp_bit_data, sub_byte_len);
                act_bit_offset += sub_bit_len;
                while (act_bit_offset > 7u) {
                    tmp_data++;
                    act_bit_offset -= 8u;
                }
                continue;
            }

            if (act_bit_offset != 0u) {
                // we need to align now, cause we are writing whole bytes
                tmp_data++;
                act_bit_offset = 0u;
            }

            ec_obj_write_object_entry(obj, sub, al_state, tmp_data, sub_byte_len);
            tmp_data += sub_byte_len;
        }
    }

    return ret;
}

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
        ec_uint8_t al_state, ec_void_t *data, ec_size_t data_len) 
{
    assert (obj != NULL);
    ec_abort_code ret = 0u;

    if (sub_index > obj->max_sub_index) {
        ret = EC_SDO_ABORT_CODE__SUBINDEX_DOES_NOT_EXIST;
    } else {
        ec_uint16_t state_read_access = (obj->entries[sub_index].object_access & EC_SDO__OBJECT_ACCESS__READ) << 1u;
        if ((state_read_access & al_state) == 0u) {
            ret = EC_SDO_ABORT_CODE__UNSUPPORTED_ACCESS;
        } 
    }

    if (ret == EC_OK) {
        if (data_len < ((obj->entries[sub_index].bit_length + 7u) / 8u)) {
            ret = EC_SDO_ABORT_CODE__OUT_OF_MEMORY;
        }
    }

    if (ret == EC_OK) {
        memcpy(data, obj->entries[sub_index].data, data_len);
    }

    return ret;
}

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
        ec_void_t *data, ec_size_t data_len) {
    assert (obj != NULL);
    ec_abort_code ret = EC_OK;

    ec_size_t obj_len = 0u;
    ret = ec_obj_get_length(obj, &obj_len);
    if (ret == EC_OK) {
        if (obj_len != data_len) {
            ret = EC_SDO_ABORT_CODE__DATA_TYPE_MISMATCH;
        }
    }

    if (ret == EC_OK) {
        ec_void_t *tmp_data = data;
        ec_size_t  act_bit_offset = 0u;

        for (unsigned sub = 0; sub < obj->max_sub_index; ++sub) {
            ec_size_t sub_byte_len = 0u;
            ec_size_t sub_bit_len = obj->entries[sub].bit_length;
            ec_obj_get_length_object_entry(obj, sub, &sub_byte_len);

            if (sub_byte_len != (sub_bit_len / 8u)) {
                // we are reading single bits
                ec_uint8_t tmp_bit_data = 0u;
                ec_uint8_t bitmask = (1 << sub_bit_len) -1;

                ec_obj_read_object_entry(obj, sub, al_state, &tmp_bit_data, sub_byte_len);
                *(ec_uint8_t *)tmp_data |= (tmp_bit_data & bitmask) << act_bit_offset;

                act_bit_offset += sub_bit_len;
                while (act_bit_offset > 7u) {
                    tmp_data++;
                    act_bit_offset -= 8u;
                }
                continue;
            }

            if (act_bit_offset != 0u) {
                // we need to align now, cause we are reading whole bytes
                tmp_data++;
                act_bit_offset = 0u;
            }

            ec_obj_read_object_entry(obj, sub, al_state, tmp_data, sub_byte_len);
            tmp_data += sub_byte_len;
        }
    }

    return ret;
}

#ifdef STANDALONE
#include "objects_gen.c"

void print_dict(void) {
    struct ec_object *obj = &objects[0];
    while (1) {
        if (obj->index == 0xFFFF) {
            printf("got end\n");
            break;
        }

        ec_size_t len;
        ec_obj_get_length(obj, &len);

        printf("got object 0x%X (%ld bits): %s\n", obj->index, len, obj->name);

        for (int i = 0; i <= obj->max_sub_index; ++i) {
            printf("  %d: %s => ", i, obj->entries[i].name);

            ec_uint8_t *tmp = (ec_uint8_t *)obj->entries[i].data;
            if (tmp) {
                for (int j = 0; j < ((obj->entries[i].bit_length+7)/8); ++j) {
                    printf("%02X", tmp[j]);
                }
            }

            printf("\n");
        }

        obj++;
    }
}

int main(int argc, char **argv) {
    ec_t esc;
    ec_abort_code ret = EC_OK;
    esc.mbx.coe.objects = &objects[0];
    print_dict();

    ec_uint8_t al_state = EC_STATE_OP;

    uint32_t tmp = 0xAFFEAFFEu;
    if ((ret = ec_obj_write_object_entry(ec_obj_get_object(&esc, 0x1018), 3, al_state, &tmp, sizeof(tmp))) != EC_OK) {
        printf("error -> 0x%X\n", ret); 
    }

    if ((ret = ec_obj_write_object_entry(ec_obj_get_object(&esc, 0x1018), 7, al_state, &tmp, sizeof(tmp))) != EC_OK) {
        printf("error -> 0x%X\n", ret); 
    }

    print_dict();
    

    return 0;
}
#endif
