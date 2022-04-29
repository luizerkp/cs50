// Modifies the volume of an audio file

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// Number of bytes in .wav header
const int HEADER_SIZE = 44;
// 2 bytes size
typedef int16_t SAMPLE;
// 1 byte size
typedef uint8_t BYTE;

int main(int argc, char *argv[])
{
    // Check command-line arguments
    if (argc != 4)
    {
        printf("Usage: ./volume input.wav output.wav factor\n");
        return 1;
    }

    // Open files and determine scaling factor
    FILE *input = fopen(argv[1], "r");
    if (input == NULL)
    {
        printf("Could not open file.\n");
        return 1;
    }

    FILE *output = fopen(argv[2], "w");
    if (output == NULL)
    {
        printf("Could not open file.\n");
        return 1;
    }
    
    float factor = atof(argv[3]);
    SAMPLE buffer[0];
    BYTE header[HEADER_SIZE];
                
    // Copy header from input file to output file
    fread(header, sizeof(BYTE), HEADER_SIZE, input);
    fwrite(header, sizeof(BYTE), HEADER_SIZE, output);

    // Read samples from input file and write updated data to output file
    while (fread(&buffer, sizeof(SAMPLE), 1, input) == 1)
    {
        buffer[0] *= factor;
        fwrite(&buffer, sizeof(SAMPLE), 1, output);
    }

    // Close files
    fclose(input);
    fclose(output);
}
