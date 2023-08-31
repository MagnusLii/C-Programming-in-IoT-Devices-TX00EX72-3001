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