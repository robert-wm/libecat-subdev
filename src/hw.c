#include <libecat-base/types.h>
#include <libecat-base/regs.h>
#include <libecat-subdev/ecsd.h>

#include <assert.h>
#include <stdlib.h>
#include <string.h>

//! \brief copy bytes from ESC memory to local memory
/*!
 * \param[in]   pecsd        Pointer to ESC struture.
 * \param[in]   adr         ESC memory address to copy from.
 * \param[out]  buf         Target buffer to copy to.
 * \param[in]   buf_len     Length of target buffer.
 *
 * \return Number of bytes copied.
 */
ec_size_t hw_read(struct ecsd *pecsd, ec_off_t adr, ec_uint8_t *buf, ec_size_t buf_len) {
    assert(pecsd != NULL);
    assert(buf != NULL);

    (void *)memcpy((void *)buf, (void *)(pecsd->hw.ecsd_base + adr), buf_len);
    return buf_len;
}

//! \brief copy bytes from local memory to ESC memory
/*!
 * \param[in]   pecsd        Pointer to ESC struture.
 * \param[in]   adr         ESC memory address to copy to.
 * \param[in]   buf         Target buffer to copy from.
 * \param[in]   buf_len     Length of target buffer.
 *
 * \return Number of bytes copied.
 */
ec_size_t hw_write(struct ecsd *pecsd, ec_off_t adr, ec_uint8_t *buf, ec_size_t buf_len) {
    assert(pecsd != NULL);
    assert(buf != NULL);

    (void *)memcpy((void *)(pecsd->hw.ecsd_base + adr), (void *)buf, buf_len);
    return buf_len;
}

//! \brief Opens and initializes ESC hardware
/*!
 * \param[in]   pecsd        Pointer to ESC struture.
 * \param[in]   ecsd_base    Base address of ESC memory.
 * \param[in]   pdi_type    Type of PDI waiting for ESC to become ready.
 *
 * \retval  OK on success.
 */
ec_uint32_t hw_open(struct ecsd *pecsd, void *ecsd_base, ec_uint16_t pdi_type) {
    assert(pecsd != NULL);
    assert(ecsd_base != NULL);

    ec_uint16_t pdi = 0u;

    pecsd->hw.ecsd_base = ecsd_base;

    do {
        hw_read(pecsd, EC_REG_PDI_CONTROL, (ec_uint8_t *)&pdi, sizeof(pdi));
    } while (pdi != pdi_type);

    return 0;
}

void hw_irq_pdi(struct ecsd *pecsd) {
    assert(pecsd != NULL);

    ec_uint16_t al_status;
    hw_read(pecsd, EC_REG_AL_STATUS, (ec_uint8_t *)&al_status, sizeof(al_status));

    if (pecsd->hw.sync0_mapped_to_al_status == EC_TRUE) {
        if (EC_REG_AL_EVENT_REQUEST__DC_SYNC0(al_status) != 0) {
            // handle DC Sync1 event
            ecsd_sync0(pecsd);
        }
    }

    if (pecsd->hw.sync1_mapped_to_al_status == EC_TRUE) {
        if (EC_REG_AL_EVENT_REQUEST__DC_SYNC1(al_status) != 0) {
            // handle DC Sync1 event
            ecsd_sync1(pecsd);
        }
    }

    if (EC_REG_AL_EVENT_REQUEST__SM_INTERRUPT(al_status) & EC_REG_AL_EVENT_REQUEST__SM_INTERRUPT__SM(2)) { 
        // outputs received
        ecsd_pdo(pecsd);
    }

    if (EC_REG_AL_EVENT_REQUEST__SM_INTERRUPT(al_status) & EC_REG_AL_EVENT_REQUEST__SM_INTERRUPT__SM(3)) { 
        // inputs read from master
        ecsd_pdi(pecsd);
    }
}

void hw_irq_sync0(struct ecsd *pecsd) {
    // handle DC Sync0 event
    ecsd_sync0(pecsd);
}

void hw_irq_sync1(struct ecsd *pecsd) {
    // handle DC Sync1 event
    ecsd_sync1(pecsd);
}

