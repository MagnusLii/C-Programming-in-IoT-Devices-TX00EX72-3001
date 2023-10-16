#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <ctype.h>
#include <stdbool.h>

typedef enum { logCritical, logWarning, logInfo, logVerbose } loglevel;

const char *log_level_to_str(loglevel level) {
    switch(level) {
        case logCritical: return "Critical";
        case logWarning: return "Warning";
        case logInfo: return "Info";
        case logVerbose: return "Verbose";
        default: return "UNKNOWN";
    }
}

int log(loglevel level, const char *format, ...) {
    if (level <= log_level) {  // log_level is a hidden global variable in the auto tester.
        va_list args;
        va_start(args, format);

        printf("LOG[%s]: ", log_level_to_str(level));
        vprintf(format, args);

        va_end(args);
    }
    return 0;
}