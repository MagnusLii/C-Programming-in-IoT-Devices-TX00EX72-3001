Implement two functions:

int log(loglevel level, const char *format, ...); works like printf but prints only when level lower than or equal to global variable log_level. When printing precedes printouts with "LOG[%s]: " where %s is the parameter level printed as a string.

const char *log_level_to_str(loglevel level); returns a constant string corresponding to the level parameter. Note that the string returned may not be a variable that is allocated on the stack.
