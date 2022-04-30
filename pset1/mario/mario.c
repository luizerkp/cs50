#include <stdio.h>


// recursive function 
void draw_pyramid(int size, int space);

int main(void)
{
    
    // obtain height input from user 
    int height;
    do
    {
        printf("height: ");
        scanf("%d", &height);
    }
    // ensure only answers btwn 1-8
    while (height <= 0 || height > 8);
    
    // calls recursive function "draw (height - 1, space + 1)" until height == 0, 
    // note we pass 0 as second argument to indicate the number of spaces per call, which increases +1 each recursive call
    draw_pyramid(height, 0);
    
}

// recursively draws pyramid 
void draw_pyramid(int size, int space)
{
    if (size == 0)
    {
        return;
    }

    // calls recursive function "draw (height - 1, space + 1)" until height == 0, 
    draw_pyramid(size - 1, space + 1);
   
   // left side 
    for (int y = 0; y < space; y++)
    {
        printf(" ");
    }
    for (int j = 0; j < size; j++)
    {
        printf("#");
    }
    printf("  ");

    // right side
    for (int i = 0; i < size; i++)
    {
        printf("#");
    }
    printf("\n");
    
}




