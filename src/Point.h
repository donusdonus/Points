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

/** @brief Base point configuration macro */
#define BASE_POINT_CONFIG
#ifdef BASE_POINT_CONFIG 
/** @brief Include null terminator in string calculations */
#define INCLUDE_NULL_END 1
/** @brief Maximum buffer size for printing operations */
#define MAX_BUFFER_PRINT 128
/** @brief Maximum characters for point names including null terminator */
#define MAX_CHAR_NAME 16 + INCLUDE_NULL_END
#endif

/**
 * @brief Enumeration of supported data types for Point objects
 * 
 * This enum defines all the data types that can be stored and managed
 * by Point objects, including primitive types and group containers.
 */
/**
 * @brief Enumeration of supported data types for Point objects
 * 
 * This enum defines all the data types that can be stored and managed
 * by Point objects, including primitive types and group containers.
 */
enum isType
{
    GROUP_T,    /**< Group container type for hierarchical data */
    CHAR_T,     /**< Single character type */
    INT8_T,     /**< 8-bit signed integer */
    UINT8_T,    /**< 8-bit unsigned integer */
    INT16_T,    /**< 16-bit signed integer */
    UINT16_T,   /**< 16-bit unsigned integer */
    INT32_T,    /**< 32-bit signed integer */
    UINT32_T,   /**< 32-bit unsigned integer */
    FLOAT_T,    /**< Single precision floating point */
    DOUBLE_T,   /**< Double precision floating point */
};

/**
 * @brief Type information structure for Point schema
 * 
 * Contains metadata about each supported data type including
 * size information, format specifiers, and access permissions.
 */
struct TypeInfo {
    const char* name;       /**< Human-readable type name */
    size_t size;            /**< Size in bytes of the data type */
    const char* format;     /**< Printf format specifier for this type */
    bool canRead;           /**< Whether this type supports read operations */
    bool canWrite;          /**< Whether this type supports write operations */
};

/**
 * @brief Static array containing type information for all supported types
 * 
 * This lookup table provides metadata for each type defined in isType enum.
 * Used internally for type validation, size calculations, and formatting.
 */
/**
 * @brief Static array containing type information for all supported types
 * 
 * This lookup table provides metadata for each type defined in isType enum.
 * Used internally for type validation, size calculations, and formatting.
 */
static const TypeInfo SchematicPoint[] = 
{
    {"GROUP" , sizeof(NULL)     ,"%s"   ,true,false},  /**< Group type - read only */
    {"CHAR"  , sizeof(char)     ,"%c"   ,true,true},   /**< Character type */
    {"INT8"  , sizeof(int8_t)   ,"%d"   ,true,true},   /**< 8-bit signed integer */
    {"UINT8" , sizeof(uint8_t)  ,"%u"   ,true,true},   /**< 8-bit unsigned integer */
    {"INT16" , sizeof(int16_t)  , "%d"  ,true,true},   /**< 16-bit signed integer */
    {"UINT16", sizeof(uint16_t) ,"%u"   ,true,true},   /**< 16-bit unsigned integer */
    {"INT32" , sizeof(int32_t)  , "%d"  ,true,true},   /**< 32-bit signed integer */
    {"UINT32", sizeof(uint32_t) ,"%u"   ,true,true},   /**< 32-bit unsigned integer */
    {"FLOAT" , sizeof(float)    ,"%.3f" ,true,true},   /**< Single precision float */
    {"DOUBLE", sizeof(double)   ,"%.3f" ,true,true}    /**< Double precision float */
};

/**
 * @brief Point class for flexible data type management
 * 
 * The Point class provides a flexible container for storing various data types
 * with type safety, automatic memory management, and support for arrays.
 * It can store primitive types (int, float, char, etc.) as well as organize
 * points into hierarchical groups.
 * 
 * Key features:
 * - Template-based type-safe value access
 * - Dynamic memory allocation for data storage
 * - Support for arrays (multiple elements per point)
 * - Linked list structure for point organization
 * - Group functionality for hierarchical data
 * 
 * @note This class uses raw pointers for memory management.
 *       Ensure proper cleanup when using in production code.
 */
class Point {
public:

    /**
     * @brief Constructor for creating a Point with specified type and name
     * @param type The data type for this point (from isType enum)
     * @param name Human-readable name for the point (max 16 characters)
     * @param numberElements Number of elements to allocate (default: 1)
     */
    Point(isType type,const char * name,size_t numberElements);

