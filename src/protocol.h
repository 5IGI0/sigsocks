#ifndef __PROTOCOL_H__
#define __PROTOCOL_H__

#include <stdint.h>

typedef struct {
    uint32_t addr; // IPv4
    uint16_t port;
    uint8_t version;
    uint8_t command;
    uint8_t reserved;
    uint8_t addrType;
} sockRequest_t;

/* commands */
#define CMD_CONNECT 0x1
#define CMD_BIND    0x2
#define CMD_UDP     0x3

/* address types */
#define ATYPE_IPV4 0x1
#define ATYPE_DNS  0x3
#define ATYPE_IPV6 0x4

/* replies status */
#define REP_SUCCEEDED 0
#define REP_FAILURE   1
#define REP_NOT_ALLOWED 2
#define REP_NETWORK_UNREACHABLE 3
#define REP_HOST_UNREACHABLE 4
#define REP_CONNECTION_REFUSED 5
#define REP_TTL_EXPIRED 6
#define REP_CMD_NOT_SUPPORTED 7
#define RAP_ADDR_TYPE_NOT_SUPPORTED 9

#endif