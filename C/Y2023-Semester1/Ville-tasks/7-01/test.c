    va_list args;
    va_start(args, format);

    int result = printf("DEBUG: ");
    result += vprintf(format, args);

    va_end(args);
    return result;