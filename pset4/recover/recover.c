#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>


typedef uint8_t BYTE;


int main(int argc, char *argv[])
{
    //checks for user not including forensic image
    if (argc != 2)
    {
        printf("Usage: ./recover image");
        return 1;
    }
    //opens forensic image input by user
    FILE *r_file = fopen(argv[1], "r");

    //check if forensic image returns NULL and exits
    if (argv[1] == NULL)
    {
        printf("Usage: ./recover image");
        return 1;
    }

    //Dynamically allocates buffer mem of 512 bytes
    unsigned char *buffer = malloc(512 * (sizeof(BYTE)));

    //checks if buffer mem returns NULL and exits
    if (buffer == NULL)
    {
        printf("malloc failed");
        return 1;
    }

    //file counter
    int number = 0;

    //pointer name img to write our photo data to
    FILE *img = NULL;

    //char array of 7bytes for image file name
    char r_img[7];

    //checks for end of file and reads memory from forensic image
    while (fread(buffer, sizeof(buffer), 1, r_file) == 1)
    {
        //checks for the first 4 bytes for the unique jpeg hexadecimal decignation
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0)
        {
            if (number == 0)
            {
                sprintf(r_img, "%03i.jpg", number); //sets file name to ###.jpg
                img = fopen(r_img, "w"); //open image file
                fwrite(buffer, sizeof(buffer), 1, img); //writes to opened image file
                number++; //keeps track of jpeg number
            }
            else
            {
                fclose(img); //if not first image closes previous image
                sprintf(r_img, "%03i.jpg", number); //new image filename
                img = fopen(r_img, "w"); //opens new image file
                fwrite(buffer, sizeof(buffer), 1, img); //writes to new image file
                number++; //keeps track of jpeg number
            }
        }
        else if (number > 0)
        {
            fwrite(buffer, sizeof(buffer), 1, img); //if image already open continues to write
        }
    }

    free(buffer); //frees previously allocated memory
    fclose(r_file); //closes forensic image file
    fclose(img); //closes last image file
    return 0;
}
