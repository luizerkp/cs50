#include <stdio.h>
#include <string.h>
#include <stdlib.h>


int main(void)
{

    long cc;
    do
    {
        printf("number: ");
        scanf("%ld", &cc);
    }
    while (cc < 0);

    // pointer to store credit card number as a string and malloc check
    char *credit_card = malloc(16 * sizeof(int));

    if (credit_card == NULL)
    {
        printf("malloc failed\n");
        return 1;
    }

    // converts long type credit card number into a string
    sprintf(credit_card, "%ld", cc);

    // checks credit card length
    int numbers = strlen(credit_card);

    // initializes the credit card array to have value  of "numbers" number of elements
    int credit_card_array[numbers];

    // keeps track of indices for credit_card_array
    int j = 0;

    // stores credit card number in reverse order into the credit_card_array so that last is first index and first is last index
    for (int i = numbers - 1; i >= 0; i--)
    {
        // converts string number in the ith element of credit_card to an int and stores it in the jth element of credit_card_array
        credit_card_array[j] = credit_card[i] - '0';
        j++;
    }

    // sum of numbers_add and numbers_multiply variables
    int checksum = 0;

    // numbers that are not multiplied get added per Luhn’s Algorithm
    int numbers_add = 0;

    // numbers multiplied by two and when greater than 9 both digits are added i.e 6*2= 12 --> 1+2 per Luhn’s Algorithm
    int numbers_multiply = 0;

    // temporarily stores the numbers multiplied by two for future checking of whether it is higher than 9
    // in which case the individual digits get added together i.e 6*2= 12 --> 1+2
    int temp;

    // calculates checksum by implementing Luhn’s Algorithm
    for (int i = 0; i < numbers; i++)
    {

        if ((i % 2) == 0)
        {
            numbers_add += credit_card_array[i];
        }
        else
        {
            temp = credit_card_array[i] * 2;
            if (temp > 9)
            {
                numbers_multiply += (temp % 10) + 1;
            }
            else
            {
                numbers_multiply += temp;
            }
        }

    }

    checksum = numbers_multiply + numbers_add;

    // checks credit card type if checksum % 10 == 0 otherwise print invalid
    if (checksum % 10 == 0)
    {
        // prints visa if credit card number stars with 4 is either 13 or 16 digits longs
        if ((numbers == 13 || numbers == 16) && credit_card[0] == '4')
        {
            printf("VISA\n");
        }
        // prints AMEX if credit card starts with 34 or 37 and is 15 digits long
        else if (numbers == 15 && credit_card[0] == '3' && (credit_card[1] == '4' || credit_card[1] == '7'))
        {
            printf("AMEX\n");
        }
        // prints MASTERCARD if credit card numeber is 16 digits long and starst with 51 through 55
        else if (numbers == 16 && credit_card[0] == '5' && (credit_card[1] >= '1' && credit_card[1] <= '5'))
        {
            printf("MASTERCARD\n");
        }
        // prints invalid if checksum % 10 == 0 but does not meet the indicators of a credit card type
        else
        {
            printf("INVALID\n");
        }
    }
    // prints INVALID if checksum % 10 != 0
    else
    {
        printf("INVALID\n");
    }

    // frees memory alloc for credit_card string
    free(credit_card);
}

