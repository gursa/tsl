#include <errno.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <linux/in.h>

#include "mem_dump.h"
#include "utils.h"

#define MAXLINE 4096 /*max text line length*/

typedef struct {
    const char *start;
    size_t len;
} token;

static char **parseMesssage(const char *str, char sep, uint32_t *count)
{
    char **array;
    uint32_t start = 0;
    uint32_t stop;
    uint32_t toks = 0;
    uint32_t t;
    token *tokens = malloc((strlen(str) + 1) * sizeof(token));
    for (stop = 0; str[stop]; stop++) {
        if (str[stop] == sep) {
            tokens[toks].start = str + start;
            tokens[toks].len = stop - start;
            toks++;
            start = stop + 1;
        }
    }
    /* Mop up the last token */
    tokens[toks].start = str + start;
    tokens[toks].len = stop - start;
    toks++;
    array = malloc((toks + 1) * sizeof(char*));
    for (t = 0; t < toks; t++) {
        /* Calloc makes it nul-terminated */
        char *token = calloc(tokens[t].len + 1, 1);
        memcpy(token, tokens[t].start, tokens[t].len);
        array[t] = token;
    }
    *count = toks;
    /* Add a sentinel */
    array[t] = NULL;
    free(tokens);
    return array;
}

int32_t makeErrorCode(int32_t error)
{
    return error | ((int32_t)1 << 31);
}

int32_t listenSocket(int32_t port, int *base_handle)
{
    if(base_handle == NULL)
    {
        return makeErrorCode(EINVAL);
    }

    *base_handle = 0;

    struct sockaddr_in address;

    /* create socket */
    int base_sd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    //cannot create socket
    if (base_sd <= 0)
    {
        return makeErrorCode(errno);
    }

    /* bind socket to port */
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);
    if (bind(base_sd, (struct sockaddr *)&address, sizeof(struct sockaddr_in)) < 0)
    {
        //cannot bind socket to port
        return makeErrorCode(errno);
    }

    /* listen on port */
    if (listen(base_sd, 5) < 0)
    {
        // cannot listen on port
        return makeErrorCode(errno);
    }

    *base_handle = base_sd;

    return 0;
}

int32_t acceptConnection(int base_handle, int *connect_handle)
{
    if(connect_handle == NULL)
    {
        return makeErrorCode(EINVAL);
    }

    *connect_handle = 0;

    struct sockaddr address;
    socklen_t address_len;

    int handle = accept(base_handle, &address, &address_len);
    if (handle <= 0)
    {
        // cannot accept connection
        return makeErrorCode(errno);
    }

    *connect_handle = handle;

    return 0;
}

void* processConnection(void *args)
{
    char buf[MAXLINE];
    int n = 0;
    int connection_handle = *(int *)args;

    while ( (n = recv(connection_handle, buf, MAXLINE, 0) ) > 0 )
    {
        uint32_t token_count;
        char** tokens = parseMesssage(buf, ' ', &token_count);
        // incorrect command
        if (token_count < 2)
        {
            break;
        }

        if (!strcmp(tokens[0], "mem"))
        {
            sendMessage(connection_handle, tokens, token_count, mem_commands);
        }
        else
        {
            // incorrect command
            break;
        }
    }

    // read error
    if (n < 0)
    {
        pthread_exit(NULL);
    }

    close(connection_handle);
    pthread_exit(NULL);
}

int32_t sendMessage(int connection_handle, char **tokens, uint32_t token_count, callback getMessage)
{
    char *message;
    int n = getMessage(tokens, token_count, &message);
    int count = send(connection_handle, message, n, 0);

    int i;
    for (i = 0; i < token_count; i++)
    {
        free (tokens[i]);
    }
    free (tokens);

    free(message);

    if (count < 0)
    {
        // send failed
        return makeErrorCode(errno);
    }
    return 0;
}
