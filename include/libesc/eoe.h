
// vim: set nowrap:

#ifndef LIBECAT_BASE_EOE_H
#define LIBECAT_BASE_EOE_H

#define EC_EOE_HDR__FRAME_TYPE__MASK                    ((ec_uint32_t)0x0000000Fu)
#define EC_EOE_HDR__FRAME_TYPE__SHIFT                   ((ec_uint8_t)0u)
#define EC_EOE_HDR__FRAME_TYPE(sdo_hdr)                 (((sdo_hdr)->hdr & ESC_COE_SDO_HDR__FRAME_TYPE__MASK) >> ESC_COE_SDO_HDR__FRAME_TYPE__SHIFT)
#define EC_EOE_HDR__FRAME_TYPE_SET(sdo_hdr, val)        ((sdo_hdr)->hdr & ~ESC_COE_SDO_HDR__FRAME_TYPE__MASK) | ((val << ESC_COE_SDO_HDR__FRAME_TYPE__SHIFT) & EC_EOE_HDR__FRAME_TYPE__MASK)

#define EC_EOE_HDR__PORT__MASK                          ((ec_uint32_t)0x000000F0u)
#define EC_EOE_HDR__PORT__SHIFT                         ((ec_uint8_t)4u)
#define EC_EOE_HDR__PORT(sdo_hdr)                       (((sdo_hdr)->hdr & ESC_COE_SDO_HDR__PORT__MASK) >> ESC_COE_SDO_HDR__PORT__SHIFT)
#define EC_EOE_HDR__PORT_SET(sdo_hdr, val)              ((sdo_hdr)->hdr & ~ESC_COE_SDO_HDR__PORT__MASK) | ((val << ESC_COE_SDO_HDR__PORT__SHIFT) & EC_EOE_HDR__PORT__MASK)

#define EC_EOE_HDR__LAST_FRAGMENT__MASK                 ((ec_uint32_t)0x00000100u)
#define EC_EOE_HDR__LAST_FRAGMENT__SHIFT                ((ec_uint8_t)8u)
#define EC_EOE_HDR__LAST_FRAGMENT(sdo_hdr)              (((sdo_hdr)->hdr & ESC_COE_SDO_HDR__LAST_FRAGMENT__MASK) >> ESC_COE_SDO_HDR__LAST_FRAGMENT__SHIFT)
#define EC_EOE_HDR__LAST_FRAGMENT_SET(sdo_hdr, val)     ((sdo_hdr)->hdr & ~ESC_COE_SDO_HDR__LAST_FRAGMENT__MASK) | ((val << ESC_COE_SDO_HDR__LAST_FRAGMENT__SHIFT) & EC_EOE_HDR__LAST_FRAGMENT__MASK)

#define EC_EOE_HDR__TIME_APPENDED__MASK                 ((ec_uint32_t)0x00000200u)
#define EC_EOE_HDR__TIME_APPENDED__SHIFT                ((ec_uint8_t)9u)
#define EC_EOE_HDR__TIME_APPENDED(sdo_hdr)              (((sdo_hdr)->hdr & ESC_COE_SDO_HDR__TIME_APPENDED__MASK) >> ESC_COE_SDO_HDR__TIME_APPENDED__SHIFT)
#define EC_EOE_HDR__TIME_APPENDED_SET(sdo_hdr, val)     ((sdo_hdr)->hdr & ~ESC_COE_SDO_HDR__TIME_APPENDED__MASK) | ((val << ESC_COE_SDO_HDR__TIME_APPENDED__SHIFT) & EC_EOE_HDR__TIME_APPENDED__MASK)

#define EC_EOE_HDR__TIME_REQUESTED__MASK                ((ec_uint32_t)0x00000400u)
#define EC_EOE_HDR__TIME_REQUESTED__SHIFT               ((ec_uint8_t)10u)
#define EC_EOE_HDR__TIME_REQUESTED(sdo_hdr)             (((sdo_hdr)->hdr & ESC_COE_SDO_HDR__TIME_REQUESTED__MASK) >> ESC_COE_SDO_HDR__TIME_REQUESTED__SHIFT)
#define EC_EOE_HDR__TIME_REQUESTED_SET(sdo_hdr, val)    ((sdo_hdr)->hdr & ~ESC_COE_SDO_HDR__TIME_REQUESTED__MASK) | ((val << ESC_COE_SDO_HDR__TIME_REQUESTED__SHIFT) & EC_EOE_HDR__TIME_REQUESTED__MASK)

