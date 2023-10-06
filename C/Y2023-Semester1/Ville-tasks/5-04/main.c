#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LEN 32

typedef struct student_ {
    char name[MAX_LEN];
    int group;
    int id;
    struct student_ *next;
} student;

int move(student **source, int group, student **target) {
    int count = 0;
    
    student *current = *source;
    student *prev = NULL;

    while (current != NULL) {
        if (current->group == group) {
            if (prev == NULL) {
                *source = current->next;
            } else {
                prev->next = current->next;
            }
            
            current->next = *target;
            *target = current;

            count++;
            current = (prev == NULL) ? *source : prev->next;
        } else {
            prev = current;
            current = current->next;
        }
    }

    return count;
}

void print_list(student *list) {
    student *current = list;
    while (current != NULL) {
        printf("Name: %s, Group: %d, ID: %d\n", current->name, current->group, current->id);
        current = current->next;
    }
}

