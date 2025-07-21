
/**
 * @file Points.h
 * @brief Points container class for managing collections of Point objects
 * @author Points Project
 * @date 2025
 * @version 1.0
 */

#ifndef __POINTS_H__
#define __POINTS_H__

// Build configuration
//#define EMBEDDED_BUILD  // Uncomment สำหรับ embedded build

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#define POINTS_DEBUG 
#define MAX_BUFFER_DISPLAY 128
#define INCLUDE_NULL_END 1

enum isType
{
    UNKNOWN,
    CHAR_T,
    INT8_T,
    UINT8_T,
    INT16_T,
    UINT16_T,
    INT32_T,
    UINT32_T,
    FLOAT_T,
    DOUBLE_T,
    STRING_T
};

struct TypeInfo {
    const char* name;       
    size_t size;            
    const char* format;     
};

// Technical “Template Type Traits”
// ✅ compile-time type mapping
template<typename PointType> constexpr isType typeOf() { return UNKNOWN; }

template<> constexpr isType typeOf<char>()        { return CHAR_T; }
template<> constexpr isType typeOf<int8_t>()      { return INT8_T; }
template<> constexpr isType typeOf<uint8_t>()     { return UINT8_T; }
template<> constexpr isType typeOf<int16_t>()     { return INT16_T; }
template<> constexpr isType typeOf<uint16_t>()    { return UINT16_T; }
template<> constexpr isType typeOf<int32_t>()     { return INT32_T; }
template<> constexpr isType typeOf<uint32_t>()    { return UINT32_T; }
template<> constexpr isType typeOf<float>()       { return FLOAT_T; }  
template<> constexpr isType typeOf<double>()      { return DOUBLE_T; }
template<> constexpr isType typeOf<const char*>() { return STRING_T; }
template<> constexpr isType typeOf<char*>()       { return STRING_T; }


static const TypeInfo SchematicPoint[] = 
{
    {"UNKNOWN",0,"%s"},
    {"CHAR_T",sizeof(char),"%c"},
    {"INT8_T",sizeof(int8_t),"%d"},
    {"UINT8_T",sizeof(uint8_t),"%u"},
    {"INT16_T",sizeof(int16_t),"%d"},
    {"UINT16_T",sizeof(uint16_t),"%u"},
    {"INT32_T",sizeof(int32_t),"%d"},
    {"UINT32_T",sizeof(uint32_t),"%u"},
    {"FLOAT_T",sizeof(float),"%.3f"},
    {"DOUBLE_T",sizeof(double),"%.3f"},
    {"STRING_T",0,"%s"}
};


struct property
{
    public:
    size_t _size_per_element = 0;
    size_t _capacity = 1;
    isType _type = isType::UNKNOWN;

    void SetCapacity(isType type, size_t capacity)
    {
        _type = type;
        _capacity = capacity;
        _size_per_element = SchematicPoint[_type].size;
    }
    size_t GetAllocatedSize()
    {
        return _size_per_element * _capacity;
    }
    size_t GetSizePerElement()
    {
        return _size_per_element;
    }
    size_t GetCapacity()
    {
        return _capacity;
    }
    isType GetType()
    {
        return _type;
    }
    const char * GetTypeName()
    {
        return SchematicPoint[_type].name;
    }
      
    void Clear()
    {
        _size_per_element = 0;
        _capacity = 0;
        _type = isType::UNKNOWN;
    }

};

struct Point
{
    public:

    char * _name = nullptr;
    void * _data = nullptr;
    property _property;
    Point * _previous = nullptr; // [TODO] Reserved for future double-linked list
    Point * _next = nullptr;
    public:

#pragma region "Single Point Method"
    
    template<typename PointType>
    Point(const char *name, PointType data, size_t capacity = 1) : Point(name,&data,capacity){}

    template<typename PointType>
    Point(const char *name, PointType *data, size_t capacity = 1)
    {
        setName(name);
        setValue(data,capacity);
    }


    bool setName(const char *Name)
    {
        size_t len = strlen(Name) + INCLUDE_NULL_END;

        if(len <= 0) return false;

        /* Clear Memory */
        free(_name);
        _name = nullptr;

        /* Allocate new memory */
        _name = (char*)malloc(sizeof(uint8_t)*len);

        if(_name == nullptr)
            return false;

        memcpy(_name,Name,len);

        return true;
    }
    const char * getName() { return _name;}

    template<typename PointType>
    bool setValue(PointType *data, size_t capacity = 1)
    {

        _property.SetCapacity(typeOf<PointType>(), capacity);

        /* clear data */
        if(_data != nullptr)
        {
            free(_data);
            _data = nullptr;
        }
 
        /* Allocate new memory */
         _data = malloc(_property.GetAllocatedSize());
        
        if(_data == nullptr) return false;

        memcpy(_data, data, _property.GetAllocatedSize());

        return true;
    }
    
    template<typename PointType>
    bool setValue(PointType  data)
    {
        return setValue(&data, 1);
    }

    void * getRawValue()
    {
        if (_data == nullptr) return nullptr;
        return _data;
    }

    template<typename t>
    t getValue(size_t index = 0)
    {

        if (_data == nullptr || index >= _property._capacity) return t{};
                                                  
        size_t offest = index * _property._size_per_element;

        if(_data+offest == nullptr) return t{};

        t *ptr = (t*)_data;
        
        return ptr[index];
    }
  
    size_t getMemoryUsage()
    {
        if (_data == nullptr) return 0;
        return _property.GetAllocatedSize()  +  strlen(_name) ;
    }

    property getProperty() { return _property; }

    void clear()
    {
        if (_name) {
            free(_name);
            _name = nullptr;
        }
        if (_data) {
            free(_data);
            _data = nullptr;
        }
        _property.Clear();
    }

    bool copy(Point src)
    {
            clear(); // Clear current data
            // Copy data
            _property.SetCapacity(src._property.GetType(), src._property.GetCapacity());
            setName(src._name);
            setValue(src._data, src._property.GetCapacity());
            _next = src._next;
            _previous = src._previous;
            return true;
    }

    Point * Next()
    {
        if( _next == nullptr)
        {
            return nullptr;
        }
        return _next;
    }

    Point * Previous()
    {
        if (_previous == nullptr)
        {
            return nullptr;
        }
        return _previous;
    }

#pragma endregion "Single Point Method"

};

class Points
{
private:
    char  bufferPrint[MAX_BUFFER_DISPLAY];
    Point **Cursor = nullptr;
    Point *Group = nullptr;
    size_t _max_elements = 0;
public:
    Points();
    bool Add(Point p);
    size_t Count();
    Point* Next();
    Point* At(size_t index);
    void Remove(int index);
    Point* Find(const char *name);


};


#endif // __mountkit_H__