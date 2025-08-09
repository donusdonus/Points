#ifndef __TAGSTATIC_H
#define __TAGSTATIC_H

#include "stdio.h"
#include "stdlib.h"
#include "stdint.h"
#include "string.h"


/** Base point configuration macro */
/** Maximum buffer size for printing operations */
#define MARK_CHAR_NULL 0x00
#define MARK_FREE_MEMORY 0x0FF
#define MAX_BUFFER_PRINT 128
#define _SECTOR_RAM_(x,y) calloc(x,y)
#define _SECTOR_PSRAM_(x,y) calloc(x,y)
#define _SECTOR_SPARE1_(x,y) calloc(x,y)
#define _SECTOR_SPARE2_(x,y) calloc(x,y)
#define RAM_FREE(x) free(x)
/** Maximum buffer size for printing operations */


#pragma pack(push,1)
/** @brief Raw memory structure for Point data
 * 
 * This structure holds a pointer to the raw memory buffer and its size.
 * It is used to manage the data associated with Point objects.
 */
struct RawMemory
{
    public:
    /// @brief Pointer to the raw memory buffer
    uint8_t *value;
    /// @brief Size of the raw memory buffer in bytes
    size_t   size;
};
#pragma pack(pop)

#pragma pack(push,1)
/// @brief Memory types for Point data allocation
/// @details This enum defines the memory types used for allocating data buffers in Point objects.
enum isMemory : uint8_t
{
    RAM,    
    PSRAM,
    SPARE_1,
    SPARE_2
};
#pragma pack(pop)

#pragma pack(push,1)
enum isType : uint8_t
{
    VAR_CHAR,     /**< Single character type */
    VAR_INT8,     /**< 8-bit signed integer */
    VAR_UINT8,    /**< 8-bit unsigned integer */
    VAR_INT16,    /**< 16-bit signed integer */
    VAR_UINT16,   /**< 16-bit unsigned integer */
    VAR_INT32,    /**< 32-bit signed integer */
    VAR_UINT32,   /**< 32-bit unsigned integer */
    VAR_FLOAT,    /**< Single precision floating point */
    VAR_DOUBLE   /**< Double precision floating point */
};
#pragma pack(pop)

#pragma pack(push,1)
struct TypeInfo {
    const char* name;       
    uint8_t element_size;        
};
#pragma pack(pop)

static const TypeInfo SchematicPoint[] = 
{
    {"CHAR\0"  , sizeof(char)     },   /**< Character type */
    {"INT8\0"  , sizeof(int8_t)   },   /**< 8-bit signed integer */
    {"UINT8\0" , sizeof(uint8_t)  },   /**< 8-bit unsigned integer */
    {"INT16\0" , sizeof(int16_t)  },   /**< 16-bit signed integer */
    {"UINT16\0", sizeof(uint16_t) },   /**< 16-bit unsigned integer */
    {"INT32\0" , sizeof(int32_t)  },   /**< 32-bit signed integer */
    {"UINT32\0", sizeof(uint32_t) },   /**< 32-bit unsigned integer */
    {"FLOAT\0" , sizeof(float)    },   /**< Single precision float */
    {"DOUBLE\0", sizeof(double)   }    /**< Double precision float */
};

static char PrintOut[MAX_BUFFER_PRINT];
typedef size_t (*PrintPointData)(size_t ,void *,size_t);
size_t static DISP_CHAR(size_t offset,void *data,size_t index){ return  snprintf(PrintOut + offset,MAX_BUFFER_PRINT - offset,"%c",((char*)data)[index]);}
size_t static DISP_INT8(size_t offset,void *data,size_t index){ return  snprintf(PrintOut + offset,MAX_BUFFER_PRINT - offset,"%d ",((int8_t*)data)[index]);}
size_t static DISP_UINT8(size_t offset,void *data,size_t index){ return  snprintf(PrintOut + offset,MAX_BUFFER_PRINT - offset,"%d ",((uint8_t*)data)[index]);}
size_t static DISP_INT16(size_t offset,void *data,size_t index){ return  snprintf(PrintOut + offset,MAX_BUFFER_PRINT - offset,"%d ",((int16_t*)data)[index]);}
size_t static DISP_UINT16(size_t offset,void *data,size_t index){ return  snprintf(PrintOut + offset,MAX_BUFFER_PRINT - offset,"%d ",((uint16_t*)data)[index]);}
size_t static DISP_INT32(size_t offset,void *data,size_t index){ return  snprintf(PrintOut + offset,MAX_BUFFER_PRINT - offset,"%d ",((int32_t*)data)[index]);}
size_t static DISP_UINT32(size_t offset,void *data,size_t index){ return  snprintf(PrintOut + offset,MAX_BUFFER_PRINT - offset,"%d ",((uint32_t*)data)[index]);}
size_t static DISP_FLOAT(size_t offset,void *data,size_t index){ return  snprintf(PrintOut + offset,MAX_BUFFER_PRINT - offset,"%.3f ",((float*)data)[index]);}
size_t static DISP_DOUBLE(size_t offset,void *data,size_t index){ return  snprintf(PrintOut + offset,MAX_BUFFER_PRINT - offset,"%.3f ",((double*)data)[index]);}

