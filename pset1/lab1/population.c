#include <stdio.h>

int main(void)
{

    // Prompt for start size
    int start_size;
    do
    {
        printf("Start Size: ");
        scanf("%d", &start_size);
    }
    while (start_size < 9);

    // Prompt for end size
    int end_size;
    do
    {
        printf("Start Size: ");
        scanf("%d", &end_size);
    }
    while (end_size < start_size);

    // Calculate number of years until we reach threshold
    int years = 0;
    int population = start_size;
    while (population < end_size)
    {
        population = population - (population / 4) + (population / 3);
        years++;
    }

    // Print number of years
    printf("Years: %d\n", years);

}