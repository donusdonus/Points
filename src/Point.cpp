#include "Point.h"

/* TEST PASS */
bool Point::ByteAllocate(isMemory memtype,size_t numberelements)
{
    _prop.DataAllocated = false;
    monitor = true;

    _data.size = SchematicPoint[_prop.VarType].element_size * numberelements;

    monitor = (_data.value != nullptr);
    if(monitor)
    {
        free(_data.value);
    }

    _data.value = nullptr;

    switch (memtype)
    {
        case isMemory::RAM : 
            _data.value = (uint8_t*)_SECTOR_RAM_(_data.size);  
        break;
        case isMemory::PSRAM : 
            _data.value = (uint8_t*)_SECTOR_PSRAM_(_data.size);
        break;
        case isMemory::SPARE_1:
            _data.value = (uint8_t*)_SECTOR_SPARE1_(_data.size);
        break;
        case isMemory::SPARE_2:
            _data.value = (uint8_t*)_SECTOR_SPARE1_(_data.size);
        break;    
    }

    monitor = (_data.value != nullptr);
    if(!monitor)
        return monitor;

    memset(_data.value,MARK_FREE_MEMORY,_data.size);

    _prop.DataAllocated = true;
    return monitor;
}

bool Point::GetBuffer(RawMemory *src)
{
    monitor = true;

    monitor = (_data.value != nullptr) ? true : false;
    if(!monitor) 
        return monitor;
    
    src = &_data;

    return monitor;
}

/* TEST PASS */
const size_t Point::Count()
{
    return _data.size/SchematicPoint[_prop.VarType].element_size;
}

/* TEST PASS */
bool Point::SetName(const char * name)
{
    _prop.NameAllocated = 0;
    monitor = true;

    if(_name.value != nullptr)
    {
        memset(_name.value,0xFF,_name.size);
        free(_name.value);
        _name.value = nullptr;
    }

    _name.size = strlen(name) + 1;

     switch (_prop.MemType)
    {
        case isMemory::RAM : 
            _name.value = (uint8_t*)_SECTOR_RAM_(_name.size);  
        break;
        case isMemory::PSRAM : 
            _name.value = (uint8_t*)_SECTOR_PSRAM_(_name.size);
        break;
        case isMemory::SPARE_1:
            _name.value = (uint8_t*)_SECTOR_SPARE1_(_name.size);
        break;
        case isMemory::SPARE_2:
            _name.value = (uint8_t*)_SECTOR_SPARE1_(_name.size);
        break;    
    }

    monitor = (_name.value != nullptr);
    if(!monitor)
        return monitor;

    if(monitor)
    {
        memcpy(_name.value,name,_name.size -1);
        _name.value[_name.size-1] = MARK_CHAR_NULL;
        _prop. NameAllocated = 1;
    }
    return monitor;
}

const char * Point::GetName()
{
    return (char*)_name.value;
}

const isType Point::GetType()
{
    return _prop.VarType;
}

const size_t Point::GetTotalByteSize()
{
    return sizeof(Point) + _name.size + _data.size ;
}

const size_t Point::GetValueByteSize()
{
    return _data.size ;
}

const FlagMark Point::GetFlagMark()
{
    return  _prop;
}

void Point::Remove()
{
    /* Clear All Memory  */

    /* _data remove memory */
    if(_data.value != nullptr)
    {
        memset(_data.value,MARK_FREE_MEMORY,_data.size);
        memset(&_data.size,MARK_FREE_MEMORY,sizeof(_data.size));
        free(_data.value);
        _data.value = nullptr;
    }
    
    /* _name remove memory */
    if(_name.value != nullptr)
    {
        memset(_name.value,MARK_FREE_MEMORY,_name.size);
        memset(&_name.size,MARK_FREE_MEMORY,sizeof(_name.size));
        free(_name.value);
        _name.value = nullptr;
    }

    if(_ptr_home != nullptr)
        _ptr_home = nullptr;

    memset(&_prop,MARK_FREE_MEMORY,sizeof(_prop));
}


/*********************  HERE *********************************/

/* TEST PASS */
/* Creat GROUP_T Only */
Point::Point(const char * name,isMemory memtype)
{
    /* Init Pointer Branch */
    _ptr_home = nullptr;
    _ptr_next = nullptr;

    /* Init Prop */
    _prop.Value = 0;
    _prop.MemType = memtype;
    _prop.VarType = isType::VAR_GROUP;
    
    ByteAllocate(_prop.MemType,SchematicPoint[_prop.VarType].element_size);
    SetName(name);
}