PrintPointData static FuncPrintValue[]   = {
                                       &DISP_CHAR,
                                       &DISP_INT8,
                                       &DISP_UINT8,
                                       &DISP_INT16,
                                       &DISP_UINT16,
                                       &DISP_INT32,
                                       &DISP_UINT32,
                                       &DISP_FLOAT,
                                       &DISP_DOUBLE
                                   };

#pragma pack(push,1)
struct Tag
{
    RawMemory name;       /**< Name of the component */
    RawMemory data;       /**< Data buffer for the component */
    isMemory memType:8;  /**< Memory type for the component */
    uint8_t external_alloc:8;
    isType type:8;        /**< Data type of the component */

    Tag *next;
    Tag *first;

    template<typename T>
    bool SetValue(T value,size_t index = 0)
    {
        bool monitor;
        size_t a , b ;

        a = sizeof(T);
        b = SchematicPoint[type].element_size;

        /* Check Elements Per Size is equal */
        monitor = (a == b);
        if(!monitor)
            return !monitor;

        /* Check is not over index */
        monitor = ((data.size / SchematicPoint[type].element_size) < index);
        if(monitor)
            return monitor;
        
        T *tmp = (T*)data.value; 

        tmp[index] = value;

        return true ;
    }

    template<typename T>
    T GetValue(size_t index = 0)
    {
        bool monitor;

        monitor = (sizeof(T) == SchematicPoint[type].element_size);
        if(!monitor)
            return T{};

        monitor = (GetArraySize() < index);
        if(monitor)
            return T{};
        
        T *tmp = (T*)data.value; 

        return tmp[index];
    }

    size_t GetArraySize()
    {
        return  data.size / SchematicPoint[type].element_size ;
    }

    const char * MonitorInfo()
    {
        size_t out = 0;
        out = sprintf(&PrintOut[0],"  %s %s[%d]\n",SchematicPoint[type].name,name.value,GetArraySize()+1);
        return &PrintOut[0];
    }

    const char * MonitorValue(size_t index)
    {
        size_t cur = 0;
        cur = sprintf(PrintOut,"    %s[%d] = ",name.value,index);
        cur += FuncPrintValue[type](cur,data.value,index);
        cur += sprintf(&PrintOut[cur],"\n");
        return &PrintOut[0];
    }
};
#pragma pack(pop)

struct TagGroup
{
    Tag Item; 
    TagGroup *Next = nullptr;
    TagGroup *First = nullptr;

    Tag * ItemAdd(Topic **src,isType type,const char * name,size_t array_size=1,isMemory memtype = isMemory::RAM)
    {
        TagGroup **cur = nullptr;

        cur = &(*src)->Group;

        /* 1. Find last elements for connect */
        while ((cur != nullptr) && (*cur != nullptr))
        {
            cur = &(*cur)->Next;
        }

        TagGroup *newItem = (TagGroup*)Allocator(memtype,array_size,sizeof(TagGroup));
        
        monitor = (newItem != nullptr);
        if(!monitor)
            return nullptr;
    
        monitor = SetName(&newItem->Item.name,name,memtype);
        if(!monitor)
            return nullptr;

        newItem->Item.data.value = (uint8_t*)Allocator(memtype,array_size,SchematicPoint[type].element_size);

        monitor = (newItem->Item.data.value != nullptr);
        if(!monitor)
            return nullptr;

        newItem->Item.data.size = SchematicPoint[type].element_size * array_size;
        newItem->Item.memType = memtype;
        newItem->Item.type = type;


        *cur = newItem;
        //(*cur)->Home = (*src)->Group;
     

        return nullptr;
    }
};

#pragma pack(push,1)
struct Topic
{
    RawMemory name;       /**< Name of the group */
    isMemory memType;
    TagGroup * Group = nullptr;
    Topic *Home = nullptr ;      /**< Pointer to the first component in the list */
    Topic *Next = nullptr ;       /**< Pointer to the home component in the list */

    const char * MonitorInfo()
    {
        size_t out = 0;
        out = sprintf(&PrintOut[0],"TOPIC %s\n",name.value);
        return &PrintOut[0];
    }

};
#pragma pack(pop)


static void * Allocator(isMemory memType, size_t elements, size_t elementSize)
{
    switch (memType)
    {
    case isMemory::RAM:
        return _SECTOR_RAM_(elements, elementSize);
        break;
    case isMemory::PSRAM:
        return _SECTOR_PSRAM_(elements, elementSize);
        break;
    case isMemory::SPARE_1:
        return _SECTOR_SPARE1_(elements, elementSize);
        break;
    case isMemory::SPARE_2:
        return _SECTOR_SPARE2_(elements, elementSize);
        break;
    default:
        return _SECTOR_RAM_(elements, elementSize);
        break;
    }
    return nullptr;
}

#endif // TAGSTATIC_H