    /**
     * @brief Template constructor for creating a Point with initial data
     * @tparam T The C++ type of the initial data
     * @param type The data type for this point (from isType enum)
     * @param name Human-readable name for the point
     * @param data Initial data value to store
     */
    template <typename T>
    Point(isType type,const char * name,T data);

    /**
     * @brief Template constructor for creating a Point with array data
     * @tparam T The C++ type of the array elements
     * @param type The data type for this point (from isType enum)
     * @param name Human-readable name for the point
     * @param data Pointer to array data to copy
     * @param numberElements Number of elements in the array (default: 1)
     */
    template <typename T>
    Point(isType type,const char * name,T *data,size_t numberElements = 1);

    /**
     * @brief Set a single value in the point
     * @tparam T The C++ type of the value
     * @param data The value to store
     * @return true if successful, false otherwise
     */
    template<typename T>
    bool SetValue(T data);

    /**
     * @brief Set array values in the point
     * @tparam T The C++ type of the array elements
     * @param data Pointer to array data to copy
     * @param size Number of elements to copy (default: 1)
     * @return true if successful, false otherwise
     */
    template<typename T>
    bool SetValue(T *data,size_t size = 1);

    /**
     * @brief Get the first value from the point
     * @tparam T The C++ type to cast the value to
     * @return The value cast to type T
     */
    template<typename T>
    T GetValue();

    /**
     * @brief Get a value at specific index from the point
     * @tparam T The C++ type to cast the value to
     * @param index Zero-based index of the element to retrieve
     * @return The value at the specified index cast to type T
     */
    template<typename T>
    T GetValue(size_t index);

    /**
     * @brief Get direct access to the raw data buffer
     * @return Pointer to the raw data buffer
     * @warning Use with caution - direct memory access without type checking
     */
    void * GetRawBuffer();
    
    /**
     * @brief Clear all data in the point (set to zero)
     */
    void ClearValue(); 

    /**
     * @brief Set the name of the point
     * @param name New name for the point (max 16 characters)
     * @return true if successful, false if name too long
     */
    bool SetName(const char name[]);
    
    /**
     * @brief Get the name of the point
     * @return Pointer to the point's name string
     */
    const char* GetName();
    
    /**
     * @brief Get type information for this point
     * @return TypeInfo structure containing metadata about the point's type
     */
    const TypeInfo GetTypeInfo();
    
    /**
     * @brief Get the number of elements in this point
     * @return Number of elements stored in the point
     */
    const size_t Count();
    
    /**
     * @brief Get the total byte size of the data
     * @return Total number of bytes allocated for data storage
     */
    const size_t GetByteSizeOfValue();

    /* Group Management Functions */
    
    /**
     * @brief Add a new slot to this point (if it's a GROUP_T)
     * @return true if successful, false if not a group or allocation failed
     */
    bool AddSlot(Point point);
    
    /**
     * @brief Find a point by name within this group
     * @param name Name of the point to search for
     * @return Pointer to the found point, or nullptr if not found
     */
    Point* FindSlot(const char name[]);
    
    /**
     * @brief Delete a point from this group
     * @param Point Pointer to the point to delete
     * @return true if successful, false if not found or not a group
     */
    bool DeleteSlot(Point *Point);

    

private:
    char  _name[MAX_CHAR_NAME];          /**< Point name storage */
    void* _data;                         /**< Raw data buffer */

    size_t _element_size = 1;            /**< Number of elements stored */
    size_t _total_byte_size = 0;         /**< Total allocated bytes */

    isType _type;                        /**< Type of data stored */
    Point *_group;                       /**< Pointer to group members (for GROUP_T) */
    Point *_next;                        /**< Next point in linked list */
    Point *_previous;                    /**< Previous point (reserved for future use) */

    /**
     * @brief Initialize point members to default values
     */
    void Init();
    
    /**
     * @brief Calculate required bytes for given type and element count
     * @param type Data type
     * @param numberElements Number of elements
     */
    void ByteCount(isType type,size_t numberElements);
    
    /**
     * @brief Allocate memory for data storage
     * @return true if successful, false on allocation failure
     */
    bool ByteAllocate();
    
    /**
     * @brief Clean up and deallocate resources
     */
    void Clear();

    bool Copy(const Point src);
    
    /**
     * @brief Fill data buffer with specified byte value
     * @param mark Byte value to fill the buffer with
     */
    void FillEveryByteInData(uint8_t mark);

};

#endif // POINT_H
