#ifndef NETWORK_UTILS_H
#define NETWORK_UTILS_H

#include <stdint.h>

typedef int(*callback)(char **, uint32_t, char **);

int32_t makeErrorCode(int32_t error);

int32_t listenSocket(int32_t port, int *base_handle);

int32_t acceptConnection(int base_handle, int *connect_handle);

void *processConnection(void *args);

int32_t sendMessage(int connection_handle, char **tokens, uint32_t token_count, callback getMessage);

#endif // NETWORK_UTILS_H
