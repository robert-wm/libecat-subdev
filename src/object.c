#include <libesc/types.h>
#include <libesc/esc.h>
#include <libesc/object.h>
#include <stdio.h>
#include <stddef.h>
#include <assert.h>
#include <string.h>

#include "objects_gen.c"

//! \brief Get object from dictionary.
/*!
 * \param[in]   pesc        Pointer to ESC structure.
 * \param[in]   index       Object index for return object.
 *
 * \return CoE object or NULL
 */
esc_object_t *esc_obj_get_object(esc_t *pesc, esc_uint16_t index) {
    assert (pesc != NULL);
    esc_object_t *ret_obj = &pesc->mbx.coe.objects[0];

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
 * \retval  ESC_OK                                          Writing object entry was successfull.
 * \retval  ESC_SDO_ABORT_CODE__SUBINDEX_DOES_NOT_EXIST     Sub index does not exist in object.
 */
esc_obj_retval_t esc_obj_get_length_object_entry(esc_object_t *obj, 
        esc_uint8_t sub_index, esc_size_t *len) 
{
    assert (obj != NULL);
    assert (len != NULL);

    esc_obj_retval_t ret = ESC_OK;

    if (sub_index > obj->max_sub_index) {
        ret = ESC_SDO_ABORT_CODE__SUBINDEX_DOES_NOT_EXIST;
        *len = 0u;
    } else {
        if (    (obj->object_code == ESC_SDO__OBJECT_CODE__RECORD) &&
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
 * \retval  ESC_OK                                          Writing object entry was successfull.
 */
esc_obj_retval_t esc_obj_get_length(esc_object_t *obj, esc_size_t *len) {
    assert (obj != NULL);
    assert (len != NULL);

    esc_obj_retval_t ret = ESC_OK;

    if (obj->object_code == ESC_SDO__OBJECT_CODE__VARIABLE) {
        *len = obj->entries[0].bit_length;
    } else if (obj->object_code == ESC_SDO__OBJECT_CODE__ARRAY) {
        *len = obj->entries[0].bit_length * obj->max_sub_index;
    } else { // ESC_SDO__OBJECT_CODE__RECORD
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
 * \retval  ESC_OK                                          Writing object entry was successfull.
 * \retval  ESC_SDO_ABORT_CODE__SUBINDEX_DOES_NOT_EXIST     Sub index does not exist in object.
 * \retval  ESC_SDO_ABORT_CODE__UNSUPPORTED_ACCESS          Writing object not supported (in current al_state).
 * \retval  ESC_SDO_ABORT_CODE__DATA_TYPE_MISMATCH          Data length mismatch.
 */
esc_obj_retval_t esc_obj_write_object_entry(esc_object_t *obj, esc_uint8_t sub_index, 
        esc_uint8_t al_state, esc_void_t *data, esc_size_t data_len) 
{
    assert (obj != NULL);
    esc_obj_retval_t ret = ESC_OK;

    if (sub_index > obj->max_sub_index) {
        ret = ESC_SDO_ABORT_CODE__SUBINDEX_DOES_NOT_EXIST;
    }

    if (ret == ESC_OK) {                
        esc_uint16_t state_write_access = (obj->entries[sub_index].object_access & ESC_SDO__OBJECT_ACCESS__WRITE) >> 2u;
        if ((state_write_access & al_state) == 0u) {
            ret = ESC_SDO_ABORT_CODE__UNSUPPORTED_ACCESS;
        } 
    }

    if (ret == ESC_OK) {
        if (data_len < ((obj->entries[sub_index].bit_length + 7u) / 8u)) {
            ret = ESC_SDO_ABORT_CODE__DATA_TYPE_MISMATCH;
        }
    }

    if (ret == ESC_OK) {
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
 * \retval  ESC_OK      Writing object was successfull.
 */
esc_obj_retval_t esc_obj_write_object(esc_object_t *obj, esc_uint8_t al_state, 
        esc_void_t *data, esc_size_t data_len) 
{
    assert (obj != NULL);
    esc_obj_retval_t ret = ESC_OK;

    esc_size_t obj_len = 0u;
    ret = esc_obj_get_length(obj, &obj_len);
    if (ret == ESC_OK) {
        if (obj_len != data_len) {
            ret = ESC_SDO_ABORT_CODE__DATA_TYPE_MISMATCH;
        }
    }

    if (ret == ESC_OK) {
        esc_void_t *tmp_data = data;
        esc_size_t  act_bit_offset = 0u;

        for (unsigned sub = 0; sub < obj->max_sub_index; ++sub) {
            esc_size_t sub_byte_len = 0u;
            esc_size_t sub_bit_len = obj->entries[sub].bit_length;
            esc_obj_get_length_object_entry(obj, sub, &sub_byte_len);

            if (sub_byte_len != (sub_bit_len / 8u)) {
                // we are writing single bits
                esc_uint8_t tmp_bit_data = 0u;
                esc_uint8_t bitmask = (1 << sub_bit_len) -1;
                tmp_bit_data = (*(esc_uint8_t *)tmp_data >> act_bit_offset) & bitmask;

                esc_obj_write_object_entry(obj, sub, al_state, &tmp_bit_data, sub_byte_len);
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

            esc_obj_write_object_entry(obj, sub, al_state, tmp_data, sub_byte_len);
            tmp_data += sub_byte_len;
        }
    }

    return ret;
}

esc_obj_retval_t esc_obj_read_object_entry(esc_object_t *obj, esc_uint8_t sub_index,
        esc_uint8_t al_state, esc_void_t *data, esc_size_t data_len) 
{
    assert (obj != NULL);
    esc_obj_retval_t ret = 0u;

    if (sub_index > obj->max_sub_index) {
        ret = ESC_SDO_ABORT_CODE__SUBINDEX_DOES_NOT_EXIST;
    } else {
        esc_uint16_t state_read_access = (obj->entries[sub_index].object_access & ESC_SDO__OBJECT_ACCESS__READ) << 1u;
        if ((state_read_access & al_state) == 0u) {
            ret = ESC_SDO_ABORT_CODE__UNSUPPORTED_ACCESS;
        } 
    }

    if (ret == ESC_OK) {
        if (data_len < ((obj->entries[sub_index].bit_length + 7u) / 8u)) {
            ret = ESC_SDO_ABORT_CODE__OUT_OF_MEMORY;
        }
    }

    if (ret == ESC_OK) {
        memcpy(data, obj->entries[sub_index].data, data_len);
    }

    return ret;
}

esc_uint32_t esc_obj_read_object(esc_t *pesc, esc_uint16_t index, esc_uint8_t sub_index,
        esc_bool_t complete_access, esc_void_t *data, esc_size_t data_len) 
{
    assert (pesc != NULL);
    esc_uint32_t ret = 0u;
    esc_object_t *obj = esc_obj_get_object(pesc, index);

    if (obj != NULL) {
        ret = ESC_SDO_ABORT_CODE__OBJECT_DOES_NOT_EXIST;
    }

    if (ret == 0u) {
        if ((complete_access == ESC_TRUE) && (sub_index != 0)) {
            // Sub index has to 0 for complete access.
            ret = ESC_SDO_ABORT_CODE__UNSUPPORTED_ACCESS;
        }
    }

    if (ret == 0u) {
        if (complete_access == ESC_TRUE) {
            esc_off_t byte_offset = 0u;
            esc_off_t bit_offset = 0u;

            for (unsigned sub = 1u; sub < obj->max_sub_index; ++sub) {
                switch (obj->entries[sub].data_type) {
                    case ESC_SDO__DATA_TYPE__BOOLEAN:
                        break;
                    case ESC_SDO__DATA_TYPE__BYTE:
                    case ESC_SDO__DATA_TYPE__INTEGER8:
                    case ESC_SDO__DATA_TYPE__UNSIGNED8:
                        if (bit_offset != 0u) { bit_offset = 0u; byte_offset++; }

                        *(esc_int8_t *)(data + byte_offset++) = *(esc_int8_t *)(obj->entries[sub].data);
                        break;
                    case ESC_SDO__DATA_TYPE__INTEGER16:
                    case ESC_SDO__DATA_TYPE__UNSIGNED16:
                        if (bit_offset != 0u) { bit_offset = 0u; byte_offset++; }

                        *(esc_int8_t *)(data + byte_offset++) = *(esc_int8_t *)(obj->entries[sub].data);
                        *(esc_int8_t *)(data + byte_offset++) = *(esc_int8_t *)(obj->entries[sub].data);
                        break;
                    case ESC_SDO__DATA_TYPE__INTEGER24:
                    case ESC_SDO__DATA_TYPE__UNSIGNED24:
                        if (bit_offset != 0u) { bit_offset = 0u; byte_offset++; }

                        *(esc_int8_t *)(data + byte_offset++) = *(esc_int8_t *)(obj->entries[sub].data);
                        *(esc_int8_t *)(data + byte_offset++) = *(esc_int8_t *)(obj->entries[sub].data);
                        *(esc_int8_t *)(data + byte_offset++) = *(esc_int8_t *)(obj->entries[sub].data);
                        break;
                    case ESC_SDO__DATA_TYPE__INTEGER32:
                    case ESC_SDO__DATA_TYPE__UNSIGNED32:
                    case ESC_SDO__DATA_TYPE__REAL32:
                        if (bit_offset != 0u) { bit_offset = 0u; byte_offset++; }

                        *(esc_int8_t *)(data + byte_offset++) = *(esc_int8_t *)(obj->entries[sub].data);
                        *(esc_int8_t *)(data + byte_offset++) = *(esc_int8_t *)(obj->entries[sub].data);
                        *(esc_int8_t *)(data + byte_offset++) = *(esc_int8_t *)(obj->entries[sub].data);
                        *(esc_int8_t *)(data + byte_offset++) = *(esc_int8_t *)(obj->entries[sub].data);
                        break;
                    case ESC_SDO__DATA_TYPE__INTEGER40:
                    case ESC_SDO__DATA_TYPE__UNSIGNED40:
                        if (bit_offset != 0u) { bit_offset = 0u; byte_offset++; }

                        *(esc_int8_t *)(data + byte_offset++) = *(esc_int8_t *)(obj->entries[sub].data);
                        *(esc_int8_t *)(data + byte_offset++) = *(esc_int8_t *)(obj->entries[sub].data);
                        *(esc_int8_t *)(data + byte_offset++) = *(esc_int8_t *)(obj->entries[sub].data);
                        *(esc_int8_t *)(data + byte_offset++) = *(esc_int8_t *)(obj->entries[sub].data);
                        *(esc_int8_t *)(data + byte_offset++) = *(esc_int8_t *)(obj->entries[sub].data);
                        break;
                    case ESC_SDO__DATA_TYPE__INTEGER48:
                    case ESC_SDO__DATA_TYPE__UNSIGNED48:
                        if (bit_offset != 0u) { bit_offset = 0u; byte_offset++; }

                        *(esc_int8_t *)(data + byte_offset++) = *(esc_int8_t *)(obj->entries[sub].data);
                        *(esc_int8_t *)(data + byte_offset++) = *(esc_int8_t *)(obj->entries[sub].data);
                        *(esc_int8_t *)(data + byte_offset++) = *(esc_int8_t *)(obj->entries[sub].data);
                        *(esc_int8_t *)(data + byte_offset++) = *(esc_int8_t *)(obj->entries[sub].data);
                        *(esc_int8_t *)(data + byte_offset++) = *(esc_int8_t *)(obj->entries[sub].data);
                        *(esc_int8_t *)(data + byte_offset++) = *(esc_int8_t *)(obj->entries[sub].data);
                        break;
                    case ESC_SDO__DATA_TYPE__INTEGER56:
                    case ESC_SDO__DATA_TYPE__UNSIGNED56:
                        if (bit_offset != 0u) { bit_offset = 0u; byte_offset++; }

                        *(esc_int8_t *)(data + byte_offset++) = *(esc_int8_t *)(obj->entries[sub].data);
                        *(esc_int8_t *)(data + byte_offset++) = *(esc_int8_t *)(obj->entries[sub].data);
                        *(esc_int8_t *)(data + byte_offset++) = *(esc_int8_t *)(obj->entries[sub].data);
                        *(esc_int8_t *)(data + byte_offset++) = *(esc_int8_t *)(obj->entries[sub].data);
                        *(esc_int8_t *)(data + byte_offset++) = *(esc_int8_t *)(obj->entries[sub].data);
                        *(esc_int8_t *)(data + byte_offset++) = *(esc_int8_t *)(obj->entries[sub].data);
                        *(esc_int8_t *)(data + byte_offset++) = *(esc_int8_t *)(obj->entries[sub].data);
                        break;
                    case ESC_SDO__DATA_TYPE__INTEGER64:
                    case ESC_SDO__DATA_TYPE__UNSIGNED64:
                    case ESC_SDO__DATA_TYPE__REAL64:
                        if (bit_offset != 0u) { bit_offset = 0u; byte_offset++; }

                        *(esc_int8_t *)(data + byte_offset++) = *(esc_int8_t *)(obj->entries[sub].data);
                        *(esc_int8_t *)(data + byte_offset++) = *(esc_int8_t *)(obj->entries[sub].data);
                        *(esc_int8_t *)(data + byte_offset++) = *(esc_int8_t *)(obj->entries[sub].data);
                        *(esc_int8_t *)(data + byte_offset++) = *(esc_int8_t *)(obj->entries[sub].data);
                        *(esc_int8_t *)(data + byte_offset++) = *(esc_int8_t *)(obj->entries[sub].data);
                        *(esc_int8_t *)(data + byte_offset++) = *(esc_int8_t *)(obj->entries[sub].data);
                        *(esc_int8_t *)(data + byte_offset++) = *(esc_int8_t *)(obj->entries[sub].data);
                        *(esc_int8_t *)(data + byte_offset++) = *(esc_int8_t *)(obj->entries[sub].data);
                        break;
                    case ESC_SDO__DATA_TYPE__VISIBLE_STRING:
                    case ESC_SDO__DATA_TYPE__OCTET_STRING:
                    case ESC_SDO__DATA_TYPE__UNICODE_STRING:
                    case ESC_SDO__DATA_TYPE__TIME_OF_DAY:
                    case ESC_SDO__DATA_TYPE__TIME_DIFFERENCE:
                    case ESC_SDO__DATA_TYPE__DOMAIN:
                    case ESC_SDO__DATA_TYPE__GUID:
                    case ESC_SDO__DATA_TYPE__BITARR8:
                    case ESC_SDO__DATA_TYPE__BITARR16:
                    case ESC_SDO__DATA_TYPE__BITARR32:
                    case ESC_SDO__DATA_TYPE__PDO_MAPPING:
                    case ESC_SDO__DATA_TYPE__IDENTITY:
                    case ESC_SDO__DATA_TYPE__COMMAND_PAR:
                    case ESC_SDO__DATA_TYPE__SYNC_PAR:
#define ADD_CASE_BITMASK(value, mask)                                                                             \
                        *(esc_uint8_t *)(data + byte_offset) |= (*(esc_uint8_t *)(obj->entries[sub].data) & (mask)) << bit_offset;   \
                        bit_offset++;                                                                                   \
                        if (bit_offset == 8u) { bit_offset = 0u; byte_offset++; }                                       \
                        break;
                    ADD_CASE_BITMASK(ESC_SDO__DATA_TYPE__BIT1, 0x01u)
                    ADD_CASE_BITMASK(ESC_SDO__DATA_TYPE__BIT2, 0x03u)
                    ADD_CASE_BITMASK(ESC_SDO__DATA_TYPE__BIT3, 0x07u)
                    ADD_CASE_BITMASK(ESC_SDO__DATA_TYPE__BIT4, 0x0Fu)
                    ADD_CASE_BITMASK(ESC_SDO__DATA_TYPE__BIT5, 0x1Fu)
                    ADD_CASE_BITMASK(ESC_SDO__DATA_TYPE__BIT6, 0x3Fu)
                    ADD_CASE_BITMASK(ESC_SDO__DATA_TYPE__BIT7, 0x7Fu)
                    ADD_CASE_BITMASK(ESC_SDO__DATA_TYPE__BIT8, 0xFFu)
                    default: 
                        break;
                }
            }
        } else {
            esc_uint16_t state_read_access = (obj->entries[sub_index].object_access & ESC_SDO__OBJECT_ACCESS__READ) << 1u;
            printf("state_read_access %X, al_state %X\n",
                    state_read_access, pesc->al_state);
            if ((state_read_access & pesc->al_state) == 0u) {
                ret = ESC_SDO_ABORT_CODE__UNSUPPORTED_ACCESS;
            } else {
                // TODO: check sizes
                memcpy(data, obj->entries[sub_index].data, data_len);
            }
        }
    }

    return ret;
}

void print_dict(void) {
    esc_object_t *obj = &objects[0];
    while (1) {
        if (obj->index == 0xFFFF) {
            printf("got end\n");
            break;
        }

        esc_size_t len;
        esc_obj_get_length(obj, &len);

        printf("got object 0x%X (%ld bits): %s\n", obj->index, len, obj->name);

        for (int i = 0; i <= obj->max_sub_index; ++i) {
            printf("  %d: %s => ", i, obj->entries[i].name);

            esc_uint8_t *tmp = (esc_uint8_t *)obj->entries[i].data;
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
    esc_t esc;
    esc_obj_retval_t ret = ESC_OK;
    esc.mbx.coe.objects = &objects[0];
    print_dict();

    esc_uint8_t al_state = ESC_STATE_OP;

    uint32_t tmp = 0xAFFEAFFEu;
    if ((ret = esc_obj_write_object_entry(esc_obj_get_object(&esc, 0x1018), 3, al_state, &tmp, sizeof(tmp))) != ESC_OK) {
        printf("error -> 0x%X\n", ret); 
    }

    if ((ret = esc_obj_write_object_entry(esc_obj_get_object(&esc, 0x1018), 7, al_state, &tmp, sizeof(tmp))) != ESC_OK) {
        printf("error -> 0x%X\n", ret); 
    }

    print_dict();
    

    return 0;
}
