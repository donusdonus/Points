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

/** Base point configuration macro */
/** Maximum buffer size for printing operations */
#define MARK_CHAR_NULL 0x00
#define MARK_FREE_MEMORY 0x0FF
#define MAX_BUFFER_PRINT 128
#define _SECTOR_RAM_(x) malloc(x)
#define _SECTOR_PSRAM_(x) malloc(x)
#define _SECTOR_SPARE1_(x) malloc(x)
#define _SECTOR_SPARE2_(x) malloc(x)
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
enum isMemory
{
    RAM,    
    PSRAM,
    SPARE_1,
    SPARE_2
};
#pragma pack(pop)

#pragma pack(push,1)
/// @brief Enumeration of supported data types for Point objects
/// @details This enum defines all the data types that can be stored and managed
/// by Point objects, including primitive types and group containers.
enum isType
{
    VAR_GROUP,    /**< Group container type for hierarchical data */
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
/// @brief Type information structure for Point data types
/// @details This structure contains metadata about each data type, including its name, size, and
/// whether it supports read and write operations.
struct TypeInfo {
    const char* name;       
    uint8_t element_size;        
    bool canRead:1;          
    bool canWrite:1;    
};
#pragma pack(pop)


/* FlagMark Properties of Point */
#pragma pack(push,1)
typedef union FlagMark
{
    uint16_t Value = 0;
    struct 
    {       
        uint8_t DataAllocated : 1;
        uint8_t NameAllocated : 1;
        isMemory MemType : 2;
        /* check type */
        isType VarType : 4; 
        /* In Group */
        uint8_t InGroup : 1;
    };   
}FlagMark;
#pragma pack(pop)

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

static const TypeInfo SchematicPoint[] = 
{
    {"GROUP"  + MARK_CHAR_NULL , 1                ,true,false},  /**< Group type - read only */
    {"CHAR"   + MARK_CHAR_NULL , sizeof(char)     ,true,true},   /**< Character type */
    {"INT8"   + MARK_CHAR_NULL , sizeof(int8_t)   ,true,true},   /**< 8-bit signed integer */
    {"UINT8"  + MARK_CHAR_NULL , sizeof(uint8_t)  ,true,true},   /**< 8-bit unsigned integer */
    {"INT16"  + MARK_CHAR_NULL , sizeof(int16_t)  ,true,true},   /**< 16-bit signed integer */
    {"UINT16" + MARK_CHAR_NULL , sizeof(uint16_t) ,true,true},   /**< 16-bit unsigned integer */
    {"INT32"  + MARK_CHAR_NULL , sizeof(int32_t)  ,true,true},   /**< 32-bit signed integer */
    {"UINT32" + MARK_CHAR_NULL , sizeof(uint32_t) ,true,true},   /**< 32-bit unsigned integer */
    {"FLOAT"  + MARK_CHAR_NULL , sizeof(float)    ,true,true},   /**< Single precision float */
    {"DOUBLE" + MARK_CHAR_NULL , sizeof(double)   ,true,true}    /**< Double precision float */
};


PrintPointData static FuncPrintValue[]   = {
                                       nullptr,
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


class Point {
public:

    /// @brief Create Point as GROUP_T Only
    /// @param name name of group
    /// @param memtype specification memory type for allocate
    Point(const char * name,isMemory memtype = isMemory::RAM);

    /// @brief Create Point With Assign type and reference addr with size
    /// @param type type of Point
    /// @param name name of Point
    /// @param addr reference address 
    /// @param numberElements Number Elements ArraySize
    /// @param memtype specification memory type for allocate
    Point(isType type , const char * name,uint8_t *addr,size_t numberElements = 1,isMemory memtype = isMemory::RAM);
     
    /// @brief Create Point by assign type with init array size 
    /// @param type type of Point
    /// @param name name of Point
    /// @param numberElements Number Elements ArraySize
    /// @param memtype specification memory type for allocate 
    Point(isType type,const char * name,size_t numberElements = 1 ,isMemory memtype = isMemory::RAM);
    
    ~Point() {
                    int index = GetPointCount();
                    if(index > 0)
                        for (size_t i = 0; i < index; i++)
                            DeleteByIndex(i);
                    Remove();
                    free(this);
             };
    
    /// @brief SetValue Point and assign index
    /// @tparam T reference enum is type
    /// @param data Single Data Point
    /// @param index assign index of elements
    /// @return true is can setvalue , false cannot setvalue
    template<typename T>
    bool SetValue(T data,size_t index = 0)
    {
        /* Init Monitor */
        monitor = true;

        /* Check Point State is Ready */
        monitor = (_prop.DataAllocated & _prop.NameAllocated);
        if(!monitor) 
            return monitor;

        /* Check State type is support write */
        monitor = SchematicPoint[_prop.VarType].canWrite;    
        if(!monitor) 
            return monitor;

        /* Check Size T is Correct */
        monitor = (sizeof(T) == SchematicPoint[_prop.VarType].element_size)? true : false;
        if(!monitor)
            return monitor;

        /* Check index is outside element */
        monitor = ((_data.size/SchematicPoint[_prop.VarType].element_size) >= index)? true : false ;
        if(!monitor)
            return monitor;

        /* Copy Data  */
      
        memcpy( 
                &_data.value[SchematicPoint[_prop.VarType].element_size*index],
                &data,
                sizeof(data)
              );

        return monitor;
    }


    /// @brief GetValue Point
    /// @tparam T reference enum is type
    /// @param Index Index of Point
    /// @return Data in Point
    template<typename T>
    T GetValue(size_t index = 0)
    {
        /* Init Monitor */
        monitor = true;

        T var{} ;

        /* Check Point State is Ready */
        monitor = (_prop.DataAllocated & _prop.NameAllocated);
        if(!monitor) 
            return T{};

        /* Check State type is support  */
        monitor = SchematicPoint[_prop.VarType].canRead;    
        if(!monitor) 
            return T{};

        /* Check Size T is Correct */
        monitor = (sizeof(T) == SchematicPoint[_prop.VarType].element_size)? true : false;
        if(!monitor)
            return T{};

        /* Check index is outside element */
        monitor = ((_data.size/SchematicPoint[_prop.VarType].element_size) > index)? true : false ;
        if(!monitor)
            return T{};

        /* Return Data  */
        T* ptr = (T*)_data.value;
        return ptr[index] ;
    }

    /// @brief Get Elements Array Count
    /// @return Size of Array Point
    const size_t Count();

    /// @brief Get Raw Buffer Data Point
    /// @param src source reference Data
    /// @return true if buffer can reference
    bool GetBuffer(RawMemory *src);
    
    bool SetName(const char *name);
    /// @brief Get Name of Point
    /// @return Point Name
    const char* GetName();
    
    /// @brief Get Point Type
    /// @return Point Type
    const isType GetType();

    /// @brief Get Summary Size of Point
    /// @return size of point
    const size_t GetTotalByteSize();

    /// @brief Get Summary Size of Data Byte Buffer
    /// @return Byte Buffer Size of Data
    const size_t GetValueByteSize();

    /// @brief Get Flag Mark of Point
    /// @details This function returns the current flag mark of the Point, which includes metadata such
    /// @return Flag Mark
    const FlagMark GetFlagMark();

    //bool Copy(Point *src);

    /// @brief Get Home Pointer of Point
    /// @param None
    /// @return Pointer to Home Point
    Point * Home(void);
    
    /// @brief Insert a new Point into GROUP_T. must start Call by GROUP_T
    /// @param type Type of Point to insert
    /// @param name Name of the new Point
    /// @param numberElements Number of elements in the new Point
    /// @param memtype Memory type for the new Point
    Point * Insert(
                    isType type , 
                    const char * name,
                    size_t numberElements = 1,
                    isMemory memtype = isMemory::RAM
                  );
 
    /// @brief Find a Point by name. must start Call by GROUP_T
    /// @param name Name of the Point to find
    /// @return Pointer to the found Point, or nullptr if not found
    Point * FindByName(const char name[]);


    /// @brief Find a Point by index. must start Call by GROUP_T
    /// @param index Index of the Point to find
    /// @return Pointer to the found Point, or nullptr if not found
    Point * FindByIndex(size_t index);

    /// @brief Get the index of the current Point
    /// @return Index of the current Point
    int  GetIndex();

    /// @brief Get the count of Points in the current group
    /// @return Number of Points in the group
    size_t GetPointCount();

    /// @brief Delete a Point by name. must start Call by GROUP_T
    /// @param name Name of the Point to delete
    /// @return true if the Point was deleted, false otherwise
    bool DeleteByName(const char name[]);

    /// @brief Delete a Point by index. must start Call by GROUP_T
    /// @param index Index of the Point to delete
    /// @return true if the Point was deleted, false otherwise
    bool DeleteByIndex(size_t index);

    /// @brief Get the context information of the Point
    /// @return Pointer to the context information string
    const char * DisplayContext();

    /// @brief Get the value of the Point as a string
    /// @param index Index of the element to display
    /// @return Pointer to the value string
    const char * DisplayValue(size_t index = 0);


private:

    #pragma region "Variable Group"
        RawMemory _name;
        RawMemory _data;
        FlagMark _prop;
    
        Point *_ptr_home;                       /**< Pointer to group members (for GROUP_T) */
        Point *_ptr_next;                       /**< Pointer to group members (for GROUP_T) */
    #pragma endregion
 
    size_t DeleteAll(void);
    bool ByteAllocate(isMemory memtype,size_t numberelements);
    void Remove();
    bool monitor = false;
};

#endif // POINT_H
