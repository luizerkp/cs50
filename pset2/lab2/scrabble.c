#include <cs50.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>

// Points assigned to each letter of the alphabet
int POINTS[] = {1, 3, 3, 2, 1, 4, 2, 4, 1, 8, 5, 1, 3, 1, 1, 3, 10, 1, 1, 1, 1, 4, 4, 8, 4, 10};

// Funciton to compute the value of a string per the POINTS array
int compute_score(string word);

int main(void)
{

    // Get input word from player 1
    char word1[50];
    printf("Player 1: ");
    scanf("%s", word1);
    
    // Get input word from player 2
    char word2[50];
    printf("Player 2: ");
    scanf("%s", word2);
    
    // Score both words
    int score1 = compute_score(word1);
    int score2 = compute_score(word2);

    // Print the winner
    if (score1 > score2)
    {
        printf("Player 1 wins!\n");
    }
    else if (score2 > score1)
    {
        printf("Player 2 wins!\n");
    }
    else
    {
        printf("Tie!\n");
    }

}
// Compute and return score for string
int compute_score(string word)
{

    int value = 0;
    int convert = 0;
    for (int i = 0, c = strlen(word); i < c; i++)
    {
        if (isalpha(word[i]))
        {
            convert = tolower(word[i]) - 'a';
            value += POINTS[convert];
        }
    }
    return value;

}
