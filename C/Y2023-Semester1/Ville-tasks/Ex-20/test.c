#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LEN 32

typedef struct student_ {
    char name[MAX_LEN];
    int group;
    int id;
} student;

typedef enum { byGroup, byLastName, byFirstName } sort_order;


int compareByGroup(const void *a, const void *b) {
    const student *studentA = (const student *)a;
    const student *studentB = (const student *)b;
    return studentA->group - studentB->group;
}


int compareByFirstName(const void *a, const void *b) {
    const student *studentA = (const student *)a;
    const student *studentB = (const student *)b;
    char firstNameA[MAX_LEN], firstNameB[MAX_LEN];
    
    sscanf(studentA->name, "%s", firstNameA);
    sscanf(studentB->name, "%s", firstNameB);
    
    return strcmp(firstNameA, firstNameB);
}


int compareByLastName(const void *a, const void *b) {
    const student *studentA = (const student *)a;
    const student *studentB = (const student *)b;
    char lastNameA[MAX_LEN], lastNameB[MAX_LEN];

    char *lastSpaceA = strrchr(studentA->name, ' ');
    char *lastSpaceB = strrchr(studentB->name, ' ');
    
    if (lastSpaceA == NULL || lastSpaceB == NULL) {
        return strcmp(studentA->name, studentB->name);
    }

    lastSpaceA++;
    lastSpaceB++;
    
    return strcmp(lastSpaceA, lastSpaceB);
}

void sort_students(student *students, int count, sort_order sb) {
    int (*compareFunction)(const void *, const void *);
    switch (sb) {
        case byGroup:
            compareFunction = compareByGroup;
            break;
        case byFirstName:
            compareFunction = compareByFirstName;
            break;
        case byLastName:
            compareFunction = compareByLastName;
            break;
        default:
            fprintf(stderr, "Invalid sort_order\n");
            return;
    }

    if (count <= 0) {
        count = 0;
        while (students[count].id != 0) {
            count++;
        }
    }

    qsort(students, count, sizeof(student), compareFunction);
}

