#include <stdio.h>
#include <Point.h>

Point root("Embedded");

int main()
{
    for(int n = 0;n<20;n++)
    {
        char name[16];

        sprintf(&name[0],"Point_%d",n);
        root.AddSlot(Point<char*>(UINT8_T,name,,));

    }

    return 0;
}