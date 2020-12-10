#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#include "config.h"
#include "libs/logger/log.h"

int readConfig(config_t *config, int argc, char const **argv) {

    size_t i = 0; // iterator

    // setup default values
    config->listening_port = 1080;

    for (i = 0; i < (size_t)argc; i++) {
        if (strcmp(argv[i],"-v") == 0 || strcmp(argv[i],"--verbose") == 0)
            logSetVerbose(true);
    }

    if (getenv("SOCKS_PORT"))
        config->listening_port = atol(getenv("SOCKS_PORT"));

    logVerbose("configuration read\n");

    return 0;
}