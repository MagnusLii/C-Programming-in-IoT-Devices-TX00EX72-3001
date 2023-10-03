#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "debug.h"

int main() {
    srand(time(NULL));

    int debug_level;
    printf("Enter debug level (0-4): ");
    scanf("%d", &debug_level);

    if (debug_level < 0 || debug_level > 4) {
        printf("Invalid debug level. Please enter a number between 0 and 4.\n");
        return 1;
    }

    set_debug_level(debug_level);

    for (int i = 1; i <= 5; i++) {
        int random_debug_level = rand() % 5; // Generate random debug level between 0 and 4
        dprintf(random_debug_level, "This is message %d\n", i);
    }

    return 0;
}
