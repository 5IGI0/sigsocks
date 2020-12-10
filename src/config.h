#ifndef __CONFIG_H__
#define __CONFIG_H__

#include <stdint.h>

typedef struct {
    uint16_t listening_port;
} config_t;

/**
 * @brief read the configuration
 * 
 * @param config configuration structure
 * @param argc argument counter
 * @param argv argument array
 * @return 0 on success, negative number on error
 */
int readConfig(config_t *config, int argc, char const **argv);

#endif