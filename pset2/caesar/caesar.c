#include <stdio.h>
#include <cs50.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>


int main(int argc, string argv[])
{

    // prompt user for valid key if none is entered //
    if (argc != 2)
    {
        printf("Usage: ./caesar key\n");
        return (1);
    }
    // prompts user for valid key is either a non numeric value or too many values are entered //
    for (int i = 0, n = strlen(argv[1]); n > i; i++)
    {

        if (isalpha(argv[1][i]) || !isdigit(argv[1][i]))
        {
            printf("Usage: ./caesar key\n");
            return (1);
        }

    }
    // prompt user for plain text to be emciphered //
    if (argc == 2)
    {
        string text = get_string("Plaintext:  ");
        int k = atoi(argv[1]);
        string ci = text;

        for (int l = 0, c = strlen(text); l < c; l++)
        {
            // ensures that  letter (case, puntuation ect.) is reatined, converts letters a - z into index number 0 - 25, and applies cipher formula  //
            if (isalpha(text[l]) && islower(text[l]))
                ci[l] = (text[l] - 'a' + k) % 26 + 'a';

            else if (isalpha(text[l]) && isupper(text[l]))
                ci[l] = (text[l] - 'A' + k) % 26 + 'A';
        }
        // display encrypted text //
        printf("ciphertext: %s\n", ci);
    }
}