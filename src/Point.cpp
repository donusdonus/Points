#include "Point.h"

typedef size_t (*PrintPointData)(size_t ,void *,size_t);
size_t DISP_CHAR(size_t offset,void *data,size_t index){ return  snprintf(PrintOut + offset,MAX_BUFFER_PRINT - offset,"%c",((char*)data)[index]);}
size_t DISP_INT8(size_t offset,void *data,size_t index){ return  snprintf(PrintOut + offset,MAX_BUFFER_PRINT - offset,"%d ",((int8_t*)data)[index]);}
size_t DISP_UINT8(size_t offset,void *data,size_t index){ return  snprintf(PrintOut + offset,MAX_BUFFER_PRINT - offset,"%d ",((uint8_t*)data)[index]);}
size_t DISP_INT16(size_t offset,void *data,size_t index){ return  snprintf(PrintOut + offset,MAX_BUFFER_PRINT - offset,"%d ",((int16_t*)data)[index]);}
size_t DISP_UINT16(size_t offset,void *data,size_t index){ return  snprintf(PrintOut + offset,MAX_BUFFER_PRINT - offset,"%d ",((uint16_t*)data)[index]);}
size_t DISP_INT32(size_t offset,void *data,size_t index){ return  snprintf(PrintOut + offset,MAX_BUFFER_PRINT - offset,"%d ",((int32_t*)data)[index]);}
size_t DISP_UINT32(size_t offset,void *data,size_t index){ return  snprintf(PrintOut + offset,MAX_BUFFER_PRINT - offset,"%d ",((uint32_t*)data)[index]);}
size_t DISP_FLOAT(size_t offset,void *data,size_t index){ return  snprintf(PrintOut + offset,MAX_BUFFER_PRINT - offset,"%.3f ",((float*)data)[index]);}
size_t DISP_DOUBLE(size_t offset,void *data,size_t index){ return  snprintf(PrintOut + offset,MAX_BUFFER_PRINT - offset,"%.3f ",((double*)data)[index]);}

PrintPointData  FuncPrintValue[]   = {
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




Point::Point(isType type , const char * name,size_t numberElements)
{
    Init();
    SetName(name);
    SetType(type);
    ByteCount(type,numberElements);
    ByteAllocate();
    FillEveryByteInData(0x00);
}


void Point::Init()
{
    _type = isType::GROUP_T;
    _data = nullptr;
    _next = nullptr;
    _previous = nullptr;
    _group = nullptr;
}

void Point::SetType(isType type)
{
    _type = type;
}

void Point::Clear()
{

    memset(_name,0xFF,MAX_CHAR_NAME);

    if(_data != nullptr)
    {
        //FillEveryByteInData(0xFF);
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

const isType Point::GetType()
{
    return _type;
}


bool Point::Copy(Point *src)
{

    if(src == nullptr) return false;
    if(src->GetRawBuffer() == nullptr) return false;

    /* try new buffer */
    if(_data != nullptr) 
    {
        FillEveryByteInData(0xFF);
        free(_data);
        _data = nullptr;
    }

    if(
        SetName(src->GetName()) == false
    ) return false;

    SetType(src->GetType());
    ByteCount(_type,src->Count());

    if(
        ByteAllocate() == false
      ) return false;

/*
    if(
        SetValue<uint8_t>((uint8_t*)src->GetRawBuffer(),src->GetByteSize()) == false
      ) return false;
*/

    memcpy(_data,src->GetRawBuffer(),src->GetByteSize());

    return true;
}

/* Issue  (MAX_BUFFER_PRINT - used) < -1   undefined behavior */
const char * Point::DisplayContext()
{
    size_t used = 0;
    
    memset(PrintOut,'\0',MAX_BUFFER_PRINT);

    used += snprintf(PrintOut + used,MAX_BUFFER_PRINT - used,"%s %s = ",GetTypeInfo().name,GetName());

    if(FuncPrintValue[GetType()] != nullptr)
    {
        for(int n = 0 ; n < Count() ; n++)
        {
            used += FuncPrintValue[GetType()](used,_data,n);
        }
    }

    used += snprintf(PrintOut+used,MAX_BUFFER_PRINT - used,"\r");

    return PrintOut;
}


bool Point::AddSlot(Point point)
{
    if( _type != isType::GROUP_T ) return false;

    Point *NewPoint = (Point*)malloc(sizeof(Point));
   
    if(NewPoint == nullptr) return false;

    NewPoint->Init();
    if(NewPoint->Copy(&point) == false) return false;

    Point **Cursor = nullptr;

    Cursor = &_group;
    
    while(*Cursor != nullptr)
    {
        Cursor = &((*Cursor)->_next);
    }

    *Cursor = NewPoint;

    return true;
}

Point* Point::FindSlot(const char name[])
{
    Point *Cursor  = _group;

    while (Cursor != nullptr )
    {
        if(strcmp(Cursor->GetName(),name) == 0) 
                return Cursor; 
        Cursor = Cursor->_next;
        /* code */
    }
    
    return nullptr;
}

bool Point::DeleteSlot(Point *point)
{
    if (point == nullptr) return false;
    if (_type != isType::GROUP_T) return false;
    Point **Cursor = &_group;

    while (*Cursor != nullptr)
    {
        if (*Cursor == point)
        {
            Point *ToDelete = *Cursor;
            *Cursor = ToDelete->_next;
            ToDelete->Clear();
            free(ToDelete);
            return true;
        }

        Cursor = &((*Cursor)->_next);
    }

    return false; // Point not found
}

