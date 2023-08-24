Write a function that receives an array of three integer pointers as a parameter.

The prototype of function is:

void sort3(int *pa[3]);

The function must sort the three pointers in the array so that the pointer to the smallest integer is the first element of the array and pointer to the largest integet is the last element of the array. The function may not modify the integer values.

For example if we have:

three integers: a, b, c

array of integer pointers int *pa[3]

and

a = 8, b = 2, c = 9 and pa[0] = &a, pa[1] = &b, pa[2] = &c

then after sort3(pa)

we have pa[0] = &b, pa[1] =&a, pa[2] = &c
