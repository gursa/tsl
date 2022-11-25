#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

#include "mem_dump.h"

char *print_bytes( void *ptr, size_t size )
{
    size_t result_size = (size+7)  * 8 * sizeof (char);
    int hex_offset = 0;
    char hex_buf[49];
    char buff[9];
    char *result = malloc(result_size);
    unsigned char *p = ptr ;
    int offset = 0;

    for( size_t i = 0; i < size; i++ )
    {
        int tail = i % 8;
        if (tail == 0)
        {
            if (i != 0)
            {
                hex_offset = 0;
                offset += sprintf(result+offset, "%-48s// %s\n", hex_buf, buff);
            }
        }
        hex_offset += sprintf(hex_buf+hex_offset, "0x%02hhX, ", p[i] );
        hex_buf[hex_offset] = '\0';
        if ((p[i] < 0x20) || (p[i] > 0x7e))
        {
            buff[i % 8] = '.';
        }
        else
        {
            buff[i % 8] = p[i];
        }
        buff[(i % 8) + 1] = '\0';
    }
    offset += sprintf(result+offset, "%-48s// %s\n", hex_buf, buff);
    result[offset] = '\0';
    return result;
}

static int mem_dump(void *addr, unsigned long len, char **str)
{

}

int mem_commands(char **tokens, uint32_t token_count, char **str)
{
    unsigned long address = strtoul(tokens[2], NULL, 0);
    return 0;
}
