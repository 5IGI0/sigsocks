#ifndef __CONNECT_H__
#define __CONNECT_H__

#include <stdint.h>
#include <arpa/inet.h>

int connectTimeout(int __fd, void *__addr, socklen_t __len, uint16_t timeout);

#endif