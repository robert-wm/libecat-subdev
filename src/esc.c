#include <libesc/esc.h>
#include <libesc/hw.h>

esc_retval_t esc_prepare_state_transition(esc_t *pesc) {
    esc_retval_t retval = ESC_OK;

    ec_state_transition_t transition;

    switch(transition) {
        case OP_2_OP:
        case OP_2_SAFEOP:
        case OP_2_PREOP:
        case OP_2_INIT:
            break;
        case INIT_2_INIT:
        case INIT_2_PREOP:
            if (pesc->mbx.supported) {
                // check SyncManager 0/1 for mailbox configuration.
                hw_read(pesc, ESC_REG_SYNC_MANAGER + (0 * ESC_REG_SYNC_MANAGER__BASE_OFFSET), 
                        (osal_uint8_t *)&pesc->mbx.sm_receive, sizeof(pesc->mbx.sm_receive));
                hw_read(pesc, ESC_REG_SYNC_MANAGER + (1 * ESC_REG_SYNC_MANAGER__BASE_OFFSET), 
                        (osal_uint8_t *)&pesc->mbx.sm_send, sizeof(pesc->mbx.sm_send));

                if ((sm0.address < 0x1000) || (sm1.address < 0x1000)) {
                    // ERROR: programmed address to small
                } else if ( 
                        ((sm0.address < sm1.address) && ((sm0.address + sm0.length) > (sm1.address + sm1.length))) ||
                        ((sm0.address > sm1.address) && ((sm0.address + sm0.length) < (sm1.address + sm1.length))) ) {
                    // ERROR: address ranges do overlap
                }
            }

            break;
    }

    return retval;
}
