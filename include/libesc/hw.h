#include <libesc/types.h>

typedef struct hw {
    osal_uint8_t *esc_base;
} hw_t;

//! \brief copy bytes from ESC memory to local memory
/*!
 * \param[in]   pesc        Pointer to ESC struture.
 * \param[in]   adr         ESC memory address to copy from.
 * \param[out]  buf         Target buffer to copy to.
 * \param[in]   buf_len     Length of target buffer.
 *
 * \return Number of bytes copied.
 */
osal_size_t hw_read(esc_t *pesc, osal_off_t adr, osal_uint8_t *buf, osal_size_t buf_len);

//! \brief copy bytes from local memory to ESC memory
/*!
 * \param[in]   pesc        Pointer to ESC struture.
 * \param[in]   adr         ESC memory address to copy to.
 * \param[in]   buf         Target buffer to copy from.
 * \param[in]   buf_len     Length of target buffer.
 *
 * \return Number of bytes copied.
 */
osal_size_t hw_write(esc_t *pesc, osal_off_t adr, osal_uint8_t *buf, osal_size_t buf_len);

//! \brief Opens and initializes ESC hardware
/*!
 * \param[in]   pesc        Pointer to ESC struture.
 * \param[in]   esc_base    Base address of ESC memory.
 * \param[in]   pdi_type    Type of PDI waiting for ESC to become ready.
 *
 * \retval  OK on success.
 */
osal_uint32_t hw_open(esc_t *pesc, void *esc_base, osal_uint16_t pdi_type);

