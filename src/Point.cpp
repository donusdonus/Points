#include "Point.h"

/* TEST PASS CALL ONE TIME ONLY */
bool Point::ByteAllocate(isMemory memtype,size_t numberelements)
{
    _prop.DataAllocated = false;
    monitor = true;

    _data.size = SchematicPoint[_prop.VarType].element_size * numberelements;

    monitor = (_data.value != nullptr);
    /*
    if(monitor)
    {
        free(_data.value);
    }
    */

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

    if((_name.value != nullptr) && (_name.size > 0))
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

    if(_item_home != nullptr)
        _item_home = nullptr;

    memset(&_prop,MARK_FREE_MEMORY,sizeof(_prop));
}


/*********************  HERE *********************************/

/* TEST PASS */
/* Creat GROUP_T Only */
Point::Point(const char * name,isMemory memtype)
{
    /*
    _item_home = nullptr;
    _item_next = nullptr;


    _prop.Value = 0;
    _prop.MemType = memtype;
    _prop.VarType = isType::VAR_GROUP;
    
    ByteAllocate(_prop.MemType,SchematicPoint[_prop.VarType].element_size);
    SetName(name);
    */

    init(name,memtype);
}

/* TEST PASS */
Point::Point(
                isType type,
                const char * name,
                size_t numberElements,
                isMemory memtype
            )
{

        /*
        _item_home = nullptr;
        _item_next = nullptr;

        _prop.Value = 0;
        _prop.MemType = memtype;
        _prop.VarType = type;
        
        _name.size = 0;

        ByteAllocate(_prop.MemType,numberElements); 
        SetName(name);
        */

         init(type,name,numberElements,memtype);
}

