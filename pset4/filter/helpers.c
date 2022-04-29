#include "helpers.h"
#include <math.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            //calculate that average value RGB by dividing their sum by 3
            int rgbtavg = roundf(((float)image[i][j].rgbtRed + (float)image[i][j].rgbtGreen + (float)image[i][j].rgbtBlue) / 3);
            image[i][j].rgbtRed = rgbtavg;
            image[i][j].rgbtGreen = rgbtavg; // assigns new grayscale pixel value
            image[i][j].rgbtBlue = rgbtavg;
        }
    }
     return;
}

// Convert image to sepia
void sepia(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            int sred, sgreen, sblue; // stores sepia values of RGB pixels

            // calculates sepia values and store it in temp variable
            sred = roundf(((float)image[i][j].rgbtRed * .393) + ((float)image[i][j].rgbtGreen * .769) + ((float)image[i][j].rgbtBlue * .189));
            sgreen = roundf(((float)image[i][j].rgbtRed * .349) + ((float)image[i][j].rgbtGreen * .686) + ((float)image[i][j].rgbtBlue * .168));
            sblue = roundf(((float)image[i][j].rgbtRed * .272) + ((float)image[i][j].rgbtGreen * .534) + ((float)image[i][j].rgbtBlue * .131));

            if (sred > 255)
            {
                sred = 255;
            }
            if (sgreen > 255)
            {
                sgreen = 255;     // ensures that any values over 255 is capped at 255
            }
            if (sblue > 255)
            {
                sblue = 255;
            }

            image[i][j].rgbtRed = sred;
            image[i][j].rgbtGreen = sgreen;  // changes pixel values to their sepia equivalent
            image[i][j].rgbtBlue = sblue;
        }
    }

    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE temp[height][width]; // temp array for swaping values

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width / 2; j++)
        {
            // use bubble sort to swap pixel values
            temp[i][j] = image[i][(width - 1) - j];
            image[i][(width - 1) - j] = image[i][j];
            image[i][j] = temp[i][j];

        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    // loop to store original image values to a temporary array for calculation of average pixel values
    RGBTRIPLE m_image[height][width];
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            m_image[i][j] = image[i][j];
        }
    }

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++) // pixel ij is the pixel located at ith column and jth row
        {
            // intializes the values of red, green and blue to zero for every pixel for calculation of average
            float bred = 0;
            float bgreen = 0;
            float bblue = 0;

            // intializes the number of pixels being used for calculation of avg to zero for each pixel.
            // **note: the value of pxls is the number we will divide the values of bred, bgreen, and bblue by.
            int pxls = 0;

            for (int si = 0; si <= 2; si++)
            {
                for (int sj = 0; sj <= 2; sj++) // same as above but with row being j + sj.
                {
                    // assings i - 1 + si to column and j - 1 + sj to row for easier undestanding
                    // use -1 to ensure the pixel prior to the current pixel in x and y directions are checked
                    int column = i - 1 + si;
                    int row = j - 1 + sj;
                    //ensures that only pixels between 0 and height - 1 or width - 1 are counted
                    if (column >= 0 && row >= 0 && column <= height - 1 && row <= width - 1)
                    {
                        // adds all surrounding pixels including original
                        bred += m_image[column][row].rgbtRed;
                        bgreen += m_image[column][row].rgbtGreen;
                        bblue += m_image[column][row].rgbtBlue;

                        // counts all the pixels including original
                        pxls++;
                    }
                }
            }
            // divides the total values of red, green and blue pixels by the number of surrounding pixels including original respectively
            image[i][j].rgbtRed = round(bred / pxls);
            image[i][j].rgbtGreen = round(bgreen / pxls);
            image[i][j].rgbtBlue = round(bblue / pxls);
            // **note all final values are rounded**
        }
    }
    return;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    // loop to store original image values to a temporary array for calculation
    RGBTRIPLE m_image[height][width];
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            m_image[i][j] = image[i][j];
        }

    }

    // 2D array to capture gx matrix,
    // think of gx matrix as a grid of 3x3 and here the 2D array can access each cell as gx[x][y]
    //i.e. gx[x][y] is xth column yth row
    int gx[3][3] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};

    // 2D array to capture gy matrix
    // same with gy where gy[x][y] is xth column yth row
    int gy[3][3] = {{-1, -2, -1}, {0, 0, 0}, {1, 2, 1}};

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // intializes the values of red, green and blue to zero for every pixel for calculation of gy and gx matrix arrays
            float gxred = 0;
            float gxgreen = 0;        // gx matrix
            float gxblue = 0;
            float gyred = 0;         // gy matrix
            float gygreen = 0;
            float gyblue = 0;
            float fred = 0;          // final values
            float fgreen = 0;
            float fblue = 0;

            for (int si = 0; si <= 2; si++)
            {
                for (int sj = 0; sj <= 2; sj++) // loops through all surrounding pixels use sj value to link gx and gy arrays.
                {
                    // assings i - 1 + si to column and j - 1 + sj to row for easier undestanding
                    // use -1 to ensure the pixel prior to the current pixel in x and y directions are checked
                    int column = i - 1 + si;
                    int row = j - 1 + sj;

                    //ensures that only pixels between 0 and height - 1 or width - 1 are counted
                    if (column >= 0 && row >= 0 && column <= height - 1 && row <= width - 1)
                    {
                        // multiplies all surrounding pixels including original by gx matrix array
                        gxred += m_image[column][row].rgbtRed * gx[si][sj];
                        gxgreen += m_image[column][row].rgbtGreen * gx[si][sj];
                        gxblue += m_image[column][row].rgbtBlue * gx[si][sj];

                        // multiplies all surrounding pixels including original by gy matrix array
                        gyred += m_image[column][row].rgbtRed * gy[si][sj];
                        gygreen += m_image[column][row].rgbtGreen * gy[si][sj];
                        gyblue += m_image[column][row].rgbtBlue * gy[si][sj];

                        // calculates the final R, G, & B value of every pixel as square root of gx^2 + gy^2
                        fred = sqrt(pow(gxred, 2) + pow(gyred, 2));
                        fgreen = sqrt(pow(gxgreen, 2) + pow(gygreen, 2));
                        fblue = sqrt(pow(gxblue, 2) + pow(gyblue, 2));

                        // caps all R, G, & B values at 255
                        if (fred > 255)
                        {
                            fred = 255;
                        }
                        if (fgreen > 255)
                        {
                            fgreen = 255;
                        }
                        if (fblue > 255)
                        {
                            fblue = 255;
                        }

                        // change all R, G & B values to their final values in original image
                        image[i][j].rgbtRed = round(fred);
                        image[i][j].rgbtGreen = round(fgreen);
                        image[i][j].rgbtBlue = round(fblue);
                        //** note all final values are rounded

                    }

                }
            }

        }
    }

    return;
}
