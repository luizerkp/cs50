#include <stdio.h>
#include <string.h>

int main(void)
{
    // prompt user to type a name
    char name[20];
    printf("what's your name?\n");

    //store name in char variable "name"
    fgets(name, sizeof(name), stdin);

    //remove newline
    int len = strlen(name);
    if (name[len - 1] == '\n')
    {
        name[len - 1] = 0;
    }

    // respond using typed name
    printf("hello, ");
    puts(name);

}