Point::Point(
                isType type , 
                const char * name,
                uint8_t *addr,
                size_t numberElements,
                isMemory memtype 
            )
{
    /*
    monitor = true;

    _item_home = nullptr;
    _item_next = nullptr;
 

    _prop.MemType = memtype;
    _prop.VarType = type;
    _prop.DataAllocated = (addr != nullptr);
    
 
    _name.size = 0;

    _data.value = addr ; 
    _data.size = numberElements;
    */

    init(type,name,numberElements,memtype);

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
Point * Point::AddItem(
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

    Point *_newpoint = init(type,name,numberElements,memtype,true);

    monitor = (_newpoint != nullptr);
    if(!monitor)
        return nullptr;

    /* add to next node */
    Point **_cursor = &_item_next;

    while((_cursor != nullptr) && (*_cursor != nullptr))
    {
            _cursor = &(*_cursor)->_item_next;
    }

    (*_cursor) = nullptr;
    (*_cursor) = _newpoint;
    
    return (*_cursor);
}

Point * Point::AddItem(
                        isType type , 
                        const char * name,
                        uint8_t *addr,
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

    Point *_newpoint = init(type,name,addr,numberElements,memtype,true);

    monitor = (_newpoint != nullptr);
    if(!monitor)
        return nullptr;

    /* add to next node */
    Point **_cursor = &_item_next;

    while((_cursor != nullptr) && (*_cursor != nullptr))
    {
            _cursor = &(*_cursor)->_item_next;
    }

    (*_cursor) = nullptr;
    (*_cursor) = _newpoint;
    
    return (*_cursor);
}




/* TEST PASS */
Point * Point::FindItem(const char name[])
{
    monitor = false;
    
    monitor = (_prop.VarType == isType::VAR_GROUP);
    if(!monitor)
        return nullptr;

    Point **_cursor = &_item_next;

    while((_cursor != nullptr) && (*_cursor != nullptr) && !monitor)
    {
         monitor = (strcmp((*_cursor)->GetName(),name) == 0);
         
         if(!monitor)
            _cursor = &(*_cursor)->_item_next;
    }

    if(monitor)
        return *_cursor;

    return nullptr;
}

/* TEST PASS */
Point * Point::FindItem(size_t Index)
{
    size_t count = 0 ;
    monitor = false;

    Point **cursor = &_item_next;  
    
    while((cursor != nullptr) && ((*cursor) != nullptr) && !monitor)
    {
        monitor = (count == Index);

        if(!monitor)
        {
            cursor = &(*cursor)->_item_next;
            count++;
        }
    }

    return (*cursor); 
}

/* TEST PASS */
Point * Point::Home(void)
{
    return _item_home;
}

int Point::GetItemIndex()
{
    int count = 0 ;

    monitor = (_item_home != nullptr);
    if(!monitor)
        return -1;

    Point **cursor = &(_item_home->_item_next);  
    
    monitor = false;

    while((cursor != nullptr) && ((*cursor) != nullptr) && !monitor)
    {
        monitor = ( (*cursor) == this);

        if(!monitor)
        {
            cursor = &(*cursor)->_item_next;
            count++;
        }
    }

    if(monitor)
        return count;

    return -1; 

}

size_t Point::GetItemCount()
{
    int count = 0;

    monitor = (_item_home != nullptr);
    if(!monitor)
        return -1;

    Point **cursor = &(_item_home->_item_next);  

    while((cursor != nullptr) && ((*cursor) != nullptr))
    {
        cursor = &(*cursor)->_item_next;
        count++;
    }

    return count; 
}

/* WAIT EDIT */
bool Point::RemoveItem(const char name[])
{
    monitor = true;

    Point * p = FindItem(name);

    monitor = (p != nullptr);
    if(!monitor)
        return false;

    int index = p->GetItemIndex();
    
    monitor = (index != -1);
    if(!monitor)
        return false;

    monitor = RemoveItem(index);
    
    return monitor;
}

/* TEST PASS */
bool Point::RemoveItem(size_t index)
{
    Point *prev = nullptr;
    Point *cur = nullptr;

    

    prev = FindItem(index-1);
    cur = FindItem(index);

    monitor = (cur != nullptr); 
    if(!monitor)
        return monitor;

    monitor = (prev != nullptr);
    if(monitor)
        prev->_item_next = cur->_item_next;
    else 
        _item_next = cur->_item_next;


    cur->Remove();
    free(cur);

    return true;
}


Point *  Point::init(const char * name,isMemory memtype,bool clone)
{
    

    if(clone == false)
    {
            /* Init Pointer Branch */
        _item_home = nullptr;
        _item_next = nullptr;

        _node_home = nullptr;
        _node_next = nullptr;

        /* Init Prop */
        _prop.Value = 0;
        _prop.MemType = memtype;
        _prop.VarType = isType::VAR_GROUP;
        
        ByteAllocate(_prop.MemType,SchematicPoint[_prop.VarType].element_size);
        SetName(name);
    
        return nullptr;
    }
    else 
    {
        Point *temp = (Point*)malloc(sizeof(Point));

        monitor = (temp != nullptr);
        if(!monitor)
            return nullptr;

        temp->_item_home = nullptr;
        temp->_item_next = nullptr;

        temp->_node_home =nullptr;
        temp->_node_next =nullptr;

        temp->_prop.Value = 0;
        temp->_prop.MemType = memtype;
        temp->_prop.VarType = isType::VAR_GROUP;
        
        temp->ByteAllocate(_prop.MemType,SchematicPoint[_prop.VarType].element_size);
        temp->SetName(name);

        return temp;

    }

    return nullptr;
}

Point * Point::init(isType type , const char * name,uint8_t *addr,size_t numberElements,isMemory memtype,bool clone)
{

    if(clone == false)
    {
        /* Init Pointer Branch */
        _item_home = nullptr;
        _item_next = nullptr;

        _node_home = nullptr;
        _node_next = nullptr;
    
        /* Init Prop */
        _prop.MemType = memtype;
        _prop.VarType = type;
        _prop.DataAllocated = (addr != nullptr);
        
        //ByteAllocate(_prop.MemType,numberElements);  
        _name.size = 0;

        _data.value = nullptr;
        _data.value = addr ; 
        _data.size = numberElements;

        SetName(name);

        return nullptr;

    }
    else 
    {
        Point *temp = (Point*)malloc(sizeof(Point));

        monitor = (temp != nullptr);
        if(!monitor)
            return nullptr;

                /* Init Pointer Branch */
        temp->_item_home = nullptr;
        temp->_item_next = nullptr;

        temp->_node_home = nullptr;
        temp->_node_next = nullptr;
    
        /* Init Prop */
        temp->_prop.MemType = memtype;
        temp->_prop.VarType = type;
        temp->_prop.DataAllocated = (addr != nullptr);
        
        //ByteAllocate(_prop.MemType,numberElements);  
        temp->_name.size = 0;

        temp->_data.value = nullptr;
        temp->_data.value = addr ; 
        temp->_data.size = numberElements;

        temp->SetName(name);

        return temp;
    }

    return nullptr ; 
}

Point * Point::init(isType type,const char * name,size_t numberElements,isMemory memtype,bool clone)
{
    if(clone == false)
    {
        monitor = true;
        /* Init Pointer Branch */
        _item_home = nullptr;
        _item_next = nullptr;

        _node_home = nullptr;
        _node_next = nullptr;

        /* Init Prop */
        _prop.Value = 0;
        _prop.MemType = memtype;
        _prop.VarType = type;
        
        _name.size = 0;

        monitor &= ByteAllocate(_prop.MemType,numberElements); 
        monitor &= SetName(name);

        return nullptr ;
    }
    else 
    {
        Point *temp = (Point*)malloc(sizeof(Point));

        monitor = (temp != nullptr);
        if(!monitor)
            return nullptr;

        /* Init Pointer Branch */
        temp->_item_home = nullptr;
        temp->_item_next = nullptr;

        temp->_node_home = nullptr;
        temp->_node_next = nullptr;

        /* Init Prop */
        temp->_prop.Value = 0;
        temp->_prop.MemType = memtype;
        temp->_prop.VarType = type;
        
        temp->_name.size = 0;

        temp->ByteAllocate(_prop.MemType,numberElements); 
        temp->SetName(name);

        return temp;
    }

    return nullptr;
}

Point * Point::add(const char * name,isMemory memtype = isMemory::RAM,bool operate_node = false){return nullptr;}
Point * Point::add(isType type , const char * name,uint8_t *addr,size_t numberElements = 1,isMemory memtype = isMemory::RAM,bool operate_node = false){return nullptr;}
Point * Point::add(isType type,const char * name,size_t numberElements = 1 ,isMemory memtype = isMemory::RAM,bool operate_node = false){return nullptr;}

Point * Point::index(size_t index,bool operate_node = false)
{
    int count = index + 1;
    Point *tmp =  home(this,operate_node);   

    monitor = (tmp != nullptr);
    if(!monitor)
        return nullptr;

    tmp = next(tmp,operate_node);
    monitor = (tmp != nullptr);
    if(!monitor)
        return nullptr;

    count--;


    while((tmp != nullptr) && (count > 0))
    {
        tmp = next(tmp,operate_node);
        count--;
    }


    return   (count == 0)? tmp : nullptr ;
}

int Point::count(bool operate_node = false)
{
    int counter = 0;
    Point *tmp =  home(this,operate_node);   

    monitor = (tmp != nullptr);
    if(!monitor)
        return -1;

    tmp = next(tmp,operate_node);
    monitor = (tmp != nullptr);
    if(!monitor)
        return -1;

    counter++;

    while((tmp != nullptr))
    {
        tmp = next(tmp,operate_node);
        
        monitor = (tmp != nullptr);
        if(monitor)
            counter++
    }


    return  counter ;
}


Point * Point::find(size_t index,bool operate_node = false)
{
    
    return nullptr;
}

Point * Point::get(bool operate_node = false)
{
    
    return nullptr;
}

bool Point::can_next(Point *src)
{
    Point **tmp = &src;
    monitor = ((tmp != nullptr) && (*tmp != nullptr));
    return monitor;
}

Point * Point::next(Point *src,bool operate_node = false)
{
    if(can_next(src))
        return (operate_node)? src->_node_next   : src->_item_next ;
    return nullptr;
}

Point * Point::home(Point *src,bool operate_node = false)
{
    return (operate_node)? src->_node_home  : src->_item_home ;
}

bool Point::remove(bool operate_node = false){return false;}


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

