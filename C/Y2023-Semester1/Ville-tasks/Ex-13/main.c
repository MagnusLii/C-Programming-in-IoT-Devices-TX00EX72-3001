/*

Assume following declarations and definitions:

#define MAX_LEN 32

typedef struct student_ {
    char name[MAX_LEN];
    int group;
    int id;
} student;

int move(student *source, int group, student *target, int size);


Implement function move() that moves elements from source to target. Second parameter group determines which elements to move.
Only the elements of matching group are moved to target array. Both source and target are arrays that are initialized by the caller.
The end of array is marked with an element where id is zero.

When you remove elements from the source you must preserve the order of remaining elements and ensure that end marker is in right place.
When you add an element to the target remember that you must not add new elements if the target already has (size-1) elements. The target must also have an end marker.

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
void check_if_target_full(student *target, int size, int *ptarget_full, int *target_zero_location);
void move_target_elements_by1(student *target, int *target_zero_location, int replacement_index);

int main()
{

    return 0;
}

int move(student *source, int group, student *target, int size)
{
    int count = 0, target_zero_location = 0;
    int target_full = 0; // 0: not full, 1: full
    int *ptarget_full = &target_full, *ptarget_size = &target_zero_location;

    int i = 0;
    int j = 0;

    while (source[i].id != 0)
    {

        // printf("%d %d\n", source[i].group, group);
        if (source[i].group == group)
        {
            // printf("%d < %d\n", j, size - 1);
            if (j < size - 1)
            {
                check_if_target_full(target, size, ptarget_full, ptarget_size);
                if (target_full == 1)
                {
                    return 0; // target is full
                }


                move_target_elements_by1(target, ptarget_size, j);
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

void check_if_target_full(student *target, int size, int *ptarget_full, int *target_zero_location)
{
    int i = 0;
    while (target[i].id != 0)
    {
        i++;
    }

    if (i == size - 1)
    {
        *ptarget_full = 1;
        *target_zero_location = i + 1;
    }
    else
    {
        *ptarget_full = 0;
        *target_zero_location = i + 1;
    }
}

void move_target_elements_by1(student *target, int *target_zero_location, int replacement_index)
{
    int i = *target_zero_location + 1;

    while (i > replacement_index)
    {
        target[i] = target[i - 1];
        i--;
    }
}