#define EC_EOE_HDR__FRAGMENT_NUMBER__MASK               ((ec_uint32_t)0x003F0000u)
#define EC_EOE_HDR__FRAGMENT_NUMBER__SHIFT              ((ec_uint8_t)16u)
#define EC_EOE_HDR__FRAGMENT_NUMBER(sdo_hdr)            (((sdo_hdr)->hdr & ESC_COE_SDO_HDR__FRAGMENT_NUMBER__MASK) >> ESC_COE_SDO_HDR__FRAGMENT_NUMBER__SHIFT)
#define EC_EOE_HDR__FRAGMENT_NUMBER_SET(sdo_hdr, val)   ((sdo_hdr)->hdr & ~ESC_COE_SDO_HDR__FRAGMENT_NUMBER__MASK) | ((val << ESC_COE_SDO_HDR__FRAGMENT_NUMBER__SHIFT) & EC_EOE_HDR__FRAGMENT_NUMBER__MASK)

#define EC_EOE_HDR__COMPLETE_SIZE__MASK                 ((ec_uint32_t)0x0FC00000u)
#define EC_EOE_HDR__COMPLETE_SIZE__SHIFT                ((ec_uint8_t)22u)
#define EC_EOE_HDR__COMPLETE_SIZE(sdo_hdr)              (((sdo_hdr)->hdr & ESC_COE_SDO_HDR__COMPLETE_SIZE__MASK) >> ESC_COE_SDO_HDR__COMPLETE_SIZE__SHIFT)
#define EC_EOE_HDR__COMPLETE_SIZE_SET(sdo_hdr, val)     ((sdo_hdr)->hdr & ~ESC_COE_SDO_HDR__COMPLETE_SIZE__MASK) | ((val << ESC_COE_SDO_HDR__COMPLETE_SIZE__SHIFT) & EC_EOE_HDR__COMPLETE_SIZE__MASK)

#define EC_EOE_HDR__FRAME_NUMBER__MASK                  ((ec_uint32_t)0xF0000000u)
#define EC_EOE_HDR__FRAME_NUMBER__SHIFT                 ((ec_uint8_t)28u)
#define EC_EOE_HDR__FRAME_NUMBER(sdo_hdr)               (((sdo_hdr)->hdr & ESC_COE_SDO_HDR__FRAME_NUMBER__MASK) >> ESC_COE_SDO_HDR__FRAME_NUMBER__SHIFT)
#define EC_EOE_HDR__FRAME_NUMBER_SET(sdo_hdr, val)      ((sdo_hdr)->hdr & ~ESC_COE_SDO_HDR__FRAME_NUMBER__MASK) | ((val << ESC_COE_SDO_HDR__FRAME_NUMBER__SHIFT) & EC_EOE_HDR__FRAME_NUMBER__MASK)

struct ec_eoe_header {
    ec_uint32_t hdr;
} __attribute((__packed__));

struct ec_initiate_eoe_req {
    struct ec_mbx_header mbx_hdr;
    struct ec_eoe_header eoe_hdr;
    ec_data_t data[0];
} __attribute((__packed__));

struct ec_initiate_eoe_resp {
    struct ec_mbx_header mbx_hdr;
    struct ec_eoe_header eoe_hdr;
    ec_uint32_t timestamp;
} __attribute((__packed__));

struct ec_eoe_fragment_req {
    struct ec_mbx_header mbx_hdr;
    struct ec_eoe_header eoe_hdr;
    ec_data_t data[0];
} __attribute((__packed__));

struct ec_eoe_set_ip_parameter_req {
    struct ec_mbx_header mbx_hdr;
    struct ec_eoe_header eoe_hdr;
    ec_uint32_t include_field;
    ec_uint8_t mac[6];
    ec_uint8_t ip_address[4];
    ec_uint8_t subnet_mask[4];
    ec_uint8_t default_gateway[4];
    ec_uint8_t dns_server[4];
    ec_char_t dns_name[32];
} __attribute((__packed__));

#define EC_EOE_PARA_HDR__FRAME_TYPE__MASK                   ((ec_uint32_t)0x0000000Fu)
#define EC_EOE_PARA_HDR__FRAME_TYPE__SHIFT                  ((ec_uint8_t)0u)
#define EC_EOE_PARA_HDR__FRAME_TYPE(sdo_hdr)                (((sdo_hdr)->hdr & ESC_COE_SDO_HDR__FRAME_TYPE__MASK) >> ESC_COE_SDO_HDR__FRAME_TYPE__SHIFT)
#define EC_EOE_PARA_HDR__FRAME_TYPE_SET(sdo_hdr, val)       ((sdo_hdr)->hdr & ~ESC_COE_SDO_HDR__FRAME_TYPE__MASK) | ((val << ESC_COE_SDO_HDR__FRAME_TYPE__SHIFT) & EC_EOE_PARA_HDR__FRAME_TYPE__MASK)

