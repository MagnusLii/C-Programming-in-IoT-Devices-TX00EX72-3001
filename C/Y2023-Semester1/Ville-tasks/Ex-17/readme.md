Assume following declarations and definitions:

#define MAX_LEN 32

typedef struct student_ {
    char name[MAX_LEN];
    int group;
    int id;
    struct student_ *next;
} student;

int move(student **source, int group, student **target);


Implement function move() that moves elements from source to target. Second parameter, group, determines which elements to move. Only the elements of matching group are moved to target list. Both source and target are linked lists that are initialized by the caller. The end of list is market by setting next pointer of the last element to NULL.

When you remove elements from the source you must preserve the order of remaining elements. When you add an element to the target it must be added at the beginning of the traget list.

Move returns the number of elements moved to the target. Function returns zero if no matching elements were found.
