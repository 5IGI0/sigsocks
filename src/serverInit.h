#ifndef __BIND_H__
#define __BIND_H__

#include "config.h"

/**
 * @brief create socket, bind, listen
 * 
 * @param config 
 * @return int positive number on success, negative number on error
 */
int serverInit(config_t config);

#endif