#define EC_EOE_PARA_HDR__PORT__MASK                         ((ec_uint32_t)0x000000F0u)
#define EC_EOE_PARA_HDR__PORT__SHIFT                        ((ec_uint8_t)4u)
#define EC_EOE_PARA_HDR__PORT(sdo_hdr)                      (((sdo_hdr)->hdr & ESC_COE_SDO_HDR__PORT__MASK) >> ESC_COE_SDO_HDR__PORT__SHIFT)
#define EC_EOE_PARA_HDR__PORT_SET(sdo_hdr, val)             ((sdo_hdr)->hdr & ~ESC_COE_SDO_HDR__PORT__MASK) | ((val << ESC_COE_SDO_HDR__PORT__SHIFT) & EC_EOE_PARA_HDR__PORT__MASK)

#define EC_EOE_PARA_HDR__LAST_FRAGMENT__MASK                ((ec_uint32_t)0x00000100u)
#define EC_EOE_PARA_HDR__LAST_FRAGMENT__SHIFT               ((ec_uint8_t)8u)
#define EC_EOE_PARA_HDR__LAST_FRAGMENT(sdo_hdr)             (((sdo_hdr)->hdr & ESC_COE_SDO_HDR__LAST_FRAGMENT__MASK) >> ESC_COE_SDO_HDR__LAST_FRAGMENT__SHIFT)
#define EC_EOE_PARA_HDR__LAST_FRAGMENT_SET(sdo_hdr, val)    ((sdo_hdr)->hdr & ~ESC_COE_SDO_HDR__LAST_FRAGMENT__MASK) | ((val << ESC_COE_SDO_HDR__LAST_FRAGMENT__SHIFT) & EC_EOE_PARA_HDR__LAST_FRAGMENT__MASK)

#define EC_EOE_PARA_HDR__TIME_APPENDED__MASK                ((ec_uint32_t)0x00000200u)
#define EC_EOE_PARA_HDR__TIME_APPENDED__SHIFT               ((ec_uint8_t)9u)
#define EC_EOE_PARA_HDR__TIME_APPENDED(sdo_hdr)             (((sdo_hdr)->hdr & ESC_COE_SDO_HDR__TIME_APPENDED__MASK) >> ESC_COE_SDO_HDR__TIME_APPENDED__SHIFT)
#define EC_EOE_PARA_HDR__TIME_APPENDED_SET(sdo_hdr, val)    ((sdo_hdr)->hdr & ~ESC_COE_SDO_HDR__TIME_APPENDED__MASK) | ((val << ESC_COE_SDO_HDR__TIME_APPENDED__SHIFT) & EC_EOE_PARA_HDR__TIME_APPENDED__MASK)

#define EC_EOE_PARA_HDR__TIME_REQUESTED__MASK               ((ec_uint32_t)0x00000400u)
#define EC_EOE_PARA_HDR__TIME_REQUESTED__SHIFT              ((ec_uint8_t)10u)
#define EC_EOE_PARA_HDR__TIME_REQUESTED(sdo_hdr)            (((sdo_hdr)->hdr & ESC_COE_SDO_HDR__TIME_REQUESTED__MASK) >> ESC_COE_SDO_HDR__TIME_REQUESTED__SHIFT)
#define EC_EOE_PARA_HDR__TIME_REQUESTED_SET(sdo_hdr, val)   ((sdo_hdr)->hdr & ~ESC_COE_SDO_HDR__TIME_REQUESTED__MASK) | ((val << ESC_COE_SDO_HDR__TIME_REQUESTED__SHIFT) & EC_EOE_PARA_HDR__TIME_REQUESTED__MASK)

#define EC_EOE_PARA_HDR__RESULT__MASK                       ((ec_uint32_t)0xFFFF0000u)
#define EC_EOE_PARA_HDR__RESULT__SHIFT                      ((ec_uint8_t)16u)
#define EC_EOE_PARA_HDR__RESULT(sdo_hdr)                    (((sdo_hdr)->hdr & ESC_COE_SDO_HDR__RESULT__MASK) >> ESC_COE_SDO_HDR__RESULT__SHIFT)
#define EC_EOE_PARA_HDR__RESULT_SET(sdo_hdr, val)           ((sdo_hdr)->hdr & ~ESC_COE_SDO_HDR__RESULT__MASK) | ((val << ESC_COE_SDO_HDR__RESULT__SHIFT) & EC_EOE_PARA_HDR__RESULT__MASK)

