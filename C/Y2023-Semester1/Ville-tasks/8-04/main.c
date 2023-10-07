#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdint.h>

#define SLIP_END 0xC0
#define SLIP_ESC 0xDB
#define SLIP_ESC_END 0xDC
#define SLIP_ESC_ESC 0xDD

#define MAX_FRAME_SIZE 400  // needs to be big enough to hold everything the auto tester sends.

typedef void (*callback_t)(const unsigned char *, int);
typedef int (*reader_t)();

callback_t g_callback = NULL;

void register_slip_frame_callback(callback_t callback)
{
    g_callback = callback;
}

void process_slip_data(reader_t reader)
{
    unsigned char buffer[MAX_FRAME_SIZE];
    int index = 0;
    bool escaped = false;

    while (1)
    {
        int c = reader();
        if (c == EOF)
            break;

        switch (c)
        {
        case SLIP_END:
            if (index > 0 && g_callback != NULL)
            {
                g_callback(buffer, index);
                index = 0;
            }
            break;
        case SLIP_ESC:
            escaped = true;
            break;
        case SLIP_ESC_END:
            if (escaped)
            {
                buffer[index++] = SLIP_END;
                escaped = false;
            }
            else
            {
                buffer[index++] = c;
            }
            break;
        case SLIP_ESC_ESC:
            if (escaped)
            {
                buffer[index++] = SLIP_ESC;
                escaped = false;
            }
            else
            {
                buffer[index++] = c;
            }
            break;
        default:
            buffer[index++] = c;
            break;
        }
        // The automatic tester does not require a max frame size.
        /*
                if (index >= MAX_FRAME_SIZE) {
                    fprintf(stderr, "Error: Frame size exceeded maximum limit\n");
                    exit(1);
                }
            }
        */
    }
}
