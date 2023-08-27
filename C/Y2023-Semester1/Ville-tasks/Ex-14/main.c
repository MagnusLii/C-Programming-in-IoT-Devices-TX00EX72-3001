/*
Write a function that takes two parameters: month number and language. The function returns a const char pointer to the name of the month in given language. If an invalid value is given as a parameter the function returns a pointer to "Error". Write a function that takes one parameter: language and returns a const char pointer to the name of the language. If an unknown language is given function returns a pointer to "Error".

Write a function that takes two parameters: month number and language. The function returns a const char pointer to the name of the month in given language. If an invalid value is given as a parameter the function returns a pointer to "Error".

Write a function that takes one parameter: language and returns a const char pointer to the name of the language. If an unknown language is given function returns a pointer to "Error"*/


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>



typedef enum { English, Italian, Spanish } language;
const char *month(int number, language lang);
const char *lang2str(language lang);

int main()
{
    
    return 0;
}

const char *month(int number, language lang)
{
    const char *months[3][12] = {
        {"January", "February", "March", "April", "May", "June", "July", "August", "September", "October",
         "November", "December"},
        {"Gennaio", "Febbraio", "Marzo", "Aprile", "Maggio", "Giugno", "Luglio", "Agosto", "Settembre",
         "Ottobre", "Novembre", "Dicembre"},
        {"Enero", "Febrero", "Marzo", "Abril", "Mayo", "Junio", "Julio", "Agosto", "Septiembre",
         "Octubre", "Noviembre", "Diciembre"}
    };

    if (number < 1 || number > 12)
    {
        return "Error";
    }

    return months[lang][number - 1];
}

const char *lang2str(language lang)
{
    const char *languages[3] = {"English", "Italian", "Spanish"};

    return languages[lang];
}