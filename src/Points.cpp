#include "Points.h"


Points::Points()
{
    /* Initialize Cursor Important !!! */
    Group = NULL;
    Cursor = NULL;\
}

bool Points::Add(Point p)
{
    /* Reference Head Node */
    Cursor = &Group;

    while (*Cursor != NULL)
    {
        /* Find Tail Node */
        Cursor = &((*Cursor)->_next);
    }

    /* New Point Stuct */
    Point *newPoint = (Point*)malloc(sizeof(Point));
    newPoint->copy(p);
    newPoint->_previous = NULL;
    newPoint->_next = NULL;
    
    *Cursor = newPoint;

    p.clear();
        
    return true;
}

size_t Points::Count()
{
    size_t count = 0;
    Cursor = &Group;
    while (*Cursor != NULL)
    {
        count++;
        Cursor = &((*Cursor)->_next);
    }
    return count;
}

Point* Points::At(size_t index)
{
    Cursor = &Group;

    size_t count = 0;

    while(count < index)
    {
        if(*Cursor != NULL)
        {
            Cursor = &(*Cursor)->_next;
        }
        count++;
    }

    return &(**Cursor);

}