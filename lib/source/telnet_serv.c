#include <pthread.h>

#include "telnet_serv.h"
#include "utils.h"

int32_t start_server(int32_t port)
{
    int base_handle;
    int error = listenSocket(port, &base_handle);
    if (error)
    {
        return error;
    }

    int connection_handle;
    pthread_t thread;
    while(1)
    {
        error = acceptConnection(base_handle, &connection_handle);
        if (error)
        {
            return error;
        }

        pthread_create(&thread, 0, processConnection, (void *)&connection_handle);
        pthread_detach(thread);
    }

    return 0;
}
