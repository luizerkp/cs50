#include <stdio.h>
#include <cs50.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

int main(void)
{

    // get text input from user
    string text = get_string("Text: ");
    float letter = 0;
    float word = 1;
    float sentence = 0;
    float lsum = 0;
    float s = 0;
    int index = 0;

    // for loop to count letters, words and sentences
    for (int l = 0, c = strlen(text); l < c; l++)
    {
        if (isalpha(text[l]))
            letter ++;
        if (isspace(text[l]))
            word++;
        if (text[l] == '!' || text[l] == '.' || text[l] == '?')
            sentence++;
    }

    // calculate the value of L here ID as lsum and S here ID as s from the the Coleman-Liau index
    lsum = ((letter * 100) / word);
    s = ((sentence * 100) / word);

    // the Coleman-Liau index = 0.0588 * L - 0.296 * S - 15.8
    index = round(0.0588 * lsum - 0.296 * s - 15.8);

    if (index >= 1 && index <= 16)
        // displays a grade between 1 and 16 on screen
        printf("Grade %i\n", index);
    else if (index < 1)
        // displays when grade is lass than 1
        printf("Before Grade 1\n");
    else if (index > 16)
        // displays when grade is above 16
        printf("Grade 16+\n");
}