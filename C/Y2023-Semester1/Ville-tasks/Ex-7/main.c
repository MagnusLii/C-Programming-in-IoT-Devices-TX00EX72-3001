void replace(char *str)
{
    int i;
    for (i = 0; i < strlen(str); i++)
    {
        if (str[i] == 't')
        {
            str[i] = '*';
        }
    }
}


int main(void)
{
char test1[] = "Two teas to thirtytwo";
char test2[] = "this is a t-test";
char test3[] = "where have all the flowers gone there's not a single one left here";

replace(test1);
replace(test2);
replace(test3);

printf("%s\n", test1);
printf("%s\n", test2);
printf("%s\n", test3);


return 0;
}