/* TEST PASS */
Point::Point(
                isType type,
                const char * name,
                size_t numberElements,
                isMemory memtype
            )
{
          /* Init Pointer Branch */
        _ptr_home = nullptr;
        _ptr_next = nullptr;

        /* Init Prop */
        _prop.Value = 0;
        _prop.MemType = memtype;
        _prop.VarType = type;
        
        _name.size = 0;

        ByteAllocate(_prop.MemType,numberElements); 
        SetName(name);
}

Point::Point(
                isType type , 
                const char * name,
                uint8_t *addr,
                size_t numberElements,
                isMemory memtype 
            )
{
    monitor = true;
    /* Init Pointer Branch */
    _ptr_home = nullptr;
    _ptr_next = nullptr;
 
    /* Init Prop */
    _prop.MemType = memtype;
    _prop.VarType = type;
    _prop.DataAllocated = (addr != nullptr);
    
    //ByteAllocate(_prop.MemType,numberElements);  
    _name.size = 0;

    _data.value = addr ; 
    _data.size = numberElements;


    SetName(name);
}


/* TEST PASS */
/* Issue  (MAX_BUFFER_PRINT - used) < -1   undefined behavior */
const char * Point::DisplayContext()
{
    size_t used = 0;
    
    memset(PrintOut,'\0',MAX_BUFFER_PRINT);

    used += snprintf(PrintOut + used,MAX_BUFFER_PRINT - used,"%s %s = ",SchematicPoint[_prop.VarType].name,GetName());

    if(FuncPrintValue[_prop.VarType] != nullptr)
    {
        for(int n = 0 ; n < Count() ; n++)
        {
            used += FuncPrintValue[_prop.VarType](used,_data.value,n);
        }
    }

    used += snprintf(PrintOut+used,MAX_BUFFER_PRINT - used,"\r");

    return PrintOut;
}

/* TEST PASS */
const char * Point::DisplayValue(size_t index)
{
    memset(PrintOut,'\0',MAX_BUFFER_PRINT);
    FuncPrintValue[_prop.VarType](0,_data.value,index);
    return &PrintOut[0] ;
}

/* TEST PASS */
Point * Point::Insert(
                    isType type,
                    const char * name,
                    size_t numberElements,
                    isMemory memtype
                  )
{
    monitor = true;

    monitor = (_prop.VarType == isType::VAR_GROUP);
    if(!monitor)
        return nullptr;

    monitor = (type != isType::VAR_GROUP);
    if(!monitor)
        return nullptr;

    Point *_newpoint = (Point *)malloc(sizeof(Point));

    _newpoint->_ptr_home = nullptr;
    _newpoint->_ptr_home = this;

    _newpoint->_ptr_next = nullptr;
    
    _newpoint->_prop.Value = 0;
    _newpoint->_prop.InGroup = true;
    _newpoint->_prop.MemType = memtype;
    _newpoint->_prop.VarType = type;

    _newpoint->_data.size = 0;
    _newpoint->_name.size = 0;

    monitor = _newpoint->ByteAllocate(_prop.MemType,numberElements);
    if(!monitor)
        return nullptr;

    monitor = _newpoint->SetName(name);
    if(!monitor)
        return nullptr;

    /* add to next node */
    Point **_cursor = &_ptr_next;

    while((_cursor != nullptr) && (*_cursor != nullptr))
    {
            _cursor = &(*_cursor)->_ptr_next;
    }

    (*_cursor) = nullptr;
    (*_cursor) = _newpoint;
    
    return (*_cursor);
}

/* TEST PASS */
Point * Point::FindByName(const char name[])
{
    monitor = false;
    
    monitor = (_prop.VarType == isType::VAR_GROUP);
    if(!monitor)
        return nullptr;

    Point **_cursor = &_ptr_next;

    while((_cursor != nullptr) && (*_cursor != nullptr) && !monitor)
    {
         monitor = (strcmp((*_cursor)->GetName(),name) == 0);
         
         if(!monitor)
            _cursor = &(*_cursor)->_ptr_next;
    }

    if(monitor)
        return *_cursor;

    return nullptr;
}

/* TEST PASS */
Point * Point::FindByIndex(size_t Index)
{
    size_t count = 0 ;
    monitor = false;

    Point **cursor = &_ptr_next;  
    
    while((cursor != nullptr) && ((*cursor) != nullptr) && !monitor)
    {
        monitor = (count == Index);

        if(!monitor)
        {
            cursor = &(*cursor)->_ptr_next;
            count++;
        }
    }

    return (*cursor); 
}