#define EC_EOE_PARA__RESULT__SUCCESS                        ((ec_uint16_t)0x0000u)
#define EC_EOE_PARA__RESULT__UNSPECIFIED_ERROR              ((ec_uint16_t)0x0001u)
#define EC_EOE_PARA__RESULT__UNSUPPORTED_FRAME_TYPE         ((ec_uint16_t)0x0002u)
#define EC_EOE_PARA__RESULT__NO_IP_SUPPORT                  ((ec_uint16_t)0x0201u)
#define EC_EOE_PARA__RESULT__NO_DHCP_SUPPORT                ((ec_uint16_t)0x0202u)
#define EC_EOE_PARA__RESULT__NO_FILTER_SUPPORT              ((ec_uint16_t)0x0401u)

struct ec_eoe_para_header {
    ec_uint32_t hdr;
} __attribute__((__packed__));

struct ec_eoe_set_ip_parameter_resp {
    struct ec_mbx_header mbx_hdr;
    struct ec_eoe_para_header eoe_hdr;
} __attribute__((__packed__));

#define EC_EOE_PARA__MAC_FILTER_CNT__MASK                   ((ec_uint16_t)0x000Fu)
#define EC_EOE_PARA__MAC_FILTER_CNT__SHIFT                  ((ec_uint8_t)0u)
#define EC_EOE_PARA__MAC_FILTER_CNT(sdo_hdr)                (((sdo_hdr)->hdr & ESC_COE_SDO__MAC_FILTER_CNT__MASK) >> ESC_COE_SDO__MAC_FILTER_CNT__SHIFT)
#define EC_EOE_PARA__MAC_FILTER_CNT_SET(sdo_hdr, val)       ((sdo_hdr)->hdr & ~ESC_COE_SDO__MAC_FILTER_CNT__MASK) | ((val << ESC_COE_SDO__MAC_FILTER_CNT__SHIFT) & EC_EOE_PARA__MAC_FILTER_CNT__MASK)

#define EC_EOE_PARA__MAC_FILTER_MASK__MASK                  ((ec_uint16_t)0x0030u)
#define EC_EOE_PARA__MAC_FILTER_MASK__SHIFT                 ((ec_uint8_t)4u)
#define EC_EOE_PARA__MAC_FILTER_MASK(sdo_hdr)               (((sdo_hdr)->hdr & ESC_COE_SDO__MAC_FILTER_MASK__MASK) >> ESC_COE_SDO__MAC_FILTER_MASK__SHIFT)
#define EC_EOE_PARA__MAC_FILTER_MASK_SET(sdo_hdr, val)      ((sdo_hdr)->hdr & ~ESC_COE_SDO__MAC_FILTER_MASK__MASK) | ((val << ESC_COE_SDO__MAC_FILTER_MASK__SHIFT) & EC_EOE_PARA__MAC_FILTER_MASK__MASK)

#define EC_EOE_PARA__INHIBIT_BROADCAST__MASK                ((ec_uint16_t)0x0080u)
#define EC_EOE_PARA__INHIBIT_BROADCAST__SHIFT               ((ec_uint8_t)7u)
#define EC_EOE_PARA__INHIBIT_BROADCAST(sdo_hdr)             (((sdo_hdr)->hdr & ESC_COE_SDO__INHIBIT_BROADCAST__MASK) >> ESC_COE_SDO__INHIBIT_BROADCAST__SHIFT)
#define EC_EOE_PARA__INHIBIT_BROADCAST_SET(sdo_hdr, val)    ((sdo_hdr)->hdr & ~ESC_COE_SDO__INHIBIT_BROADCAST__MASK) | ((val << ESC_COE_SDO__INHIBIT_BROADCAST__SHIFT) & EC_EOE_PARA__INHIBIT_BROADCAST__MASK)

struct ec_eoe_set_mac_filter_req {
    struct ec_mbx_header mbx_hdr;
    struct ec_eoe_header eoe_hdr;
    ec_uint16_t eoe_parameter;
    ec_data_t data[0];
} __attribute__((__packed__));

#endif // LIBECAT_BASE_EOE_H

