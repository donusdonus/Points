
/**
 * @file Points.h
 * @brief Points container class for managing collections of Point objects
 * @author Points Project
 * @date 2025
 * @version 1.0
 */

#ifndef __POINT_H__
#define __POINT_H__

// Build configuration
//#define EMBEDDED_BUILD  // Uncomment สำหรับ embedded build

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <Point.h>


class PointList
{
    //using Point::Point;
private:
    bool monitor = false;
    Point *next;
    Point *home;
    /* data */
public:
    //~PointList();

    bool Insert(Point **p);
};


#endif // __mountkit_H__
