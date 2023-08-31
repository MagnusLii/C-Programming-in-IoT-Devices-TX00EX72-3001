#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>

typedef enum
{
    English,
    Italian,
    Spanish
} language;
const char *month(int number, language lang);
const char *lang2str(language lang);

int main()
{
    const char *monthName = month(1, English);
    printf("Month: %s\n", monthName);

    return 0;
}

const char *month(int number, language lang)
{
    printf("month: %d, lang: %d\n", number, lang);
    const char *months[3][12] = {
        {"January", "February", "March", "April", "May", "June", "July", "August", "September", "October",
         "November", "December"},
        {"Gennaio", "Febbraio", "Marzo", "Aprile", "Maggio", "Giugno", "Luglio", "Agosto", "Settembre",
         "Ottobre", "Novembre", "Dicembre"},
        {"Enero", "Febrero", "Marzo", "Abril", "Mayo", "Junio", "Julio", "Agosto", "Septiembre",
         "Octubre", "Noviembre", "Diciembre"}};
    const char *error = "Error";

    if (number < 1 || number > 12)
    {
        return error;
    }

    // printf("%s", months[lang][number - 1]);

    return months[lang][number - 1];
}

const char *lang2str(language lang)
{
    // printf("lang: %d\n", lang);
    const char *languages[3] = {"English", "Italian", "Spanish"};
    const char *error = "Error";

    if (lang < 0 || lang > 3)
    {
        return error;
    }
    else
    {
        return languages[lang];
    }
}