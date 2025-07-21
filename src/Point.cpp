#include "Point.h"

Point::Point(isType type , const char * name,size_t numberElements = 1)
{
    Init();
    SetName(name);
    ByteCount(type,numberElements);
    ByteAllocate();
    FillEveryByteInData(0x00);
}

template <typename T>
Point::Point(isType type,const char * name,T data)
{

    Init();
    SetName(name);
    ByteCount(type,1);
    ByteAllocate();
    FillEveryByteInData(0x00);
    SetValue(data);
}

template <typename T>
Point::Point(isType type,const char * name,T *data,size_t numberElements =1)
{
    Init();
    SetName(name);
    ByteCount(type,numberElements);
    ByteAllocate();
    FillEveryByteInData(0x00);
    SetValue(data,numberElements);   
}

void Point::Init()
{
    _type = isType::GROUP_T;
    _data = nullptr;
    _next = nullptr;
    _previous = nullptr;
}

void Point::Clear()
{

    memset(_name,0xFF,MAX_CHAR_NAME);

    if(_data != nullptr)
    {
        FillEveryByteInData(0xFF);
        free(_data);
        _data = nullptr;
    }

    if(_next != nullptr)
    {
        free(_next);
        _next = nullptr;
    }
    
    if(_previous != nullptr)
    {
        free(_previous);
        _previous = nullptr;
    }

    if(_group != nullptr)
    {
        free(_group);
        _group= nullptr;
    }

    
}

void Point::ByteCount(isType type,size_t numberElements)
{
    _element_size = numberElements;
    _total_byte_size = SchematicPoint[type].size * _element_size;
}

bool Point::ByteAllocate()
{
    if(_total_byte_size <= 0) return false;
    
    if(_data != nullptr)
        free(_data);

    _data = nullptr;

    _data = malloc(_total_byte_size);

    return (_data != nullptr)? true : false ;
}

bool Point::SetName(const char *name)
{
    if((MAX_CHAR_NAME - 1) < strlen(name))
        return false;
    
    memcpy(&_name[0],name,strlen(name));

    _name[MAX_CHAR_NAME-1] = '\0';

    return true;
}

const char * Point::GetName()
{
    return _name;
}

const TypeInfo Point::GetTypeInfo()
{
    return SchematicPoint[_type];
}


void Point::ClearValue()
{
    FillEveryByteInData(0x00);
}


void Point::FillEveryByteInData(uint8_t mark)
{
    if(_data != nullptr)
        memset(_data,mark,_total_byte_size);
}

template<typename T>
bool Point::SetValue(T data)
{
    return SetValue(&data,1);
}

template<typename T>
bool Point::SetValue(T *data,size_t size = 1)
{
    if(GetTypeInfo().canWrite == false) return false;

    if(_total_byte_size < (sizeof(T)*size)) return false;

    if(_data == nullptr) return false;

    if(data == nullptr) return false;

    memcpy(_data,data,sizeof(T)*size);

    return true;
}

template<typename T>
T Point::GetValue()
{
    T var{} ;
    if(_data == nullptr) return var;
    return *((T*)_data);
}

template<typename T>
T Point::GetValue(size_t index)
{
    T var{} ;
    if(index >= _element_size) return var;
    if(_data == nullptr) return var;
    return ((T*)_data)[index];
}

void * Point::GetRawBuffer()
{
    if(_data == nullptr) return nullptr;
    return _data;
}


const size_t Point::GetByteSize()
{
    return _total_byte_size;
}

const size_t Point::Count()
{
    return _element_size;
}