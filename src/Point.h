/**
 * @file Point.h
 * @brief Point class for flexible data type management
 * @author Points Project
 * @date 2025
 * @version 1.0
 */

#ifndef ___POINT_H___
#define ___POINT_H___

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "PointStatic.h"



class Point {

//friend class PointList;

public:

    template<typename T>
    bool SetValue(T data,size_t index = 0)
    {   
        return monitor;
    }

    template<typename T>
    T GetValue(size_t index = 0)
    {
        return T{} ;
    }

  
    bool SetName(const char *name);
    const char* GetName();
    const isType GetType();
    const size_t ByteSize();
    const size_t Count();
    const char * DisplayContext();
    const char * DisplayValue(size_t index = 0);

private:

    bool monitor = false;

    #pragma region "Variable Group"

        void *_item_home;                       /**< Pointer to group members (for GROUP_T) */
        void *_item_next;                       /**< Pointer to group members (for GROUP_T) */
    
        int a = sizeof(Point);

    #pragma endregion
 
    bool ByteAllocate(isMemory memtype,size_t numberelements);
    void Remove();

    Point * init(isType type , const char * name,uint8_t *addr,size_t numberElements = 1,isMemory memtype = isMemory::RAM,bool clone = false);
    Point * init(isType type,const char * name,size_t numberElements = 1 ,isMemory memtype = isMemory::RAM,bool clone = false);
    
    /* modified */
    Point * index(size_t index,bool operate_node = false);
    Point * find(const char * name,bool operate_node = false);
    Point * find(size_t index,bool operate_node = false);
    Point * get(bool operate_node = false);
   
};

#endif // POINT_H
