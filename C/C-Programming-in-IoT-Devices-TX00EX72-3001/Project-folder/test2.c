#include <stdio.h>
#include <time.h>
#include <stdbool.h>

bool dtimeString(char *stringToStoreTo, size_t bufferSize);

int main()
{

char buffer[3] = "\0";  // Choose an appropriate buffer size (e.g., 20 characters).

printf("sizeof(buffer) = %d\n", sizeof(buffer));

if (dtimeString(buffer, sizeof(buffer))) {
    printf("Formatted date and time: %s\n", buffer);
} else {
    // Handle the error.
}

    
    return 0;
}


// Gets the current date and time and stores it in the 'stringToStoreTo' buffer.
bool dtimeString(char *stringToStoreTo, size_t bufferSize){
    if (stringToStoreTo == NULL) {
        fprintf(stderr, "Error: Invalid pointer in dtimeString.\n");
        return false;
    }

    if (bufferSize < 7) { // The eventual string will be 6 characters long.
        fprintf(stderr, "Error: Buffer string too short in dtimeString.\n");
        return false;
    }

    time_t current_time = time(NULL);
    strftime(stringToStoreTo, bufferSize, "%Y%m", localtime(&current_time));
    return true;
}