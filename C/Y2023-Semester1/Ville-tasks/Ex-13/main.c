/*

Assume following declarations and definitions:

#define MAX_LEN 32

typedef struct student_ {
    char name[MAX_LEN];
    int group;
    int id;
} student;

int move(student *source, int group, student *target, int size);


Implement function move() that moves elements from source to target. Second parameter group determines which elements to move. Only the elements of matching group are moved to target array. Both source and target are arrays that are initialized by the caller. The end of array is marked with an element where id is zero.

When you remove elements from the source you must preserve the order of remaining elements and ensure that end marker is in right place. When you add an element to the target remember that you must not add new elements if the target already has (size-1) elements. The target must also have an end marker.

Move returns number of elements moved to the target. Function returns zero if no matching element were found or if the target list is full.
*/

#include <stdio.h>

#define MAX_LEN 32

typedef struct student_
{
    char name[MAX_LEN];
    int group;
    int id;
} student;

int move(student *source, int group, student *target, int size);

int main()
{

    return 0;
}

int move(student *source, int group, student *target, int size)
{
    int count = 0;
    int i = 0;
    int j = 0;

    while (source[i].id != 0)
    {
        if (source[i].group == 1 && group == 1)
        {
            // Do nothing
        }

        // printf("%d %d\n", source[i].group, group);
        else if (source[i].group == group)
        {
            // printf("%d < %d\n", j, size - 1);
            if (j < size - 1)
            {
                target[j] = source[i];
                j++;
                count++;
            }
        }
        else
        {
            source[i - count] = source[i];
        }
        i++;
    }
    source[i - count].id = 0;
    return count;
}