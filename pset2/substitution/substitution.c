#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>


int main(int argc, char *argv[])
{

    // ensures that no key entered gives an error
    if (argc == 1)
    {
        printf("Usage: ./substitution key\n");
        return (1);
    }

    for (int i = 0, n = strlen(argv[1]); n > i; i++)
    {

        // error message for when multible keys, non- alphabetic characters, spaces or punctuations are entered
        if (argc != 2 || isdigit(argv[1][i]) || ispunct(argv[1][i]))
        {
            printf("Usage: ./substitution key\n");
            i++;
            return (1);
        }
        // makes sure that key can only be 26 characters
        if (n != 26)
        {
            printf("Key must contain 26 characters\n");
            return (1);
        }
        for (int j = 0; i > j; j++)
        {
            // ensure that no repeating characters are entered regardless of letter case
            if (argv[1][i] == argv[1][j] || argv[1][i] == (argv[1][j] + 32) || argv[1][i] == argv[1][j] - 32)
            {
                printf("Key must not contain repeated characters\n");
                i++;
                return (1);
            }
        }
    }

    if (argc == 2)
    {
        char *text = malloc(sizeof(char) * 1024);
        printf("Plaintext (limit: 1,000 chars):  ");

        //store plain text in text variable
        fgets(text, sizeof(char) * 1024, stdin);

        //remove newline
        int len = strlen(text);
        if (text[len - 1] == '\n')
        {
            text[len - 1] = 0;
        }

        char *ci = text;
        int k = atoi(text);

        for (int l = 0, c = strlen(text); l < c; l++)
        {

            // lower case index turns a - z to 0 - 25 and stores value in k
            if (isalpha(text[l]) && islower(text[l]))
            {
                k = text[l] - 'a';
            }
            // upper case index turns A - Z to 0 - 25 and stores value in k
            else if (isalpha(text[l]) && isupper(text[l]))
            {
                k = text[l] - 'A';
            }
            // ensures that output text retains case
            if (isupper(argv[1][k]) && islower(text[l]))
            {
                ci[l] = argv[1][k] + 32;
            }
            if (islower(argv[1][k]) && isupper(text[l]))
            {
                ci[l] = argv[1][k] - 32;
            }
            //retains case when both key and text have same case
            if ((islower(argv[1][k]) && islower(text[l])) || (isupper(argv[1][k]) && isupper(text[l])))
            {
                ci[l] = argv[1][k];
            }

        }
        // prints string value stored in ci as an encoded version of the original text, while retaining space, punctuation, letter case and non-alphabetic characters
        printf("ciphertext: %s\n", ci);
        free(text);
    }


}