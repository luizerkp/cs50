#include <stdio.h>
#include <cs50.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

int main(void)
{
    string text = get_string ("Text: ");


    float letter = 0;
    float word = 1;
    float sentence = 0;
    float lsum = 0;
    float s = 0;
    int index = 0;
    for (int l = 0, c = strlen(text); l < c; l++)
        {
        if (isalpha(text[l]))
            {
                 letter++;

            }
        if (isspace(text[l]))
        {
            word++;
        }
        if (text[l] == 33 || text[l] == 46 || text[l] == 63)
        {
            sentence++;
        }
        }
        {
             lsum = (letter * 100) / word;
        }
        {
             s = (sentence * 100) / word;
        }
        {
            index = roundf(0.0588 * lsum - 0.296 * s - 15.8);
        }
        printf("Grade %i\n", index);


}