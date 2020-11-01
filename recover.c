#include <stdio.h>
#include <stdlib.h>


int main(int argc, char *argv[])
{


    // Check for invalid usage
    if (argc != 2)
    {
        return 1;
    }

    //open the file
    FILE *file = fopen(argv[1], "r");
    if (!file)
    {
        return 1;
    }
    //setup buffer
    unsigned char buffer[512];
    int image_count = 0;

    char filename[8];
    FILE *outptr = NULL;
    int repeat = 1;
    // Read buffers until there are none left
    while (repeat)
    {
        //grad the next buffer
        int read = fread(buffer, 512, 1, file);
        //if I'm at the end of the file break
        if (read == 0 && feof(file))
        {
            break;
        }

        // look for the 4 byte starter
        //Check those first three bytes
        //0xff 0xd8 0xff
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff)
        {
            printf("Maybe\n");
            //check the fourth byte
            if ((buffer[3] & 0xf0) == 0xe0)
            {
                printf("Yep! %d", image_count);
                image_count++;
            }

        }
        else
        {
            printf("No\n");
        }
        // Close file
        fclose(file);

        //grab all the bytes into an array (I wonder what size)
        //when I get to the same four bytes close the file and save it.

    }

}
