#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <assert.h>
#include <time.h>
#include <Points.h>

Points tags;

int main()
{
    tags.Add(Point("Point1",32));
    tags.Add(Point("Point2",32));
    tags.Add(Point("Point3",2.3f));

    int count = tags.Count();

    Point *point = tags.At(2);

    if(point != NULL)
    {
        float a = point->getValue<float>(0);
      
        point->setValue(115.12f);
       
        float x = point->getValue<float>(0);
        int d=0;
    }
    
    return 0;
}