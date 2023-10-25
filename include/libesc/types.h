
#ifndef LIBESC_TYPES_H
#define LIBESC_TYPES_H

typedef uint16_t    osal_uint16_t;
typedef uint8_t     osal_uint8_t;
typedef uint32_t    osal_uint32_t;

#define PACKED      __attribute__((__packed__))

typedef int32_t     esc_retval_t;

typedef uint64_t    osal_size_t;
typedef uint64_t    osal_off_t;

typedef uint8_t     osal_bool_t;

typedef uint16_t ec_state_transition_t;
#define ESC_OK      0
        
#define OP_2_OP         0x0808
#define OP_2_SAFEOP     0x0804
#define OP_2_PREOP      0x0802
#define OP_2_INIT       0x0801
#define INIT_2_INIT     0x0101
#define INIT_2_PREOP    0x0102

#endif
