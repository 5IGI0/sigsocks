#ifndef __HANDLER_H__
#define __HANDLER_H__

#include "config.h"

void connectionHandler(int sock, config_t config, struct sockaddr *sin);

#endif