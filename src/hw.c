//! \brief copy bytes from ESC memory to local memory
/*!
 * \param[in]   pesc        Pointer to ESC struture.
 * \param[in]   adr         ESC memory address to copy from.
 * \param[out]  buf         Target buffer to copy to.
 * \param[in]   buf_len     Length of target buffer.
 *
 * \return Number of bytes copied.
 */
osal_size_t hw_read(esc_t *pesc, osal_off_t adr, osal_uint8_t *buf, osal_size_t buf_len) {
    assert(pesc != NULL);
    assert(buf != NULL);

    (void *)memcpy((void *)buf, (void *)(pesc->hw.esc_base + adr), buf_len);
    return buf_len;
}

//! \brief copy bytes from local memory to ESC memory
/*!
 * \param[in]   pesc        Pointer to ESC struture.
 * \param[in]   adr         ESC memory address to copy to.
 * \param[in]   buf         Target buffer to copy from.
 * \param[in]   buf_len     Length of target buffer.
 *
 * \return Number of bytes copied.
 */
osal_size_t hw_write(esc_t *pesc, osal_off_t adr, osal_uint8_t *buf, osal_size_t buf_len) {
    assert(pesc != NULL);
    assert(buf != NULL);

    (void *)memcpy((void *)(pesc->hw.esc_base + adr), (void *)buf, buf_len);
    return buf_len;
}

//! \brief Opens and initializes ESC hardware
/*!
 * \param[in]   pesc        Pointer to ESC struture.
 * \param[in]   esc_base    Base address of ESC memory.
 * \param[in]   pdi_type    Type of PDI waiting for ESC to become ready.
 *
 * \retval  OK on success.
 */
osal_uint32_t hw_open(esc_t *pesc, void *esc_base, osal_uint16_t pdi_type) {
    assert(phw != NULL);

    osal_uint16_t pdi = 0u;

    pesc->hw.esc_base = esc_base;

    do {
        hw_read(pesc, ESC_REG_PDI_CONTROL, (osal_uint8_t *)pdi, sizeof(pdi));
    } while (pdi != pdi_type);

    return 0;
}

void hw_irq_pdi(esc_t *pesc) {
    osal_uint16_t al_status;
    hw_read(pesc, ESC_REG_AL_STATUS, (osal_uint8_t *)al_status, sizeof(al_status));

    if (pesc->hw.sync0_mapped_to_al_status == OSAL_TRUE) {
        if (al_status & ESC_REG_AL_STATUS__SYNC0) {
            // handle DC Sync1 event
            esc_sync0(pesc);
        }
    }

    if (pesc->hw.sync1_mapped_to_al_status == OSAL_TRUE) {
        if (al_status & ESC_REG_AL_STATUS__SYNC1) {
            // handle DC Sync1 event
            esc_sync1(pesc);
        }
    }

    if (al_status & ESC_REG_AL_STATUS__SM2) { 
        // outputs received
        esc_pdo(pesc);
    }

    if (al_status & ESC_REG_AL_STATUS__SM3) { 
        // inputs read from master
        esc_pdi(pesc);
    }
}

void hw_irq_sync0() {
    // handle DC Sync0 event
    esc_sync0(pesc);
}

void hw_irq_sync1() {
    // handle DC Sync1 event
    esc_sync1(pesc);
}

