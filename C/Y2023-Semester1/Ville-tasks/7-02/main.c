#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <ctype.h>
#include <stdbool.h>

typedef enum { logCritical, logWarning, logInfo, logVerbose } loglevel;

// Log levels as strings: "Critical", "Warning", "Info", "Verbose"
const char *log_level_to_str(loglevel level) {
    switch (level) {
        case logCritical:
            return "Critical";
        case logWarning:
            return "Warning";
        case logInfo:
            return "Info";
        case logVerbose:
            return "Verbose";
        default:
            return "Unknown";
    }
}

static loglevel global_log_level = logInfo; // Default log level is Info

int log(loglevel level, const char *format, ...) {
    if (level <= global_log_level) {
        const char *level_str = log_level_to_str(level);
        printf("LOG[%s]: ", level_str);

        va_list args;
        va_start(args, format);
        int result = vprintf(format, args);
        va_end(args);

        return result;
    }

    return 0; // Nothing was printed
}

void set_log_level(loglevel level) {
    global_log_level = level;
}

int main() {
    // Set the global log level (change as needed)
    set_log_level(logVerbose);

    log(logCritical, "This is a critical message.\n");
    log(logWarning, "This is a warning message.\n");
    log(logInfo, "This is an info message.\n");
    log(logVerbose, "This is a verbose message.\n");

    return 0;
}
