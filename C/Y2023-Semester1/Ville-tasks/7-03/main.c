#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <ctype.h>
#include <stdbool.h>


typedef struct {
const char *text;
int position;
} text_reader_ctx;


// implement the following two functions
void text_reader_init(text_reader_ctx *ctc, const char *str);
int text_reader_read(text_reader_ctx *ctx, char *buffer, int size);

void text_reader_init(text_reader_ctx *ctx, const char *str) {
    ctx->text = str;
    ctx->position = 0;
}

/*
    -Text_reader_init sets the position to zero and stores the pointer in the context
    -Text_reader_read takes context pointer, buffer and buffer size as parameters, the function copies charaters
    from the text in context, starting form current position, until a line feed is encountered, end of text is reached
    or buffer is full.
    -If reading stops at a line feed the line feed is copied into string.
    -Text_reader_read returns the number of characters copied to buffer.
    -When end of text has been reached following reads must return a zero.
*/

int text_reader_read(text_reader_ctx *ctx, char *buffer, int size) {
    int i = 0;
    while (i < size - 1 && ctx->text[ctx->position] != '\0' && ctx->text[ctx->position] != '\n') {
        buffer[i] = ctx->text[ctx->position];
        i++;
        ctx->position++;
    }
    if (ctx->text[ctx->position] == '\n' && i < size - 1) {
        buffer[i] = ctx->text[ctx->position];
        i++;
        ctx->position++;
    }
    buffer[i] = '\0';
    return i;
}
