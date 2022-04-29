#include <stdio.h>
#include <cs50.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

int main(void)
{
    string text = get_string("text: ");

        int k = atoi(text);
    for (int l = 0, c = strlen(text); l < c; l++)
    {
        if (isalpha(text[l]) && islower(text[l]))
        {
            k = text[l]- 'a';
            l++;
        }
        else if (isalpha(text[l]) && isupper(text[l]))
        {
            k = text[l] - 'A';
            l++;
        }
        else if (ispunct(text[l]) || isspace(text[l]))
        {
            l++;
        }
        ;
    
    }
}