#include <stdio.h>
#include <math.h>

// defines the max number coin denominations
#define MAX_COINS 4

// stores coin denominations in coins_array
int coins_array[MAX_COINS] = {25, 10, 5, 1};

// prompt user for non zero positive value //
int get_number_coins(int change_remaining);

int main(void)
{
    // gets input on how much change is owed and stores it in variable
    float change;
    do
    {
        printf("change owed: ");
        scanf("%f", &change);
    }
    while (change <= 0);

    // multiply by 100 and round float value of change to $change
    int $change = (float) roundf(change * 100);

    // calls function that calculates the least number  of coins
    int coins_change = get_number_coins($change);

    // displays final amount of coins used to complete transaction
    printf("%d\n", coins_change);

}

// claculates the least number of coins needed to complete transaction
int get_number_coins(int change_remaining)
{

    int coins = 0;
    for (int i = 0; i < MAX_COINS; i++)
    {

        while (change_remaining >= coins_array[i])
        {
            coins++;
            change_remaining -= coins_array[i];
        }
        if (change_remaining == 0)
        {
            return coins;
        }

    }

    return coins;

}