/* TEST PASS */
Point * Point::Home(void)
{
    return _ptr_home;
}

int Point::GetIndex()
{
    int count = 0 ;

    monitor = (_ptr_home != nullptr);
    if(!monitor)
        return -1;

    Point **cursor = &(_ptr_home->_ptr_next);  
    
    monitor = false;

    while((cursor != nullptr) && ((*cursor) != nullptr) && !monitor)
    {
        monitor = ( (*cursor) == this);

        if(!monitor)
        {
            cursor = &(*cursor)->_ptr_next;
            count++;
        }
    }

    if(monitor)
        return count;

    return -1; 

}

size_t Point::GetPointCount()
{
    int count = 0;

    monitor = (_ptr_home != nullptr);
    if(!monitor)
        return -1;

    Point **cursor = &(_ptr_home->_ptr_next);  

    while((cursor != nullptr) && ((*cursor) != nullptr))
    {
        cursor = &(*cursor)->_ptr_next;
        count++;
    }

    return count; 
}

/* WAIT EDIT */
bool Point::DeleteByName(const char name[])
{
    monitor = true;

    Point * p = FindByName(name);

    monitor = (p != nullptr);
    if(!monitor)
        return false;

    int index = p->GetIndex();
    
    monitor = (index != -1);
    if(!monitor)
        return false;

    monitor = DeleteByIndex(index);
    
    return monitor;
}

/* TEST PASS */
bool Point::DeleteByIndex(size_t index)
{
    Point *prev = nullptr;
    Point *cur = nullptr;

    prev = FindByIndex(index-1);
    cur = FindByIndex(index);

    monitor = (cur != nullptr); 
    if(!monitor)
        return monitor;

    monitor = (prev != nullptr);
    if(monitor)
        prev->_ptr_next = cur->_ptr_next;
    else 
        _ptr_next = cur->_ptr_next;


    cur->Remove();
    free(cur);

    return true;
}



/*


bool Point::AddSlot(const char * name,isMemory memtype = isMemory::RAM)
{
    Point *p = malloc()

    Point *NewPoint = (Point*)malloc(sizeof(Point));
   
    ret = (NewPoint == nullptr)? true : false;
    if(ret) return ret;

    NewPoint->Init();

    ret = NewPoint->Copy(&point)? true : false;
    if(ret == false) return ret;

    Point **Cursor = nullptr;

    Cursor = &_group;
    
    while(*Cursor != nullptr)
    {
        Cursor = &((*Cursor)->_next);
    }

    *Cursor = NewPoint;

    return ret;
}

Point* Point::FindSlot(const char name[])
{
    Point *Cursor  = _group;

    while (Cursor != nullptr )
    {
        if(strcmp(Cursor->GetName(),name) == 0) 
                return Cursor; 
        Cursor = Cursor->_next;
      
    }
    
    return nullptr;
}

bool Point::DeleteSlot(Point *point)
{
    if (point == nullptr) return false;
    if (_type != isType::VAR_GROUP) return false;
    Point **Cursor = &_group;

    while (*Cursor != nullptr)
    {
        if (strcmp((*Cursor)->GetName(),point->GetName()) == 0)
        {
            (*Cursor)->Clear();
            free(*Cursor);
            *Cursor = nullptr;
            //Point *ToDelete = *Cursor;
            //*Cursor = ToDelete->_next;
            //ToDelete->Clear();
            //free(ToDelete);
            //ToDelete = nullptr;

            return true;
        }

        Cursor = &((*Cursor)->_next);
    }

    return false; // Point not found
}



*/

/*
bool Point::Copy(Point *src)
{
    monitor = true;


    monitor = (src->GetType() != isType::VAR_GROUP) ? true : false;
    if(!monitor) 
        return monitor;


    monitor = (_prop.VarType != isType::VAR_GROUP) ? true : false;
    if(!monitor) 
        return monitor;
        


    _prop = src->GetFlagMark();


    monitor = SetName(src->GetName());
    if(!monitor)
        return monitor;

    RawMemory *temp = nullptr;
    monitor = src->GetBuffer(temp);
    if(!monitor)
        return monitor;

    monitor = ByteAllocate(_prop.MemType,src->GetValueByteSize()); 
    if(!monitor)
        return monitor;

    memcpy(_data.value,temp->value,_data.size);

    return monitor;
}
*/
